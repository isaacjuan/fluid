#include "Demos.h"

#include <iostream>
#include <string>

#include "Composable.h"
#include "Fluent.h"
#include "PersonaFluent.h"
#include "StaticFluent.h"

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
    auto result = num.apply([](int& x) { x *= 2; })
                      .map([](int x) { return x + 3; })
                      .map([](int x) { return x * x; });

    std::cout << result.get() << std::endl; // (5*2 + 3)^2 = 169
}

void demoFluentFactoryFrom() {
    printDemoTitle("Demo Fluent from (factory)");

    auto result = Fluent<int>::from(5)
                      .apply([](int& x) { x *= 2; })
                      .map([](int x) { return x + 3; })
                      .map([](int x) { return x * x; });

    std::cout << result.get() << std::endl; // 169
}

void demoFluentFlatMap() {
    printDemoTitle("Demo Fluent flatMap");

    auto result = Fluent<int>::from(5)
                      .map([](int x) { return x * 2; })
                      .flatMap([](int x) {
                          return Fluent<std::string>::from("Result: " + std::to_string(x));
                      });

    std::cout << result.get() << std::endl; // Result: 10
}

void demoFluentTapThen() {
    printDemoTitle("Demo Fluent tap + then");

    auto result = Fluent<int>::from(10)
                      .tap([](const int& x) { std::cout << "tap: " << x << std::endl; })
                      .then([](int x) { return x + 5; });

    std::cout << result.get() << std::endl; // 15
}

void demoFluentFlatten() {
    printDemoTitle("Demo Fluent flatten");

    auto nested = Fluent<Fluent<int>>::from(Fluent<int>::from(42));
    auto flattened = nested.flatten().map([](int x) { return x + 1; });

    std::cout << flattened.get() << std::endl; // 43
}

void demoFluentFilterAndOrElse() {
    printDemoTitle("Demo Fluent filter + mapIfPresent + orElse");

    auto filteredOk = Fluent<int>::from(12)
                          .filter([](int x) { return x > 10; })
                          .mapIfPresent([](int x) { return x * 2; });

    auto filteredNo = Fluent<int>::from(7)
                          .filter([](int x) { return x > 10; })
                          .mapIfPresent([](int x) { return x * 2; });

    auto recoveredOk = filteredOk.orElse(0).map([](int x) { return x + 1; });
    auto recoveredNo = filteredNo.orElse(99).map([](int x) { return x + 1; });

    std::cout << filteredOk.get().value_or(-1) << std::endl; // 24
    std::cout << filteredNo.get().value_or(-1) << std::endl; // -1
    std::cout << recoveredOk.get() << std::endl; // 25
    std::cout << recoveredNo.get() << std::endl; // 100
}

void demoStaticFluent() {
    printDemoTitle("Demo StaticFluent");

    int result = StaticFluent<int>::apply(5, [](int& x) { x *= 2; });
    result = StaticFluent<int>::transform(result, [](int x) { return x + 3; });
    result = StaticFluent<int>::transform(result, [](int x) { return x * x; });

    std::cout << result << std::endl; // 169
}

void demoCrtpFluentInheritance() {
    printDemoTitle("Demo CRTP (herencia fluida)");

    PersonaFluent persona;
    persona.setNombre("Juan")
        .setEdad(30)
        .saludar()
        .imprimir();
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
}
