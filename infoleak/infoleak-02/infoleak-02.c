// infoleak-02.c

#include <unistd.h>
#include <stdint.h>
#include <stdio.h>

const char *mode_easy = "easy";
const char *mode_hard = "hard";

int main(int argc, char **argv) {
    int8_t i = 0; 
    read(STDIN_FILENO, &i, 1);

    const char *secret_token = "secret";
    const char *mode[3] = { 0, mode_easy, mode_hard };
    printf("%s\n", mode[i]);
}

