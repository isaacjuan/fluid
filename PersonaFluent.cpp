#include "PersonaFluent.h"

#include <string>

std::string PersonaFluent::saludo() const {
    return "Hola, soy " + nombre;
}
