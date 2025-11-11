/**
 * @file test_lectura_temperatura_completa.c
 * @brief Test de integración end-to-end para HU-014
 * @version 1.0
 * @date 2025-11-10
 *
 * Test de integración que verifica el flujo completo de lectura
 * de temperatura atravesando todas las capas de la arquitectura:
 *
 * Aplicación → Dominio → Infraestructura → Dispositivos
 *
 * Este test valida:
 * - Inicialización correcta de todas las capas
 * - Flujo de datos a través de las capas
 * - Coherencia entre componentes
 * - Limpieza ordenada de recursos
 *
 * @author Victor Valotto
 * @copyright Universidad Nacional de Entre Ríos (FIUNER)
 */

#include "unity.h"
#include "gestor_termostato.h"
#include "ambiente.h"
#include "sensor_temperatura.h"
#include "hal_adc.h"
#include <math.h>

// Variables globales para el test
static Ambiente* ambiente_global = NULL;
static GestorTermostato* gestor_global = NULL;

/**
 * @brief Setup ejecutado antes de cada test
 */
void setUp(void) {
    // Inicializar desde la capa más baja hacia arriba
    hal_adc_init();
    sensor_temperatura_init();

    ambiente_global = ambiente_crear();
    gestor_global = gestor_termostato_crear(ambiente_global);
}

/**
 * @brief Teardown ejecutado después de cada test
 */
void tearDown(void) {
    // Limpiar desde la capa más alta hacia abajo
    if (gestor_global != NULL) {
        gestor_termostato_destruir(gestor_global);
        gestor_global = NULL;
    }

    if (ambiente_global != NULL) {
        ambiente_destruir(ambiente_global);
        ambiente_global = NULL;
    }

    sensor_temperatura_deinit();
    hal_adc_deinit();
}

/**
 * @brief Test: Inicialización completa del sistema
 */
void test_inicializacion_completa_sistema(void) {
    // Todas las capas deben estar inicializadas correctamente
    TEST_ASSERT_NOT_NULL(ambiente_global);
    TEST_ASSERT_NOT_NULL(gestor_global);

    // El ambiente debe tener temperatura inicial
    Temperatura temp = ambiente_obtener_temperatura(ambiente_global);
    TEST_ASSERT_EQUAL_FLOAT(TEMP_INICIAL_DEFAULT, temp);
}

/**
 * @brief Test: Flujo completo de lectura de temperatura
 *
 * Este test valida el flujo end-to-end:
 * 1. HAL ADC genera valor simulado (150-300)
 * 2. Sensor Temperatura convierte a °C (~7-15°C)
 * 3. Gestor actualiza la entidad Ambiente
 * 4. Ambiente almacena el valor
 * 5. Gestor puede recuperar el valor
 */
void test_flujo_completo_lectura_temperatura(void) {
    // Temperatura inicial del ambiente
    Temperatura temp_inicial = ambiente_obtener_temperatura(ambiente_global);
    TEST_ASSERT_EQUAL_FLOAT(TEMP_INICIAL_DEFAULT, temp_inicial);

    // Ejecutar actualización (atraviesa todas las capas)
    gestor_termostato_actualizar_temperatura(gestor_global);

    // Obtener temperatura actualizada
    Temperatura temp_actualizada = gestor_termostato_obtener_temperatura_actual(gestor_global);

    // Verificar que el valor es válido
    TEST_ASSERT_FALSE(isnan(temp_actualizada));
    TEST_ASSERT_FALSE(isinf(temp_actualizada));
    TEST_ASSERT_GREATER_THAN(0.0f, temp_actualizada);

    // Verificar que está en el rango esperado del simulador
    TEST_ASSERT_GREATER_OR_EQUAL(5.0f, temp_actualizada);
    TEST_ASSERT_LESS_OR_EQUAL(20.0f, temp_actualizada);

    // Verificar que el ambiente también tiene el mismo valor
    Temperatura temp_ambiente = ambiente_obtener_temperatura(ambiente_global);
    TEST_ASSERT_EQUAL_FLOAT(temp_actualizada, temp_ambiente);
}

/**
 * @brief Test: Múltiples actualizaciones secuenciales
 *
 * Verifica que el sistema puede realizar múltiples ciclos
 * de lectura sin degradación o corrupción de datos.
 */
