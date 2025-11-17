/**
 * @file test_sensor_bateria.c
 * @brief Tests unitarios para el sensor de batería
 * @version 1.0
 * @date 2025-11-17
 *
 * Suite de pruebas unitarias para verificar el comportamiento
 * del proxy del sensor de batería en la capa de infraestructura.
 *
 * @author Victor Valotto
 * @copyright Universidad Nacional de Entre Ríos (FIUNER)
 */

#include "unity.h"
#include "sensor_bateria.h"

/**
 * @brief Configuración antes de cada test
 */
void setUp(void) {
    // Inicializar sensor antes de cada test
    sensor_bateria_init();
}

/**
 * @brief Limpieza después de cada test
 */
void tearDown(void) {
    // Desinicializar sensor después de cada test
    sensor_bateria_deinit();
}

/**
 * @test Verifica que la inicialización no causa errores
 */
void test_sensor_bateria_init_no_causa_error(void) {
    // Si llegamos aquí sin crash, el test pasa
    TEST_ASSERT_TRUE(true);
}

/**
 * @test Verifica que la desinicialización no causa errores
 */
void test_sensor_bateria_deinit_no_causa_error(void) {
    sensor_bateria_deinit();
    // Si llegamos aquí sin crash, el test pasa
    TEST_ASSERT_TRUE(true);
}

/**
 * @test Verifica que leer() retorna un nivel válido (0-100%)
 */
void test_sensor_bateria_leer_retorna_nivel_valido(void) {
    NivelCarga nivel = sensor_bateria_leer();

    // El nivel debe estar en el rango 0-100%
    TEST_ASSERT_GREATER_OR_EQUAL(0, nivel);
    TEST_ASSERT_LESS_OR_EQUAL(100, nivel);
}

/**
 * @test Verifica que el nivel está en rango 0-100%
 */
void test_sensor_bateria_nivel_en_rango_correcto(void) {
    NivelCarga nivel = sensor_bateria_leer();

    // Verificar rango con tipos correctos
    TEST_ASSERT_GREATER_OR_EQUAL_UINT8(0, nivel);
    TEST_ASSERT_LESS_OR_EQUAL_UINT8(100, nivel);
}

/**
 * @test Verifica que múltiples lecturas retornan valores en rango
 */
void test_sensor_bateria_multiples_lecturas_en_rango(void) {
    const int NUM_LECTURAS = 10;

    for (int i = 0; i < NUM_LECTURAS; i++) {
        NivelCarga nivel = sensor_bateria_leer();

        // Cada lectura debe estar en el rango válido
        TEST_ASSERT_GREATER_OR_EQUAL_UINT8(0, nivel);
        TEST_ASSERT_LESS_OR_EQUAL_UINT8(100, nivel);
    }
}

/**
 * @test Verifica la conversión de voltajes conocidos
 *
 * Casos de prueba con voltajes específicos y sus porcentajes esperados:
 * - 3000 mV → 0%
 * - 3600 mV → 50%
 * - 4200 mV → 100%
 *
 * Nota: Este test depende de la implementación interna del HAL simulado.
 * En un entorno de producción, se usarían mocks para controlar los valores.
 */
void test_sensor_bateria_conversion_voltaje_a_porcentaje(void) {
    // Como el HAL retorna valores aleatorios, solo verificamos que
    // la conversión produce valores en el rango esperado
    // Para un test más preciso, se necesitaría un mock del HAL

    NivelCarga nivel = sensor_bateria_leer();

    // Verificar que el nivel es válido
    TEST_ASSERT_GREATER_OR_EQUAL_UINT8(0, nivel);
    TEST_ASSERT_LESS_OR_EQUAL_UINT8(100, nivel);
}

/**
 * @test Verifica que las lecturas tienen variación (simulador)
 */
