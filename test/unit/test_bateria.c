/**
 * @file test_bateria.c
 * @brief Tests unitarios para la entidad Bateria
 * @version 1.0
 * @date 2025-11-17
 *
 * Suite de pruebas unitarias para verificar el comportamiento
 * de la entidad Bateria del dominio.
 *
 * @author Victor Valotto
 * @copyright Universidad Nacional de Entre Ríos (FIUNER)
 */

#include "unity.h"
#include "bateria.h"

/**
 * @brief Configuración antes de cada test
 *
 * Se ejecuta automáticamente antes de cada caso de prueba.
 */
void setUp(void) {
    // No hay setup necesario por ahora
}

/**
 * @brief Limpieza después de cada test
 *
 * Se ejecuta automáticamente después de cada caso de prueba.
 */
void tearDown(void) {
    // No hay teardown necesario por ahora
}

/**
 * @test Verifica que bateria_crear() retorna un puntero válido
 */
void test_bateria_crear_retorna_puntero_valido(void) {
    Bateria* bateria = bateria_crear();

    TEST_ASSERT_NOT_NULL(bateria);

    bateria_destruir(bateria);
}

/**
 * @test Verifica que el nivel inicial es 100%
 */
void test_bateria_crear_nivel_inicial_es_100(void) {
    Bateria* bateria = bateria_crear();

    NivelCarga nivel = bateria_obtener_nivel(bateria);
    TEST_ASSERT_EQUAL_UINT8(NIVEL_CARGA_INICIAL, nivel);
    TEST_ASSERT_EQUAL_UINT8(100, nivel);

    bateria_destruir(bateria);
}

/**
 * @test Verifica que el estado inicial es NORMAL
 */
void test_bateria_crear_estado_inicial_es_normal(void) {
    Bateria* bateria = bateria_crear();

    EstadoBateria estado = bateria_obtener_estado(bateria);
    TEST_ASSERT_EQUAL(BATERIA_ESTADO_NORMAL, estado);

    bateria_destruir(bateria);
}

/**
 * @test Verifica que se puede establecer el nivel de carga
 */
void test_bateria_establecer_nivel_actualiza_correctamente(void) {
    Bateria* bateria = bateria_crear();

    bateria_establecer_nivel(bateria, 75);
    TEST_ASSERT_EQUAL_UINT8(75, bateria_obtener_nivel(bateria));

    bateria_destruir(bateria);
}

/**
 * @test Verifica que establecer NULL no causa crash
 */
void test_bateria_establecer_nivel_con_null_no_causa_crash(void) {
    // No debe causar crash ni error
    bateria_establecer_nivel(NULL, 50);

    // Si llegamos aquí, el test pasa
    TEST_ASSERT_TRUE(true);
}

/**
 * @test Verifica que obtener nivel de NULL retorna 0
 */
void test_bateria_obtener_nivel_con_null_retorna_cero(void) {
    NivelCarga nivel = bateria_obtener_nivel(NULL);

    TEST_ASSERT_EQUAL_UINT8(0, nivel);
}

/**
 * @test Verifica que obtener estado de NULL retorna CRITICO
 */
void test_bateria_obtener_estado_con_null_retorna_critico(void) {
    EstadoBateria estado = bateria_obtener_estado(NULL);

    TEST_ASSERT_EQUAL(BATERIA_ESTADO_CRITICO, estado);
}

/**
 * @test Verifica que el estado es NORMAL cuando nivel >= 20%
 */
void test_bateria_estado_normal_cuando_nivel_mayor_o_igual_20(void) {
    Bateria* bateria = bateria_crear();

    // Probar con 20% (justo en el umbral)
    bateria_establecer_nivel(bateria, 20);
    TEST_ASSERT_EQUAL(BATERIA_ESTADO_NORMAL, bateria_obtener_estado(bateria));

    // Probar con 50%
    bateria_establecer_nivel(bateria, 50);
    TEST_ASSERT_EQUAL(BATERIA_ESTADO_NORMAL, bateria_obtener_estado(bateria));

    // Probar con 100%
    bateria_establecer_nivel(bateria, 100);
    TEST_ASSERT_EQUAL(BATERIA_ESTADO_NORMAL, bateria_obtener_estado(bateria));

    bateria_destruir(bateria);
}

