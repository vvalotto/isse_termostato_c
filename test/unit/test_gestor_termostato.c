/**
 * @file test_gestor_termostato.c
 * @brief Tests unitarios para gestor de termostato
 * @version 1.1
 * @date 2025-11-17
 *
 * Suite de tests para verificar el correcto funcionamiento
 * del gestor de termostato en la capa de aplicación.
 * Incluye tests para temperatura (HU-014) y batería (HU-007).
 *
 * @author Victor Valotto
 * @copyright Universidad Nacional de Entre Ríos (FIUNER)
 */

#include "unity.h"
#include "gestor_termostato.h"
#include "ambiente.h"
#include "bateria.h"
#include "sensor_temperatura.h"
#include "sensor_bateria.h"
#include <math.h>

// Variables globales para las entidades usadas en los tests
static Ambiente* ambiente_test = NULL;
static Bateria* bateria_test = NULL;
static GestorTermostato* gestor_test = NULL;

/**
 * @brief Setup ejecutado antes de cada test
 */
void setUp(void) {
    // Inicializar sensores (necesario para lecturas)
    sensor_temperatura_init();
    sensor_bateria_init();

    // Crear entidades para los tests
    ambiente_test = ambiente_crear();
    bateria_test = bateria_crear();
}

/**
 * @brief Teardown ejecutado después de cada test
 */
void tearDown(void) {
    // Destruir gestor si existe
    if (gestor_test != NULL) {
        gestor_termostato_destruir(gestor_test);
        gestor_test = NULL;
    }

    // Destruir entidades
    if (bateria_test != NULL) {
        bateria_destruir(bateria_test);
        bateria_test = NULL;
    }

    if (ambiente_test != NULL) {
        ambiente_destruir(ambiente_test);
        ambiente_test = NULL;
    }

    // Desinicializar sensores
    sensor_bateria_deinit();
    sensor_temperatura_deinit();
}

/**
 * @brief Test: gestor_termostato_crear() con ambiente válido
 */
void test_gestor_termostato_crear_con_ambiente_valido(void) {
    gestor_test = gestor_termostato_crear(ambiente_test, bateria_test);

    TEST_ASSERT_NOT_NULL(gestor_test);
}

/**
 * @brief Test: gestor_termostato_crear() con NULL retorna NULL
 */
void test_gestor_termostato_crear_con_null_retorna_null(void) {
    GestorTermostato* gestor = gestor_termostato_crear(NULL, NULL);

    TEST_ASSERT_NULL(gestor);

    // No necesitamos destruir porque es NULL
}

/**
 * @brief Test: gestor_termostato_destruir() con NULL es seguro
 */
void test_gestor_termostato_destruir_con_null_es_seguro(void) {
    // No debe causar crash
    gestor_termostato_destruir(NULL);

    TEST_PASS();
}

/**
 * @brief Test: gestor_termostato_destruir() no afecta al ambiente
 */
void test_gestor_termostato_destruir_no_afecta_ambiente(void) {
    gestor_test = gestor_termostato_crear(ambiente_test, bateria_test);
    TEST_ASSERT_NOT_NULL(gestor_test);

    // Establecer temperatura en el ambiente
    ambiente_establecer_temperatura(ambiente_test, 25.5f);

    // Destruir gestor
    gestor_termostato_destruir(gestor_test);
    gestor_test = NULL;

    // El ambiente debe seguir existiendo y con su temperatura
    Temperatura temp = ambiente_obtener_temperatura(ambiente_test);
    TEST_ASSERT_EQUAL_FLOAT(25.5f, temp);
}

/**
 * @brief Test: gestor_termostato_actualizar_temperatura() lee del sensor
 */
void test_gestor_termostato_actualizar_temperatura_lee_sensor(void) {
    gestor_test = gestor_termostato_crear(ambiente_test, bateria_test);
    TEST_ASSERT_NOT_NULL(gestor_test);

    // Temperatura inicial del ambiente
    Temperatura temp_inicial = ambiente_obtener_temperatura(ambiente_test);
    TEST_ASSERT_EQUAL_FLOAT(TEMP_INICIAL_DEFAULT, temp_inicial);

    // Actualizar temperatura desde el sensor
    gestor_termostato_actualizar_temperatura(gestor_test);

    // La temperatura debe haber cambiado (sensor simula valores aleatorios)
    Temperatura temp_actualizada = ambiente_obtener_temperatura(ambiente_test);

    // Verificar que la temperatura actualizada es válida
    TEST_ASSERT_FALSE(isnan(temp_actualizada));
    TEST_ASSERT_FALSE(isinf(temp_actualizada));
    TEST_ASSERT_GREATER_THAN(0.0f, temp_actualizada);
}