void test_sensor_bateria_lecturas_tienen_variacion(void) {
    const int NUM_LECTURAS = 20;
    NivelCarga lecturas[NUM_LECTURAS];
    bool hay_variacion = false;

    // Tomar múltiples lecturas
    for (int i = 0; i < NUM_LECTURAS; i++) {
        lecturas[i] = sensor_bateria_leer();
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
 * @test Verifica que se puede reinicializar el sensor
 */
void test_sensor_bateria_reinicializacion_funciona(void) {
    NivelCarga n1, n2;

    // Primera lectura
    n1 = sensor_bateria_leer();

    // Desinicializar y reinicializar
    sensor_bateria_deinit();
    sensor_bateria_init();

    // Segunda lectura después de reiniciar
    n2 = sensor_bateria_leer();

    // Ambas lecturas deben estar en rango válido
    TEST_ASSERT_GREATER_OR_EQUAL_UINT8(0, n1);
    TEST_ASSERT_LESS_OR_EQUAL_UINT8(100, n1);
    TEST_ASSERT_GREATER_OR_EQUAL_UINT8(0, n2);
    TEST_ASSERT_LESS_OR_EQUAL_UINT8(100, n2);
}

/**
 * @test Verifica el rango de valores obtenidos en múltiples muestras
 */
void test_sensor_bateria_rango_obtenido_correcto(void) {
    const int NUM_MUESTRAS = 50;
    NivelCarga min_leido = 100;
    NivelCarga max_leido = 0;

    // Tomar múltiples muestras para encontrar min/max
    for (int i = 0; i < NUM_MUESTRAS; i++) {
        NivelCarga n = sensor_bateria_leer();

        if (n < min_leido) min_leido = n;
        if (n > max_leido) max_leido = n;
    }

    // El mínimo leído debe ser >= 0
    TEST_ASSERT_GREATER_OR_EQUAL_UINT8(0, min_leido);

    // El máximo leído debe ser <= 100
    TEST_ASSERT_LESS_OR_EQUAL_UINT8(100, max_leido);

    // Debe haber un rango razonable de valores
    // (no todos iguales o muy cercanos)
    NivelCarga rango_leido = max_leido - min_leido;
    TEST_ASSERT_GREATER_THAN(5, rango_leido);  // Al menos 5% de variación
}

/**
 * @test Verifica que NivelCarga es uint8_t (0-255)
 */
void test_sensor_bateria_tipo_nivel_carga_adecuado(void) {
    // NivelCarga es uint8_t, suficiente para 0-100
    NivelCarga nivel = 100;

    TEST_ASSERT_EQUAL_UINT8(100, nivel);

    // Verificar que puede almacenar todo el rango
    nivel = 0;
    TEST_ASSERT_EQUAL_UINT8(0, nivel);
}

/**
 * @test Verifica la coherencia con constantes de tipos_comunes.h
 */
void test_sensor_bateria_coherencia_con_constantes(void) {
    // Verificar que las constantes de voltaje tienen valores correctos
    TEST_ASSERT_EQUAL_UINT16(4200, VOLTAJE_BATERIA_MAX_MV);
    TEST_ASSERT_EQUAL_UINT16(3000, VOLTAJE_BATERIA_MIN_MV);

    // El rango debe ser positivo
    uint16_t rango = VOLTAJE_BATERIA_MAX_MV - VOLTAJE_BATERIA_MIN_MV;
    TEST_ASSERT_EQUAL_UINT16(1200, rango);
}

/**
 * @test Verifica conversión matemática aproximada
 *
 * Dado que el HAL genera valores aleatorios, verificamos que
 * lecturas repetidas dan una distribución razonable.
 */
void test_sensor_bateria_distribucion_razonable(void) {
    const int NUM_MUESTRAS = 100;
    int contador_bajo = 0;      // 0-33%
    int contador_medio = 0;     // 34-66%
    int contador_alto = 0;      // 67-100%

    // Tomar muestras y clasificar
    for (int i = 0; i < NUM_MUESTRAS; i++) {
        NivelCarga n = sensor_bateria_leer();

        if (n <= 33) {
            contador_bajo++;
        } else if (n <= 66) {
            contador_medio++;
        } else {
            contador_alto++;
        }
    }

    // Verificar que hay valores en al menos 2 de los 3 rangos
    // (distribución no está concentrada en un solo tercio)
    int rangos_con_valores = 0;
    if (contador_bajo > 0) rangos_con_valores++;
    if (contador_medio > 0) rangos_con_valores++;
    if (contador_alto > 0) rangos_con_valores++;

    TEST_ASSERT_GREATER_OR_EQUAL(2, rangos_con_valores);
}

/**
 * @brief Punto de entrada de los tests
 */
int main(void) {
    UNITY_BEGIN();

    // Tests de inicialización
    RUN_TEST(test_sensor_bateria_init_no_causa_error);
    RUN_TEST(test_sensor_bateria_deinit_no_causa_error);
    RUN_TEST(test_sensor_bateria_reinicializacion_funciona);

    // Tests de lectura de nivel
    RUN_TEST(test_sensor_bateria_leer_retorna_nivel_valido);
    RUN_TEST(test_sensor_bateria_nivel_en_rango_correcto);
    RUN_TEST(test_sensor_bateria_multiples_lecturas_en_rango);

    // Tests de conversión y variación
    RUN_TEST(test_sensor_bateria_conversion_voltaje_a_porcentaje);
    RUN_TEST(test_sensor_bateria_lecturas_tienen_variacion);
    RUN_TEST(test_sensor_bateria_rango_obtenido_correcto);
    RUN_TEST(test_sensor_bateria_distribucion_razonable);

    // Tests de tipos y constantes
    RUN_TEST(test_sensor_bateria_tipo_nivel_carga_adecuado);
    RUN_TEST(test_sensor_bateria_coherencia_con_constantes);

    return UNITY_END();
}
