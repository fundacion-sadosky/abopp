// vuln-01.c

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* En este ejercicio estaremos usando una estructura de datos
 * definida y documentada en el archivo file.h
 */
#include "file.h"

/* parse_index
 * El programa emula un patch sobre un archivo existente.
 * La posición dentro del archivo a partir de la cuál comienza
 * la escritura se recibe como argumento y se parsea en esta función.
 */
int parse_index(char *index) {
    const char MAX_INDEX_DIGITS = 3;
    if (strnlen(index, MAX_INDEX_DIGITS + 1) > MAX_INDEX_DIGITS) {
        exit(1);
    }
    return (int) strtol(index, 0, 10);
}

void validate_argc(int argc) {
    if (argc < 2) {
        exit(1);
    }
}

int main(int argc, char **argv) {
    char data_a[BLOCK_DATA_LENGTH];
    memset(data_a, 'A', sizeof(data_a));

    /* Inicializamos un archivo con algunos datos básicos.
     * Esto emula un archivo ya existente con alunos bloques de datos.
     */
    file *f = create_file("myfile");
    block *a = create_block(data_a, BLOCK_DATA_LENGTH);
    block *b = create_block("my data", 7);
    append_block(a, f);
    append_block(b, f);

    /* A partir de aquí se realiza el patch en base al índice
     * y a los datos provistos al programa. El índice lo recibimos como
     * primer argumento; los datos los recibimos por entrada estándar.
     */
    validate_argc(argc);
    int index = parse_index(argv[1]);

    char input_data[1024];
    size_t data_length = read(STDIN_FILENO, &input_data, 1024);
    patch(f, index, input_data, data_length);

    printf("\nContents: ");
    print_file(f);
    printf("\n");
    printf("Total data length: %ld\n\n", file_length(f));

    /* Se libera la memoria dinámica utilizada por el archivo
     * y finaliza la ejecución del programa
     */
    release_file(f);
    return 0;
}
