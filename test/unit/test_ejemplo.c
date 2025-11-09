/**
 * @file test_ejemplo.c
 * @brief Test de ejemplo para verificar Unity Framework
 *
 * Este test verifica que el framework Unity está correctamente instalado
 * y funcionando.
 */

#include "unity.h"

/**
 * @brief Configuración antes de cada test
 */
void setUp(void) {
    // Se ejecuta antes de cada test
}

/**
 * @brief Limpieza después de cada test
 */
void tearDown(void) {
    // Se ejecuta después de cada test
}

/**
 * @brief Test básico que siempre pasa
 */
void test_unity_framework_funciona(void) {
    TEST_ASSERT_TRUE(1);
    TEST_ASSERT_EQUAL(1, 1);
}

/**
 * @brief Test de suma simple
 */
void test_suma_basica(void) {
    int resultado = 2 + 2;
    TEST_ASSERT_EQUAL(4, resultado);
}

/**
 * @brief Test de comparación de strings
 */
void test_strings(void) {
    const char* esperado = "ISSE Termostato";
    const char* actual = "ISSE Termostato";
    TEST_ASSERT_EQUAL_STRING(esperado, actual);
}

/**
 * @brief Test de valores de punto flotante
 */
void test_valores_flotantes(void) {
    float temperatura = 20.5f;
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 20.5f, temperatura);
}

/**
 * @brief Función principal del test
 */
int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_unity_framework_funciona);
    RUN_TEST(test_suma_basica);
    RUN_TEST(test_strings);
    RUN_TEST(test_valores_flotantes);

    return UNITY_END();
}
