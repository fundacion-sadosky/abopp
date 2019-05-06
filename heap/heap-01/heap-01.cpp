// heap-01.cpp

#include <iostream>
#include <stdint.h>

class Interface {
public:
    virtual void virtual_method() = 0;
};

class Implementation : public Interface {
public:
    virtual void virtual_method() {
        std::cout << "Intenta de vuelta." << std::endl;
    }
};

void runme() {
  std::cout << "Ganaste." << std::endl;
}

int main(int argc, char **argv) {
    uint64_t *buffer = new uint64_t[128];
    Interface *x = new Implementation();
    
    void (*vt)() = &runme;
    buffer[(unsigned char)argv[1][0]] = (uint64_t)&vt;
    
    x->virtual_method();
    delete x;
}