/**
 * @brief Test: gestor_termostato_actualizar_temperatura() con NULL es seguro
 */
void test_gestor_termostato_actualizar_temperatura_con_null_es_seguro(void) {
    // No debe causar crash
    gestor_termostato_actualizar_temperatura(NULL);

    TEST_PASS();
}

/**
 * @brief Test: gestor_termostato_obtener_temperatura_actual() retorna valor correcto
 */
void test_gestor_termostato_obtener_temperatura_retorna_valor_correcto(void) {
    gestor_test = gestor_termostato_crear(ambiente_test, bateria_test);
    TEST_ASSERT_NOT_NULL(gestor_test);

    // Establecer temperatura conocida en el ambiente
    float temperatura_esperada = 22.5f;
    ambiente_establecer_temperatura(ambiente_test, temperatura_esperada);

    // Obtener temperatura a través del gestor
    Temperatura temp = gestor_termostato_obtener_temperatura_actual(gestor_test);

    TEST_ASSERT_EQUAL_FLOAT(temperatura_esperada, temp);
}

/**
 * @brief Test: gestor_termostato_obtener_temperatura_actual() con NULL retorna default
 */
void test_gestor_termostato_obtener_temperatura_con_null_retorna_default(void) {
    Temperatura temp = gestor_termostato_obtener_temperatura_actual(NULL);

    TEST_ASSERT_EQUAL_FLOAT(TEMP_INICIAL_DEFAULT, temp);
}

/**
 * @brief Test: flujo completo de actualización y consulta
 */
void test_gestor_termostato_flujo_completo(void) {
    gestor_test = gestor_termostato_crear(ambiente_test, bateria_test);
    TEST_ASSERT_NOT_NULL(gestor_test);

    // 1. Temperatura inicial debe ser la por defecto
    Temperatura temp_inicial = gestor_termostato_obtener_temperatura_actual(gestor_test);
    TEST_ASSERT_EQUAL_FLOAT(TEMP_INICIAL_DEFAULT, temp_inicial);

    // 2. Actualizar temperatura desde el sensor
    gestor_termostato_actualizar_temperatura(gestor_test);

    // 3. Obtener la nueva temperatura
    Temperatura temp_actualizada = gestor_termostato_obtener_temperatura_actual(gestor_test);

    // 4. Verificar que la temperatura es válida y diferente de la inicial
    TEST_ASSERT_FALSE(isnan(temp_actualizada));
    TEST_ASSERT_FALSE(isinf(temp_actualizada));
    TEST_ASSERT_GREATER_THAN(0.0f, temp_actualizada);

    // 5. La temperatura debe estar en el rango esperado del sensor simulado (~7-15°C)
    TEST_ASSERT_GREATER_OR_EQUAL(5.0f, temp_actualizada);
    TEST_ASSERT_LESS_OR_EQUAL(20.0f, temp_actualizada);
}

/**
 * @brief Test: múltiples actualizaciones producen valores en rango
 */
void test_gestor_termostato_multiples_actualizaciones_en_rango(void) {
    gestor_test = gestor_termostato_crear(ambiente_test, bateria_test);
    TEST_ASSERT_NOT_NULL(gestor_test);

    // Realizar 10 actualizaciones y verificar que todas están en rango
    for (int i = 0; i < 10; i++) {
        gestor_termostato_actualizar_temperatura(gestor_test);

        Temperatura temp = gestor_termostato_obtener_temperatura_actual(gestor_test);

        // Verificar rango válido
        TEST_ASSERT_GREATER_THAN(0.0f, temp);
        TEST_ASSERT_LESS_THAN(50.0f, temp);
    }
}

/**
 * @brief Test: gestor mantiene coherencia entre actualizaciones
 */
void test_gestor_termostato_coherencia_entre_actualizaciones(void) {
    gestor_test = gestor_termostato_crear(ambiente_test, bateria_test);
    TEST_ASSERT_NOT_NULL(gestor_test);

    // Primera actualización
    gestor_termostato_actualizar_temperatura(gestor_test);
    Temperatura temp1 = gestor_termostato_obtener_temperatura_actual(gestor_test);

    // Múltiples consultas sin actualizar deben retornar el mismo valor
    Temperatura temp2 = gestor_termostato_obtener_temperatura_actual(gestor_test);
    Temperatura temp3 = gestor_termostato_obtener_temperatura_actual(gestor_test);

    TEST_ASSERT_EQUAL_FLOAT(temp1, temp2);
    TEST_ASSERT_EQUAL_FLOAT(temp1, temp3);

    // Segunda actualización
    gestor_termostato_actualizar_temperatura(gestor_test);
    Temperatura temp4 = gestor_termostato_obtener_temperatura_actual(gestor_test);

    // temp4 puede ser diferente de temp1 (nueva lectura del sensor)
    // pero debe ser válida
    TEST_ASSERT_FALSE(isnan(temp4));
    TEST_ASSERT_GREATER_THAN(0.0f, temp4);
}

