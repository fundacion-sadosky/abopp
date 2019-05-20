// ptr-01.c

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

/*
 * Nota: utilizamos un FIFO porque es la forma más simple de leer datos binarios
 * luego de emitir mensajes por salida estándar, dando la oportunidad de generar
 * la entrada en función de los datos emitidos.
 *
 */
char *fifo_path = "/tmp/ptr-01-input";

int start_fifo() {
  mkfifo(fifo_path, 0600);
  return open(fifo_path, O_RDONLY);
}

void finish_fifo(int fd) {
  close(fd);
  unlink(fifo_path);
}

void aid() {
  asm("pop %rdi; ret");
  asm("pop %rsi; ret");
  asm("pop %rdx; ret");
}

void hint() {
  asm("xchg %rsi, %rsp; ret");
}

int main(int argc, char **argv) {
  void (*fn)() = 0;
  char buffer[1024];

  /* Supongamos que contamos con primitivas potentes de lectura. Utilizando
   * dichas primitivas logramos leer los siguientes valores:
   */
  printf("\n");

  printf("&mprotect\n");
  printf("%p\n\n", &mprotect); 

  printf("&buffer\n");
  printf("%p\n\n", &buffer);
    
  printf("&aid\n");
  printf("%p\n\n", &aid);
  
  
  /* Escribir a /tmp/ptr01-input una entrada que logre que este programa
   * muestre el mensaje "Hello, world!" en la terminal.
   */
  int fifo = start_fifo();
  read(fifo, &fn, 8);
  read(fifo, &buffer, 1024);
  finish_fifo(fifo);

  fn();
  return 0;
}
