// stack-01.c

#include <stdio.h>
#include <stdint.h>

int main(char argc, char **argv) {
    if (argc != 2) return 1;
    
    int buffer[8];
    int k = 0;
    
    buffer[(int8_t) argv[1][0]] = 1;
    
    if (k == 1) {
        printf("Hello, world!\n");
    } else {
        printf("Try again!\n");
    }
    
    return 0;
}