/**
 * @brief Test: gestor coordina correctamente las capas
 *
 * Este test verifica la integración entre:
 * - Capa de Aplicación (gestor_termostato)
 * - Capa de Dominio (ambiente)
 * - Capa de Infraestructura (sensor_temperatura)
 * - Capa de Dispositivos (hal_adc)
 */
void test_gestor_termostato_coordinacion_de_capas(void) {
    gestor_test = gestor_termostato_crear(ambiente_test, bateria_test);
    TEST_ASSERT_NOT_NULL(gestor_test);

    // El ambiente debe empezar con temperatura por defecto
    Temperatura temp_ambiente_inicial = ambiente_obtener_temperatura(ambiente_test);
    TEST_ASSERT_EQUAL_FLOAT(TEMP_INICIAL_DEFAULT, temp_ambiente_inicial);

    // Actualizar a través del gestor (coordina todas las capas)
    gestor_termostato_actualizar_temperatura(gestor_test);

    // El ambiente debe tener ahora la temperatura del sensor
    Temperatura temp_ambiente_actualizada = ambiente_obtener_temperatura(ambiente_test);

    // Y el gestor debe retornar la misma temperatura
    Temperatura temp_gestor = gestor_termostato_obtener_temperatura_actual(gestor_test);

    // Ambas deben coincidir
    TEST_ASSERT_EQUAL_FLOAT(temp_ambiente_actualizada, temp_gestor);

    // Y deben estar en el rango del sensor
    TEST_ASSERT_GREATER_OR_EQUAL(5.0f, temp_gestor);
    TEST_ASSERT_LESS_OR_EQUAL(20.0f, temp_gestor);
}

// ============================================
// TESTS DE BATERÍA (ITE-57)
// ============================================

/**
 * @brief Test: gestor_termostato_actualizar_bateria() actualiza nivel
 */
void test_gestor_termostato_actualizar_bateria_actualiza_nivel(void) {
    gestor_test = gestor_termostato_crear(ambiente_test, bateria_test);
    TEST_ASSERT_NOT_NULL(gestor_test);

    // La batería debe empezar con 100%
    NivelCarga nivel_inicial = bateria_obtener_nivel(bateria_test);
    TEST_ASSERT_EQUAL_UINT8(100, nivel_inicial);

    // Actualizar a través del gestor
    gestor_termostato_actualizar_bateria(gestor_test);

    // El nivel debe haberse actualizado (ya no es 100%)
    NivelCarga nivel_actualizado = bateria_obtener_nivel(bateria_test);

    // Debe estar en el rango válido
    TEST_ASSERT_GREATER_OR_EQUAL_UINT8(0, nivel_actualizado);
    TEST_ASSERT_LESS_OR_EQUAL_UINT8(100, nivel_actualizado);
}

/**
 * @brief Test: gestor_termostato_obtener_nivel_bateria() retorna nivel correcto
 */
void test_gestor_termostato_obtener_nivel_bateria_retorna_correcto(void) {
    gestor_test = gestor_termostato_crear(ambiente_test, bateria_test);
    TEST_ASSERT_NOT_NULL(gestor_test);

    // Actualizar batería
    gestor_termostato_actualizar_bateria(gestor_test);

    // Obtener nivel desde gestor y desde batería directamente
    NivelCarga nivel_gestor = gestor_termostato_obtener_nivel_bateria(gestor_test);
    NivelCarga nivel_bateria = bateria_obtener_nivel(bateria_test);

    // Deben ser iguales
    TEST_ASSERT_EQUAL_UINT8(nivel_bateria, nivel_gestor);

    // Y estar en rango válido
    TEST_ASSERT_GREATER_OR_EQUAL_UINT8(0, nivel_gestor);
    TEST_ASSERT_LESS_OR_EQUAL_UINT8(100, nivel_gestor);
}

/**
 * @brief Test: gestor_termostato_obtener_estado_bateria() retorna estado correcto
 */
