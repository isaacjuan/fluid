#include "UseCases.h"

#include "BusinessRules.h"
#include "Fluent.h"

#include <vector>

namespace UseCases {

const char* toString(EstadoDecision estado) {
    switch (estado) {
    case EstadoDecision::Aprobada:
        return "APROBADA";
    case EstadoDecision::RevisionManual:
        return "REVISION_MANUAL";
    case EstadoDecision::Rechazada:
        return "RECHAZADA";
    }

    return "DESCONOCIDA";
}

EvaluacionOperacion evaluarOperacion(int scoreEntrada) {
    const bool superaFiltroInicial = BusinessRules::superaUmbral(scoreEntrada);

    const int scoreNormalizado = Fluent<int>::from(scoreEntrada)
                                                                        .let(BusinessRules::duplicarValor)
                                                                        .let(BusinessRules::sumarTres)
                                                                        .let(BusinessRules::elevarAlCuadrado)
                                    .get();

    const int scoreRecuperado = Fluent<int>::from(scoreEntrada)
                                                                    .takeIf(BusinessRules::superaUmbral)
                                  .mapIfPresent(BusinessRules::duplicarValor)
                                  .orElse(BusinessRules::fallbackConservador())
                                                                    .let(BusinessRules::incrementarUno)
                                  .get();

    const int scoreFinal = (scoreNormalizado + scoreRecuperado) / 2;

    EstadoDecision estado = EstadoDecision::Rechazada;
    if (scoreFinal >= 180) {
        estado = EstadoDecision::Aprobada;
    } else if (scoreFinal >= 90) {
        estado = EstadoDecision::RevisionManual;
    }

    std::vector<std::string> razones;
    if (superaFiltroInicial) {
        razones.push_back("El score de entrada supera el umbral inicial");
    } else {
        razones.push_back("El score de entrada no supera el umbral; se uso fallback");
    }

    if (scoreFinal >= 180) {
        razones.push_back("Score final alto segun politica de aprobacion");
    } else if (scoreFinal >= 90) {
        razones.push_back("Score final intermedio; requiere validacion humana");
    } else {
        razones.push_back("Score final bajo segun politica de riesgo");
    }

    return EvaluacionOperacion{
        scoreNormalizado,
        scoreRecuperado,
        scoreFinal,
        estado,
        toString(estado),
        std::move(razones),
        "v2.0-score-combinado",
    };
}

} // namespace UseCases
