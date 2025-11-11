/**
 * @file hal_adc.h
 * @brief Hardware Abstraction Layer para ADC
 * @version 1.0
 * @date 2025-11-10
 *
 * Abstracción del ADC (Analog-to-Digital Converter) del microcontrolador.
 * Esta versión implementa un simulador para desarrollo sin hardware.
 *
 * @note Versiones futuras: hal_adc_stm32.c, hal_adc_avr.c, hal_adc_esp32.c
 *
 * @author Victor Valotto
 * @copyright Universidad Nacional de Entre Ríos (FIUNER)
 */

#ifndef HAL_ADC_H
#define HAL_ADC_H

#include <stdint.h>

/**
 * @brief Inicializa el HAL del ADC
 *
 * Configura el ADC para lectura. En versión simulada, inicializa
 * el generador de números aleatorios.
 *
 * @note En hardware real: configuraría registros, clock, canales, etc.
 *
 * @code
 * hal_adc_init();
 * uint16_t valor = hal_adc_leer_simulado();
 * hal_adc_deinit();
 * @endcode
 */
void hal_adc_init(void);

/**
 * @brief Desinicializa el HAL del ADC
 *
 * Libera recursos asociados al ADC si los hay.
 * En versión simulada es un stub vacío.
 */
void hal_adc_deinit(void);

/**
 * @brief Lee un valor del ADC simulado
 *
 * Retorna un valor simulado que representa una lectura de ADC.
 * Los valores están en el rango [150-300] para simular ~15-30°C
 * después de la conversión.
 *
 * @return Valor ADC de 10 bits (0-1023), típicamente 150-300
 *
 * @note En hardware real: leería un canal específico del ADC físico
 * @note El rango simulado es deliberadamente limitado para testing
 * @see FACTOR_CONVERSION_ADC_A_CELSIUS en sensor_temperatura.c
 */
uint16_t hal_adc_leer_simulado(void);

/**
 * @brief Constantes del ADC
 */
#define HAL_ADC_RESOLUCION_BITS 10      /**< Resolución: 10 bits (0-1023) */
#define HAL_ADC_MAX_VALUE ((1 << HAL_ADC_RESOLUCION_BITS) - 1)  /**< Valor máximo: 1023 */

#endif // HAL_ADC_H
