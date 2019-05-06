// heap-03.cpp

#include "Interpreter-v2.h"

int main(int argc, char **argv) {
  Interpreter interpreter;

  /* Instanciamos un intérprete que procesará un programa cuyas
   * instrucciones deben ser provistas como argumentos.
   */
  for (unsigned int i = 1; i < argc; i++) {
    std::string current_command(argv[i]);
    interpreter.handle_command(current_command);
  }

  return 0;
}
