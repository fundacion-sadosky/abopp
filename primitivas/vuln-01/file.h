/* file.c
 * Estructura de datos utilizada en el ejercicio vuln-01.
 * Abstrae el concepto de un archivo compuesto por bloques de datos,
 * cada uno de una cierta longitud máxima.
 */

#define HEADER_NAME_SIZE 56
#define BLOCK_DATA_LENGTH 508

typedef struct file file;
typedef struct block block;

/* file
 * Un struct file encapsula el concepto de un archivo. El archivo está
 * compuesto por varios bloques, cada uno conteniendo parte de la
 * información total del mismo. Entonces, el primer bloque guardará
 * una parte de los datos, el siguiente guardará otra parte, y así
 * sucesivamente hasta el último bloque. Las operaciones sobre el
 * archivo deben iterar procesando bloque por bloque.
 *
 * Internamente los bloques se organizan en forma de
 * lista enlazada, con cada bloque manteniendo una referencia al siguiente;
 * de entre esos bloques, file solo mantiene referencia a dos:
 * al primero y al último.
 *
 */
struct file {
 block *first_block;
 block *last_block;
 char name[HEADER_NAME_SIZE];
} __attribute__((packed));

/* block
 * Un struct block encapsula el concepto de un bloque parte de un archivo.
 * Cada bloque tiene un buffer de datos y un puntero al siguiente nodo
 * en la lista enlazada. El último bloque de la lista
 * siempre deberá tener un puntero next_block nulo.
 */
struct block {
 size_t data_length;
 block *next_block;
 char data[BLOCK_DATA_LENGTH];
} __attribute__((packed));

/* create_file
 * La operación create_file crea un archivo con el nombre dado.
 * Por defecto el archivo no tiene bloques; cada bloque debe
 * ser agregado mediante una llamada a append_block.
 */
file *create_file(char *name) {
 file *f = malloc(sizeof(file));
 memset(f, 0, sizeof(file));
 strncpy(f->name, name, strlen(name));
 return f;
}

/* append_block
 * Agrega un bloque b a un archivo f, encargándose de mantener
 * la estructura de la lista enlazada. Si no hay bloques en la lista,
 * el nuevo bloque será el primero y el último del archivo; si ya
 * hay bloques en la lista, el nuevo bloque será el nuevo último
 * de la lista, y también el siguiente al último original.
 */
void append_block(block *b, file *f) {
 if (f->first_block == 0) {
   f->first_block = b;
 } else {
   f->last_block->next_block = b;
 }
 f->last_block = b;
}

/* create_block
 * Crea un bloque con los datos provistos
 * e inicializa los punteros en 0.
 */
block *create_block(char *data, size_t data_length) {
 block *b = malloc(sizeof(block));
 memset(b, 0, sizeof(block));
 memcpy(b->data, data, data_length);
 b->data_length = data_length;
 return b;
}

/* data_length
 * Devuelve la longitud de los datos almacenados en un bloque.
 */
size_t get_data_length(block *b) {
   return b->data_length;
}

/* file_length
 * Calcula la longitud de un archivo iterando por cada bloque
 * y calculando su longitud.
 */
size_t file_length(file *f) {
   size_t total_size = 0;
   block *b = f->first_block;
   while (b != 0) {
       total_size += get_data_length(b);
       b = b->next_block;
   }
   return total_size;
}

/* print_file
 * Muestra por consola los contenidos del archivo como un string.
 * Internamente itera por cada bloque, copiando el contenido de cada
 * uno a un buffer intermedio, delimitando el buffer con un byte nulo
 * para el caso en que el bloque esté lleno, y mostrándolo luego
 * por pantalla con printf. Si el archivo no es un archivo de texto,
 * podría darse que printf no muestre el contenido completo.
 */
void print_file(file *f) {
   char buf[BLOCK_DATA_LENGTH + 1];
   block *b = f->first_block;
   while (b != 0) {
       size_t block_length = get_data_length(b);
       memcpy(buf, b->data, block_length);
       buf[block_length] = 0;
       printf("%s", buf);
       b = b->next_block;
   }
   printf("\n");
}

/* release_file
 * Libera el contenido del archivo del heap,
 * iterando bloque por bloque y llamando a free.
 */
void release_file(file *f) {
   block *b = f->first_block;
   while (b != 0) {
       f->first_block = b->next_block;
       free(b);
       b = f->first_block;
   }
}

/* patch
 * La operación patch parchea un archivo a partir de una cierta
 * posición dada. El algoritmo reemplazará los bytes a partir del
 * índice provisto por el contenido del buffer data. Se copiarán
 * data_bytes desde data; en caso de extenderse el contenido sobre
 * el final del archivo, el sobrante será ignorado.
 *
 * Ejemplo: el archivo f tiene el contenido
 * 0123456789\0
 *
 * El resultado de la función patch(f, 5, "ABC", 3) será entonces
 * 01234ABC89\0
 */
void patch(file *f, long position, char *data, size_t data_length) {
   long base = 0;
   long accumulated_size = 0;
   block *b = f->first_block;
   while (b != 0) {
       base = accumulated_size;
       accumulated_size += get_data_length(b);
       if (position < accumulated_size) {
           long patch_position = position - base;
           long block_size = accumulated_size - base;
           long remainder = block_size - patch_position;
           long length = 0;

           if (data_length < remainder) {
             length = data_length;
           } else {
             length = remainder;
           }

           if (length < remainder) {
               memcpy(b->data + patch_position, data, length);
               return;
           } else {
               memcpy(b->data + patch_position, data, remainder);
               data += remainder;
               position += remainder;
               data_length -= remainder;
           }
       }
       b = b->next_block;
   }
}
