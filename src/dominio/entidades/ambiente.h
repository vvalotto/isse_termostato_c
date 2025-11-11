/**
 * @file ambiente.h
 * @brief Entidad de dominio que representa el ambiente
 * @version 1.0
 * @date 2025-11-10
 *
 * Entidad del dominio que modela el ambiente cuya temperatura
 * es controlada por el termostato. Esta es una entidad pura
 * sin dependencias de infraestructura.
 *
 * @author Victor Valotto
 * @copyright Universidad Nacional de Entre Ríos (FIUNER)
 */

#ifndef AMBIENTE_H
#define AMBIENTE_H

#include "tipos_comunes.h"

/**
 * @brief Estructura opaca que representa el ambiente
 *
 * La definición interna está en ambiente.c para ocultar
 * detalles de implementación (patrón opaque pointer).
 */
typedef struct Ambiente Ambiente;

/**
 * @brief Crea una nueva instancia de Ambiente
 *
 * Asigna memoria dinámicamente para una instancia de Ambiente
 * e inicializa la temperatura al valor por defecto.
 *
 * @return Puntero a Ambiente creado, o NULL si falla la asignación
 *
 * @note El caller es responsable de liberar la memoria con ambiente_destruir()
 * @see ambiente_destruir()
 *
 * @code
 * Ambiente* ambiente = ambiente_crear();
 * if (ambiente != NULL) {
 *     // usar ambiente
 *     ambiente_destruir(ambiente);
 * }
 * @endcode
 */
Ambiente* ambiente_crear(void);

/**
 * @brief Destruye una instancia de Ambiente y libera memoria
 *
 * Libera la memoria asignada dinámicamente para el Ambiente.
 * Después de llamar a esta función, el puntero queda inválido.
 *
 * @param ambiente Puntero a la instancia a destruir (puede ser NULL)
 *
 * @note Es seguro pasar NULL a esta función (no hace nada)
 */
void ambiente_destruir(Ambiente* ambiente);

/**
 * @brief Establece la temperatura actual del ambiente
 *
 * Actualiza el valor de temperatura almacenado en la entidad.
 *
 * @param ambiente Puntero a la instancia de Ambiente (no debe ser NULL)
 * @param temperatura Nueva temperatura en grados Celsius
 *
 * @warning Si ambiente es NULL, la función retorna sin hacer nada
 */
void ambiente_establecer_temperatura(Ambiente* ambiente, Temperatura temperatura);

/**
 * @brief Obtiene la temperatura actual del ambiente
 *
 * Retorna el valor de temperatura almacenado actualmente.
 *
 * @param ambiente Puntero a la instancia de Ambiente (no debe ser NULL)
 * @return Temperatura actual en grados Celsius
 *
 * @warning Si ambiente es NULL, retorna TEMP_INICIAL_DEFAULT
 */
Temperatura ambiente_obtener_temperatura(const Ambiente* ambiente);

#endif // AMBIENTE_H
