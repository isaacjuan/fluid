#include "BusinessRules.h"

#include <string>

namespace BusinessRules {

void duplicarEnSitio(int& value) {
    value *= 2;
}

int sumarTres(int value) {
    return value + 3;
}

int elevarAlCuadrado(int value) {
    return value * value;
}

int duplicarValor(int value) {
    return value * 2;
}

int incrementarUno(int value) {
    return value + 1;
}

bool superaUmbral(int value) {
    return value > 10;
}

std::string formatearResultado(int value) {
    return "Result: " + std::to_string(value);
}

} // namespace BusinessRules
