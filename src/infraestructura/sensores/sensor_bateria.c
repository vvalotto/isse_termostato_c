/**
 * @file sensor_bateria.c
 * @brief Implementación del proxy del sensor de batería
 * @version 1.0
 * @date 2025-11-17
 *
 * Implementa la lógica de conversión de voltaje a porcentaje
 * y delega la lectura física al HAL.
 *
 * @author Victor Valotto
 * @copyright Universidad Nacional de Entre Ríos (FIUNER)
 */

#include "sensor_bateria.h"
#include "hal_bateria.h"

void sensor_bateria_init(void) {
    // Delegar inicialización al HAL
    hal_bateria_init();
}

void sensor_bateria_deinit(void) {
    // Delegar desinicialización al HAL
    hal_bateria_deinit();
}

NivelCarga sensor_bateria_leer(void) {
    // 1. Leer voltaje desde HAL (en milivolts)
    Voltaje voltaje_mv = hal_bateria_leer_voltaje();

    // 2. Validar y limitar al rango válido
    if (voltaje_mv < VOLTAJE_BATERIA_MIN_MV) {
        voltaje_mv = VOLTAJE_BATERIA_MIN_MV;
    }
    if (voltaje_mv > VOLTAJE_BATERIA_MAX_MV) {
        voltaje_mv = VOLTAJE_BATERIA_MAX_MV;
    }

    // 3. Convertir a porcentaje usando fórmula lineal
    //
    // Fórmula: Porcentaje = (Voltaje - V_min) * 100 / (V_max - V_min)
    //
    // Donde:
    // - V_min = 3000 mV (batería descargada)
    // - V_max = 4200 mV (batería cargada)
    // - Rango = 4200 - 3000 = 1200 mV
    //
    // Ejemplo:
    // - 3000 mV: (3000 - 3000) * 100 / 1200 = 0%
    // - 3600 mV: (3600 - 3000) * 100 / 1200 = 50%
    // - 4200 mV: (4200 - 3000) * 100 / 1200 = 100%

    uint16_t rango = VOLTAJE_BATERIA_MAX_MV - VOLTAJE_BATERIA_MIN_MV;
    uint16_t delta = voltaje_mv - VOLTAJE_BATERIA_MIN_MV;

    // Conversión con aritmética entera para evitar punto flotante
    // Multiplicar primero por 100 para mantener precisión
    NivelCarga porcentaje = (NivelCarga)((delta * 100) / rango);

    // 4. Retornar nivel de carga (0-100%)
    return porcentaje;
}
