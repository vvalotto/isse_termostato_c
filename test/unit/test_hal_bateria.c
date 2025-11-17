/**
 * @file test_hal_bateria.c
 * @brief Tests unitarios para el HAL de batería
 * @version 1.0
 * @date 2025-11-17
 *
 * Suite de pruebas unitarias para verificar el comportamiento
 * del HAL de lectura de voltaje de batería (versión simulada).
 *
 * @author Victor Valotto
 * @copyright Universidad Nacional de Entre Ríos (FIUNER)
 */

#include "unity.h"
#include "hal_bateria.h"

/**
 * @brief Configuración antes de cada test
 */
void setUp(void) {
    // Inicializar HAL antes de cada test
    hal_bateria_init();
}

/**
 * @brief Limpieza después de cada test
 */
void tearDown(void) {
    // Desinicializar HAL después de cada test
    hal_bateria_deinit();
}

/**
 * @test Verifica que la inicialización no causa errores
 */
void test_hal_bateria_init_no_causa_error(void) {
    // Si llegamos aquí sin crash, el test pasa
    TEST_ASSERT_TRUE(true);
}

/**
 * @test Verifica que la desinicialización no causa errores
 */
void test_hal_bateria_deinit_no_causa_error(void) {
    hal_bateria_deinit();
    // Si llegamos aquí sin crash, el test pasa
    TEST_ASSERT_TRUE(true);
}

/**
 * @test Verifica que leer_voltaje retorna un valor válido
 */
void test_hal_bateria_leer_voltaje_retorna_valor_valido(void) {
    Voltaje voltaje = hal_bateria_leer_voltaje();

    // El voltaje debe estar en el rango de batería Li-ion
    TEST_ASSERT_GREATER_OR_EQUAL(VOLTAJE_BATERIA_MIN_MV, voltaje);
    TEST_ASSERT_LESS_OR_EQUAL(VOLTAJE_BATERIA_MAX_MV, voltaje);
}

/**
 * @test Verifica que el voltaje está en rango mínimo-máximo (3000-4200 mV)
 */
void test_hal_bateria_voltaje_en_rango_correcto(void) {
    Voltaje voltaje = hal_bateria_leer_voltaje();

    // Verificar límite inferior (3000 mV = 3.0V)
    TEST_ASSERT_GREATER_OR_EQUAL_UINT16(3000, voltaje);

    // Verificar límite superior (4200 mV = 4.2V)
    TEST_ASSERT_LESS_OR_EQUAL_UINT16(4200, voltaje);
}

/**
 * @test Verifica que múltiples lecturas retornan valores en rango
 */
void test_hal_bateria_multiples_lecturas_en_rango(void) {
    const int NUM_LECTURAS = 10;

    for (int i = 0; i < NUM_LECTURAS; i++) {
        Voltaje voltaje = hal_bateria_leer_voltaje();

        // Cada lectura debe estar en el rango válido
        TEST_ASSERT_GREATER_OR_EQUAL(VOLTAJE_BATERIA_MIN_MV, voltaje);
        TEST_ASSERT_LESS_OR_EQUAL(VOLTAJE_BATERIA_MAX_MV, voltaje);
    }
}

/**
 * @test Verifica que las lecturas tienen variación (simulador)
 *
 * El simulador debe generar valores diferentes para simular
 * variación natural del voltaje de batería.
 */
void test_hal_bateria_lecturas_tienen_variacion(void) {
    const int NUM_LECTURAS = 20;
    Voltaje lecturas[NUM_LECTURAS];
    bool hay_variacion = false;

    // Tomar múltiples lecturas
    for (int i = 0; i < NUM_LECTURAS; i++) {
        lecturas[i] = hal_bateria_leer_voltaje();
    }

    // Verificar que al menos algunas son diferentes
    for (int i = 1; i < NUM_LECTURAS; i++) {
        if (lecturas[i] != lecturas[0]) {
            hay_variacion = true;
            break;
        }
    }

    // Debe haber al menos alguna variación en 20 lecturas
    TEST_ASSERT_TRUE_MESSAGE(hay_variacion,
        "El simulador debe generar valores variados");
}

/**
 * @test Verifica que se puede reinicializar el HAL
 */
