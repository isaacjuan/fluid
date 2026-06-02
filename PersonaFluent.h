#pragma once

#include "PersonaBase.h"

class PersonaFluent : public PersonaBase<PersonaFluent> {
public:
    PersonaFluent& saludar();
};
