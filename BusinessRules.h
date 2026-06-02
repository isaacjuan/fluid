#pragma once

#include <string>

namespace BusinessRules {

void duplicarEnSitio(int& value);
int sumarTres(int value);
int elevarAlCuadrado(int value);
int duplicarValor(int value);
int incrementarUno(int value);
bool superaUmbral(int value);
std::string formatearResultado(int value);

constexpr int fallbackEstricto() {
    return 0;
}

constexpr int fallbackConservador() {
    return 99;
}

} // namespace BusinessRules
