/**
 * @file bateria.c
 * @brief Implementación de la entidad Bateria
 * @version 1.0
 * @date 2025-11-17
 *
 * Implementa la lógica de negocio de la batería del sistema.
 *
 * @author Victor Valotto
 * @copyright Universidad Nacional de Entre Ríos (FIUNER)
 */

#include "bateria.h"
#include <stdlib.h>

/**
 * @brief Estructura interna de la entidad Bateria
 *
 * Almacena el estado completo de la batería.
 * Esta estructura es opaca para los clientes.
 */
struct Bateria {
    NivelCarga nivel_carga;    /**< Nivel de carga actual (0-100%) */
    EstadoBateria estado;       /**< Estado operacional actual */
};

/**
 * @brief Actualiza el estado de la batería según el nivel de carga
 *
 * Función auxiliar privada que determina el estado basándose en
 * los umbrales definidos en tipos_comunes.h
 *
 * @param bateria Puntero a la batería (no debe ser NULL)
 */
static void actualizar_estado(Bateria* bateria) {
    if (bateria->nivel_carga < UMBRAL_BATERIA_CRITICA) {
        bateria->estado = BATERIA_ESTADO_CRITICO;
    } else if (bateria->nivel_carga < UMBRAL_BATERIA_BAJA) {
        bateria->estado = BATERIA_ESTADO_BAJO;
    } else {
        bateria->estado = BATERIA_ESTADO_NORMAL;
    }
}

Bateria* bateria_crear(void) {
    Bateria* bateria = (Bateria*)malloc(sizeof(Bateria));

    if (bateria != NULL) {
        // Inicializar con batería completamente cargada
        bateria->nivel_carga = NIVEL_CARGA_INICIAL;
        bateria->estado = BATERIA_ESTADO_NORMAL;
    }

    return bateria;
}

void bateria_destruir(Bateria* bateria) {
    // Es seguro llamar free() con NULL
    free(bateria);
}

void bateria_establecer_nivel(Bateria* bateria, NivelCarga nivel) {
    // Validación defensiva: verificar que bateria no sea NULL
    if (bateria == NULL) {
        return;
    }

    // Actualizar nivel de carga
    bateria->nivel_carga = nivel;

    // Recalcular estado automáticamente
    actualizar_estado(bateria);
}

NivelCarga bateria_obtener_nivel(const Bateria* bateria) {
    // Retornar 0 si bateria es NULL (batería vacía)
    if (bateria == NULL) {
        return 0;
    }

    return bateria->nivel_carga;
}

EstadoBateria bateria_obtener_estado(const Bateria* bateria) {
    // Retornar estado más crítico si bateria es NULL
    if (bateria == NULL) {
        return BATERIA_ESTADO_CRITICO;
    }

    return bateria->estado;
}

bool bateria_esta_baja(const Bateria* bateria) {
    // Si bateria es NULL, considerarla como baja
    if (bateria == NULL) {
        return true;
    }

    return bateria->nivel_carga < UMBRAL_BATERIA_BAJA;
}

bool bateria_esta_critica(const Bateria* bateria) {
    // Si bateria es NULL, considerarla como crítica
    if (bateria == NULL) {
        return true;
    }

    return bateria->nivel_carga < UMBRAL_BATERIA_CRITICA;
}
