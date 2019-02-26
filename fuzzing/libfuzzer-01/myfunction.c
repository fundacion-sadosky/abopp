#include <stdint.h>

void MyFunction(const uint8_t *input, size_t size) {
    uint8_t buffer[64];
    
    for (int i = 0; i < size; i++) {
        buffer[i] = input[i];
    }
}
