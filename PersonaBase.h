#pragma once

#include <string>

// CRTP (Curiously Recurring Template Pattern):
// la clase concreta se pasa a la base como parametro de plantilla.
// Ejemplo de uso: class PersonaFluent : public PersonaBase<PersonaFluent> {};
template <typename Derived>
class PersonaBase {
protected:
    std::string nombre;
    int edad = 0;

public:
    // Retorna Derived& para mantener el tipo concreto en el chaining.
    Derived& setNombre(const std::string& n) {
        nombre = n;
        return self();
    }

    // Misma idea: la cadena sigue en la clase hija, no en la base.
    Derived& setEdad(int e) {
        edad = e;
        return self();
    }

    const std::string& getNombre() const {
        return nombre;
    }

    int getEdad() const {
        return edad;
    }

private:
    // Version no const: se usa desde metodos no const (setNombre, setEdad).
    Derived& self() {
        return static_cast<Derived&>(*this);
    }

    // Version const: se usa desde metodos const.
    const Derived& self() const {
        return static_cast<const Derived&>(*this);
    }
};
