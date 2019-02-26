// black-box-01.c

#include <unistd.h>

int main(int argc, char **argv) {
    char buffer[64];
    
    ssize_t r = read(STDIN_FILENO, &buffer, 64);
    
    if (r > 0 && buffer[0] == 'H')
        if (r > 1 && buffer[1] == 'I')
            if (r > 2 && buffer[2] == '!')
                *(char*)(0) = 0;
    
    return 0;
}
