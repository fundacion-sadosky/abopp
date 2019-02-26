// rop-01.c

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

void aid() {
  asm("pop %rdi; ret");
  asm("pop %rsi; ret");
  asm("pop %rdx; ret");
}

int main(int argc, char **argv) {
    char buffer[256];

    // Obtenemos el valor del base pointer.
    register uint64_t rbp asm("rbp");

    // Obtenemos el valor del canario.
    uint64_t canario = *(uint64_t *)(rbp - 8);

    printf("\n");
    printf("Explotando una vulnerabilidad de lectura obtenemos "
           "los siguientes valores:\n\n");

    printf("Canario:\n");
    printf("0x%lx\n\n", canario);

    printf("&buffer:\n");
    printf("%p\n\n", &buffer);

    printf("&mprotect:\n");
    printf("%p\n\n", mprotect);

    printf("&aid:\n");
    printf("%p\n\n", aid);

    printf("Inyectar shellcode en el stack y ejecutarlo.\n");
    getchar();
    
    int fd = open("/tmp/rop-01/input.bin", O_RDONLY);
    
    read(fd, &buffer, 512);
    return 0;
}
