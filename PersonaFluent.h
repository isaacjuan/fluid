#pragma once

#include "PersonaBase.h"

#include <string>
#include <utility>

// La clase se auto-referencia en la base para conservar el tipo en la API fluida.
class PersonaFluent : public PersonaBase<PersonaFluent> {
public:
    std::string saludo() const;
};

// Tambien funciona cuando la clase derivada es template.
// Cada instanciacion (PersonaFluentT<A>, PersonaFluentT<B>) es un tipo distinto.
template <typename Tag>
class PersonaFluentT : public PersonaBase<PersonaFluentT<Tag>> {
    std::string etiqueta;

public:
    PersonaFluentT& setEtiqueta(std::string e) {
        etiqueta = std::move(e);
        return *this;
    }

    std::string saludoConEtiqueta() const {
        return "Hola, soy " + this->nombre + " [" + etiqueta + "]";
    }

    const std::string& getEtiqueta() const {
        return etiqueta;
    }
};
