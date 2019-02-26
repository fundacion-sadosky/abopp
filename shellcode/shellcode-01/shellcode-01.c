// shellcode-01.c

#include <sys/mman.h>
#include <stdint.h>
#include <unistd.h>

int main(int argc, char **argv) {
  uint8_t buffer[256];
    
  // Leemos hasta 256 bytes de la entrada estándar.
  read(STDIN_FILENO, buffer, 256);
    
  // Haremos ejecutable al buffer usando mprotect.
  // Primero obtenemos el tamaño de página para alinear la dirección del buffer.
  long psize = sysconf(_SC_PAGESIZE);
    
  // Hacemos al buffer ejecutable.
  mprotect((void*)((uint64_t)&buffer & ~(psize-1)), 256, 7);
    
  // Ejecutamos el buffer.
  ((void (*)(void))&buffer)();
}
