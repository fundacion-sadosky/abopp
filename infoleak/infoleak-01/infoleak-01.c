// infoleak-01.c

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

const char* secret = "secret";

typedef struct {
    char data[48];
    char secret_token[16];
} user_t ;

int main(int argc, char **argv) {
    user_t user;
    bzero((void*)&user, 64);
    memcpy(&user.secret_token, secret, 6);
    strncpy((char*)&user.data, argv[1], 48);
    printf("%s\n", (char*)&user.data);
}
