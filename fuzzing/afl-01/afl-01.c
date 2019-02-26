#include <unistd.h>
#include <stdint.h>

// hint: Definir AFL_DONT_OPTIMIZE=1 al momento de compilar!

int main(int argc, char **argv) {
  uint8_t buffer[64];
  uint8_t index = 0;

  ssize_t r = read(STDIN_FILENO, &index, sizeof(uint8_t));

  buffer[index] = 0;
  return 0;
}
