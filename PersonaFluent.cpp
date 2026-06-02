#include "PersonaFluent.h"

#include <iostream>

PersonaFluent& PersonaFluent::saludar() {
    std::cout << "Hola, soy " << nombre << std::endl;
    return *this;
}
