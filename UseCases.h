#pragma once

#include <string>
#include <vector>

namespace UseCases {

enum class EstadoDecision {
    Aprobada,
    RevisionManual,
    Rechazada,
};

const char* toString(EstadoDecision estado);

struct EvaluacionOperacion {
    int scoreNormalizado;
    int scoreRecuperado;
    int scoreFinal;
    EstadoDecision estado;
    std::string decision;
    std::vector<std::string> razones;
    std::string versionPolitica;

    bool aprobada() const {
        return estado == EstadoDecision::Aprobada;
    }
};

EvaluacionOperacion evaluarOperacion(int scoreEntrada);

} // namespace UseCases
