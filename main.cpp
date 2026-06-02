#include <iostream>
#include <string>

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
    Composable obj;
    obj.setValue(10).setText("Hello").add(5).append(" World!").print();

    // Output: Value: 15, Text: Hello World!

    Fluent<int> num(5);

    auto result = num.apply([](int& x) { x *= 2; })
                      .map([](int x) { return x + 3; })
                      .map([](int x) { return x * x; });

    std::cout << result.get() << std::endl; // (5*2 + 3)^2 = 169

    auto factoryResult = Fluent<int>::from(5)
                             .apply([](int& x) { x *= 2; })
                             .map([](int x) { return x + 3; })
                             .map([](int x) { return x * x; });

    std::cout << factoryResult.get() << std::endl; // 169

    auto chainedResult = Fluent<int>::from(5)
                             .map([](int x) { return x * 2; })
                             .flatMap([](int x) {
                                 return Fluent<std::string>::from("Result: " + std::to_string(x));
                             });

    std::cout << chainedResult.get() << std::endl; // Result: 10

    int staticResult = StaticFluent<int>::apply(5, [](int& x) { x *= 2; });
    staticResult = StaticFluent<int>::transform(staticResult, [](int x) { return x + 3; });
    staticResult = StaticFluent<int>::transform(staticResult, [](int x) { return x * x; });

    std::cout << staticResult << std::endl; // 169
}
