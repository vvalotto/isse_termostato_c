/**
 * @file gestor_termostato.c
 * @brief Implementación del gestor del termostato
 * @version 1.0
 * @date 2025-11-10
 *
 * Implementa el gestor del termostato que orquesta la lectura
 * del sensor de temperatura y actualiza el modelo de dominio.
 *
 * @author Victor Valotto
 * @copyright Universidad Nacional de Entre Ríos (FIUNER)
 */

#include "gestor_termostato.h"
#include "sensor_temperatura.h"
#include <stdlib.h>

/**
 * @brief Estructura del gestor de termostato
 *
 * Almacena la referencia al ambiente. El gestor NO es propietario
 * del ambiente; solo mantiene una referencia para actualizarlo.
 */
struct GestorTermostato {
    /**
     * @brief Referencia a la entidad Ambiente
     *
     * El gestor no es propietario; solo usa esta referencia
     * para consultar y actualizar la temperatura.
     */
    Ambiente* ambiente;
};

GestorTermostato* gestor_termostato_crear(Ambiente* ambiente) {
    // Validación defensiva: ambiente no debe ser NULL
    if (ambiente == NULL) {
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

    return gestor;
}

void gestor_termostato_destruir(GestorTermostato* gestor) {
    // Seguro con NULL
    if (gestor == NULL) {
        return;
    }

    // Liberar memoria del gestor
    // NOTA: NO destruimos el ambiente porque no somos propietarios
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

/**
 * @note Flujo de datos completo en gestor_termostato_actualizar_temperatura():
 *
 * 1. gestor_termostato_actualizar_temperatura()
 *    └─> sensor_temperatura_leer()
 *        └─> hal_adc_leer_simulado()
 *            └─> [Hardware físico o simulación]
 *    └─> ambiente_establecer_temperatura()
 *        └─> [Actualiza entidad de dominio]
 *
 * Este flujo atraviesa las capas de la arquitectura:
 * - Aplicación (gestor_termostato)
 * - Dominio (ambiente)
 * - Infraestructura (sensor_temperatura)
 * - Dispositivos (hal_adc)
 *
 * Cumpliendo con la arquitectura por capas y separación de responsabilidades.
 */
