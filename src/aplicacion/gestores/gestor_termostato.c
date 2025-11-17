/**
 * @file gestor_termostato.c
 * @brief Implementación del gestor del termostato
 * @version 1.1
 * @date 2025-11-17
 *
 * Implementa el gestor del termostato que orquesta:
 * - Lectura del sensor de temperatura y actualización de Ambiente
 * - Lectura del sensor de batería y actualización de Bateria
 *
 * @author Victor Valotto
 * @copyright Universidad Nacional de Entre Ríos (FIUNER)
 */

#include "gestor_termostato.h"
#include "sensor_temperatura.h"
#include "sensor_bateria.h"
#include <stdlib.h>

/**
 * @brief Estructura del gestor de termostato
 *
 * Almacena referencias a las entidades de dominio. El gestor NO es
 * propietario de estas entidades; solo mantiene referencias para
 * actualizarlas.
 */
struct GestorTermostato {
    /**
     * @brief Referencia a la entidad Ambiente
     *
     * El gestor no es propietario; solo usa esta referencia
     * para consultar y actualizar la temperatura.
     */
    Ambiente* ambiente;

    /**
     * @brief Referencia a la entidad Bateria (ITE-57)
     *
     * El gestor no es propietario; solo usa esta referencia
     * para consultar y actualizar el nivel de carga.
     */
    Bateria* bateria;
};

GestorTermostato* gestor_termostato_crear(Ambiente* ambiente, Bateria* bateria) {
    // Validación defensiva: ambiente y bateria no deben ser NULL
    if (ambiente == NULL || bateria == NULL) {
        return NULL;
    }

    // Asignar memoria para el gestor
    GestorTermostato* gestor = (GestorTermostato*)malloc(sizeof(GestorTermostato));
    if (gestor == NULL) {
        // Fallo de asignación de memoria
        return NULL;
    }

    // Inicializar campos
    gestor->ambiente = ambiente;
    gestor->bateria = bateria;

    return gestor;
}

void gestor_termostato_destruir(GestorTermostato* gestor) {
    // Seguro con NULL
    if (gestor == NULL) {
        return;
    }

    // Liberar memoria del gestor
    // NOTA: NO destruimos ambiente ni bateria porque no somos propietarios
    free(gestor);
}

void gestor_termostato_actualizar_temperatura(GestorTermostato* gestor) {
    // Validación defensiva
    if (gestor == NULL || gestor->ambiente == NULL) {
        return;
    }

    // Paso 1: Leer temperatura del sensor físico
    // El sensor_temperatura internamente llama al HAL ADC
    // y realiza la conversión a grados Celsius
    Temperatura temperatura_leida = sensor_temperatura_leer();

    // Paso 2: Actualizar la entidad Ambiente con el valor leído
    // Esto actualiza el modelo de dominio con la temperatura actual
    ambiente_establecer_temperatura(gestor->ambiente, temperatura_leida);
}

Temperatura gestor_termostato_obtener_temperatura_actual(const GestorTermostato* gestor) {
    // Validación defensiva
    if (gestor == NULL || gestor->ambiente == NULL) {
        // En caso de error, retornar temperatura por defecto
        return TEMP_INICIAL_DEFAULT;
    }

    // Consultar temperatura del modelo de dominio
    return ambiente_obtener_temperatura(gestor->ambiente);
}

// ============================================
// OPERACIONES DE BATERÍA (ITE-57)
// ============================================

void gestor_termostato_actualizar_bateria(GestorTermostato* gestor) {
    // Validación defensiva
    if (gestor == NULL || gestor->bateria == NULL) {
        return;
    }

    // Paso 1: Leer nivel de carga del sensor de batería
    // El sensor_bateria internamente llama al HAL batería
    // y realiza la conversión de voltaje a porcentaje
    NivelCarga nivel_leido = sensor_bateria_leer();

    // Paso 2: Actualizar la entidad Bateria con el valor leído
    // Esto actualiza el modelo de dominio con el nivel actual
    bateria_establecer_nivel(gestor->bateria, nivel_leido);
}

NivelCarga gestor_termostato_obtener_nivel_bateria(const GestorTermostato* gestor) {
    // Validación defensiva
    if (gestor == NULL || gestor->bateria == NULL) {
        // En caso de error, retornar 0% (batería vacía)
        return 0;
    }

    // Consultar nivel del modelo de dominio
    return bateria_obtener_nivel(gestor->bateria);
}

EstadoBateria gestor_termostato_obtener_estado_bateria(const GestorTermostato* gestor) {
    // Validación defensiva
    if (gestor == NULL || gestor->bateria == NULL) {
        // En caso de error, retornar estado crítico
        return BATERIA_ESTADO_CRITICO;
    }

    // Consultar estado del modelo de dominio
    return bateria_obtener_estado(gestor->bateria);
}

bool gestor_termostato_bateria_baja(const GestorTermostato* gestor) {
    // Validación defensiva
    if (gestor == NULL || gestor->bateria == NULL) {
        // En caso de error, considerar batería baja
        return true;
    }

    // Consultar si la batería está baja
    return bateria_esta_baja(gestor->bateria);
}

/**
 * @note Flujos de datos completos:
 *
 * TEMPERATURA (HU-014):
 * gestor_termostato_actualizar_temperatura()
 *    └─> sensor_temperatura_leer()
 *        └─> hal_adc_leer_simulado()
 *            └─> [Hardware físico o simulación]
 *    └─> ambiente_establecer_temperatura()
 *        └─> [Actualiza entidad de dominio]
 *
 * BATERÍA (HU-007):
 * gestor_termostato_actualizar_bateria()
 *    └─> sensor_bateria_leer()
 *        └─> hal_bateria_leer_voltaje()
 *            └─> [Hardware físico o simulación]
 *        └─> [Conversión voltaje → porcentaje]
 *    └─> bateria_establecer_nivel()
 *        └─> [Actualiza entidad de dominio]
 *        └─> [Actualiza estado automáticamente]
 *
 * Ambos flujos atraviesan las capas de la arquitectura:
 * - Aplicación (gestor_termostato)
 * - Dominio (ambiente, bateria)
 * - Infraestructura (sensor_temperatura, sensor_bateria)
 * - Dispositivos (hal_adc, hal_bateria)
 *
 * Cumpliendo con la arquitectura por capas y separación de responsabilidades.
 */
