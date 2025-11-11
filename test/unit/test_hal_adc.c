/**
 * @file test_hal_adc.c
 * @brief Tests unitarios para HAL ADC
 * @version 1.0
 * @date 2025-11-10
 *
 * Suite de tests para verificar el correcto funcionamiento
 * del HAL ADC simulado.
 *
 * @author Victor Valotto
 * @copyright Universidad Nacional de Entre Ríos (FIUNER)
 */

#include "unity.h"
#include "hal_adc.h"
#include <stdbool.h>

/**
 * @brief Setup ejecutado antes de cada test
 */
void setUp(void) {
    hal_adc_init();
}

/**
 * @brief Teardown ejecutado después de cada test
 */
void tearDown(void) {
    hal_adc_deinit();
}

/**
 * @brief Test: hal_adc_init() no causa errores
 */
void test_hal_adc_init_no_falla(void) {
    // Si llegamos aquí, init() funcionó
    TEST_PASS();
}

/**
 * @brief Test: hal_adc_leer_simulado() retorna valor en rango válido de 10 bits
 */
void test_hal_adc_leer_retorna_valor_valido(void) {
    uint16_t valor = hal_adc_leer_simulado();

    // Debe estar en rango de 10 bits (0-1023)
    TEST_ASSERT_LESS_OR_EQUAL(HAL_ADC_MAX_VALUE, valor);
    TEST_ASSERT_GREATER_OR_EQUAL(0, valor);
}

/**
 * @brief Test: hal_adc_leer_simulado() retorna valores en rango esperado
 */
void test_hal_adc_leer_retorna_en_rango_esperado(void) {
    uint16_t valor = hal_adc_leer_simulado();

    // El simulador debe retornar valores entre 150-300
    TEST_ASSERT_GREATER_OR_EQUAL(150, valor);
    TEST_ASSERT_LESS_THAN(300, valor);
}

/**
 * @brief Test: múltiples lecturas producen variación
 *
 * Este test verifica que el simulador no retorna siempre el mismo valor,
 * lo cual sería poco realista.
 */
void test_hal_adc_multiples_lecturas_varian(void) {
    uint16_t valores[10];
    bool hay_variacion = false;

    // Leer 10 valores
    for (int i = 0; i < 10; i++) {
        valores[i] = hal_adc_leer_simulado();
    }

    // Verificar que al menos 2 valores sean diferentes
    // (Probabilidad extremadamente alta con rango de 150 valores)
    for (int i = 1; i < 10; i++) {
        if (valores[i] != valores[0]) {
            hay_variacion = true;
            break;
        }
    }

    TEST_ASSERT_TRUE(hay_variacion);
}

/**
 * @brief Test: valores son consistentes en rango
 *
 * Verifica que múltiples lecturas siempre están en el rango esperado
 */
void test_hal_adc_consistencia_de_rango(void) {
    // Leer 20 valores y verificar que todos están en rango
    for (int i = 0; i < 20; i++) {
        uint16_t valor = hal_adc_leer_simulado();

        TEST_ASSERT_GREATER_OR_EQUAL(150, valor);
        TEST_ASSERT_LESS_THAN(300, valor);
        TEST_ASSERT_LESS_OR_EQUAL(HAL_ADC_MAX_VALUE, valor);
    }
}

/**
 * @brief Test: hal_adc_deinit() no causa errores
 */
void test_hal_adc_deinit_no_falla(void) {
    hal_adc_deinit();
    // Si llegamos aquí, deinit() funcionó
    TEST_PASS();
}

/**
 * @brief Test: reinicialización funciona correctamente
 */
void test_hal_adc_reinicializacion(void) {
    // Primera inicialización (ya hecha en setUp)
    uint16_t valor1 = hal_adc_leer_simulado();

    // Desinicializar
    hal_adc_deinit();

    // Reinicializar
    hal_adc_init();

    // Leer nuevamente
    uint16_t valor2 = hal_adc_leer_simulado();

    // Ambos valores deben estar en rango válido
    TEST_ASSERT_GREATER_OR_EQUAL(150, valor1);
    TEST_ASSERT_LESS_THAN(300, valor1);
    TEST_ASSERT_GREATER_OR_EQUAL(150, valor2);
    TEST_ASSERT_LESS_THAN(300, valor2);
}

/**
 * @brief Punto de entrada de los tests
 */
int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_hal_adc_init_no_falla);
    RUN_TEST(test_hal_adc_leer_retorna_valor_valido);
    RUN_TEST(test_hal_adc_leer_retorna_en_rango_esperado);
    RUN_TEST(test_hal_adc_multiples_lecturas_varian);
    RUN_TEST(test_hal_adc_consistencia_de_rango);
    RUN_TEST(test_hal_adc_deinit_no_falla);
    RUN_TEST(test_hal_adc_reinicializacion);

    return UNITY_END();
}
