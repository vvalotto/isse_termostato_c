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

#endif // TIPOS_COMUNES_H
