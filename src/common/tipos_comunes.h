/**
 * @file tipos_comunes.h
 * @brief Tipos y constantes comunes del sistema termostato
 * @version 1.0
 * @date 2025-11-10
 *
 * Define tipos base y constantes utilizadas en todo el sistema.
 * Este módulo no tiene dependencias externas del proyecto.
 *
 * @author Victor Valotto
 * @copyright Universidad Nacional de Entre Ríos (FIUNER)
 */

#ifndef TIPOS_COMUNES_H
#define TIPOS_COMUNES_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Tipo para representar temperatura en grados Celsius
 *
 * Se usa float para permitir decimales y compatibilidad con
 * operaciones matemáticas sin necesidad de conversión.
 *
 * Ejemplo de uso:
 * @code
 * Temperatura temp_actual = 22.5f;
 * Temperatura temp_deseada = 20.0f;
 * @endcode
 */
typedef float Temperatura;

/**
 * @brief Temperatura inicial por defecto del ambiente
 *
 * Valor usado al crear una nueva instancia de Ambiente si no
 * se especifica otra temperatura inicial. Representa una
 * temperatura ambiente típica de interior.
 */
#define TEMP_INICIAL_DEFAULT 20.0f

// ============================================
// TIPOS PARA BATERÍA (ITE-57)
// ============================================

/**
 * @brief Tipo para representar el nivel de carga de la batería
 *
 * Representa el porcentaje de carga de la batería en el rango 0-100.
 * Se usa uint8_t para optimizar memoria (suficiente para 0-255).
 *
 * Ejemplo de uso:
 * @code
 * NivelCarga nivel = 75;  // 75%
 * if (nivel < UMBRAL_BATERIA_BAJA) {
 *     // Batería baja
 * }
 * @endcode
 */
typedef uint8_t NivelCarga;

/**
 * @brief Tipo para representar voltaje en milivolts (mV)
 *
 * Se usa uint16_t para representar voltajes hasta 65.535V con
 * precisión de 1mV. Para baterías Li-ion típicas (3.0-4.2V),
 * el rango es más que suficiente.
 *
 * Ejemplo de uso:
 * @code
 * Voltaje voltaje_actual = 3700;  // 3.7V = 3700mV
 * @endcode
 */
typedef uint16_t Voltaje;

/**
 * @brief Estados posibles de la batería según su nivel de carga
 *
 * Define los diferentes estados operacionales de la batería
 * basados en umbrales de carga. Permite al sistema tomar
 * decisiones según el estado de energía disponible.
 */
typedef enum {
    BATERIA_ESTADO_NORMAL,    /**< Nivel de carga > 20% - Operación normal */
    BATERIA_ESTADO_BAJO,      /**< Nivel de carga 10-20% - Advertencia */
    BATERIA_ESTADO_CRITICO    /**< Nivel de carga < 10% - Crítico */
} EstadoBateria;

/**
 * @brief Voltaje máximo de batería Li-ion completamente cargada (mV)
 *
 * Voltaje nominal de una batería Li-ion 3.7V completamente cargada.
 * No se debe superar este voltaje durante la carga para evitar daños.
 */
#define VOLTAJE_BATERIA_MAX_MV   4200

/**
 * @brief Voltaje mínimo de batería Li-ion descargada (mV)
 *
 * Voltaje mínimo seguro de descarga para batería Li-ion.
 * Por debajo de este valor, la batería puede sufrir daños permanentes.
 * El circuito de protección debe evitar descargas por debajo de este límite.
 */
#define VOLTAJE_BATERIA_MIN_MV   3000

/**
 * @brief Voltaje nominal de batería Li-ion (mV)
 *
 * Voltaje de referencia típico de una batería Li-ion estándar.
 * Corresponde aproximadamente al 50% de carga.
 */
#define VOLTAJE_BATERIA_NOM_MV   3700

/**
 * @brief Nivel de carga inicial de la batería al crear la entidad
 *
 * Por defecto, se asume batería completamente cargada (100%).
 */
#define NIVEL_CARGA_INICIAL      100

/**
 * @brief Umbral de batería baja (%)
 *
 * Nivel de carga por debajo del cual se considera que la batería
 * está baja y puede requerir atención del usuario.
 */
#define UMBRAL_BATERIA_BAJA      20

/**
 * @brief Umbral de batería crítica (%)
 *
 * Nivel de carga por debajo del cual se considera que la batería
 * está en estado crítico y el sistema debe tomar acciones de
 * conservación de energía o apagado ordenado.
 */
#define UMBRAL_BATERIA_CRITICA   10

#endif // TIPOS_COMUNES_H
