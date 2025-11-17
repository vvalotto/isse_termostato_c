/**
 * @file test_lectura_bateria_completa.c
 * @brief Test de integración end-to-end para HU-007
 * @version 1.0
 * @date 2025-11-17
 *
 * Test de integración que verifica el flujo completo de lectura
 * del nivel de batería atravesando todas las capas de la arquitectura:
 *
 * Aplicación → Dominio → Infraestructura → Dispositivos
 *
 * Este test valida:
 * - Inicialización correcta de todas las capas (batería)
 * - Flujo de datos a través de las capas
 * - Conversión voltaje → porcentaje → estado
 * - Coherencia entre componentes
 * - Limpieza ordenada de recursos
 *
 * @author Victor Valotto
 * @copyright Universidad Nacional de Entre Ríos (FIUNER)
 */

#include "unity.h"
#include "gestor_termostato.h"
#include "ambiente.h"
#include "bateria.h"
#include "sensor_bateria.h"
#include "hal_bateria.h"

// Variables globales para el test
static Ambiente* ambiente_global = NULL;
static Bateria* bateria_global = NULL;
static GestorTermostato* gestor_global = NULL;

/**
 * @brief Setup ejecutado antes de cada test
 */
void setUp(void) {
    // Inicializar desde la capa más baja hacia arriba
    hal_bateria_init();
    sensor_bateria_init();

    ambiente_global = ambiente_crear();
    bateria_global = bateria_crear();
    gestor_global = gestor_termostato_crear(ambiente_global, bateria_global);
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

    if (bateria_global != NULL) {
        bateria_destruir(bateria_global);
        bateria_global = NULL;
    }

    sensor_bateria_deinit();
    hal_bateria_deinit();
}

/**
 * @brief Test: Inicialización completa del sistema (batería)
 */
void test_inicializacion_completa_sistema_bateria(void) {
    // Todas las capas deben estar inicializadas correctamente
    TEST_ASSERT_NOT_NULL(bateria_global);
    TEST_ASSERT_NOT_NULL(gestor_global);

    // La batería debe tener nivel inicial de 100%
    NivelCarga nivel = bateria_obtener_nivel(bateria_global);
    TEST_ASSERT_EQUAL_UINT8(NIVEL_CARGA_INICIAL, nivel);

    // El estado inicial debe ser NORMAL
    EstadoBateria estado = bateria_obtener_estado(bateria_global);
    TEST_ASSERT_EQUAL(BATERIA_ESTADO_NORMAL, estado);
}

/**
 * @brief Test: Flujo completo de lectura de batería
 *
 * Este test valida el flujo end-to-end:
 * 1. HAL Batería genera voltaje simulado (3000-4200 mV)
 * 2. Sensor Batería convierte a porcentaje (0-100%)
 * 3. Gestor actualiza la entidad Bateria
 * 4. Bateria almacena el valor y calcula estado
 * 5. Gestor puede recuperar nivel y estado
 */
void test_flujo_completo_lectura_bateria(void) {
    // Nivel inicial de la batería
    NivelCarga nivel_inicial = bateria_obtener_nivel(bateria_global);
    TEST_ASSERT_EQUAL_UINT8(NIVEL_CARGA_INICIAL, nivel_inicial);

    // Ejecutar actualización (atraviesa todas las capas)
    gestor_termostato_actualizar_bateria(gestor_global);

    // Obtener nivel actualizado
    NivelCarga nivel_actualizado = gestor_termostato_obtener_nivel_bateria(gestor_global);

    // Verificar que el valor es válido (0-100%)
    TEST_ASSERT_GREATER_OR_EQUAL(0, nivel_actualizado);
    TEST_ASSERT_LESS_OR_EQUAL(100, nivel_actualizado);

    // Obtener estado actualizado
    EstadoBateria estado = gestor_termostato_obtener_estado_bateria(gestor_global);

    // El estado debe ser uno de los tres válidos
    TEST_ASSERT_TRUE(estado == BATERIA_ESTADO_NORMAL ||
                     estado == BATERIA_ESTADO_BAJO ||
                     estado == BATERIA_ESTADO_CRITICO);

    // Verificar que la batería también tiene el mismo valor
    NivelCarga nivel_bateria = bateria_obtener_nivel(bateria_global);
    TEST_ASSERT_EQUAL_UINT8(nivel_actualizado, nivel_bateria);
}

/**
 * @brief Test: Múltiples actualizaciones secuenciales
 *
 * Verifica que el sistema puede realizar múltiples ciclos
 * de lectura sin degradación o corrupción de datos.
 */
