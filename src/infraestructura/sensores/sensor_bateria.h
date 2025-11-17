/**
 * @file sensor_bateria.h
 * @brief Proxy del sensor de batería en la capa de infraestructura
 * @version 1.0
 * @date 2025-11-17
 *
 * Actúa como proxy entre la capa de aplicación y el hardware de batería.
 * Se encarga de leer el voltaje desde el HAL y convertirlo a porcentaje
 * de carga para consumo de las capas superiores.
 *
 * Responsabilidades:
 * - Delegar lectura de voltaje al HAL
 * - Convertir voltaje (mV) a nivel de carga (%)
 * - Validar y limitar valores al rango esperado
 *
 * @author Victor Valotto
 * @copyright Universidad Nacional de Entre Ríos (FIUNER)
 */

#ifndef SENSOR_BATERIA_H
#define SENSOR_BATERIA_H

#include "tipos_comunes.h"

/**
 * @brief Inicializa el sensor de batería
 *
 * Configura el hardware subyacente necesario para la lectura
 * del nivel de carga de la batería. Internamente inicializa
 * el HAL de batería.
 *
 * @note Debe llamarse una vez al inicio del sistema, antes de
 *       realizar cualquier lectura de batería.
 *
 * Ejemplo de uso:
 * @code
 * sensor_bateria_init();
 * NivelCarga nivel = sensor_bateria_leer();
 * printf("Batería: %u%%\n", nivel);
 * sensor_bateria_deinit();
 * @endcode
 */
void sensor_bateria_init(void);

/**
 * @brief Desinicializa el sensor de batería
 *
 * Libera recursos y apaga el hardware de lectura de batería.
 * Internamente desinicializa el HAL de batería.
 *
 * @note Después de llamar a esta función, se debe llamar a
 *       sensor_bateria_init() nuevamente antes de realizar lecturas.
 */
void sensor_bateria_deinit(void);

/**
 * @brief Lee el nivel de carga actual de la batería
 *
 * Realiza una lectura del nivel de carga de la batería y retorna
 * el valor en porcentaje (0-100%).
 *
 * Proceso de lectura:
 * 1. Lee voltaje desde HAL (en milivolts)
 * 2. Valida que esté en rango válido (3000-4200 mV)
 * 3. Convierte a porcentaje usando fórmula lineal
 * 4. Retorna nivel de carga (0-100%)
 *
 * Conversión voltaje → porcentaje:
 *
 * Fórmula lineal simplificada para batería Li-ion:
 * ```
 * Porcentaje = (Voltaje - V_min) × 100 / (V_max - V_min)
 * Porcentaje = (Voltaje - 3000) × 100 / (4200 - 3000)
 * Porcentaje = (Voltaje - 3000) × 100 / 1200
 * ```
 *
 * Ejemplos de conversión:
 * - 3000 mV → 0%
 * - 3600 mV → 50%
 * - 3900 mV → 75%
 * - 4200 mV → 100%
 *
 * @note Esta implementación usa conversión lineal simplificada.
 *       En un sistema real, se debería usar la curva de descarga
 *       específica del modelo de batería (no lineal).
 *
 * @note Si el voltaje está fuera de rango, se limita automáticamente:
 *       - Si < 3000 mV → retorna 0%
 *       - Si > 4200 mV → retorna 100%
 *
 * @return Nivel de carga de la batería en porcentaje (0-100)
 *
 * Curva de descarga real vs. lineal:
 *
 * En baterías Li-ion reales, la relación voltaje-capacidad no es lineal.
 * La curva típica es más plana en el rango medio y cae abruptamente
 * cerca de los extremos. Para mayor precisión en producción, usar una
 * lookup table (LUT) con la curva real del fabricante.
 *
 * Ejemplo de uso en ciclo de control:
 * @code
 * void actualizar_sistema(void) {
 *     NivelCarga nivel = sensor_bateria_leer();
 *
 *     if (nivel < UMBRAL_BATERIA_CRITICA) {
 *         // Modo de bajo consumo
 *         activar_modo_ahorro_energia();
 *     } else if (nivel < UMBRAL_BATERIA_BAJA) {
 *         // Advertencia al usuario
 *         mostrar_alerta_bateria_baja();
 *     }
 * }
 * @endcode
 */
NivelCarga sensor_bateria_leer(void);

#endif // SENSOR_BATERIA_H
