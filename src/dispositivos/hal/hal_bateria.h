/**
 * @file hal_bateria.h
 * @brief HAL (Hardware Abstraction Layer) para lectura de voltaje de batería
 * @version 1.0
 * @date 2025-11-17
 *
 * Proporciona una abstracción del hardware para la lectura del voltaje
 * de la batería del sistema. Soporta múltiples plataformas mediante
 * compilación condicional.
 *
 * Plataformas soportadas:
 * - Simulador (desarrollo sin hardware)
 * - STM32 (futuro)
 * - AVR (futuro)
 * - ESP32 (futuro)
 *
 * @author Victor Valotto
 * @copyright Universidad Nacional de Entre Ríos (FIUNER)
 */

#ifndef HAL_BATERIA_H
#define HAL_BATERIA_H

#include "tipos_comunes.h"

/**
 * @brief Inicializa el HAL de batería
 *
 * Configura el hardware necesario para la lectura de voltaje de batería.
 *
 * Implementación por plataforma:
 * - Simulador: Inicializa generador de números aleatorios
 * - STM32: Configura ADC y pines de lectura de batería
 * - AVR: Configura ADC interno con AREF
 * - ESP32: Configura ADC con atenuación apropiada
 *
 * @note Debe llamarse una vez al inicio del sistema, antes de
 *       cualquier lectura de batería.
 */
void hal_bateria_init(void);

/**
 * @brief Desinicializa el HAL de batería
 *
 * Libera recursos y apaga el hardware de lectura de batería
 * para ahorrar energía.
 *
 * @note Después de llamar a esta función, se debe llamar a
 *       hal_bateria_init() nuevamente antes de realizar lecturas.
 */
void hal_bateria_deinit(void);

/**
 * @brief Lee el voltaje actual de la batería
 *
 * Realiza una lectura del voltaje de la batería y retorna el valor
 * en milivolts (mV).
 *
 * Implementación por plataforma:
 *
 * SIMULADOR:
 * - Genera valor aleatorio entre 3000-4200 mV
 * - Simula variación natural de voltaje
 * - No requiere hardware real
 *
 * STM32 (futuro):
 * - Lee ADC conectado a divisor de voltaje
 * - Divisor R1/R2 para adaptar 4.2V → 3.3V
 * - Conversión: V_bat = ADC * (3.3V / 4095) * (R1+R2) / R2
 *
 * AVR (futuro):
 * - Lee ADC interno (10 bits)
 * - AREF = 5V o AVCC
 * - Divisor de voltaje externo
 *
 * ESP32 (futuro):
 * - ADC con atenuación 11dB (rango 0-3.3V)
 * - Divisor de voltaje para 4.2V → 3.3V
 * - Calibración de fábrica
 *
 * Rango de voltaje típico de batería Li-ion:
 * - Completamente cargada: 4200 mV (4.2V)
 * - Nominal: 3700 mV (3.7V)
 * - Descargada: 3000 mV (3.0V)
 * - Protección bajo voltaje: 3000 mV (no descargar más)
 *
 * @return Voltaje de la batería en milivolts (mV)
 *
 * @warning En hardware real, asegurarse de que el voltaje de batería
 *          esté adaptado mediante divisor de voltaje antes de conectar
 *          al ADC del microcontrolador.
 *
 * Ejemplo de uso:
 * @code
 * hal_bateria_init();
 * Voltaje v = hal_bateria_leer_voltaje();
 * printf("Voltaje: %u mV\n", v);
 * hal_bateria_deinit();
 * @endcode
 */
Voltaje hal_bateria_leer_voltaje(void);

#endif // HAL_BATERIA_H