/**
 * @test Verifica que el estado es BAJO cuando 10% <= nivel < 20%
 */
void test_bateria_estado_bajo_cuando_nivel_entre_10_y_20(void) {
    Bateria* bateria = bateria_crear();

    // Probar con 19% (justo debajo del umbral NORMAL)
    bateria_establecer_nivel(bateria, 19);
    TEST_ASSERT_EQUAL(BATERIA_ESTADO_BAJO, bateria_obtener_estado(bateria));

    // Probar con 15% (en medio del rango)
    bateria_establecer_nivel(bateria, 15);
    TEST_ASSERT_EQUAL(BATERIA_ESTADO_BAJO, bateria_obtener_estado(bateria));

    // Probar con 10% (justo en el umbral)
    bateria_establecer_nivel(bateria, 10);
    TEST_ASSERT_EQUAL(BATERIA_ESTADO_BAJO, bateria_obtener_estado(bateria));

    bateria_destruir(bateria);
}

/**
 * @test Verifica que el estado es CRITICO cuando nivel < 10%
 */
void test_bateria_estado_critico_cuando_nivel_menor_10(void) {
    Bateria* bateria = bateria_crear();

    // Probar con 9% (justo debajo del umbral)
    bateria_establecer_nivel(bateria, 9);
    TEST_ASSERT_EQUAL(BATERIA_ESTADO_CRITICO, bateria_obtener_estado(bateria));

    // Probar con 5%
    bateria_establecer_nivel(bateria, 5);
    TEST_ASSERT_EQUAL(BATERIA_ESTADO_CRITICO, bateria_obtener_estado(bateria));

    // Probar con 0%
    bateria_establecer_nivel(bateria, 0);
    TEST_ASSERT_EQUAL(BATERIA_ESTADO_CRITICO, bateria_obtener_estado(bateria));

    bateria_destruir(bateria);
}

/**
 * @test Verifica que bateria_esta_baja() funciona correctamente
 */
void test_bateria_esta_baja_funciona_correctamente(void) {
    Bateria* bateria = bateria_crear();

    // No está baja cuando nivel >= 20%
    bateria_establecer_nivel(bateria, 20);
    TEST_ASSERT_FALSE(bateria_esta_baja(bateria));

    bateria_establecer_nivel(bateria, 50);
    TEST_ASSERT_FALSE(bateria_esta_baja(bateria));

    // Está baja cuando nivel < 20%
    bateria_establecer_nivel(bateria, 19);
    TEST_ASSERT_TRUE(bateria_esta_baja(bateria));

    bateria_establecer_nivel(bateria, 10);
    TEST_ASSERT_TRUE(bateria_esta_baja(bateria));

    bateria_establecer_nivel(bateria, 5);
    TEST_ASSERT_TRUE(bateria_esta_baja(bateria));

    bateria_destruir(bateria);
}

/**
 * @test Verifica que bateria_esta_critica() funciona correctamente
 */
void test_bateria_esta_critica_funciona_correctamente(void) {
    Bateria* bateria = bateria_crear();

    // No está crítica cuando nivel >= 10%
    bateria_establecer_nivel(bateria, 10);
    TEST_ASSERT_FALSE(bateria_esta_critica(bateria));

    bateria_establecer_nivel(bateria, 50);
    TEST_ASSERT_FALSE(bateria_esta_critica(bateria));

    // Está crítica cuando nivel < 10%
    bateria_establecer_nivel(bateria, 9);
    TEST_ASSERT_TRUE(bateria_esta_critica(bateria));

    bateria_establecer_nivel(bateria, 5);
    TEST_ASSERT_TRUE(bateria_esta_critica(bateria));

    bateria_establecer_nivel(bateria, 0);
    TEST_ASSERT_TRUE(bateria_esta_critica(bateria));

    bateria_destruir(bateria);
}

