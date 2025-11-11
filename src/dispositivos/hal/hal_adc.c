/**
 * @file hal_adc.c
 * @brief Implementación simulada del HAL ADC
 * @version 1.0
 * @date 2025-11-10
 *
 * Implementación del HAL ADC para plataforma simulada.
 * Genera valores aleatorios que simulan lecturas de un sensor
 * de temperatura.
 *
 * @author Victor Valotto
 * @copyright Universidad Nacional de Entre Ríos (FIUNER)
 */

#include "hal_adc.h"
#include <stdlib.h>
#include <time.h>

void hal_adc_init(void) {
    // Inicializar generador de números aleatorios con seed basada en tiempo
    // Esto asegura que cada ejecución produce valores diferentes
    srand((unsigned int)time(NULL));
}

void hal_adc_deinit(void) {
    // Stub vacío: no hay recursos que liberar en versión simulada
    // En hardware real, aquí se apagaría el periférico ADC
}

uint16_t hal_adc_leer_simulado(void) {
    // Simular lectura de ADC con valores entre 150 y 300
    // Esto corresponde aproximadamente a 7-15°C después de conversión
    //
    // Fórmula: valor_base + (valor_aleatorio % rango)
    // 150 + (0..149) = 150..299
    //
    // Notas:
    // - Rango limitado para simular ambiente interior típico
    // - En producción, el rango sería 0-1023 (10 bits)

    uint16_t valor_adc = 150 + (rand() % 150);

    return valor_adc;
}

/**
 * @note Implementación futura para STM32:
 *
 * uint16_t hal_adc_leer_stm32(uint8_t canal) {
 *     // Configurar canal ADC
 *     ADC1->SQR3 = canal;
 *
 *     // Iniciar conversión
 *     ADC1->CR2 |= ADC_CR2_SWSTART;
 *
 *     // Esperar fin de conversión
 *     while (!(ADC1->SR & ADC_SR_EOC));
 *
 *     // Leer resultado
 *     return ADC1->DR;
 * }
 */

/**
 * @note Implementación futura para AVR:
 *
 * uint16_t hal_adc_leer_avr(uint8_t canal) {
 *     // Seleccionar canal
 *     ADMUX = (ADMUX & 0xF0) | (canal & 0x0F);
 *
 *     // Iniciar conversión
 *     ADCSRA |= (1 << ADSC);
 *
 *     // Esperar fin de conversión
 *     while (ADCSRA & (1 << ADSC));
 *
 *     // Leer resultado
 *     return ADC;
 * }
 */
