/**
 * @file bateria.h
 * @brief Entidad de dominio que modela la batería del sistema
 * @version 1.0
 * @date 2025-11-17
 *
 * Representa el concepto de negocio de la batería del termostato.
 * Almacena el nivel de carga y determina el estado operacional
 * según umbrales definidos.
 *
 * Esta es una entidad de dominio pura sin dependencias externas,
 * siguiendo los principios de Domain-Driven Design.
 *
 * @author Victor Valotto
 * @copyright Universidad Nacional de Entre Ríos (FIUNER)
 */

#ifndef BATERIA_H
#define BATERIA_H

#include "tipos_comunes.h"
#include <stdbool.h>

/**
 * @brief Tipo opaco para la entidad Bateria
 *
 * La implementación interna está oculta para garantizar encapsulación.
 * Los clientes solo interactúan mediante las funciones públicas.
 */
typedef struct Bateria Bateria;

/**
 * @brief Crea una nueva instancia de Bateria
 *
 * Crea una batería con nivel de carga inicial al 100% y estado NORMAL.
 * La memoria es asignada dinámicamente y debe ser liberada con
 * bateria_destruir().
 *
 * @return Puntero a la nueva instancia de Bateria, o NULL si falla
 *
 * Ejemplo de uso:
 * @code
 * Bateria* bat = bateria_crear();
 * if (bat != NULL) {
 *     // Usar batería
 *     bateria_destruir(bat);
 * }
 * @endcode
 */
Bateria* bateria_crear(void);

/**
 * @brief Destruye una instancia de Bateria y libera memoria
 *
 * Libera toda la memoria asociada con la batería.
 * Es seguro pasar NULL (no hace nada).
 *
 * @param bateria Puntero a la batería a destruir
 *
 * @warning Después de llamar a esta función, el puntero queda inválido
 *          y no debe ser usado nuevamente.
 */
void bateria_destruir(Bateria* bateria);

/**
 * @brief Establece el nivel de carga de la batería
 *
 * Actualiza el nivel de carga y recalcula automáticamente el estado
 * (NORMAL, BAJO, CRITICO) según los umbrales definidos.
 *
 * Umbrales:
 * - CRITICO: nivel < 10%
 * - BAJO: nivel >= 10% y < 20%
 * - NORMAL: nivel >= 20%
 *
 * @param bateria Puntero a la batería
 * @param nivel Nivel de carga en porcentaje (0-100)
 *
 * @note Si bateria es NULL, la función no hace nada
 * @note El nivel se acepta tal como viene (validación en capas superiores)
 */
void bateria_establecer_nivel(Bateria* bateria, NivelCarga nivel);

/**
 * @brief Obtiene el nivel de carga actual de la batería
 *
 * @param bateria Puntero a la batería
 * @return Nivel de carga en porcentaje (0-100), o 0 si bateria es NULL
 */
NivelCarga bateria_obtener_nivel(const Bateria* bateria);

/**
 * @brief Obtiene el estado actual de la batería
 *
 * El estado se calcula automáticamente al establecer el nivel.
 *
 * @param bateria Puntero a la batería
 * @return Estado actual (NORMAL, BAJO, CRITICO), o CRITICO si bateria es NULL
 */
EstadoBateria bateria_obtener_estado(const Bateria* bateria);

/**
 * @brief Verifica si la batería está en nivel bajo
 *
 * Determina si el nivel de carga está por debajo del umbral de batería baja.
 *
 * @param bateria Puntero a la batería
 * @return true si nivel < UMBRAL_BATERIA_BAJA (20%), false en caso contrario
 */
bool bateria_esta_baja(const Bateria* bateria);

/**
 * @brief Verifica si la batería está en nivel crítico
 *
 * Determina si el nivel de carga está por debajo del umbral crítico.
 *
 * @param bateria Puntero a la batería
 * @return true si nivel < UMBRAL_BATERIA_CRITICA (10%), false en caso contrario
 */
bool bateria_esta_critica(const Bateria* bateria);

#endif // BATERIA_H