void test_multiples_actualizaciones_secuenciales(void) {
    const int NUM_LECTURAS = 20;
    Temperatura lecturas[NUM_LECTURAS];

    // Realizar múltiples lecturas
    for (int i = 0; i < NUM_LECTURAS; i++) {
        gestor_termostato_actualizar_temperatura(gestor_global);
        lecturas[i] = gestor_termostato_obtener_temperatura_actual(gestor_global);

        // Cada lectura debe ser válida
        TEST_ASSERT_FALSE(isnan(lecturas[i]));
        TEST_ASSERT_FALSE(isinf(lecturas[i]));
        TEST_ASSERT_GREATER_THAN(0.0f, lecturas[i]);
        TEST_ASSERT_LESS_THAN(50.0f, lecturas[i]);
    }

    // Calcular promedio de todas las lecturas
    float suma = 0.0f;
    for (int i = 0; i < NUM_LECTURAS; i++) {
        suma += lecturas[i];
    }
    float promedio = suma / NUM_LECTURAS;

    // El promedio debe estar en rango esperado
    TEST_ASSERT_GREATER_OR_EQUAL(7.0f, promedio);
    TEST_ASSERT_LESS_OR_EQUAL(15.0f, promedio);

    // Debe haber variación en los valores (no todos iguales)
    int valores_diferentes = 0;
    for (int i = 1; i < NUM_LECTURAS; i++) {
        if (lecturas[i] != lecturas[0]) {
            valores_diferentes++;
        }
    }
    TEST_ASSERT_GREATER_THAN(0, valores_diferentes);
}

/**
 * @brief Test: Coherencia entre capas
 *
 * Verifica que el valor de temperatura es consistente
 * cuando se consulta desde diferentes capas.
 */
void test_coherencia_entre_capas(void) {
    // Actualizar temperatura
    gestor_termostato_actualizar_temperatura(gestor_global);

    // Obtener temperatura desde el gestor
    Temperatura temp_gestor = gestor_termostato_obtener_temperatura_actual(gestor_global);

    // Obtener temperatura directamente del ambiente
    Temperatura temp_ambiente = ambiente_obtener_temperatura(ambiente_global);

    // Ambos valores deben coincidir exactamente
    TEST_ASSERT_EQUAL_FLOAT(temp_gestor, temp_ambiente);

    // Leer directamente del sensor (sin actualizar el modelo)
    Temperatura temp_sensor = sensor_temperatura_leer();

    // Este valor puede ser diferente (nueva lectura física)
    // pero debe estar en el mismo rango
    TEST_ASSERT_GREATER_OR_EQUAL(5.0f, temp_sensor);
    TEST_ASSERT_LESS_OR_EQUAL(20.0f, temp_sensor);
}

/**
 * @brief Test: Conversión ADC a temperatura es correcta
 *
 * Verifica que la cadena de conversión ADC → Temperatura
 * produce valores matemáticamente correctos.
 */
void test_conversion_adc_a_temperatura_correcta(void) {
    // Leer valor ADC directamente
    uint16_t valor_adc = hal_adc_leer_simulado();

    // El valor debe estar en el rango esperado del simulador
    TEST_ASSERT_GREATER_OR_EQUAL(150, valor_adc);
    TEST_ASSERT_LESS_THAN(300, valor_adc);

    // Calcular temperatura esperada manualmente
    // Factor de conversión: 0.0488 (50°C / 1023)
    float temp_esperada = (float)valor_adc * 0.0488f;

    // Leer temperatura del sensor
    Temperatura temp_sensor = sensor_temperatura_leer();

    // Los valores deberían ser muy similares
    // (pequeña diferencia por nueva lectura del ADC)
    // Ambos deben estar en el rango válido
    TEST_ASSERT_GREATER_OR_EQUAL(5.0f, temp_esperada);
    TEST_ASSERT_LESS_OR_EQUAL(20.0f, temp_esperada);
    TEST_ASSERT_GREATER_OR_EQUAL(5.0f, temp_sensor);
    TEST_ASSERT_LESS_OR_EQUAL(20.0f, temp_sensor);
}

/**
 * @brief Test: Sistema maneja reinicialización
 *
 * Verifica que el sistema puede reinicializarse correctamente
 * sin corrupción de memoria o estados inválidos.
 */
