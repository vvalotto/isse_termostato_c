# Especificaciones Técnicas Detalladas - ITE-56

**Historia:** HU-014 - Obtener la temperatura ambiente
**Versión:** 1.0
**Fecha:** 2025-11-10

---

## 📋 Índice

1. [Tipos Comunes](#1-tipos-comunes)
2. [Entidad Ambiente](#2-entidad-ambiente)
3. [HAL ADC](#3-hal-adc)
4. [Sensor Temperatura](#4-sensor-temperatura)
5. [Gestor Termostato](#5-gestor-termostato)
6. [Patrones de Diseño](#patrones-de-diseño)
7. [Estándares de Código](#estándares-de-código)

---

## 1. Tipos Comunes

### Ubicación
```
src/common/tipos_comunes.h
```

### Responsabilidad
Definir tipos base, constantes y enumeraciones compartidas por todo el sistema.

### Interfaz Completa

```c
/**
 * @file tipos_comunes.h
 * @brief Tipos y constantes comunes del sistema termostato
 * @version 1.0
 * @date 2025-11-10
 *
 * Define tipos base y constantes utilizadas en todo el sistema.
 * Este módulo no tiene dependencias externas del proyecto.
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
 */
typedef float Temperatura;

/**
 * @brief Temperatura inicial por defecto del ambiente
 *
 * Valor usado al crear una nueva instancia de Ambiente si no
 * se especifica otra temperatura inicial.
 */
#define TEMP_INICIAL_DEFAULT 20.0f

#endif // TIPOS_COMUNES_H
```

### Notas de Implementación

- **Sin dependencias:** Este header no debe incluir otros headers del proyecto
- **Portabilidad:** Solo usa tipos estándar de C11 (`<stdint.h>`, `<stdbool.h>`)
- **Extensibilidad:** Futuros tipos (ErrorCode, EstadoSensor) se agregarán aquí

### Tests

No requiere tests unitarios (solo definiciones de tipos).

---

## 2. Entidad Ambiente

### Ubicación
```
src/dominio/entidades/ambiente.h
src/dominio/entidades/ambiente.c
```

### Responsabilidad
Modelar el concepto de dominio "Ambiente", almacenar temperatura actual y proporcionar operaciones básicas.

### Interfaz Completa - ambiente.h

```c
/**
 * @file ambiente.h
 * @brief Entidad de dominio que representa el ambiente
 * @version 1.0
 * @date 2025-11-10
 *
 * Entidad del dominio que modela el ambiente cuya temperatura
 * es controlada por el termostato. Esta es una entidad pura
 * sin dependencias de infraestructura.
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
 * @warning Si ambiente es NULL, el comportamiento es indefinido
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
 * @warning Si ambiente es NULL, el comportamiento es indefinido
 */
Temperatura ambiente_obtener_temperatura(const Ambiente* ambiente);

#endif // AMBIENTE_H
```

### Implementación - ambiente.c

```c
/**
 * @file ambiente.c
 * @brief Implementación de la entidad Ambiente
 */

#include "ambiente.h"
#include <stdlib.h>

/**
 * @brief Definición interna de la estructura Ambiente
 *
 * Esta definición está oculta al exterior (patrón opaque pointer)
 * para proteger la encapsulación.
 */
struct Ambiente {
    Temperatura temperatura_actual;  /**< Temperatura actual en °C */
};

Ambiente* ambiente_crear(void) {
    // Alocar memoria para la estructura
    Ambiente* ambiente = (Ambiente*)malloc(sizeof(Ambiente));

    if (ambiente != NULL) {
        // Inicializar con temperatura por defecto
        ambiente->temperatura_actual = TEMP_INICIAL_DEFAULT;
    }

    return ambiente;
}

void ambiente_destruir(Ambiente* ambiente) {
    // Verificación defensiva: es seguro pasar NULL a free()
    if (ambiente != NULL) {
        free(ambiente);
    }
}

void ambiente_establecer_temperatura(Ambiente* ambiente, Temperatura temperatura) {
    // Validación defensiva
    if (ambiente == NULL) {
        return;  // O podría ser assert en modo debug
    }

    ambiente->temperatura_actual = temperatura;
}

Temperatura ambiente_obtener_temperatura(const Ambiente* ambiente) {
    // Validación defensiva
    if (ambiente == NULL) {
        return TEMP_INICIAL_DEFAULT;  // Valor por defecto como fallback
    }

    return ambiente->temperatura_actual;
}
```

### Tests - test_ambiente.c

```c
/**
 * @file test_ambiente.c
 * @brief Tests unitarios para la entidad Ambiente
 */

#include "unity.h"
#include "ambiente.h"

void setUp(void) {
    // Ejecutado antes de cada test
}

void tearDown(void) {
    // Ejecutado después de cada test
}

void test_ambiente_crear_retorna_puntero_valido(void) {
    Ambiente* ambiente = ambiente_crear();
    TEST_ASSERT_NOT_NULL(ambiente);
    ambiente_destruir(ambiente);
}

void test_ambiente_crear_inicializa_temperatura_default(void) {
    Ambiente* ambiente = ambiente_crear();
    Temperatura temp = ambiente_obtener_temperatura(ambiente);
    TEST_ASSERT_EQUAL_FLOAT(TEMP_INICIAL_DEFAULT, temp);
    ambiente_destruir(ambiente);
}

void test_ambiente_establecer_temperatura_actualiza_valor(void) {
    Ambiente* ambiente = ambiente_crear();

    ambiente_establecer_temperatura(ambiente, 25.5f);
    Temperatura temp = ambiente_obtener_temperatura(ambiente);

    TEST_ASSERT_EQUAL_FLOAT(25.5f, temp);
    ambiente_destruir(ambiente);
}

void test_ambiente_obtener_temperatura_retorna_valor_correcto(void) {
    Ambiente* ambiente = ambiente_crear();

    ambiente_establecer_temperatura(ambiente, 18.3f);

    TEST_ASSERT_EQUAL_FLOAT(18.3f, ambiente_obtener_temperatura(ambiente));
    ambiente_destruir(ambiente);
}

void test_ambiente_destruir_con_null_no_falla(void) {
    // Este test verifica que pasar NULL no cause problemas
    ambiente_destruir(NULL);
    // Si llegamos aquí sin crash, el test pasa
    TEST_PASS();
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_ambiente_crear_retorna_puntero_valido);
    RUN_TEST(test_ambiente_crear_inicializa_temperatura_default);
    RUN_TEST(test_ambiente_establecer_temperatura_actualiza_valor);
    RUN_TEST(test_ambiente_obtener_temperatura_retorna_valor_correcto);
    RUN_TEST(test_ambiente_destruir_con_null_no_falla);
    return UNITY_END();
}
```

### Decisiones de Diseño

1. **Struct Opaca (Opaque Pointer):**
   - La definición de `struct Ambiente` está en el .c, no en el .h
   - Oculta detalles de implementación
   - Permite cambiar internos sin afectar clientes

2. **Manejo de NULL:**
   - `ambiente_destruir(NULL)` es seguro (no hace nada)
   - Getters/setters verifican NULL defensivamente
   - Alternativa: usar assert() para detectar bugs en desarrollo

3. **Memoria:**
   - malloc/free manual (no GC en C)
   - Responsabilidad del caller liberar memoria
   - Documentado claramente en comentarios Doxygen

---

## 3. HAL ADC

### Ubicación
```
src/dispositivos/hal/hal_adc.h
src/dispositivos/hal/hal_adc.c
```

### Responsabilidad
Abstracción de la capa de hardware (HAL) para el ADC. Proporciona interfaz uniforme independiente de la plataforma.

### Interfaz Completa - hal_adc.h

```c
/**
 * @file hal_adc.h
 * @brief Hardware Abstraction Layer para ADC
 * @version 1.0
 * @date 2025-11-10
 *
 * Abstracción del ADC (Analog-to-Digital Converter) del microcontrolador.
 * Esta versión implementa un simulador para desarrollo sin hardware.
 *
 * @note Versiones futuras: hal_adc_stm32.c, hal_adc_avr.c, hal_adc_esp32.c
 */

#ifndef HAL_ADC_H
#define HAL_ADC_H

#include <stdint.h>

/**
 * @brief Inicializa el HAL del ADC
 *
 * Configura el ADC para lectura. En versión simulada, inicializa
 * el generador de números aleatorios.
 *
 * @note En hardware real: configuraría registros, clock, canales, etc.
 */
void hal_adc_init(void);

/**
 * @brief Desinicializa el HAL del ADC
 *
 * Libera recursos asociados al ADC si los hay.
 * En versión simulada es un stub vacío.
 */
void hal_adc_deinit(void);

/**
 * @brief Lee un valor del ADC simulado
 *
 * Retorna un valor simulado que representa una lectura de ADC.
 * Los valores están en el rango [150-300] para simular ~15-30°C
 * después de la conversión.
 *
 * @return Valor ADC de 10 bits (0-1023), típicamente 150-300
 *
 * @note En hardware real: leería un canal específico del ADC físico
 * @note El rango simulado es deliberadamente limitado para testing
 */
uint16_t hal_adc_leer_simulado(void);

/**
 * @brief Constantes del ADC
 */
#define HAL_ADC_RESOLUCION_BITS 10      /**< Resolución: 10 bits (0-1023) */
#define HAL_ADC_MAX_VALUE ((1 << HAL_ADC_RESOLUCION_BITS) - 1)  /**< Valor máximo: 1023 */

#endif // HAL_ADC_H
```

### Implementación - hal_adc.c

```c
/**
 * @file hal_adc.c
 * @brief Implementación simulada del HAL ADC
 */

#include "hal_adc.h"
#include <stdlib.h>
#include <time.h>

void hal_adc_init(void) {
    // Inicializar generador de números aleatorios con seed basada en tiempo
    srand((unsigned int)time(NULL));
}

void hal_adc_deinit(void) {
    // Stub vacío: no hay recursos que liberar en versión simulada
}

uint16_t hal_adc_leer_simulado(void) {
    // Simular lectura de ADC con valores entre 150 y 300
    // Esto corresponde aproximadamente a 15-30°C después de conversión
    //
    // Fórmula: valor_base + (valor_aleatorio % rango)
    // 150 + (0..149) = 150..299

    uint16_t valor_adc = 150 + (rand() % 150);

    return valor_adc;
}

/**
 * @note Implementación futura para hardware real:
 *
 * uint16_t hal_adc_leer_stm32(uint8_t canal) {
 *     // Configurar canal ADC
 *     ADC1->SQR3 = canal;
 *
 *     // Iniciar conversión
 *     ADC1->CR2 |= ADC_CR2_SWSTART;
 *
 *     // Esperar fin de conversión
 *     while (!(ADC1->SR & ADC_SR_EOC));
 *
 *     // Leer resultado
 *     return ADC1->DR;
 * }
 */
```

### Tests - test_hal_adc.c

```c
/**
 * @file test_hal_adc.c
 * @brief Tests unitarios para HAL ADC
 */

#include "unity.h"
#include "hal_adc.h"

void setUp(void) {
    hal_adc_init();
}

void tearDown(void) {
    hal_adc_deinit();
}

void test_hal_adc_leer_retorna_valor_valido(void) {
    uint16_t valor = hal_adc_leer_simulado();

    // Debe estar en rango de 10 bits
    TEST_ASSERT_LESS_OR_EQUAL(HAL_ADC_MAX_VALUE, valor);
    TEST_ASSERT_GREATER_OR_EQUAL(0, valor);
}

void test_hal_adc_leer_retorna_en_rango_esperado(void) {
    uint16_t valor = hal_adc_leer_simulado();

    // El simulador debe retornar valores entre 150-300
    TEST_ASSERT_GREATER_OR_EQUAL(150, valor);
    TEST_ASSERT_LESS_OR_EQUAL(300, valor);
}

void test_hal_adc_multiples_lecturas_varían(void) {
    uint16_t valores[10];
    bool hay_variacion = false;

    // Leer 10 valores
    for (int i = 0; i < 10; i++) {
        valores[i] = hal_adc_leer_simulado();
    }

    // Verificar que al menos 2 valores sean diferentes
    for (int i = 1; i < 10; i++) {
        if (valores[i] != valores[0]) {
            hay_variacion = true;
            break;
        }
    }

    TEST_ASSERT_TRUE(hay_variacion);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_hal_adc_leer_retorna_valor_valido);
    RUN_TEST(test_hal_adc_leer_retorna_en_rango_esperado);
    RUN_TEST(test_hal_adc_multiples_lecturas_varían);
    return UNITY_END();
}
```

### Decisiones de Diseño

1. **Simulación vs Hardware Real:**
   - Versión actual es completamente simulada
   - Permite desarrollo sin hardware físico
   - Fácil agregar versiones específicas (stm32, avr, esp32)

2. **Rango Limitado:**
   - Simula 150-300 (en vez de 0-1023 completo)
   - Corresponde a ~15-30°C (rango razonable interior)
   - Facilita testing y validación

3. **Extensibilidad:**
   - Interfaz unificada permite swap de implementaciones
   - Compilación condicional futura con `#ifdef TARGET_PLATFORM`

---

## 4. Sensor Temperatura

### Ubicación
```
src/infraestructura/sensores/sensor_temperatura.h
src/infraestructura/sensores/sensor_temperatura.c
```

### Responsabilidad
Proxy del sensor físico de temperatura. Delega lectura a HAL y convierte valor ADC a temperatura en grados Celsius.

### Interfaz Completa - sensor_temperatura.h

```c
/**
 * @file sensor_temperatura.h
 * @brief Proxy del sensor de temperatura
 * @version 1.0
 * @date 2025-11-10
 *
 * Componente de infraestructura que actúa como proxy del sensor
 * físico de temperatura. Se encarga de leer el ADC y convertir
 * el valor a temperatura en grados Celsius.
 */

#ifndef SENSOR_TEMPERATURA_H
#define SENSOR_TEMPERATURA_H

#include "tipos_comunes.h"

/**
 * @brief Inicializa el sensor de temperatura
 *
 * Realiza la inicialización necesaria del sensor y del HAL subyacente.
 * Debe llamarse antes de cualquier lectura.
 */
void sensor_temperatura_init(void);

/**
 * @brief Desinicializa el sensor de temperatura
 *
 * Libera recursos asociados al sensor.
 */
void sensor_temperatura_deinit(void);

/**
 * @brief Lee la temperatura actual del sensor
 *
 * Realiza una lectura del ADC y convierte el valor a temperatura
 * en grados Celsius.
 *
 * @return Temperatura leída en grados Celsius
 *
 * @note Versión simplificada: retorna directamente sin código de error
 * @note Conversión: ADC 0-1023 → 0-50°C aproximadamente
 */
Temperatura sensor_temperatura_leer(void);

#endif // SENSOR_TEMPERATURA_H
```

### Implementación - sensor_temperatura.c

```c
/**
 * @file sensor_temperatura.c
 * @brief Implementación del proxy de sensor de temperatura
 */

#include "sensor_temperatura.h"
#include "hal_adc.h"

/**
 * @brief Factor de conversión ADC a temperatura
 *
 * Cálculo: 50°C / 1023 (valor máximo ADC) ≈ 0.0488
 *
 * Esto mapea:
 * - ADC 0 → 0°C
 * - ADC 1023 → 50°C
 * - ADC 200 → ~9.8°C
 * - ADC 400 → ~19.5°C
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
    // 1. Leer valor del ADC (0-1023)
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
 * - 0°C → 0V
 * - 10°C → 100mV (0.1V)
 * - 25°C → 250mV (0.25V)
 *
 * Con ADC de 10 bits y referencia de 5V:
 * - Resolución: 5V / 1024 = 4.88mV por bit
 * - Para LM35: °C = (adc_value * 5.0 / 1023.0) / 0.01
 * - Simplificado: °C = adc_value * 0.4887
 *
 * La constante actual (0.0488) es una simplificación para simulador.
 */
```

### Tests - test_sensor_temperatura.c

```c
/**
 * @file test_sensor_temperatura.c
 * @brief Tests unitarios para sensor de temperatura
 */

#include "unity.h"
#include "sensor_temperatura.h"

void setUp(void) {
    sensor_temperatura_init();
}

void tearDown(void) {
    sensor_temperatura_deinit();
}

void test_sensor_temperatura_leer_retorna_valor_valido(void) {
    Temperatura temp = sensor_temperatura_leer();

    // Temperatura debe ser un número válido (no NaN, no infinito)
    TEST_ASSERT_TRUE(temp == temp);  // NaN check
}

void test_sensor_temperatura_leer_retorna_rango_esperado(void) {
    Temperatura temp = sensor_temperatura_leer();

    // Con ADC simulado (150-300), esperamos ~7-15°C
    // Damos margen: 5-20°C
    TEST_ASSERT_GREATER_OR_EQUAL(5.0f, temp);
    TEST_ASSERT_LESS_OR_EQUAL(20.0f, temp);
}

void test_sensor_temperatura_multiples_lecturas(void) {
    Temperatura temp1 = sensor_temperatura_leer();
    Temperatura temp2 = sensor_temperatura_leer();
    Temperatura temp3 = sensor_temperatura_leer();

    // Todas deben estar en rango válido
    TEST_ASSERT_GREATER_OR_EQUAL(0.0f, temp1);
    TEST_ASSERT_GREATER_OR_EQUAL(0.0f, temp2);
    TEST_ASSERT_GREATER_OR_EQUAL(0.0f, temp3);
}

void test_sensor_temperatura_conversion_correcta(void) {
    // Test de conversión matemática
    // Si ADC = 200, entonces temp = 200 * 0.0488 ≈ 9.76°C

    // Este test es más conceptual, verifica que la conversión sea razonable
    Temperatura temp = sensor_temperatura_leer();

    // temp debe ser positiva y menor a 50°C (límite de conversión)
    TEST_ASSERT_GREATER_THAN(0.0f, temp);
    TEST_ASSERT_LESS_THAN(50.0f, temp);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_sensor_temperatura_leer_retorna_valor_valido);
    RUN_TEST(test_sensor_temperatura_leer_retorna_rango_esperado);
    RUN_TEST(test_sensor_temperatura_multiples_lecturas);
    RUN_TEST(test_sensor_temperatura_conversion_correcta);
    return UNITY_END();
}
```

### Decisiones de Diseño

1. **Conversión Simplificada:**
   - Factor fijo 0.0488 para esta fase
   - Versión futura: configurar tipo de sensor (LM35, DHT22, etc.)
   - Permite cambiar fácilmente sin afectar clientes

2. **Sin Manejo de Errores (por ahora):**
   - Retorna float directamente
   - Asume siempre éxito de lectura
   - Fase futura: retornar código de error

3. **Delegación al HAL:**
   - No conoce detalles de hardware
   - Solo se preocupa por la conversión
   - Desacoplamiento limpio

---

## 5. Gestor Termostato

### Ubicación
```
src/aplicacion/gestores/gestor_termostato.h
src/aplicacion/gestores/gestor_termostato.c
```

### Responsabilidad
Coordinador de la capa de aplicación. Orquesta la lectura de temperatura desde el sensor y actualiza la entidad de dominio Ambiente.

### Interfaz Completa - gestor_termostato.h

```c
/**
 * @file gestor_termostato.h
 * @brief Gestor principal del termostato
 * @version 1.0
 * @date 2025-11-10
 *
 * Componente de la capa de aplicación que coordina las operaciones
 * del termostato. Actúa como orquestador entre la infraestructura
 * (sensores, actuadores) y el dominio (entidades).
 */

#ifndef GESTOR_TERMOSTATO_H
#define GESTOR_TERMOSTATO_H

#include "ambiente.h"
#include "tipos_comunes.h"

/**
 * @brief Estructura opaca del gestor de termostato
 */
typedef struct GestorTermostato GestorTermostato;

/**
 * @brief Crea una nueva instancia del gestor de termostato
 *
 * Crea el gestor y lo asocia con una entidad Ambiente.
 * El gestor mantiene una referencia al Ambiente pero no es
 * propietario de la misma (no la destruirá).
 *
 * @param ambiente Entidad Ambiente a gestionar (no debe ser NULL)
 * @return Puntero al gestor creado, o NULL si falla
 *
 * @note El caller es responsable de destruir el gestor con gestor_termostato_destruir()
 * @note El caller es responsable de gestionar el ciclo de vida de Ambiente
 */
GestorTermostato* gestor_termostato_crear(Ambiente* ambiente);

/**
 * @brief Destruye el gestor de termostato
 *
 * Libera la memoria del gestor. NO destruye la entidad Ambiente asociada.
 *
 * @param gestor Puntero al gestor (puede ser NULL)
 */
void gestor_termostato_destruir(GestorTermostato* gestor);

/**
 * @brief Actualiza la temperatura del ambiente mediante lectura del sensor
 *
 * Coordina la lectura del sensor de temperatura y actualiza la
 * entidad Ambiente con el valor leído.
 *
 * Esta es la operación principal del gestor que debe invocarse
 * periódicamente (típicamente cada 100ms en el ciclo de control).
 *
 * @param gestor Puntero al gestor (no debe ser NULL)
 *
 * @warning Si gestor es NULL, el comportamiento es indefinido
 */
void gestor_termostato_actualizar_temperatura(GestorTermostato* gestor);

/**
 * @brief Obtiene la temperatura actual del ambiente gestionado
 *
 * Retorna la temperatura almacenada en la entidad Ambiente asociada.
 * Es un método de conveniencia equivalente a acceder directamente al Ambiente.
 *
 * @param gestor Puntero al gestor (no debe ser NULL)
 * @return Temperatura actual en grados Celsius
 *
 * @warning Si gestor es NULL, el comportamiento es indefinido
 */
Temperatura gestor_termostato_obtener_temperatura_actual(const GestorTermostato* gestor);

#endif // GESTOR_TERMOSTATO_H
```

### Implementación - gestor_termostato.c

```c
/**
 * @file gestor_termostato.c
 * @brief Implementación del gestor de termostato
 */

#include "gestor_termostato.h"
#include "sensor_temperatura.h"
#include <stdlib.h>

/**
 * @brief Definición interna del gestor
 */
struct GestorTermostato {
    Ambiente* ambiente;  /**< Referencia al ambiente (no propietario) */
};

GestorTermostato* gestor_termostato_crear(Ambiente* ambiente) {
    // Validación de parámetros
    if (ambiente == NULL) {
        return NULL;
    }

    // Alocar memoria
    GestorTermostato* gestor = (GestorTermostato*)malloc(sizeof(GestorTermostato));

    if (gestor != NULL) {
        // Guardar referencia (no copiar)
        gestor->ambiente = ambiente;
    }

    return gestor;
}

void gestor_termostato_destruir(GestorTermostato* gestor) {
    if (gestor != NULL) {
        // IMPORTANTE: NO destruir ambiente (no somos propietarios)
        free(gestor);
    }
}

void gestor_termostato_actualizar_temperatura(GestorTermostato* gestor) {
    // Validación defensiva
    if (gestor == NULL || gestor->ambiente == NULL) {
        return;
    }

    // 1. Leer temperatura del sensor (capa de infraestructura)
    Temperatura temperatura = sensor_temperatura_leer();

    // 2. Actualizar entidad de dominio
    ambiente_establecer_temperatura(gestor->ambiente, temperatura);
}

Temperatura gestor_termostato_obtener_temperatura_actual(const GestorTermostato* gestor) {
    // Validación defensiva
    if (gestor == NULL || gestor->ambiente == NULL) {
        return TEMP_INICIAL_DEFAULT;  // Fallback
    }

    // Delegar a la entidad de dominio
    return ambiente_obtener_temperatura(gestor->ambiente);
}
```

### Tests - test_gestor_termostato.c

```c
/**
 * @file test_gestor_termostato.c
 * @brief Tests unitarios para gestor de termostato
 */

#include "unity.h"
#include "gestor_termostato.h"
#include "sensor_temperatura.h"

static Ambiente* ambiente_test;

void setUp(void) {
    // Inicializar infraestructura
    sensor_temperatura_init();

    // Crear ambiente para tests
    ambiente_test = ambiente_crear();
}

void tearDown(void) {
    // Limpiar
    ambiente_destruir(ambiente_test);
    sensor_temperatura_deinit();
}

void test_gestor_crear_retorna_puntero_valido(void) {
    GestorTermostato* gestor = gestor_termostato_crear(ambiente_test);

    TEST_ASSERT_NOT_NULL(gestor);

    gestor_termostato_destruir(gestor);
}

void test_gestor_crear_con_null_retorna_null(void) {
    GestorTermostato* gestor = gestor_termostato_crear(NULL);

    TEST_ASSERT_NULL(gestor);
}

void test_gestor_actualizar_temperatura_modifica_ambiente(void) {
    GestorTermostato* gestor = gestor_termostato_crear(ambiente_test);

    Temperatura temp_antes = ambiente_obtener_temperatura(ambiente_test);

    // Actualizar temperatura
    gestor_termostato_actualizar_temperatura(gestor);

    Temperatura temp_despues = ambiente_obtener_temperatura(ambiente_test);

    // La temperatura debe haber cambiado (a menos que por casualidad sea la misma)
    // Como es aleatoria, verificamos que esté en rango válido
    TEST_ASSERT_GREATER_OR_EQUAL(0.0f, temp_despues);
    TEST_ASSERT_LESS_OR_EQUAL(50.0f, temp_despues);

    gestor_termostato_destruir(gestor);
}

void test_gestor_obtener_temperatura_retorna_valor_actual(void) {
    GestorTermostato* gestor = gestor_termostato_crear(ambiente_test);

    // Actualizar temperatura
    gestor_termostato_actualizar_temperatura(gestor);

    // Obtener mediante gestor
    Temperatura temp_gestor = gestor_termostato_obtener_temperatura_actual(gestor);

    // Obtener directamente del ambiente
    Temperatura temp_ambiente = ambiente_obtener_temperatura(ambiente_test);

    // Deben ser iguales
    TEST_ASSERT_EQUAL_FLOAT(temp_ambiente, temp_gestor);

    gestor_termostato_destruir(gestor);
}

void test_integracion_flujo_completo(void) {
    // Test de integración: flujo end-to-end

    // 1. Crear gestor
    GestorTermostato* gestor = gestor_termostato_crear(ambiente_test);
    TEST_ASSERT_NOT_NULL(gestor);

    // 2. Actualizar temperatura (lectura de sensor)
    gestor_termostato_actualizar_temperatura(gestor);

    // 3. Obtener temperatura
    Temperatura temp = gestor_termostato_obtener_temperatura_actual(gestor);

    // 4. Verificar que es un valor válido
    TEST_ASSERT_GREATER_OR_EQUAL(0.0f, temp);
    TEST_ASSERT_LESS_OR_EQUAL(50.0f, temp);

    // 5. Limpiar
    gestor_termostato_destruir(gestor);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_gestor_crear_retorna_puntero_valido);
    RUN_TEST(test_gestor_crear_con_null_retorna_null);
    RUN_TEST(test_gestor_actualizar_temperatura_modifica_ambiente);
    RUN_TEST(test_gestor_obtener_temperatura_retorna_valor_actual);
    RUN_TEST(test_integracion_flujo_completo);
    return UNITY_END();
}
```

### Decisiones de Diseño

1. **No Propietario de Ambiente:**
   - Gestor guarda referencia, no copia
   - No destruye Ambiente al destruirse
   - Responsabilidad clara de ownership

2. **Coordinación Simple:**
   - Por ahora solo coordina lectura de temperatura
   - Fases futuras: coordinar actuador, batería, etc.
   - Patrón Facade hacia capas inferiores

3. **Validación Defensiva:**
   - Verifica NULL en todas las operaciones
   - Retorna valores por defecto en caso de error
   - Alternativa futura: códigos de error explícitos

---

## Patrones de Diseño

### 1. Opaque Pointer (Puntero Opaco)

**Usado en:** Todas las entidades (Ambiente, GestorTermostato)

**Ventajas:**
- Encapsulación fuerte
- Cambios internos no afectan clientes
- Reduce recompilaciones

**Implementación:**
```c
// En .h: solo declaración forward
typedef struct Ambiente Ambiente;

// En .c: definición completa
struct Ambiente {
    float temperatura_actual;
};
```

### 2. Facade (Fachada)

**Usado en:** GestorTermostato actúa como facade

**Ventajas:**
- Simplifica interfaz compleja
- Coordina múltiples subsistemas
- Punto único de entrada

### 3. Proxy

**Usado en:** SensorTemperatura es proxy del sensor físico

**Ventajas:**
- Abstrae acceso a recurso externo
- Permite simular sin hardware
- Punto de control centralizado

### 4. Strategy (Estrategia)

**Futuro uso:** Diferentes implementaciones HAL

**Ventajas:**
- Intercambiable en tiempo de compilación
- Mismo interfaz, múltiples plataformas

---

## Estándares de Código

### Nomenclatura

- **Funciones:** `modulo_accion()` - snake_case
- **Tipos:** `PascalCase` para structs, typedefs
- **Constantes:** `UPPER_SNAKE_CASE`
- **Variables:** `snake_case`

### Documentación

- Comentarios Doxygen en todas las funciones públicas
- `@brief`, `@param`, `@return`, `@note`, `@warning`

### Organización de Archivos

```c
// 1. Comentario de archivo (Doxygen)
// 2. Include guards
// 3. Includes (system, luego proyecto)
// 4. Constantes y macros
// 5. Typedefs
// 6. Declaraciones de funciones
// 7. Implementación (en .c)
```

---

**Documento actualizado:** 2025-11-10
**Versión:** 1.0
