#include <iostream>

class Entity {
protected:
  enum Type {
    INTEGER,
    INTEGER_VECTOR
  } type;

  Entity(Type type) {
    this->type = type;
  }

public:
  bool instance_of(Type type) const {
    return this->type == type;
  }

  virtual ~Entity() {}

  virtual void add(const Entity &entity) = 0;
  virtual void print() = 0;

};

class Integer : public Entity {
private:
  unsigned long value;

public:
  Integer(unsigned long value) : Entity(INTEGER) {
    this->value = value;
  }

  unsigned long get_value() const {
    return this->value;
  }

  void add(const Entity &other) {
    if (!other.instance_of(INTEGER)) {
      return;
    }

    const Integer *other_int = static_cast<const Integer*>(&other);
    this->value += other_int->get_value();
  }

  void print() {
    std::cout << this->value << std::endl;
  }

};

class IntVector : public Entity {
private:
  unsigned long length;
  unsigned long *data;

public:
  IntVector(unsigned long length) : Entity(INTEGER_VECTOR) {
    data = new unsigned long[length];
    this->length = length;
  }

  unsigned long get(unsigned long i) const {
    return data[i];
  }

  void set(unsigned long i, unsigned long value) {
    if (i < length)
      data[i] = value;
  }

  unsigned long get_length() const {
    return length;
  }

  void add(const Entity &other) {
    if (!other.instance_of(INTEGER_VECTOR)) {
      return; // El otro elemento no es un vector de enteros.
    }

    const IntVector *other_int_vector = static_cast<const IntVector*>(&other);
    if (other_int_vector->get_length() != this->length) {
      return; // Solo se pueden sumar vectores de la misma longitud.
    }

    // Sumamos in place.
    for (unsigned int i = 0; i < length; i++) {
      data[i] += other_int_vector->get(i);
    }
  }

  void print() {
    for (unsigned int i = 0; i < length; i++) {
      char separator = i < length - 1 ? ',' : ';';
      std::cout << data[i] << separator;
    }
    std::cout << std::endl;
  }

  virtual ~IntVector() {
    delete data;
  }

};
