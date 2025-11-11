/**
 * @file ambiente.c
 * @brief Implementación de la entidad Ambiente
 * @version 1.0
 * @date 2025-11-10
 *
 * @author Victor Valotto
 * @copyright Universidad Nacional de Entre Ríos (FIUNER)
 */

#include "ambiente.h"
#include <stdlib.h>

/**
 * @brief Definición interna de la estructura Ambiente
 *
 * Esta definición está oculta al exterior (patrón opaque pointer)
 * para proteger la encapsulación y permitir cambios internos
 * sin afectar a los clientes del módulo.
 */
struct Ambiente {
    Temperatura temperatura_actual;  /**< Temperatura actual en °C */
};

Ambiente* ambiente_crear(void) {
    // Alocar memoria para la estructura
    Ambiente* ambiente = (Ambiente*)malloc(sizeof(Ambiente));

    if (ambiente != NULL) {
        // Inicializar con temperatura por defecto
        ambiente->temperatura_actual = TEMP_INICIAL_DEFAULT;
    }

    return ambiente;
}

void ambiente_destruir(Ambiente* ambiente) {
    // Verificación defensiva: es seguro pasar NULL a free()
    if (ambiente != NULL) {
        free(ambiente);
    }
}

void ambiente_establecer_temperatura(Ambiente* ambiente, Temperatura temperatura) {
    // Validación defensiva
    if (ambiente == NULL) {
        return;  // Silenciosamente no hace nada si es NULL
    }

    ambiente->temperatura_actual = temperatura;
}

Temperatura ambiente_obtener_temperatura(const Ambiente* ambiente) {
    // Validación defensiva
    if (ambiente == NULL) {
        return TEMP_INICIAL_DEFAULT;  // Valor por defecto como fallback
    }

    return ambiente->temperatura_actual;
}
