/**
 * @file gestor_termostato.h
 * @brief Gestor del termostato (Capa de Aplicación)
 * @version 1.1
 * @date 2025-11-17
 *
 * El gestor del termostato coordina las operaciones principales del sistema:
 * - HU-014: Obtener la temperatura ambiente
 * - HU-007: Obtener el nivel de carga de la batería
 *
 * Responsabilidades:
 * - Orquestar la lectura del sensor de temperatura
 * - Actualizar la entidad Ambiente con la temperatura leída
 * - Orquestar la lectura del sensor de batería
 * - Actualizar la entidad Bateria con el nivel leído
 * - Proporcionar acceso a temperatura y estado de batería
 *
 * @author Victor Valotto
 * @copyright Universidad Nacional de Entre Ríos (FIUNER)
 */

#ifndef GESTOR_TERMOSTATO_H
#define GESTOR_TERMOSTATO_H

#include "tipos_comunes.h"
#include "ambiente.h"
#include "bateria.h"

/**
 * @brief Tipo opaco para el gestor del termostato
 *
 * Oculta los detalles de implementación siguiendo el patrón
 * de puntero opaco (Opaque Pointer).
 */
typedef struct GestorTermostato GestorTermostato;

/**
 * @brief Crea una instancia del gestor de termostato
 *
 * Inicializa el gestor con referencias a las entidades Ambiente y Bateria.
 * El gestor NO toma posesión de estas entidades; el llamador es responsable
 * de su ciclo de vida.
 *
 * @param ambiente Puntero a la entidad Ambiente (no NULL)
 * @param bateria Puntero a la entidad Bateria (no NULL)
 * @return Puntero al gestor creado, o NULL si falla la asignación
 *
 * @note Las entidades deben existir durante toda la vida del gestor
 * @note Llamar a gestor_termostato_destruir() cuando termine de usarse
 *
 * @code
 * Ambiente* ambiente = ambiente_crear();
 * Bateria* bateria = bateria_crear();
 * GestorTermostato* gestor = gestor_termostato_crear(ambiente, bateria);
 * // ... usar gestor ...
 * gestor_termostato_destruir(gestor);
 * bateria_destruir(bateria);
 * ambiente_destruir(ambiente);
 * @endcode
 */
GestorTermostato* gestor_termostato_crear(Ambiente* ambiente, Bateria* bateria);

/**
 * @brief Destruye una instancia del gestor de termostato
 *
 * Libera la memoria asociada al gestor. NO destruye el ambiente
 * referenciado, ya que el gestor no es su propietario.
 *
 * @param gestor Puntero al gestor a destruir (puede ser NULL)
 *
 * @note Seguro llamar con NULL (no hace nada)
 * @note No modifica el ambiente asociado
 */
void gestor_termostato_destruir(GestorTermostato* gestor);

/**
 * @brief Actualiza la temperatura leyendo del sensor
 *
 * Este método implementa el flujo principal del caso de uso HU-014:
 * 1. Lee la temperatura del sensor físico
 * 2. Actualiza la entidad Ambiente con el valor leído
 *
 * @param gestor Puntero al gestor (no NULL)
 *
 * @note Llamar periódicamente desde el loop principal
 * @note Requiere que sensor_temperatura_init() haya sido llamado previamente
 *
 * @code
 * // En el loop principal
 * while (sistema_activo) {
 *     gestor_termostato_actualizar_temperatura(gestor);
 *     // ... otras operaciones ...
 * }
 * @endcode
 */
void gestor_termostato_actualizar_temperatura(GestorTermostato* gestor);

/**
 * @brief Obtiene la temperatura actual del ambiente
 *
 * Consulta la temperatura almacenada en la entidad Ambiente.
 *
 * @param gestor Puntero al gestor (no NULL)
 * @return Temperatura actual en grados Celsius
 *
 * @note No realiza lectura del sensor; usa el último valor actualizado
 * @note Para obtener temperatura fresca, llamar primero a
 *       gestor_termostato_actualizar_temperatura()
 */
Temperatura gestor_termostato_obtener_temperatura_actual(const GestorTermostato* gestor);

// ============================================
// OPERACIONES DE BATERÍA (ITE-57)
// ============================================

/**
 * @brief Actualiza el nivel de batería leyendo del sensor
 *
 * Este método implementa el flujo principal del caso de uso HU-007:
 * 1. Lee el nivel de carga del sensor de batería
 * 2. Actualiza la entidad Bateria con el valor leído
 *
 * @param gestor Puntero al gestor (no NULL)
 *
 * @note Llamar periódicamente desde el loop principal
 * @note Requiere que sensor_bateria_init() haya sido llamado previamente
 *
 * @code
 * // En el loop principal
 * while (sistema_activo) {
 *     gestor_termostato_actualizar_temperatura(gestor);
 *     gestor_termostato_actualizar_bateria(gestor);
 *     // ... otras operaciones ...
 * }
 * @endcode
 */
void gestor_termostato_actualizar_bateria(GestorTermostato* gestor);

/**
 * @brief Obtiene el nivel de carga actual de la batería
 *
 * Consulta el nivel de carga almacenado en la entidad Bateria.
 *
 * @param gestor Puntero al gestor (no NULL)
 * @return Nivel de carga en porcentaje (0-100%)
 *
 * @note No realiza lectura del sensor; usa el último valor actualizado
 * @note Para obtener nivel fresco, llamar primero a
 *       gestor_termostato_actualizar_bateria()
 */
NivelCarga gestor_termostato_obtener_nivel_bateria(const GestorTermostato* gestor);

/**
 * @brief Obtiene el estado actual de la batería
 *
 * Consulta el estado de la batería (NORMAL, BAJO, CRITICO) desde
 * la entidad Bateria.
 *
 * @param gestor Puntero al gestor (no NULL)
 * @return Estado actual de la batería
 *
 * @note El estado se calcula automáticamente según el nivel de carga
 */
EstadoBateria gestor_termostato_obtener_estado_bateria(const GestorTermostato* gestor);

/**
 * @brief Verifica si la batería está en nivel bajo
 *
 * Determina si el nivel de carga está por debajo del umbral de batería baja.
 *
 * @param gestor Puntero al gestor (no NULL)
 * @return true si la batería está baja (< 20%), false en caso contrario
 *
 * @note Útil para mostrar advertencias al usuario
 */
bool gestor_termostato_bateria_baja(const GestorTermostato* gestor);

#endif // GESTOR_TERMOSTATO_H
