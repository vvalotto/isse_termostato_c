/**
 * @file sensor_temperatura.c
 * @brief Implementación del proxy de sensor de temperatura
 * @version 1.0
 * @date 2025-11-10
 *
 * Implementa el proxy del sensor de temperatura que delega
 * la lectura física al HAL ADC y realiza la conversión a
 * grados Celsius.
 *
 * @author Victor Valotto
 * @copyright Universidad Nacional de Entre Ríos (FIUNER)
 */

#include "sensor_temperatura.h"
#include "hal_adc.h"

/**
 * @brief Factor de conversión ADC a temperatura
 *
 * Cálculo: 50°C / 1023 (valor máximo ADC) ≈ 0.0488
 *
 * Este factor mapea linealmente:
 * - ADC 0 → 0°C
 * - ADC 1023 → 50°C
 * - ADC 200 → ~9.8°C
 * - ADC 400 → ~19.5°C
 *
 * Ejemplos con rango simulado (150-300):
 * - ADC 150 → ~7.3°C
 * - ADC 225 → ~11.0°C
 * - ADC 300 → ~14.6°C
 *
 * @note Este factor es una simplificación didáctica.
 * En producción dependería del sensor específico (LM35, DHT22, etc.)
 */
#define FACTOR_CONVERSION_ADC_A_CELSIUS 0.0488f

void sensor_temperatura_init(void) {
    // Inicializar HAL subyacente
    hal_adc_init();
}

void sensor_temperatura_deinit(void) {
    // Desinicializar HAL
    hal_adc_deinit();
}

Temperatura sensor_temperatura_leer(void) {
    // 1. Leer valor del ADC (0-1023, típicamente 150-300 en simulador)
    uint16_t valor_adc = hal_adc_leer_simulado();

    // 2. Convertir a temperatura en °C
    // Fórmula simplificada: temp = adc_value * factor
    Temperatura temperatura = (Temperatura)valor_adc * FACTOR_CONVERSION_ADC_A_CELSIUS;

    return temperatura;
}

/**
 * @note Conversión para sensor LM35 real:
 *
 * El LM35 entrega 10mV por °C:
 * - 0°C → 0V (0mV)
 * - 10°C → 100mV (0.1V)
 * - 25°C → 250mV (0.25V)
 * - 100°C → 1000mV (1.0V)
 *
 * Con ADC de 10 bits y referencia de 5V:
 * - Resolución: 5V / 1024 = ~4.88mV por bit
 * - Para LM35: °C = (adc_value * 5.0 / 1023.0) / 0.01
 * - Simplificado: °C = adc_value * 0.4887
 *
 * La constante actual (0.0488) es 1/10 de la real,
 * ajustada para el rango simulado del HAL.
 */

/**
 * @note Para DHT22:
 *
 * El DHT22 es un sensor digital, no analógico.
 * - Comunicación: protocolo propietario de 1-wire
 * - Resolución: 0.1°C
 * - Rango: -40 a 80°C
 * - No requiere conversión ADC
 * - Requeriría un driver específico diferente
 */
