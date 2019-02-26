// stack-02.c

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void runme() {
    printf("Hello, world!\n");
    exit(0);
}

int main(char argc, char **argv) {
    if (argc != 2) return 1;
    
    // Creamos un arreglo de punteros a funciones.
    void (*buffer[8])(void);
    
    buffer[(int8_t) argv[1][0]] = runme;
    return 0;
}