/**
 * @test Verifica que bateria_esta_baja() con NULL retorna true
 */
void test_bateria_esta_baja_con_null_retorna_true(void) {
    TEST_ASSERT_TRUE(bateria_esta_baja(NULL));
}

/**
 * @test Verifica que bateria_esta_critica() con NULL retorna true
 */
void test_bateria_esta_critica_con_null_retorna_true(void) {
    TEST_ASSERT_TRUE(bateria_esta_critica(NULL));
}

/**
 * @test Verifica que se puede cambiar el nivel múltiples veces
 */
void test_bateria_cambios_multiples_de_nivel(void) {
    Bateria* bateria = bateria_crear();

    // Secuencia de cambios
    bateria_establecer_nivel(bateria, 80);
    TEST_ASSERT_EQUAL_UINT8(80, bateria_obtener_nivel(bateria));
    TEST_ASSERT_EQUAL(BATERIA_ESTADO_NORMAL, bateria_obtener_estado(bateria));

    bateria_establecer_nivel(bateria, 15);
    TEST_ASSERT_EQUAL_UINT8(15, bateria_obtener_nivel(bateria));
    TEST_ASSERT_EQUAL(BATERIA_ESTADO_BAJO, bateria_obtener_estado(bateria));

    bateria_establecer_nivel(bateria, 5);
    TEST_ASSERT_EQUAL_UINT8(5, bateria_obtener_nivel(bateria));
    TEST_ASSERT_EQUAL(BATERIA_ESTADO_CRITICO, bateria_obtener_estado(bateria));

    bateria_establecer_nivel(bateria, 100);
    TEST_ASSERT_EQUAL_UINT8(100, bateria_obtener_nivel(bateria));
    TEST_ASSERT_EQUAL(BATERIA_ESTADO_NORMAL, bateria_obtener_estado(bateria));

    bateria_destruir(bateria);
}

/**
 * @test Verifica que destruir NULL no causa crash
 */
void test_bateria_destruir_null_no_causa_crash(void) {
    // No debe causar crash
    bateria_destruir(NULL);

    // Si llegamos aquí, el test pasa
    TEST_ASSERT_TRUE(true);
}

/**
 * @brief Punto de entrada de los tests
 *
 * Ejecuta todos los casos de prueba definidos.
 */
int main(void) {
    UNITY_BEGIN();

    // Tests de creación y destrucción
    RUN_TEST(test_bateria_crear_retorna_puntero_valido);
    RUN_TEST(test_bateria_crear_nivel_inicial_es_100);
    RUN_TEST(test_bateria_crear_estado_inicial_es_normal);
    RUN_TEST(test_bateria_destruir_null_no_causa_crash);

    // Tests de establecer y obtener nivel
    RUN_TEST(test_bateria_establecer_nivel_actualiza_correctamente);
    RUN_TEST(test_bateria_establecer_nivel_con_null_no_causa_crash);
    RUN_TEST(test_bateria_obtener_nivel_con_null_retorna_cero);
    RUN_TEST(test_bateria_obtener_estado_con_null_retorna_critico);

    // Tests de estados según umbrales
    RUN_TEST(test_bateria_estado_normal_cuando_nivel_mayor_o_igual_20);
    RUN_TEST(test_bateria_estado_bajo_cuando_nivel_entre_10_y_20);
    RUN_TEST(test_bateria_estado_critico_cuando_nivel_menor_10);

    // Tests de funciones de verificación
    RUN_TEST(test_bateria_esta_baja_funciona_correctamente);
    RUN_TEST(test_bateria_esta_critica_funciona_correctamente);
    RUN_TEST(test_bateria_esta_baja_con_null_retorna_true);
    RUN_TEST(test_bateria_esta_critica_con_null_retorna_true);

    // Tests de casos complejos
    RUN_TEST(test_bateria_cambios_multiples_de_nivel);

    return UNITY_END();
}
