#include <iostream>
#include <optional>
#include <string>
#include <type_traits>
#include <utility>

// Declaracion adelantada para poder detectar Fluent<T> en traits.
template <typename T>
class Fluent;

// Trait base: por defecto un tipo no es Fluent.
template <typename T>
struct is_fluent : std::false_type {};

// Especializacion: Fluent<T> si es Fluent.
template <typename T>
struct is_fluent<Fluent<T>> : std::true_type {};

// Trait base: por defecto un tipo no es optional.
template <typename T>
struct is_optional : std::false_type {};

// Especializacion: std::optional<T> si es optional.
template <typename T>
struct is_optional<std::optional<T>> : std::true_type {};

class Composable {
    int value;
    std::string text;

public:
    Composable() : value(0), text("") {}

    // Métodos que devuelven *this para permitir chaining
    Composable& setValue(int v) {
        value = v;
        return *this;
    }

    Composable& setText(const std::string& t) {
        text = t;
        return *this;
    }

    Composable& add(int n) {
        value += n;
        return *this;
    }

    Composable& append(const std::string& s) {
        text += s;
        return *this;
    }

    void print() const {
        std::cout << "Value: " << value << ", Text: " << text << std::endl;
    }
};

template <typename T>
class Fluent {
    T value;

public:
    Fluent(T v) : value(v) {}

    static Fluent from(T v) {
        return Fluent(v);
    }

    // map: transforma el valor y devuelve un nuevo Fluent
    template <typename F>
    auto map(F&& func) {
        return Fluent(func(value));
    }

    // flatMap: encadena funciones que ya retornan Fluent<...>
    template <typename F>
    auto flatMap(F&& func) {
        return func(value);
    }

    // filter: convierte T en optional<T> para representar "valor presente/ausente".
    template <typename P>
    auto filter(P&& pred) {
        if (pred(value)) {
            return Fluent<std::optional<T>>::from(std::optional<T>{value});
        }
        return Fluent<std::optional<T>>::from(std::nullopt);
    }

    // tap: permite observar el valor sin transformarlo
    template <typename F>
    Fluent& tap(F&& func) {
        func(value);
        return *this;
    }

    // then: alias semantico de map para lecturas estilo pipeline.
    template <typename F>
    auto then(F&& func) {
        return map(std::forward<F>(func));
    }

    // flatten: desenvuelve Fluent<Fluent<U>> en Fluent<U>.
    // Se habilita solo cuando T ya es un Fluent<...>.
    template <typename U = T>
    auto flatten() -> std::enable_if_t<is_fluent<U>::value, U> {
        return value;
    }

    // mapIfPresent: opera sobre optional solo cuando hay dato.
    // Si no hay dato, mantiene nullopt y evita evaluaciones innecesarias.
    template <typename F, typename U = T>
    auto mapIfPresent(F&& func) -> std::enable_if_t<is_optional<U>::value,
        Fluent<std::optional<decltype(func(*std::declval<U>()))>>> {
        using OutputType = decltype(func(*std::declval<U>()));
        if (value.has_value()) {
            return Fluent<std::optional<OutputType>>::from(
                std::optional<OutputType>{func(*value)});
        }
        return Fluent<std::optional<OutputType>>::from(std::nullopt);
    }

    // orElse: vuelve de optional<T> a T usando un fallback cuando no hay valor.
    template <typename U = T>
    auto orElse(typename U::value_type defaultValue)
        -> std::enable_if_t<is_optional<U>::value, Fluent<typename U::value_type>> {
        using ValueType = typename U::value_type;
        if (value.has_value()) {
            return Fluent<ValueType>::from(*value);
        }
        return Fluent<ValueType>::from(std::move(defaultValue));
    }

    // Método para operaciones que modifican el estado
    template <typename F>
    Fluent& apply(F&& func) {
        func(value);
        return *this;
    }

    T get() const { return value; }
};

template <typename T>
class StaticFluent {
public:
    template <typename F>
    static auto transform(T value, F&& func) {
        return func(value);
    }

    template <typename F>
    static T apply(T value, F&& func) {
        func(value);
        return value;
    }
};

int main() {
    // 1) Chaining clasico sobre un objeto mutable.
    Composable obj;
    obj.setValue(10).setText("Hello").add(5).append(" World!").print();

    // Output: Value: 15, Text: Hello World!

    Fluent<int> num(5);

    // 2) apply muta el estado interno; map transforma y crea un nuevo Fluent.
    auto result = num.apply([](int& x) { x *= 2; })
                      .map([](int x) { return x + 3; })
                      .map([](int x) { return x * x; });

    std::cout << result.get() << std::endl; // (5*2 + 3)^2 = 169

    // 3) from permite iniciar la cadena sin construir una variable intermedia.
    auto factoryResult = Fluent<int>::from(5)
                             .apply([](int& x) { x *= 2; })
                             .map([](int x) { return x + 3; })
                             .map([](int x) { return x * x; });

    std::cout << factoryResult.get() << std::endl; // 169

    // 4) flatMap cambia el contexto Fluent<int> -> Fluent<string> sin anidar.
    auto chainedResult = Fluent<int>::from(5)
                             .map([](int x) { return x * 2; })
                             .flatMap([](int x) {
                                 return Fluent<std::string>::from("Result: " + std::to_string(x));
                             });

    std::cout << chainedResult.get() << std::endl; // Result: 10

    // 5) tap observa (log) y then encadena una transformacion.
    auto tapThenResult = Fluent<int>::from(10)
                             .tap([](const int& x) { std::cout << "tap: " << x << std::endl; })
                             .then([](int x) { return x + 5; });

    std::cout << tapThenResult.get() << std::endl; // 15

    // 6) flatten evita tener Fluent<Fluent<int>> durante el pipeline.
    auto nested = Fluent<Fluent<int>>::from(Fluent<int>::from(42));
    auto flattened = nested.flatten().map([](int x) { return x + 1; });

    std::cout << flattened.get() << std::endl; // 43

    // 7) filter + mapIfPresent: si no cumple, la cadena continua con nullopt.
    auto filteredOk = Fluent<int>::from(12)
                          .filter([](int x) { return x > 10; })
                          .mapIfPresent([](int x) { return x * 2; });

    auto filteredNo = Fluent<int>::from(7)
                          .filter([](int x) { return x > 10; })
                          .mapIfPresent([](int x) { return x * 2; });

    // 8) orElse recupera un valor concreto para seguir como Fluent<int>.
    auto recoveredOk = filteredOk.orElse(0).map([](int x) { return x + 1; });
    auto recoveredNo = filteredNo.orElse(99).map([](int x) { return x + 1; });

    std::cout << filteredOk.get().value_or(-1) << std::endl; // 24
    std::cout << filteredNo.get().value_or(-1) << std::endl; // -1
    std::cout << recoveredOk.get() << std::endl; // 25
    std::cout << recoveredNo.get() << std::endl; // 100

    // 9) Variante estatica: util cuando prefieres estilo funcional sin estado interno.
    int staticResult = StaticFluent<int>::apply(5, [](int& x) { x *= 2; });
    staticResult = StaticFluent<int>::transform(staticResult, [](int x) { return x + 3; });
    staticResult = StaticFluent<int>::transform(staticResult, [](int x) { return x * x; });

    std::cout << staticResult << std::endl; // 169
}
