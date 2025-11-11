/**
 * @file sensor_temperatura.h
 * @brief Proxy del sensor de temperatura
 * @version 1.0
 * @date 2025-11-10
 *
 * Componente de infraestructura que actúa como proxy del sensor
 * físico de temperatura. Se encarga de leer el ADC y convertir
 * el valor a temperatura en grados Celsius.
 *
 * @author Victor Valotto
 * @copyright Universidad Nacional de Entre Ríos (FIUNER)
 */

#ifndef SENSOR_TEMPERATURA_H
#define SENSOR_TEMPERATURA_H

#include "tipos_comunes.h"

/**
 * @brief Inicializa el sensor de temperatura
 *
 * Realiza la inicialización necesaria del sensor y del HAL subyacente.
 * Debe llamarse antes de cualquier lectura.
 *
 * @code
 * sensor_temperatura_init();
 * Temperatura temp = sensor_temperatura_leer();
 * sensor_temperatura_deinit();
 * @endcode
 */
void sensor_temperatura_init(void);

/**
 * @brief Desinicializa el sensor de temperatura
 *
 * Libera recursos asociados al sensor y desinicializa el HAL.
 */
void sensor_temperatura_deinit(void);

/**
 * @brief Lee la temperatura actual del sensor
 *
 * Realiza una lectura del ADC y convierte el valor a temperatura
 * en grados Celsius usando una fórmula de conversión simplificada.
 *
 * @return Temperatura leída en grados Celsius
 *
 * @note Versión simplificada: retorna directamente sin código de error
 * @note Conversión: ADC 0-1023 → 0-50°C aproximadamente
 * @note Factor de conversión: 0.0488 (50°C / 1023)
 *
 * @see hal_adc_leer_simulado()
 */
Temperatura sensor_temperatura_leer(void);

#endif // SENSOR_TEMPERATURA_H
