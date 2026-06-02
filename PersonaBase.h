#pragma once

#include <iostream>
#include <string>

template <typename Derived>
class PersonaBase {
protected:
    std::string nombre;
    int edad = 0;

public:
    Derived& setNombre(const std::string& n) {
        nombre = n;
        return self();
    }

    Derived& setEdad(int e) {
        edad = e;
        return self();
    }

    void imprimir() const {
        std::cout << "Nombre: " << nombre << ", Edad: " << edad << std::endl;
    }

private:
    Derived& self() {
        return static_cast<Derived&>(*this);
    }

    const Derived& self() const {
        return static_cast<const Derived&>(*this);
    }
};
