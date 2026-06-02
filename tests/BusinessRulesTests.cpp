#include <cassert>
#include <iostream>
#include <string>

#include "../BusinessRules.h"

namespace {

void testDuplicarEnSitio() {
    int value = 7;
    BusinessRules::duplicarEnSitio(value);
    assert(value == 14);
}

void testSumarTres() {
    assert(BusinessRules::sumarTres(10) == 13);
}

void testElevarAlCuadrado() {
    assert(BusinessRules::elevarAlCuadrado(12) == 144);
}

void testDuplicarValor() {
    assert(BusinessRules::duplicarValor(9) == 18);
}

void testIncrementarUno() {
    assert(BusinessRules::incrementarUno(41) == 42);
}

void testSuperaUmbral() {
    assert(BusinessRules::superaUmbral(11));
    assert(!BusinessRules::superaUmbral(10));
}

void testFormatearResultado() {
    assert(BusinessRules::formatearResultado(8) == "Result: 8");
}

void testFallbacks() {
    assert(BusinessRules::fallbackEstricto() == 0);
    assert(BusinessRules::fallbackConservador() == 99);
}

} // namespace

int main() {
    testDuplicarEnSitio();
    testSumarTres();
    testElevarAlCuadrado();
    testDuplicarValor();
    testIncrementarUno();
    testSuperaUmbral();
    testFormatearResultado();
    testFallbacks();

    std::cout << "BusinessRules tests OK" << std::endl;
    return 0;
}