void test_hal_bateria_reinicializacion_funciona(void) {
    Voltaje v1, v2;

    // Primera lectura
    v1 = hal_bateria_leer_voltaje();

    // Desinicializar y reinicializar
    hal_bateria_deinit();
    hal_bateria_init();

    // Segunda lectura después de reiniciar
    v2 = hal_bateria_leer_voltaje();

    // Ambas lecturas deben estar en rango válido
    TEST_ASSERT_GREATER_OR_EQUAL(VOLTAJE_BATERIA_MIN_MV, v1);
    TEST_ASSERT_LESS_OR_EQUAL(VOLTAJE_BATERIA_MAX_MV, v1);
    TEST_ASSERT_GREATER_OR_EQUAL(VOLTAJE_BATERIA_MIN_MV, v2);
    TEST_ASSERT_LESS_OR_EQUAL(VOLTAJE_BATERIA_MAX_MV, v2);
}

/**
 * @test Verifica los valores de las constantes de voltaje
 */
void test_hal_bateria_constantes_correctas(void) {
    // Verificar que las constantes tienen valores esperados
    TEST_ASSERT_EQUAL_UINT16(4200, VOLTAJE_BATERIA_MAX_MV);
    TEST_ASSERT_EQUAL_UINT16(3000, VOLTAJE_BATERIA_MIN_MV);
    TEST_ASSERT_EQUAL_UINT16(3700, VOLTAJE_BATERIA_NOM_MV);

    // Verificar relaciones lógicas
    TEST_ASSERT_LESS_THAN(VOLTAJE_BATERIA_MAX_MV, VOLTAJE_BATERIA_NOM_MV);
    TEST_ASSERT_GREATER_THAN(VOLTAJE_BATERIA_MIN_MV, VOLTAJE_BATERIA_NOM_MV);
}

/**
 * @test Verifica el rango de valores simulados
 */
void test_hal_bateria_rango_simulado_correcto(void) {
    const int NUM_MUESTRAS = 50;
    Voltaje min_leido = VOLTAJE_BATERIA_MAX_MV;
    Voltaje max_leido = VOLTAJE_BATERIA_MIN_MV;

    // Tomar múltiples muestras para encontrar min/max
    for (int i = 0; i < NUM_MUESTRAS; i++) {
        Voltaje v = hal_bateria_leer_voltaje();

        if (v < min_leido) min_leido = v;
        if (v > max_leido) max_leido = v;
    }

    // El mínimo leído debe ser >= límite inferior
    TEST_ASSERT_GREATER_OR_EQUAL(VOLTAJE_BATERIA_MIN_MV, min_leido);

    // El máximo leído debe ser <= límite superior
    TEST_ASSERT_LESS_OR_EQUAL(VOLTAJE_BATERIA_MAX_MV, max_leido);

    // Debe haber un rango razonable de valores
    // (no todos iguales o muy cercanos)
    Voltaje rango_leido = max_leido - min_leido;
    TEST_ASSERT_GREATER_THAN(100, rango_leido);  // Al menos 100mV de variación
}

/**
 * @test Verifica que tipo Voltaje es adecuado para el rango
 */
void test_hal_bateria_tipo_voltaje_adecuado(void) {
    // Voltaje es uint16_t, que puede representar 0-65535
    // El rango de batería (3000-4200) está bien dentro de este límite
    Voltaje v_min = VOLTAJE_BATERIA_MIN_MV;
    Voltaje v_max = VOLTAJE_BATERIA_MAX_MV;

    TEST_ASSERT_EQUAL_UINT16(3000, v_min);
    TEST_ASSERT_EQUAL_UINT16(4200, v_max);

    // Verificar que no hay overflow
    TEST_ASSERT_LESS_THAN(65536, v_max);
}

/**
 * @brief Punto de entrada de los tests
 */
int main(void) {
    UNITY_BEGIN();

    // Tests de inicialización
    RUN_TEST(test_hal_bateria_init_no_causa_error);
    RUN_TEST(test_hal_bateria_deinit_no_causa_error);
    RUN_TEST(test_hal_bateria_reinicializacion_funciona);

    // Tests de lectura de voltaje
    RUN_TEST(test_hal_bateria_leer_voltaje_retorna_valor_valido);
    RUN_TEST(test_hal_bateria_voltaje_en_rango_correcto);
    RUN_TEST(test_hal_bateria_multiples_lecturas_en_rango);
    RUN_TEST(test_hal_bateria_lecturas_tienen_variacion);
    RUN_TEST(test_hal_bateria_rango_simulado_correcto);

    // Tests de constantes y tipos
    RUN_TEST(test_hal_bateria_constantes_correctas);
    RUN_TEST(test_hal_bateria_tipo_voltaje_adecuado);

    return UNITY_END();
}
