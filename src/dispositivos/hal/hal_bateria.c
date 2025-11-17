/**
 * @file hal_bateria.c
 * @brief Implementación del HAL de batería (versión simulada)
 * @version 1.0
 * @date 2025-11-17
 *
 * Implementación para simulador (desarrollo sin hardware).
 * Para plataformas reales (STM32, AVR, ESP32), este archivo
 * debe ser reemplazado o extendido con compilación condicional.
 *
 * @author Victor Valotto
 * @copyright Universidad Nacional de Entre Ríos (FIUNER)
 */

#include "hal_bateria.h"
#include <stdlib.h>
#include <time.h>

#ifdef TARGET_SIMULATOR

/**
 * @brief Inicializa el simulador de batería
 *
 * Inicializa el generador de números aleatorios con la hora actual
 * para simular lecturas variables de voltaje.
 */
void hal_bateria_init(void) {
    // Inicializar generador de números aleatorios
    // Usando time() como semilla para variación en cada ejecución
    srand((unsigned int)time(NULL));
}

/**
 * @brief Desinicializa el simulador de batería
 *
 * En el simulador no hay recursos que liberar.
 */
void hal_bateria_deinit(void) {
    // No hay recursos que liberar en el simulador
}

/**
 * @brief Simula la lectura de voltaje de batería
 *
 * Genera un valor aleatorio entre VOLTAJE_BATERIA_MIN_MV (3000 mV)
 * y VOLTAJE_BATERIA_MAX_MV (4200 mV) para simular una batería Li-ion.
 *
 * En una implementación real, este valor vendría de:
 * 1. ADC del microcontrolador
 * 2. Conectado a un divisor de voltaje
 * 3. Que adapta el voltaje de batería (0-4.2V) al rango del ADC (0-3.3V o 0-5V)
 *
 * Cálculo de divisor de voltaje (ejemplo para STM32 con ADC 3.3V):
 * - Batería máxima: 4.2V
 * - ADC máximo: 3.3V
 * - Factor de división: 4.2V / 3.3V ≈ 1.27
 * - Divisor sugerido: R1=10kΩ, R2=27kΩ
 * - Factor real: (10k + 27k) / 27k = 1.37
 * - Voltaje en ADC a 4.2V: 4.2V / 1.37 ≈ 3.06V ✓ (seguro)
 *
 * @return Voltaje simulado entre 3000-4200 mV
 */
Voltaje hal_bateria_leer_voltaje(void) {
    // Calcular rango de voltajes posibles
    uint16_t rango = VOLTAJE_BATERIA_MAX_MV - VOLTAJE_BATERIA_MIN_MV;

    // Generar valor aleatorio en el rango
    // rand() % rango da un valor entre 0 y (rango-1)
    uint16_t valor_aleatorio = rand() % rango;

    // Sumar el mínimo para obtener el rango final (3000-4200 mV)
    Voltaje voltaje_simulado = VOLTAJE_BATERIA_MIN_MV + valor_aleatorio;

    return voltaje_simulado;
}

#elif defined(TARGET_STM32)

// TODO: Implementación para STM32
// - Configurar ADC (ADC1 o ADC2)
// - Configurar pin de entrada (ej: PA0)
// - Leer ADC (12 bits: 0-4095)
// - Aplicar conversión con factor de divisor de voltaje
//
// Ejemplo de implementación:
//
// void hal_bateria_init(void) {
//     // Habilitar clock de ADC
//     RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
//
//     // Configurar pin como entrada analógica
//     // ...
//
//     // Configurar ADC
//     ADC1->CR2 |= ADC_CR2_ADON;
// }
//
// Voltaje hal_bateria_leer_voltaje(void) {
//     // Iniciar conversión
//     ADC1->CR2 |= ADC_CR2_SWSTART;
//
//     // Esperar fin de conversión
//     while (!(ADC1->SR & ADC_SR_EOC));
//
//     // Leer valor (0-4095)
//     uint16_t adc_value = ADC1->DR;
//
//     // Convertir a voltaje considerando divisor
//     // Factor: (R1 + R2) / R2
//     // V_bat = (adc_value * 3300 / 4095) * factor
//     uint32_t v_adc = (adc_value * 3300) / 4095;  // mV en ADC
//     uint32_t v_bat = (v_adc * 137) / 100;        // Factor 1.37
//
//     return (Voltaje)v_bat;
// }

#error "Implementación STM32 no disponible aún. Use TARGET_SIMULATOR."

#elif defined(TARGET_AVR)

// TODO: Implementación para AVR (ATmega328p, etc.)
// - Configurar ADC interno (10 bits: 0-1023)
// - Configurar AREF (AVCC o AREF externo)
// - Leer ADC del pin analógico
// - Aplicar conversión con factor de divisor
//
// Ejemplo de implementación:
//
// void hal_bateria_init(void) {
//     // Configurar ADC
//     ADMUX = (1 << REFS0);  // AVCC como referencia
//     ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1); // Prescaler 64
// }
//
// Voltaje hal_bateria_leer_voltaje(void) {
//     // Seleccionar canal (ej: ADC0)
//     ADMUX = (ADMUX & 0xF0) | 0;
//
//     // Iniciar conversión
//     ADCSRA |= (1 << ADSC);
//
//     // Esperar fin de conversión
//     while (ADCSRA & (1 << ADSC));
//
//     // Leer valor (0-1023)
//     uint16_t adc_value = ADC;
//
//     // Convertir a voltaje (AVCC = 5V)
//     uint32_t v_adc = (adc_value * 5000UL) / 1023;
//     uint32_t v_bat = (v_adc * 137) / 100;  // Factor divisor
//
//     return (Voltaje)v_bat;
// }

#error "Implementación AVR no disponible aún. Use TARGET_SIMULATOR."

#elif defined(TARGET_ESP32)

// TODO: Implementación para ESP32
// - Usar driver ADC de ESP-IDF
// - Configurar atenuación 11dB (rango 0-3.3V)
// - Leer ADC del pin GPIO
// - Aplicar calibración de fábrica
// - Aplicar factor de divisor de voltaje
//
// Ejemplo de implementación:
//
// #include "driver/adc.h"
// #include "esp_adc_cal.h"
//
// static esp_adc_cal_characteristics_t adc_chars;
//
// void hal_bateria_init(void) {
//     // Configurar ADC1 Canal 0 (GPIO36)
//     adc1_config_width(ADC_WIDTH_BIT_12);
//     adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11);
//
//     // Caracterizar ADC
//     esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11,
//                              ADC_WIDTH_BIT_12, 1100, &adc_chars);
// }
//
// Voltaje hal_bateria_leer_voltaje(void) {
//     // Leer ADC raw
//     uint32_t adc_reading = adc1_get_raw(ADC1_CHANNEL_0);
//
//     // Convertir a voltaje (mV)
//     uint32_t v_adc = esp_adc_cal_raw_to_voltage(adc_reading, &adc_chars);
//
//     // Aplicar factor de divisor (1.37)
//     uint32_t v_bat = (v_adc * 137) / 100;
//
//     return (Voltaje)v_bat;
// }

#error "Implementación ESP32 no disponible aún. Use TARGET_SIMULATOR."

#else
    #error "Plataforma no soportada. Defina TARGET_SIMULATOR, TARGET_STM32, TARGET_AVR o TARGET_ESP32"
#endif
