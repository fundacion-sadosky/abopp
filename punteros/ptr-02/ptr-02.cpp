/* ptr-02.cpp
 *
 * Compilar con g++ ptr-02.cpp -o ptr-02 -ldl
 *
 * Aparentemente, C++ nos hace un poco más difícil obtener las direcciones
 * de funciones en libc. Para ello hemos tenido que usar dlsym. Para usar dlsym,
 * sin embargo, es necesario pasarle el flag -ldl a g++. Nótese que esto solo
 * aplica a nivel código fuente. El procedimiento de explotación sigue siendo
 * el mismo que venimos estudiando.
 *
 */

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <unistd.h>
#include <iostream>

//================================================================================
// Packet
//--------------------------------------------------------------------------------

/*
 * La clase Packet representa algún tipo de paquete de datos. Por ejemplo,
 * un paquete TCP, o un mensaje en algún formato. Para nuestros fines, el 
 * contenido en concreto no importa demasiado, por lo que hemos dejado
 * la clase en blanco.
 *
 */
class Packet {
};

//================================================================================
// Filter
//--------------------------------------------------------------------------------

/*
 * La clase filter representa un filtro que actua sobre objetos Packet.
 * Un Filter es un nodo en una cadena de filtros, estructurada como una
 * lista enlazada. Las implementaciones concretas solo deben implementar
 * el método filter, sin preocuparse por la estructura interna de la 
 * cadena.
 *
 */
class Filter {

private:
  Filter *next;

public:

  Filter() {
    this->next = 0;
  }

  Filter *get_next() {
    return this->next;
  }

  void set_next(Filter *next) {
    this->next = next;
  }

  virtual void filter(Packet &p) = 0;

};

//================================================================================
// FilterChain
//--------------------------------------------------------------------------------

/*
 * La clase FilterChain representa una cadena de filtros, a la cuál se pueden
 * agregar instancias una por una.
 *
 */
class FilterChain {

private:
  Filter *first;
  Filter *last;

public:

  FilterChain() {
    this->first = this->last = 0;
  }

  bool filter(Packet &p) {
    Filter *current = first;

    while (current) {
      current->filter(p);
      current = current->get_next();
    }
  }

  void add_filter(Filter *filter) {
    if (last) {
      last->set_next(filter);
    } else {
      first = last = filter;
    }
  }

};

//================================================================================
// EncodingFilter
//--------------------------------------------------------------------------------

/*
 * EncodingFilter es un ejemplo de una implementación concreta de un filtro.
 *
 */
class EncodingFilter : public Filter {
public:

  void filter(Packet &p) {
    /* Hace algo con p, posiblemente modificándolo en el proceso.
     * La implementación concreta no nos interesa demasiado.
     */
  }

};


//================================================================================
// main y otras cosas
//--------------------------------------------------------------------------------

const char *fifo_path = "/tmp/ptr-02-input";

void write_to(void *where) {
  mkfifo(fifo_path, 0600);
  int fifo = open(fifo_path, O_RDONLY);
  read(fifo, where, 1024);
  close(fifo);
  unlink(fifo_path);
}


int main() {
  FilterChain chain;
  EncodingFilter encoding_filter;
  chain.add_filter(&encoding_filter);
    

  // Explotando primitivas de lectura obtenemos o calculamos los siguientes valores:
  std::cout << std::endl;

  // &mprotect
  std::cout << "&mprotect: " << dlsym(RTLD_NEXT, "mprotect") << std::endl << std::endl;

  // &chain
  std::cout << "&chain: " << &chain << std::endl << std::endl;


  // Supongamos que explotamos una primitiva de escritura que nos permite
  // escribir a partir de &chain:
  write_to(&chain);

  // Objetivo: lograr ejecutar shellcode.

  Packet p;
  chain.filter(p);
  return 0;
}