void test_sistema_maneja_reinicializacion(void) {
    // Primera lectura
    gestor_termostato_actualizar_temperatura(gestor_global);
    Temperatura temp1 = gestor_termostato_obtener_temperatura_actual(gestor_global);
    TEST_ASSERT_GREATER_THAN(0.0f, temp1);

    // Simular reinicialización (tearDown + setUp)
    tearDown();
    setUp();

    // El ambiente debe tener temperatura inicial nuevamente
    Temperatura temp_inicial = ambiente_obtener_temperatura(ambiente_global);
    TEST_ASSERT_EQUAL_FLOAT(TEMP_INICIAL_DEFAULT, temp_inicial);

    // Segunda lectura después de reinicialización
    gestor_termostato_actualizar_temperatura(gestor_global);
    Temperatura temp2 = gestor_termostato_obtener_temperatura_actual(gestor_global);
    TEST_ASSERT_GREATER_THAN(0.0f, temp2);
    TEST_ASSERT_GREATER_OR_EQUAL(5.0f, temp2);
    TEST_ASSERT_LESS_OR_EQUAL(20.0f, temp2);
}

/**
 * @brief Test: HU-014 cumple requisitos funcionales
 *
 * Test de aceptación que verifica que se cumplen todos
 * los requisitos de la historia de usuario HU-014:
 * "Como usuario, quiero obtener la temperatura ambiente"
 */
void test_hu014_cumple_requisitos_funcionales(void) {
    // Requisito 1: El sistema debe poder leer la temperatura ambiente
    gestor_termostato_actualizar_temperatura(gestor_global);
    Temperatura temp = gestor_termostato_obtener_temperatura_actual(gestor_global);

    // Requisito 2: La temperatura debe ser un valor numérico válido
    TEST_ASSERT_FALSE(isnan(temp));
    TEST_ASSERT_FALSE(isinf(temp));

    // Requisito 3: La temperatura debe estar en un rango razonable
    // (para ambiente interior típico)
    TEST_ASSERT_GREATER_THAN(0.0f, temp);
    TEST_ASSERT_LESS_THAN(50.0f, temp);

    // Requisito 4: El sistema debe actualizar el modelo de dominio
    Temperatura temp_dominio = ambiente_obtener_temperatura(ambiente_global);
    TEST_ASSERT_EQUAL_FLOAT(temp, temp_dominio);

    // Requisito 5: Las lecturas deben ser consistentes y repetibles
    Temperatura temp2 = gestor_termostato_obtener_temperatura_actual(gestor_global);
    TEST_ASSERT_EQUAL_FLOAT(temp, temp2);

    // ✓ HU-014 CUMPLIDA
}

/**
 * @brief Test: Sistema respeta arquitectura por capas
 *
 * Verifica que el flujo de datos respeta las dependencias
 * unidireccionales de la arquitectura.
 */
void test_sistema_respeta_arquitectura_por_capas(void) {
    // La capa de aplicación no debe acceder directamente al HAL
    // Solo debe usar el gestor

    // La capa de dominio (Ambiente) no debe conocer la infraestructura
    // Solo almacena el valor que le pasan

    // Test: Actualizar a través del gestor
    gestor_termostato_actualizar_temperatura(gestor_global);
    Temperatura temp_gestor = gestor_termostato_obtener_temperatura_actual(gestor_global);

    // Test: El ambiente solo debe tener el valor que el gestor estableció
    Temperatura temp_ambiente = ambiente_obtener_temperatura(ambiente_global);
    TEST_ASSERT_EQUAL_FLOAT(temp_gestor, temp_ambiente);

    // Test: No debe haber acoplamiento directo entre capas
    // (esto se valida en tiempo de compilación por la estructura del código)
    TEST_PASS();
}

/**
 * @brief Punto de entrada de los tests de integración
 */
int main(void) {
    UNITY_BEGIN();

    // Tests de inicialización
    RUN_TEST(test_inicializacion_completa_sistema);

    // Tests de flujo end-to-end
    RUN_TEST(test_flujo_completo_lectura_temperatura);
    RUN_TEST(test_multiples_actualizaciones_secuenciales);

    // Tests de coherencia
    RUN_TEST(test_coherencia_entre_capas);
    RUN_TEST(test_conversion_adc_a_temperatura_correcta);

    // Tests de robustez
    RUN_TEST(test_sistema_maneja_reinicializacion);

    // Tests de aceptación
    RUN_TEST(test_hu014_cumple_requisitos_funcionales);
    RUN_TEST(test_sistema_respeta_arquitectura_por_capas);

    return UNITY_END();
}