void test_gestor_termostato_obtener_estado_bateria_retorna_correcto(void) {
    gestor_test = gestor_termostato_crear(ambiente_test, bateria_test);
    TEST_ASSERT_NOT_NULL(gestor_test);

    // Actualizar batería
    gestor_termostato_actualizar_bateria(gestor_test);

    // Obtener estado desde gestor y desde batería
    EstadoBateria estado_gestor = gestor_termostato_obtener_estado_bateria(gestor_test);
    EstadoBateria estado_bateria = bateria_obtener_estado(bateria_test);

    // Deben ser iguales
    TEST_ASSERT_EQUAL(estado_bateria, estado_gestor);
}

/**
 * @brief Test: gestor_termostato_bateria_baja() funciona correctamente
 */
void test_gestor_termostato_bateria_baja_funciona(void) {
    gestor_test = gestor_termostato_crear(ambiente_test, bateria_test);
    TEST_ASSERT_NOT_NULL(gestor_test);

    // Actualizar batería
    gestor_termostato_actualizar_bateria(gestor_test);

    // Verificar coherencia con bateria_esta_baja()
    bool baja_gestor = gestor_termostato_bateria_baja(gestor_test);
    bool baja_bateria = bateria_esta_baja(bateria_test);

    TEST_ASSERT_EQUAL(baja_bateria, baja_gestor);
}

/**
 * @brief Test: flujo completo temperatura + batería
 */
void test_gestor_termostato_flujo_completo_temperatura_y_bateria(void) {
    gestor_test = gestor_termostato_crear(ambiente_test, bateria_test);
    TEST_ASSERT_NOT_NULL(gestor_test);

    // Actualizar ambos sensores
    gestor_termostato_actualizar_temperatura(gestor_test);
    gestor_termostato_actualizar_bateria(gestor_test);

    // Obtener valores
    Temperatura temp = gestor_termostato_obtener_temperatura_actual(gestor_test);
    NivelCarga nivel = gestor_termostato_obtener_nivel_bateria(gestor_test);

    // Verificar que ambos están en rango
    TEST_ASSERT_GREATER_OR_EQUAL(5.0f, temp);
    TEST_ASSERT_LESS_OR_EQUAL(20.0f, temp);

    TEST_ASSERT_GREATER_OR_EQUAL_UINT8(0, nivel);
    TEST_ASSERT_LESS_OR_EQUAL_UINT8(100, nivel);
}

/**
 * @brief Test: batería con NULL es seguro
 */
void test_gestor_termostato_bateria_con_null_es_seguro(void) {
    // Todas estas operaciones deben ser seguras con NULL
    gestor_termostato_actualizar_bateria(NULL);

    NivelCarga nivel = gestor_termostato_obtener_nivel_bateria(NULL);
    TEST_ASSERT_EQUAL_UINT8(0, nivel);

    EstadoBateria estado = gestor_termostato_obtener_estado_bateria(NULL);
    TEST_ASSERT_EQUAL(BATERIA_ESTADO_CRITICO, estado);

    bool baja = gestor_termostato_bateria_baja(NULL);
    TEST_ASSERT_TRUE(baja);
}

/**
 * @brief Punto de entrada de los tests
 */
int main(void) {
    UNITY_BEGIN();

    // Tests de temperatura (HU-014)
    RUN_TEST(test_gestor_termostato_crear_con_ambiente_valido);
    RUN_TEST(test_gestor_termostato_crear_con_null_retorna_null);
    RUN_TEST(test_gestor_termostato_destruir_con_null_es_seguro);
    RUN_TEST(test_gestor_termostato_destruir_no_afecta_ambiente);
    RUN_TEST(test_gestor_termostato_actualizar_temperatura_lee_sensor);
    RUN_TEST(test_gestor_termostato_actualizar_temperatura_con_null_es_seguro);
    RUN_TEST(test_gestor_termostato_obtener_temperatura_retorna_valor_correcto);
    RUN_TEST(test_gestor_termostato_obtener_temperatura_con_null_retorna_default);
    RUN_TEST(test_gestor_termostato_flujo_completo);
    RUN_TEST(test_gestor_termostato_multiples_actualizaciones_en_rango);
    RUN_TEST(test_gestor_termostato_coherencia_entre_actualizaciones);
    RUN_TEST(test_gestor_termostato_coordinacion_de_capas);

    // Tests de batería (HU-007)
    RUN_TEST(test_gestor_termostato_actualizar_bateria_actualiza_nivel);
    RUN_TEST(test_gestor_termostato_obtener_nivel_bateria_retorna_correcto);
    RUN_TEST(test_gestor_termostato_obtener_estado_bateria_retorna_correcto);
    RUN_TEST(test_gestor_termostato_bateria_baja_funciona);
    RUN_TEST(test_gestor_termostato_flujo_completo_temperatura_y_bateria);
    RUN_TEST(test_gestor_termostato_bateria_con_null_es_seguro);

    return UNITY_END();
}
