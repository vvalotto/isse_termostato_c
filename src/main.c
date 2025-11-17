/**
 * @file main.c
 * @brief Punto de entrada del Sistema Termostato ISSE
 * @version 1.0.0
 * @date 2025-11-10
 *
 * Sistema embebido de control de temperatura con arquitectura por capas.
 * Implementa control automático de climatización residencial mediante
 * sensores de temperatura y actuadores de calefacción/refrigeración.
 *
 * Versión actual: Implementa HU-014 (Obtener temperatura ambiente)
 *
 * @author Victor Valotto
 * @copyright Universidad Nacional de Entre Ríos (FIUNER)
 */

#include <stdio.h>
#include <stdlib.h>

#ifdef TARGET_SIMULATOR
#include <unistd.h>  // Para usleep() en simulador
#endif

// TODO: Incluir headers de aspectos transversales cuando estén disponibles
// #include "logging.h"
// #include "configuracion.h"
// #include "auditoria.h"

// Capa de Aplicación
#include "gestor_termostato.h"

// Capa de Dominio
#include "ambiente.h"
#include "bateria.h"

// TODO: Incluir cuando estén disponibles
// #include "climatizador.h"

// Capa de Infraestructura
#include "sensor_temperatura.h"

// TODO: Incluir cuando estén disponibles
// #include "actuador_climatizador.h"

// Capa de Dispositivos
#include "hal_adc.h"

// TODO: Incluir cuando estén disponibles
// #include "hal_gpio.h"

/**
 * @brief Función principal del sistema
 *
 * Punto de entrada que inicializa todas las capas de la arquitectura
 * en el orden correcto (de abajo hacia arriba) y ejecuta el ciclo
 * principal de control del termostato.
 *
 * @return int Código de retorno (0 = éxito, 1 = error)
 */
