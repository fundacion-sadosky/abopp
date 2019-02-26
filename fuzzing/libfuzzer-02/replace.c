#include <stdint.h>

int ReplaceInDataSection(uint8_t *buffer, char replace, char replacement) {

    if (buffer[0] != 'H' || buffer[1] != 'I') {
        return -1;
    }

    uint32_t version = *(uint32_t*)(buffer + 4);

    if (version != 10) {
        return -2;
    }

    uint32_t data_size = *(uint32_t*)(buffer + 8);
    uint32_t count = 0;

    uint8_t *data_buffer = buffer + 12;

    for (uint32_t i = 0; i < data_size; i++) {
        if ((char)data_buffer[i] == replace) {
            data_buffer[i] = (uint8_t)replacement;
            count++;
        }
    }
   
    return count;
}
