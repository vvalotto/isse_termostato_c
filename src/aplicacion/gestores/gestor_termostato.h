/**
 * @file gestor_termostato.h
 * @brief Gestor del termostato (Capa de Aplicación)
 * @version 1.0
 * @date 2025-11-10
 *
 * El gestor del termostato coordina la lectura del sensor de temperatura
 * y la actualización de la entidad Ambiente. Es el orquestador principal
 * del caso de uso HU-014: Obtener la temperatura ambiente.
 *
 * Responsabilidades:
 * - Orquestar la lectura del sensor de temperatura
 * - Actualizar la entidad Ambiente con la temperatura leída
 * - Proporcionar acceso a la temperatura actual
 *
 * @author Victor Valotto
 * @copyright Universidad Nacional de Entre Ríos (FIUNER)
 */

#ifndef GESTOR_TERMOSTATO_H
#define GESTOR_TERMOSTATO_H

#include "tipos_comunes.h"
#include "ambiente.h"

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
 * Inicializa el gestor con una referencia a la entidad Ambiente.
 * El gestor NO toma posesión del ambiente; el llamador es responsable
 * de su ciclo de vida.
 *
 * @param ambiente Puntero a la entidad Ambiente (no NULL)
 * @return Puntero al gestor creado, o NULL si falla la asignación
 *
 * @note El ambiente debe existir durante toda la vida del gestor
 * @note Llamar a gestor_termostato_destruir() cuando termine de usarse
 *
 * @code
 * Ambiente* ambiente = ambiente_crear();
 * GestorTermostato* gestor = gestor_termostato_crear(ambiente);
 * // ... usar gestor ...
 * gestor_termostato_destruir(gestor);
 * ambiente_destruir(ambiente);
 * @endcode
 */
GestorTermostato* gestor_termostato_crear(Ambiente* ambiente);

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

#endif // GESTOR_TERMOSTATO_H
