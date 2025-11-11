/**
 * @file test_ambiente.c
 * @brief Tests unitarios para la entidad Ambiente
 * @version 1.0
 * @date 2025-11-10
 *
 * Suite de tests para verificar el correcto funcionamiento
 * de la entidad de dominio Ambiente.
 *
 * @author Victor Valotto
 * @copyright Universidad Nacional de Entre Ríos (FIUNER)
 */

#include "unity.h"
#include "ambiente.h"

/**
 * @brief Setup ejecutado antes de cada test
 */
void setUp(void) {
    // Nada que hacer por ahora
}

/**
 * @brief Teardown ejecutado después de cada test
 */
void tearDown(void) {
    // Nada que hacer por ahora
}

/**
 * @brief Test: ambiente_crear() retorna un puntero válido
 */
void test_ambiente_crear_retorna_puntero_valido(void) {
    Ambiente* ambiente = ambiente_crear();

    TEST_ASSERT_NOT_NULL(ambiente);

    ambiente_destruir(ambiente);
}

/**
 * @brief Test: ambiente_crear() inicializa con temperatura por defecto
 */
void test_ambiente_crear_inicializa_temperatura_default(void) {
    Ambiente* ambiente = ambiente_crear();

    Temperatura temp = ambiente_obtener_temperatura(ambiente);

    TEST_ASSERT_EQUAL_FLOAT(TEMP_INICIAL_DEFAULT, temp);

    ambiente_destruir(ambiente);
}

/**
 * @brief Test: ambiente_establecer_temperatura() actualiza correctamente
 */
void test_ambiente_establecer_temperatura_actualiza_valor(void) {
    Ambiente* ambiente = ambiente_crear();

    ambiente_establecer_temperatura(ambiente, 25.5f);
    Temperatura temp = ambiente_obtener_temperatura(ambiente);

    TEST_ASSERT_EQUAL_FLOAT(25.5f, temp);

    ambiente_destruir(ambiente);
}

/**
 * @brief Test: ambiente_obtener_temperatura() retorna el valor correcto
 */
void test_ambiente_obtener_temperatura_retorna_valor_correcto(void) {
    Ambiente* ambiente = ambiente_crear();

    ambiente_establecer_temperatura(ambiente, 18.3f);

    TEST_ASSERT_EQUAL_FLOAT(18.3f, ambiente_obtener_temperatura(ambiente));

    ambiente_destruir(ambiente);
}

/**
 * @brief Test: ambiente_destruir() con NULL no causa problemas
 */
void test_ambiente_destruir_con_null_no_falla(void) {
    // Este test verifica que pasar NULL no cause crash
    ambiente_destruir(NULL);

    // Si llegamos aquí sin crash, el test pasa
    TEST_PASS();
}

/**
 * @brief Test: ambiente_establecer_temperatura() con NULL no causa crash
 */
void test_ambiente_establecer_con_null_no_falla(void) {
    ambiente_establecer_temperatura(NULL, 25.0f);

    // Si llegamos aquí sin crash, el test pasa
    TEST_PASS();
}

/**
 * @brief Test: ambiente_obtener_temperatura() con NULL retorna default
 */
void test_ambiente_obtener_con_null_retorna_default(void) {
    Temperatura temp = ambiente_obtener_temperatura(NULL);

    TEST_ASSERT_EQUAL_FLOAT(TEMP_INICIAL_DEFAULT, temp);
}

/**
 * @brief Test: Múltiples cambios de temperatura
 */
void test_ambiente_multiples_cambios_temperatura(void) {
    Ambiente* ambiente = ambiente_crear();

    // Cambio 1
    ambiente_establecer_temperatura(ambiente, 15.0f);
    TEST_ASSERT_EQUAL_FLOAT(15.0f, ambiente_obtener_temperatura(ambiente));

    // Cambio 2
    ambiente_establecer_temperatura(ambiente, 30.5f);
    TEST_ASSERT_EQUAL_FLOAT(30.5f, ambiente_obtener_temperatura(ambiente));

    // Cambio 3
    ambiente_establecer_temperatura(ambiente, 22.0f);
    TEST_ASSERT_EQUAL_FLOAT(22.0f, ambiente_obtener_temperatura(ambiente));

    ambiente_destruir(ambiente);
}

/**
 * @brief Punto de entrada de los tests
 */
int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_ambiente_crear_retorna_puntero_valido);
    RUN_TEST(test_ambiente_crear_inicializa_temperatura_default);
    RUN_TEST(test_ambiente_establecer_temperatura_actualiza_valor);
    RUN_TEST(test_ambiente_obtener_temperatura_retorna_valor_correcto);
    RUN_TEST(test_ambiente_destruir_con_null_no_falla);
    RUN_TEST(test_ambiente_establecer_con_null_no_falla);
    RUN_TEST(test_ambiente_obtener_con_null_retorna_default);
    RUN_TEST(test_ambiente_multiples_cambios_temperatura);

    return UNITY_END();
}
