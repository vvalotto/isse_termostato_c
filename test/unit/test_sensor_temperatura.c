/**
 * @file test_sensor_temperatura.c
 * @brief Tests unitarios para sensor de temperatura
 * @version 1.0
 * @date 2025-11-10
 *
 * Suite de tests para verificar el correcto funcionamiento
 * del proxy de sensor de temperatura.
 *
 * @author Victor Valotto
 * @copyright Universidad Nacional de Entre Ríos (FIUNER)
 */

#include "unity.h"
#include "sensor_temperatura.h"
#include <math.h>

/**
 * @brief Setup ejecutado antes de cada test
 */
void setUp(void) {
    sensor_temperatura_init();
}

/**
 * @brief Teardown ejecutado después de cada test
 */
void tearDown(void) {
    sensor_temperatura_deinit();
}

/**
 * @brief Test: sensor_temperatura_init() no causa errores
 */
void test_sensor_temperatura_init_no_falla(void) {
    // Si llegamos aquí, init() funcionó correctamente
    TEST_PASS();
}

/**
 * @brief Test: sensor_temperatura_leer() retorna un valor válido
 */
void test_sensor_temperatura_leer_retorna_valor_valido(void) {
    Temperatura temp = sensor_temperatura_leer();

    // Temperatura debe ser un número válido (no NaN, no infinito)
    TEST_ASSERT_FALSE(isnan(temp));
    TEST_ASSERT_FALSE(isinf(temp));

    // Debe ser mayor que cero (con el simulador actual)
    TEST_ASSERT_GREATER_THAN(0.0f, temp);
}

/**
 * @brief Test: sensor_temperatura_leer() retorna rango esperado
 *
 * Con ADC simulado (150-300), esperamos temperaturas en rango:
 * - Mínimo: 150 * 0.0488 ≈ 7.3°C
 * - Máximo: 300 * 0.0488 ≈ 14.6°C
 *
 * Damos margen de seguridad: 5-20°C
 */
void test_sensor_temperatura_leer_retorna_rango_esperado(void) {
    Temperatura temp = sensor_temperatura_leer();

    // Verificar rango con margen
    TEST_ASSERT_GREATER_OR_EQUAL(5.0f, temp);
    TEST_ASSERT_LESS_OR_EQUAL(20.0f, temp);
}

/**
 * @brief Test: múltiples lecturas son consistentes en rango
 */
void test_sensor_temperatura_multiples_lecturas_en_rango(void) {
    // Leer 10 veces y verificar que todas están en rango válido
    for (int i = 0; i < 10; i++) {
        Temperatura temp = sensor_temperatura_leer();

        TEST_ASSERT_GREATER_THAN(0.0f, temp);
        TEST_ASSERT_LESS_THAN(50.0f, temp);  // Límite superior teórico
    }
}

/**
 * @brief Test: conversión matemática es correcta
 *
 * Verifica que la conversión ADC → °C es consistente
 */
void test_sensor_temperatura_conversion_correcta(void) {
    Temperatura temp = sensor_temperatura_leer();

    // La temperatura debe ser positiva
    TEST_ASSERT_GREATER_THAN(0.0f, temp);

    // Y menor al máximo teórico (1023 * 0.0488 ≈ 50°C)
    TEST_ASSERT_LESS_THAN(50.0f, temp);

    // Valores del simulador (150-300) deben dar ~7-15°C
    TEST_ASSERT_GREATER_OR_EQUAL(7.0f, temp);
    TEST_ASSERT_LESS_OR_EQUAL(15.0f, temp);
}

/**
 * @brief Test: múltiples lecturas producen variación
 *
 * Debido a la naturaleza aleatoria del simulador,
 * múltiples lecturas deben producir valores diferentes
 */
void test_sensor_temperatura_multiples_lecturas_varian(void) {
    Temperatura temp1 = sensor_temperatura_leer();
    Temperatura temp2 = sensor_temperatura_leer();
    Temperatura temp3 = sensor_temperatura_leer();
    Temperatura temp4 = sensor_temperatura_leer();
    Temperatura temp5 = sensor_temperatura_leer();

    // Al menos 2 valores deben ser diferentes
    // (Con 150 valores posibles, probabilidad altísima)
    int valores_diferentes = 0;
    if (temp2 != temp1) valores_diferentes++;
    if (temp3 != temp1) valores_diferentes++;
    if (temp4 != temp1) valores_diferentes++;
    if (temp5 != temp1) valores_diferentes++;

    TEST_ASSERT_GREATER_THAN(0, valores_diferentes);
}

/**
 * @brief Test: reinicialización funciona correctamente
 */
void test_sensor_temperatura_reinicializacion(void) {
    // Primera lectura
    Temperatura temp1 = sensor_temperatura_leer();
    TEST_ASSERT_GREATER_THAN(0.0f, temp1);

    // Desinicializar
    sensor_temperatura_deinit();

    // Reinicializar
    sensor_temperatura_init();

    // Segunda lectura
    Temperatura temp2 = sensor_temperatura_leer();
    TEST_ASSERT_GREATER_THAN(0.0f, temp2);

    // Ambas deben estar en rango válido
    TEST_ASSERT_GREATER_OR_EQUAL(5.0f, temp1);
    TEST_ASSERT_LESS_OR_EQUAL(20.0f, temp1);
    TEST_ASSERT_GREATER_OR_EQUAL(5.0f, temp2);
    TEST_ASSERT_LESS_OR_EQUAL(20.0f, temp2);
}

/**
 * @brief Test: valores son físicamente razonables
 *
 * Verifica que las temperaturas simuladas representan
 * un ambiente interior típico
 */
void test_sensor_temperatura_valores_fisicamente_razonables(void) {
    // Tomar 20 muestras y verificar que el promedio es razonable
    float suma = 0.0f;
    int n = 20;

    for (int i = 0; i < n; i++) {
        Temperatura temp = sensor_temperatura_leer();
        suma += temp;
    }

    float promedio = suma / n;

    // El promedio debe estar en rango típico de interior (~7-15°C con simulador)
    TEST_ASSERT_GREATER_OR_EQUAL(7.0f, promedio);
    TEST_ASSERT_LESS_OR_EQUAL(15.0f, promedio);
}

/**
 * @brief Punto de entrada de los tests
 */
int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_sensor_temperatura_init_no_falla);
    RUN_TEST(test_sensor_temperatura_leer_retorna_valor_valido);
    RUN_TEST(test_sensor_temperatura_leer_retorna_rango_esperado);
    RUN_TEST(test_sensor_temperatura_multiples_lecturas_en_rango);
    RUN_TEST(test_sensor_temperatura_conversion_correcta);
    RUN_TEST(test_sensor_temperatura_multiples_lecturas_varian);
    RUN_TEST(test_sensor_temperatura_reinicializacion);
    RUN_TEST(test_sensor_temperatura_valores_fisicamente_razonables);

    return UNITY_END();
}
