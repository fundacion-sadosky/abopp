#include <string>
#include <sstream>
#include <map>

#include "Entity.h"

class Interpreter {
private:

  const std::string TOKEN_DECLARE_INT = "int";
  const std::string TOKEN_DECLARE_INT_VECTOR = "int_v";
  const std::string TOKEN_ADD = "add";
  const std::string TOKEN_VECTOR_SET = "set_v";
  const std::string TOKEN_PRINT = "print";
  const std::string TOKEN_DELETE = "delete";

  std::map<std::string, Entity*> entities;

  void create_int(std::istringstream &input_stream) {

    // Obtenemos el identificador (el nombre) de la variable entera.
    std::string int_id;
    std::getline(input_stream, int_id, ' ');

    // Obtenemos el valor de la variable entera.
    unsigned long value;
    input_stream >> value;

    // Creamos la variable y la guardamos en el mapa de entidades.
    entities[int_id] = new Integer(value);
  }

  void create_int_vector(std::istringstream &istream) {
    std::string id;
    std::string current_value;

    // Obtenemos el nombre del vector.
    std::getline(istream, id, ' ');

    // Obtenemos la longitud del vector.
    std::getline(istream, current_value, ' ');
    unsigned long length = std::stol(current_value);

    IntVector *vector = new IntVector(length);

    // Obtenemos los elementos del vector.
    for (unsigned long i = 0; std::getline(istream, current_value, ','); i++) {
      unsigned long current_element = std::stol(current_value);
      vector->set(i, current_element);
    }

    // Guardamos el vector.
    entities[id] = static_cast<Entity*>(vector);
  }

  void add(std::istringstream &input_stream) {
    std::string a_id;
    std::string b_id;

    std::getline(input_stream, a_id, ',');
    std::getline(input_stream, b_id, ';');

    Entity *a = entities.find(a_id)->second;
    Entity *b = entities.find(b_id)->second;

    if (a && b) {
      a->add(*b);
    }
  }

  void vector_set(std::istringstream &input_stream) {
    std::string vector_id;
    std::string current_value;

    std::getline(input_stream, vector_id, ' ');

    if (Entity *entity = entities.find(vector_id)->second) {
      IntVector *vector = static_cast<IntVector*>(entity);

      // Obtenemos el índice donde cambiar el valor.
      std::getline(input_stream, current_value, ' ');
      unsigned long index = stol(current_value);

      // Obtenemos el nuevo valor.
      std::getline(input_stream, current_value, ';');
      unsigned long value = stol(current_value);

      // Cambiamos el valor.
      vector->set(index, value);
    }
  }

  void print(std::istringstream &input_stream) {
    std::string id;
    std::getline(input_stream, id, ';');
    if (Entity *p = entities.find(id)->second) {
      p->print();
    }
  }

  void delete_entity(std::istringstream &input_stream) {
    std::string id;
    std::getline(input_stream, id, ';');
    if (Entity *p = entities.find(id)->second) {
      delete p;
    }
  }

public:

  void handle_command(const std::string &command) {
    std::istringstream input_stream(command);
    std::string token;

    std::getline(input_stream, token, ' ');

    if (TOKEN_DECLARE_INT == token) {
      create_int(input_stream);
    } else if (TOKEN_DECLARE_INT_VECTOR == token) {
      create_int_vector(input_stream);
    } else if (TOKEN_ADD == token) {
      add(input_stream);
    } else if (TOKEN_VECTOR_SET == token) {
      vector_set(input_stream);
    } else if (TOKEN_PRINT == token) {
      print(input_stream);
    } else if (TOKEN_DELETE == token) {
      delete_entity(input_stream);
    }
  }

};
