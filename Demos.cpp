#include "Demos.h"

#include <iostream>
#include <string>

#include "BusinessRules.h"
#include "Composable.h"
#include "Fluent.h"
#include "PersonaFluent.h"
#include "StaticFluent.h"
#include "UseCases.h"

namespace {

void printDemoTitle(const std::string& title) {
    std::cout << "\n-- " << title << " --" << std::endl;
}

void demoComposableChaining() {
    printDemoTitle("Demo Composable (objeto mutable)");

    Composable obj;
    obj.setValue(10).setText("Hello").add(5).append(" World!").print();
}

void demoFluentApplyAndMap() {
    printDemoTitle("Demo Fluent apply + map");

    Fluent<int> num(5);
    auto result = num.apply(BusinessRules::duplicarEnSitio)
                      .map(BusinessRules::sumarTres)
                      .map(BusinessRules::elevarAlCuadrado);

    std::cout << result.get() << std::endl; // (5*2 + 3)^2 = 169
}

void demoFluentFactoryFrom() {
    printDemoTitle("Demo Fluent from (factory)");

    auto result = Fluent<int>::from(5)
                      .apply(BusinessRules::duplicarEnSitio)
                      .map(BusinessRules::sumarTres)
                      .map(BusinessRules::elevarAlCuadrado);

    std::cout << result.get() << std::endl; // 169
}

void demoFluentFlatMap() {
    printDemoTitle("Demo Fluent flatMap");

    auto result = Fluent<int>::from(5)
                      .map(BusinessRules::duplicarValor)
                      .flatMap([](int x) {
                          return Fluent<std::string>::from(BusinessRules::formatearResultado(x));
                      });

    std::cout << result.get() << std::endl; // Result: 10
}

void demoFluentTapThen() {
    printDemoTitle("Demo Fluent tap + then");

    auto result = Fluent<int>::from(10)
                      .tap([](const int& x) { std::cout << "tap: " << x << std::endl; })
                      .then(BusinessRules::incrementarUno)
                      .then([](int x) { return x + 4; });

    std::cout << result.get() << std::endl; // 15
}

void demoFluentFlatten() {
    printDemoTitle("Demo Fluent flatten");

    auto nested = Fluent<Fluent<int>>::from(Fluent<int>::from(42));
    auto flattened = nested.flatten().map(BusinessRules::incrementarUno);

    std::cout << flattened.get() << std::endl; // 43
}

void demoFluentFilterAndOrElse() {
    printDemoTitle("Demo Fluent filter + mapIfPresent + orElse");

    auto filteredOk = Fluent<int>::from(12)
                          .filter(BusinessRules::superaUmbral)
                          .mapIfPresent(BusinessRules::duplicarValor);

    auto filteredNo = Fluent<int>::from(7)
                          .filter(BusinessRules::superaUmbral)
                          .mapIfPresent(BusinessRules::duplicarValor);

    auto recoveredOk = filteredOk.orElse(BusinessRules::fallbackEstricto())
                          .map(BusinessRules::incrementarUno);
    auto recoveredNo = filteredNo.orElse(BusinessRules::fallbackConservador())
                          .map(BusinessRules::incrementarUno);

    std::cout << filteredOk.get().value_or(-1) << std::endl; // 24
    std::cout << filteredNo.get().value_or(-1) << std::endl; // -1
    std::cout << recoveredOk.get() << std::endl; // 25
    std::cout << recoveredNo.get() << std::endl; // 100
}

void demoStaticFluent() {
    printDemoTitle("Demo StaticFluent");

    int result = StaticFluent<int>::apply(5, BusinessRules::duplicarEnSitio);
    result = StaticFluent<int>::transform(result, BusinessRules::sumarTres);
    result = StaticFluent<int>::transform(result, BusinessRules::elevarAlCuadrado);

    std::cout << result << std::endl; // 169
}

void demoCrtpFluentInheritance() {
    printDemoTitle("Demo CRTP (herencia fluida)");

    PersonaFluent persona;
    // setNombre/setEdad viven en la base y la lectura/salida queda fuera del dominio.
    persona.setNombre("Juan")
        .setEdad(30);

    std::cout << persona.saludo() << std::endl;
    std::cout << "Nombre: " << persona.getNombre() << ", Edad: " << persona.getEdad() << std::endl;
}

void demoCrtpTemplatedDerived() {
    printDemoTitle("Demo CRTP con clase derivada template");

    struct EmpleadoTag {};
    struct ClienteTag {};

    PersonaFluentT<EmpleadoTag> empleado;
    empleado.setNombre("Ana")
        .setEdad(28)
        .setEtiqueta("empleada");

    std::cout << empleado.saludoConEtiqueta() << std::endl;
    std::cout << "Nombre: " << empleado.getNombre() << ", Edad: " << empleado.getEdad()
              << std::endl;

    PersonaFluentT<ClienteTag> cliente;
    cliente.setNombre("Luis")
        .setEdad(41)
        .setEtiqueta("cliente");

    std::cout << cliente.saludoConEtiqueta() << std::endl;
    std::cout << "Nombre: " << cliente.getNombre() << ", Edad: " << cliente.getEdad()
              << std::endl;
}

void demoUseCaseAbstractionLevel() {
    printDemoTitle("Demo UseCase (nivel mas abstracto)");

    const auto casoA = UseCases::evaluarOperacion(7);
    const auto casoB = UseCases::evaluarOperacion(12);

    auto printEvaluacion = [](const char* etiqueta, const UseCases::EvaluacionOperacion& caso) {
        std::cout << etiqueta << " -> normalizado: " << caso.scoreNormalizado
                  << ", recuperado: " << caso.scoreRecuperado
                  << ", final: " << caso.scoreFinal
                  << ", estado: " << UseCases::toString(caso.estado)
                  << ", aprobada: " << (caso.aprobada() ? "si" : "no")
                  << ", politica: " << caso.versionPolitica << std::endl;

        for (const auto& razon : caso.razones) {
            std::cout << "  - " << razon << std::endl;
        }
    };

    printEvaluacion("Caso A", casoA);
    printEvaluacion("Caso B", casoB);
}

} // namespace

void runAllDemos() {
    demoComposableChaining();
    demoFluentApplyAndMap();
    demoFluentFactoryFrom();
    demoFluentFlatMap();
    demoFluentTapThen();
    demoFluentFlatten();
    demoFluentFilterAndOrElse();
    demoStaticFluent();
    demoCrtpFluentInheritance();
    demoCrtpTemplatedDerived();
    demoUseCaseAbstractionLevel();
}