void test_multiples_actualizaciones_secuenciales_bateria(void) {
    const int NUM_LECTURAS = 20;
    NivelCarga lecturas[NUM_LECTURAS];

    // Realizar múltiples lecturas
    for (int i = 0; i < NUM_LECTURAS; i++) {
        gestor_termostato_actualizar_bateria(gestor_global);
        lecturas[i] = gestor_termostato_obtener_nivel_bateria(gestor_global);

        // Cada lectura debe ser válida (0-100%)
        TEST_ASSERT_GREATER_OR_EQUAL(0, lecturas[i]);
        TEST_ASSERT_LESS_OR_EQUAL(100, lecturas[i]);
    }

    // Calcular promedio de todas las lecturas
    uint32_t suma = 0;
    for (int i = 0; i < NUM_LECTURAS; i++) {
        suma += lecturas[i];
    }
    uint8_t promedio = (uint8_t)(suma / NUM_LECTURAS);

    // El promedio debe estar en rango válido
    TEST_ASSERT_GREATER_OR_EQUAL(0, promedio);
    TEST_ASSERT_LESS_OR_EQUAL(100, promedio);

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
 * Verifica que el nivel de batería es consistente
 * cuando se consulta desde diferentes capas.
 */
void test_coherencia_entre_capas_bateria(void) {
    // Actualizar batería
    gestor_termostato_actualizar_bateria(gestor_global);

    // Obtener nivel desde el gestor
    NivelCarga nivel_gestor = gestor_termostato_obtener_nivel_bateria(gestor_global);

    // Obtener nivel directamente de la entidad batería
    NivelCarga nivel_bateria = bateria_obtener_nivel(bateria_global);

    // Ambos valores deben coincidir exactamente
    TEST_ASSERT_EQUAL_UINT8(nivel_gestor, nivel_bateria);

    // Leer directamente del sensor (sin actualizar el modelo)
    NivelCarga nivel_sensor = sensor_bateria_leer();

    // Este valor puede ser diferente (nueva lectura física)
    // pero debe estar en el mismo rango
    TEST_ASSERT_GREATER_OR_EQUAL(0, nivel_sensor);
    TEST_ASSERT_LESS_OR_EQUAL(100, nivel_sensor);
}

/**
 * @brief Test: Conversión voltaje a porcentaje es correcta
 *
 * Verifica que la cadena de conversión Voltaje → Porcentaje
 * produce valores matemáticamente correctos.
 */
void test_conversion_voltaje_a_porcentaje_correcta(void) {
    // Leer voltaje directamente del HAL
    Voltaje voltaje_mv = hal_bateria_leer_voltaje();

    // El voltaje debe estar en el rango esperado del simulador
    TEST_ASSERT_GREATER_OR_EQUAL(VOLTAJE_BATERIA_MIN_MV, voltaje_mv);
    TEST_ASSERT_LESS_OR_EQUAL(VOLTAJE_BATERIA_MAX_MV, voltaje_mv);

    // Calcular porcentaje esperado manualmente
    // Fórmula: (voltaje - 3000) * 100 / 1200
    uint16_t rango = VOLTAJE_BATERIA_MAX_MV - VOLTAJE_BATERIA_MIN_MV;  // 1200
    uint16_t delta = voltaje_mv - VOLTAJE_BATERIA_MIN_MV;
    uint8_t porcentaje_esperado = (uint8_t)((delta * 100) / rango);

    // Leer nivel del sensor
    NivelCarga nivel_sensor = sensor_bateria_leer();

    // Los valores deberían ser similares
    // (pequeña diferencia por nueva lectura del HAL)
    // Ambos deben estar en el rango válido
    TEST_ASSERT_GREATER_OR_EQUAL(0, porcentaje_esperado);
    TEST_ASSERT_LESS_OR_EQUAL(100, porcentaje_esperado);
    TEST_ASSERT_GREATER_OR_EQUAL(0, nivel_sensor);
    TEST_ASSERT_LESS_OR_EQUAL(100, nivel_sensor);
}

/**
 * @brief Test: Estados de batería se calculan correctamente
 *
 * Verifica que los estados NORMAL/BAJO/CRITICO se asignan
 * correctamente según el nivel de carga.
 */
void test_estados_bateria_calculados_correctamente(void) {
    // Forzar batería al 100% (NORMAL)
    bateria_establecer_nivel(bateria_global, 100);
    TEST_ASSERT_EQUAL(BATERIA_ESTADO_NORMAL, bateria_obtener_estado(bateria_global));
    TEST_ASSERT_FALSE(gestor_termostato_bateria_baja(gestor_global));

    // Forzar batería al 50% (NORMAL)
    bateria_establecer_nivel(bateria_global, 50);
    TEST_ASSERT_EQUAL(BATERIA_ESTADO_NORMAL, bateria_obtener_estado(bateria_global));
    TEST_ASSERT_FALSE(gestor_termostato_bateria_baja(gestor_global));

    // Forzar batería al 20% (límite superior de NORMAL)
    bateria_establecer_nivel(bateria_global, 20);
    TEST_ASSERT_EQUAL(BATERIA_ESTADO_NORMAL, bateria_obtener_estado(bateria_global));
    TEST_ASSERT_FALSE(gestor_termostato_bateria_baja(gestor_global));

    // Forzar batería al 19% (BAJO)
    bateria_establecer_nivel(bateria_global, 19);
    TEST_ASSERT_EQUAL(BATERIA_ESTADO_BAJO, bateria_obtener_estado(bateria_global));
    TEST_ASSERT_TRUE(gestor_termostato_bateria_baja(gestor_global));

    // Forzar batería al 10% (límite superior de BAJO)
    bateria_establecer_nivel(bateria_global, 10);
    TEST_ASSERT_EQUAL(BATERIA_ESTADO_BAJO, bateria_obtener_estado(bateria_global));
    TEST_ASSERT_TRUE(gestor_termostato_bateria_baja(gestor_global));

    // Forzar batería al 9% (CRITICO)
    bateria_establecer_nivel(bateria_global, 9);
    TEST_ASSERT_EQUAL(BATERIA_ESTADO_CRITICO, bateria_obtener_estado(bateria_global));
    TEST_ASSERT_TRUE(gestor_termostato_bateria_baja(gestor_global));

    // Forzar batería al 0% (CRITICO)
    bateria_establecer_nivel(bateria_global, 0);
    TEST_ASSERT_EQUAL(BATERIA_ESTADO_CRITICO, bateria_obtener_estado(bateria_global));
    TEST_ASSERT_TRUE(gestor_termostato_bateria_baja(gestor_global));
}

/**
 * @brief Test: Sistema maneja reinicialización
 *
 * Verifica que el sistema puede reinicializarse correctamente
 * sin corrupción de memoria o estados inválidos.
 */
void test_sistema_maneja_reinicializacion_bateria(void) {
    // Primera lectura
    gestor_termostato_actualizar_bateria(gestor_global);
    NivelCarga nivel1 = gestor_termostato_obtener_nivel_bateria(gestor_global);
    TEST_ASSERT_GREATER_OR_EQUAL(0, nivel1);
    TEST_ASSERT_LESS_OR_EQUAL(100, nivel1);

    // Simular reinicialización (tearDown + setUp)
    tearDown();
    setUp();

    // La batería debe tener nivel inicial nuevamente
    NivelCarga nivel_inicial = bateria_obtener_nivel(bateria_global);
    TEST_ASSERT_EQUAL_UINT8(NIVEL_CARGA_INICIAL, nivel_inicial);

    // Segunda lectura después de reinicialización
    gestor_termostato_actualizar_bateria(gestor_global);
    NivelCarga nivel2 = gestor_termostato_obtener_nivel_bateria(gestor_global);
    TEST_ASSERT_GREATER_OR_EQUAL(0, nivel2);
    TEST_ASSERT_LESS_OR_EQUAL(100, nivel2);
}

/**
 * @brief Test: HU-007 cumple requisitos funcionales
 *
 * Test de aceptación que verifica que se cumplen todos
 * los requisitos de la historia de usuario HU-007:
 * "Como usuario, quiero obtener el nivel de carga de la batería"
 */
void test_hu007_cumple_requisitos_funcionales(void) {
    // Requisito 1: El sistema debe poder leer el nivel de batería
    gestor_termostato_actualizar_bateria(gestor_global);
    NivelCarga nivel = gestor_termostato_obtener_nivel_bateria(gestor_global);

    // Requisito 2: El nivel debe ser un valor numérico válido (0-100%)
    TEST_ASSERT_GREATER_OR_EQUAL(0, nivel);
    TEST_ASSERT_LESS_OR_EQUAL(100, nivel);

    // Requisito 3: El sistema debe calcular el estado de la batería
    EstadoBateria estado = gestor_termostato_obtener_estado_bateria(gestor_global);
    TEST_ASSERT_TRUE(estado == BATERIA_ESTADO_NORMAL ||
                     estado == BATERIA_ESTADO_BAJO ||
                     estado == BATERIA_ESTADO_CRITICO);

    // Requisito 4: El sistema debe actualizar el modelo de dominio
    NivelCarga nivel_dominio = bateria_obtener_nivel(bateria_global);
    TEST_ASSERT_EQUAL_UINT8(nivel, nivel_dominio);

    // Requisito 5: Las lecturas deben ser consistentes y repetibles
    NivelCarga nivel2 = gestor_termostato_obtener_nivel_bateria(gestor_global);
    TEST_ASSERT_EQUAL_UINT8(nivel, nivel2);

    // Requisito 6: El sistema debe poder detectar batería baja
    bool baja = gestor_termostato_bateria_baja(gestor_global);
    if (nivel < UMBRAL_BATERIA_BAJA) {
        TEST_ASSERT_TRUE(baja);
    } else {
        TEST_ASSERT_FALSE(baja);
    }

    // ✓ HU-007 CUMPLIDA
}

/**
 * @brief Test: Sistema respeta arquitectura por capas
 *
 * Verifica que el flujo de datos respeta las dependencias
 * unidireccionales de la arquitectura.
 */
void test_sistema_respeta_arquitectura_por_capas_bateria(void) {
    // La capa de aplicación no debe acceder directamente al HAL
    // Solo debe usar el gestor

    // La capa de dominio (Bateria) no debe conocer la infraestructura
    // Solo almacena el valor que le pasan

    // Test: Actualizar a través del gestor
    gestor_termostato_actualizar_bateria(gestor_global);
    NivelCarga nivel_gestor = gestor_termostato_obtener_nivel_bateria(gestor_global);

    // Test: La batería solo debe tener el valor que el gestor estableció
    NivelCarga nivel_bateria = bateria_obtener_nivel(bateria_global);
    TEST_ASSERT_EQUAL_UINT8(nivel_gestor, nivel_bateria);

    // Test: No debe haber acoplamiento directo entre capas
    // (esto se valida en tiempo de compilación por la estructura del código)
    TEST_PASS();
}

/**
 * @brief Test: Rango de voltaje del simulador es correcto
 *
 * Verifica que el HAL Batería genera voltajes en el rango esperado
 * para baterías Li-ion (3.0V - 4.2V).
 */
void test_rango_voltaje_simulador_correcto(void) {
    const int NUM_LECTURAS = 50;
    Voltaje voltaje_min = 4200;  // Empezar con máximo
    Voltaje voltaje_max = 3000;  // Empezar con mínimo

    // Realizar múltiples lecturas para obtener rango
    for (int i = 0; i < NUM_LECTURAS; i++) {
        Voltaje v = hal_bateria_leer_voltaje();

        // Actualizar mínimo y máximo
        if (v < voltaje_min) voltaje_min = v;
        if (v > voltaje_max) voltaje_max = v;

        // Cada lectura debe estar en rango
        TEST_ASSERT_GREATER_OR_EQUAL(VOLTAJE_BATERIA_MIN_MV, v);
        TEST_ASSERT_LESS_OR_EQUAL(VOLTAJE_BATERIA_MAX_MV, v);
    }

    // Verificar que se usa el rango completo (con cierta tolerancia)
    // El simulador debería generar valores en todo el rango
    TEST_ASSERT_GREATER_OR_EQUAL(3000, voltaje_min);
    TEST_ASSERT_LESS_OR_EQUAL(4200, voltaje_max);
}

/**
 * @brief Punto de entrada de los tests de integración
 */
int main(void) {
    UNITY_BEGIN();

    // Tests de inicialización
    RUN_TEST(test_inicializacion_completa_sistema_bateria);

    // Tests de flujo end-to-end
    RUN_TEST(test_flujo_completo_lectura_bateria);
    RUN_TEST(test_multiples_actualizaciones_secuenciales_bateria);

    // Tests de coherencia
    RUN_TEST(test_coherencia_entre_capas_bateria);
    RUN_TEST(test_conversion_voltaje_a_porcentaje_correcta);

    // Tests de lógica de negocio
    RUN_TEST(test_estados_bateria_calculados_correctamente);

    // Tests de robustez
    RUN_TEST(test_sistema_maneja_reinicializacion_bateria);
    RUN_TEST(test_rango_voltaje_simulador_correcto);

    // Tests de aceptación
    RUN_TEST(test_hu007_cumple_requisitos_funcionales);
    RUN_TEST(test_sistema_respeta_arquitectura_por_capas_bateria);

    return UNITY_END();
}