int main(void) {
    printf("═══════════════════════════════════════════════════════════════\n");
    printf("  ISSE Termostato - Sistema de Control de Temperatura\n");
    printf("═══════════════════════════════════════════════════════════════\n");
    printf("  Versión:      1.0.0\n");
    printf("  Plataforma:   Simulador (desarrollo)\n");
    printf("  Arquitectura: 5 Capas + Aspectos Transversales\n");
    printf("═══════════════════════════════════════════════════════════════\n\n");

    // -------------------------------------------------------------------------
    // FASE 1: Inicialización de Aspectos Transversales
    // -------------------------------------------------------------------------
    printf("[ INIT ] Inicializando aspectos transversales...\n");

    // TODO: Inicializar sistema de logging
    // if (logging_init() != 0) {
    //     fprintf(stderr, "[ERROR] Fallo al inicializar logging\n");
    //     return EXIT_FAILURE;
    // }
    // printf("  ✓ Logging inicializado\n");

    // TODO: Inicializar sistema de configuración
    // if (configuracion_init() != 0) {
    //     fprintf(stderr, "[ERROR] Fallo al cargar configuración\n");
    //     return EXIT_FAILURE;
    // }
    // printf("  ✓ Configuración cargada\n");

    // TODO: Inicializar sistema de auditoría
    // if (auditoria_init() != 0) {
    //     fprintf(stderr, "[ERROR] Fallo al inicializar auditoría\n");
    //     return EXIT_FAILURE;
    // }
    // printf("  ✓ Auditoría inicializada\n");

    printf("  (Aspectos transversales: pendiente de implementación)\n\n");

    // -------------------------------------------------------------------------
    // FASE 2: Inicialización de Capa de Dispositivos (HAL)
    // -------------------------------------------------------------------------
    printf("[ INIT ] Inicializando capa de dispositivos (HAL)...\n");

    // TODO: Inicializar HAL GPIO para control de actuadores
    // if (hal_gpio_init() != 0) {
    //     fprintf(stderr, "[ERROR] Fallo al inicializar GPIO\n");
    //     return EXIT_FAILURE;
    // }
    // printf("  ✓ GPIO inicializado\n");

    // Inicializar HAL ADC para lectura de sensores
    hal_adc_init();
    printf("  ✓ ADC inicializado (simulador)\n\n");

    // -------------------------------------------------------------------------
    // FASE 3: Inicialización de Capa de Infraestructura
    // -------------------------------------------------------------------------
    printf("[ INIT ] Inicializando capa de infraestructura...\n");

    // Inicializar proxy de sensor de temperatura
    sensor_temperatura_init();
    printf("  ✓ Sensor de temperatura inicializado\n");

    // TODO: Inicializar actuador de climatizador
    // if (actuador_climatizador_init() != 0) {
    //     fprintf(stderr, "[ERROR] Fallo al inicializar climatizador\n");
    //     return EXIT_FAILURE;
    // }
    // printf("  ✓ Climatizador inicializado\n");

    printf("\n");

    // -------------------------------------------------------------------------
    // FASE 4: Inicialización de Capa de Dominio
    // -------------------------------------------------------------------------
    printf("[ INIT ] Inicializando capa de dominio...\n");

    // Crear entidad Ambiente con temperatura inicial
    Ambiente* ambiente = ambiente_crear();
    if (ambiente == NULL) {
        fprintf(stderr, "[ERROR] Fallo al crear entidad Ambiente\n");
        sensor_temperatura_deinit();
        hal_adc_deinit();
        return EXIT_FAILURE;
    }
    printf("  ✓ Entidad Ambiente creada (temp inicial: %.1f°C)\n", TEMP_INICIAL_DEFAULT);

    // Crear entidad Batería con carga inicial
    Bateria* bateria = bateria_crear();
    if (bateria == NULL) {
        fprintf(stderr, "[ERROR] Fallo al crear entidad Bateria\n");
        ambiente_destruir(ambiente);
        sensor_temperatura_deinit();
        hal_adc_deinit();
        return EXIT_FAILURE;
    }
    printf("  ✓ Entidad Bateria creada (nivel inicial: %u%%)\n", bateria_obtener_nivel(bateria));

    // TODO: Crear entidad Climatizador en estado apagado
    // Climatizador* climatizador = climatizador_crear();
    // printf("  ✓ Entidad Climatizador creada (estado: APAGADO)\n");

    printf("\n");

    // -------------------------------------------------------------------------
    // FASE 5: Inicialización de Capa de Aplicación
    // -------------------------------------------------------------------------
    printf("[ INIT ] Inicializando capa de aplicación...\n");

    // Crear gestor de termostato (orquestador principal)
    GestorTermostato* gestor = gestor_termostato_crear(ambiente, bateria);
    if (gestor == NULL) {
        fprintf(stderr, "[ERROR] Fallo al crear gestor de termostato\n");
        bateria_destruir(bateria);
        ambiente_destruir(ambiente);
        sensor_temperatura_deinit();
        hal_adc_deinit();
        return EXIT_FAILURE;
    }
    printf("  ✓ Gestor de Termostato creado\n\n");

    // -------------------------------------------------------------------------
    // FASE 6: Demostración de HU-014 (Obtener Temperatura Ambiente)
    // -------------------------------------------------------------------------
    printf("[ READY ] Sistema inicializado correctamente\n");
    printf("[ INFO  ] Demostrando HU-014: Obtener temperatura ambiente\n\n");

    printf("═══════════════════════════════════════════════════════════════\n");
    printf("  Lectura de Temperatura Ambiente (5 muestras)\n");
    printf("═══════════════════════════════════════════════════════════════\n\n");

    // Demostración: Leer temperatura 5 veces con intervalo de 500ms
    for (int i = 1; i <= 5; i++) {
        // CU-006: Obtener la temperatura ambiente
        gestor_termostato_actualizar_temperatura(gestor);
        Temperatura temp = gestor_termostato_obtener_temperatura_actual(gestor);

        printf("  Lectura #%d: %.2f °C\n", i, temp);

        // Esperar 500ms entre lecturas (en simulador)
        #ifdef TARGET_SIMULATOR
        usleep(500000);  // 500ms = 500,000 microsegundos
        #endif
    }

    printf("\n═══════════════════════════════════════════════════════════════\n");
    printf("  HU-014 completado exitosamente\n");
    printf("═══════════════════════════════════════════════════════════════\n\n");

    // TODO: Implementar ciclo de control completo para futuras historias
    // while (1) {
    //     // CU-004: Controlar Termostato (cada 100ms)
    //     gestor_termostato_ejecutar_ciclo(gestor);
    //
    //     // CU-008: Mostrar Estado del Termostato
    //     gestor_termostato_mostrar_estado(gestor);
    //
    //     // Delay de 100ms para cumplir requisito de performance
    //     hal_delay_ms(100);
    // }

    printf("[ INFO  ] Pendientes de implementación:\n");
    printf("  - CU-001 a CU-005: Control del termostato\n");
    printf("  - CU-007: Verificación de batería\n");
    printf("  - CU-008: Visualización de estado\n");
    printf("  - Máquina de estados del termostato\n\n");

    // -------------------------------------------------------------------------
    // FASE 7: Limpieza y Finalización
    // -------------------------------------------------------------------------
    printf("[ EXIT  ] Finalizando sistema...\n");

    // Liberar recursos de capa de aplicación
    gestor_termostato_destruir(gestor);
    printf("  ✓ Gestor de Termostato destruido\n");

    // Liberar recursos de capa de dominio
    ambiente_destruir(ambiente);
    printf("  ✓ Entidad Ambiente destruida\n");

    bateria_destruir(bateria);
    printf("  ✓ Entidad Bateria destruida\n");

    // TODO: Liberar cuando estén disponibles
    // climatizador_destruir(climatizador);

    // Desinicializar capa de infraestructura
    sensor_temperatura_deinit();
    printf("  ✓ Sensor de temperatura desinicializado\n");

    // TODO: Desinicializar cuando estén disponibles
    // actuador_climatizador_deinit();

    // Desinicializar capa de dispositivos
    hal_adc_deinit();
    printf("  ✓ ADC desinicializado\n");

    // TODO: Desinicializar cuando estén disponibles
    // hal_gpio_deinit();

    // TODO: Desinicializar aspectos transversales cuando estén disponibles
    // auditoria_deinit();
    // configuracion_deinit();
    // logging_deinit();

    printf("\n═══════════════════════════════════════════════════════════════\n");
    printf("  Sistema finalizado correctamente\n");
    printf("═══════════════════════════════════════════════════════════════\n");

    return EXIT_SUCCESS;
}
