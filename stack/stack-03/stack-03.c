/*
 * stack-03.c
 *
 * Nuestro objetivo final es lograr, sin modificar el código fuente,
 * que el programa ejecute la función runme.
 *
 * Instrucciones:
 * 
 * 1. Compilar el programa con gcc stack-03.c -o stack-03
 *
 * 2. Desarrollar una entrada que cambie la dirección de retorno de main 
 *    para que apunte a runme. ¿Porqué no funciona? ¿Porqué main no retorna a runme?
 *
 * 3. Parchear el binario para lograr que main retorne a runme.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

void runme() {
    printf("Hello, world!\n");
    exit(0);
}

int main(int argc, char **argv) {
    uint8_t buffer[64];
    uint8_t sz = 0;

    read(STDIN_FILENO, &sz, sizeof(uint8_t));
    size_t count = read(STDIN_FILENO, &buffer, sz);
    
    *(uint64_t *)&buffer[count] = (uint64_t)runme;
    
    return 0;
}
