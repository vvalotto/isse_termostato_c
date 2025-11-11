# Checklist de Implementación - ITE-56

**Historia:** HU-014 - Obtener la temperatura ambiente
**Sprint:** Sprint 1 (10-14 Nov 2025)
**Estado:** 🔴 No iniciado

---

## 📋 Progreso General

**Completado:** 0/7 fases (0%)

```
[                                                  ] 0%
```

---

## ✅ Fase 1: Tipos Comunes (5 min)

**Estado:** ⚪ Pendiente

### Tareas

- [ ] Crear archivo `src/common/tipos_comunes.h`
- [ ] Agregar include guards (`#ifndef TIPOS_COMUNES_H`)
- [ ] Incluir headers estándar (`<stdint.h>`, `<stdbool.h>`)
- [ ] Definir `typedef float Temperatura;`
- [ ] Definir `#define TEMP_INICIAL_DEFAULT 20.0f`
- [ ] Agregar comentarios Doxygen
- [ ] Verificar compilación: `./build.sh`

### Criterios de Validación

- [ ] Compila sin errores
- [ ] Compila sin warnings
- [ ] Puede incluirse desde otros módulos

**Tiempo estimado:** 5 min
**Tiempo real:** ___ min

---

## ✅ Fase 2: Entidad Ambiente (20 min)

**Estado:** ⚪ Pendiente

### Archivos a Crear

- [ ] `src/dominio/entidades/ambiente.h`
- [ ] `src/dominio/entidades/ambiente.c`
- [ ] `test/unit/test_ambiente.c`

### Tareas - ambiente.h

- [ ] Crear header con include guards
- [ ] Incluir `tipos_comunes.h`
- [ ] Declarar `typedef struct Ambiente Ambiente;` (opaca)
- [ ] Declarar `Ambiente* ambiente_crear(void);`
- [ ] Declarar `void ambiente_destruir(Ambiente* ambiente);`
- [ ] Declarar `void ambiente_establecer_temperatura(Ambiente*, Temperatura);`
- [ ] Declarar `Temperatura ambiente_obtener_temperatura(const Ambiente*);`
- [ ] Agregar comentarios Doxygen

### Tareas - ambiente.c

- [ ] Incluir `ambiente.h`
- [ ] Incluir `<stdlib.h>` (para malloc/free)
- [ ] Definir `struct Ambiente { Temperatura temperatura_actual; }`
- [ ] Implementar `ambiente_crear()`
  - [ ] Alocar memoria con `malloc()`
  - [ ] Inicializar temperatura a `TEMP_INICIAL_DEFAULT`
  - [ ] Validar puntero no NULL
  - [ ] Retornar puntero
- [ ] Implementar `ambiente_destruir()`
  - [ ] Validar puntero no NULL
  - [ ] Liberar memoria con `free()`
- [ ] Implementar `ambiente_establecer_temperatura()`
  - [ ] Validar puntero no NULL
  - [ ] Asignar valor
- [ ] Implementar `ambiente_obtener_temperatura()`
  - [ ] Validar puntero no NULL
  - [ ] Retornar valor

### Tareas - test_ambiente.c

- [ ] Incluir `unity.h` y `ambiente.h`
- [ ] Implementar `setUp()` y `tearDown()`
- [ ] Test: `test_ambiente_crear_retorna_puntero_valido()`
- [ ] Test: `test_ambiente_destruir_no_falla()`
- [ ] Test: `test_ambiente_establece_temperatura_correctamente()`
- [ ] Test: `test_ambiente_obtiene_temperatura_inicial()`
- [ ] Test: `test_ambiente_obtiene_temperatura_actualizada()`
- [ ] Agregar test a CMakeLists.txt: `add_unit_test(test_ambiente test/unit/test_ambiente.c)`
- [ ] Ejecutar: `./build.sh test`

### Criterios de Validación

- [ ] Compila sin errores ni warnings
- [ ] Todos los tests pasan (5/5)
- [ ] `ctest -R test_ambiente -V` exitoso
- [ ] No hay memory leaks (verificar con valgrind si disponible)

**Tiempo estimado:** 20 min
**Tiempo real:** ___ min

---

## ✅ Fase 3: HAL ADC Simulado (20 min)

**Estado:** ⚪ Pendiente

### Archivos a Crear

- [ ] `src/dispositivos/hal/hal_adc.h`
- [ ] `src/dispositivos/hal/hal_adc.c`
- [ ] `test/unit/test_hal_adc.c`

### Tareas - hal_adc.h

- [ ] Crear header con include guards
- [ ] Incluir `<stdint.h>`
- [ ] Declarar `void hal_adc_init(void);`
- [ ] Declarar `void hal_adc_deinit(void);`
- [ ] Declarar `uint16_t hal_adc_leer_simulado(void);`
- [ ] Agregar comentarios Doxygen
- [ ] Comentar: "TODO: Versiones para STM32/AVR/ESP32 en fases futuras"

### Tareas - hal_adc.c

- [ ] Incluir `hal_adc.h`
- [ ] Incluir `<stdlib.h>` (para rand)
- [ ] Incluir `<time.h>` (para srand)
- [ ] Implementar `hal_adc_init()`
  - [ ] Inicializar semilla aleatoria: `srand(time(NULL));`
- [ ] Implementar `hal_adc_deinit()`
  - [ ] Stub vacío por ahora
- [ ] Implementar `hal_adc_leer_simulado()`
  - [ ] Retornar: `150 + (rand() % 150)`
  - [ ] Comentar: "Simula ADC 150-300 → ~15-30°C"

### Tareas - test_hal_adc.c

- [ ] Incluir `unity.h` y `hal_adc.h`
- [ ] Test: `test_hal_adc_init_no_falla()`
- [ ] Test: `test_hal_adc_leer_retorna_valor_en_rango()`
  - [ ] Verificar: `valor >= 0 && valor <= 1023`
- [ ] Test: `test_hal_adc_multiples_lecturas_retornan_valores_distintos()`
  - [ ] Leer 10 veces
  - [ ] Verificar al menos 2 valores diferentes
- [ ] Agregar test a CMakeLists.txt
- [ ] Ejecutar: `./build.sh test`

### Criterios de Validación

- [ ] Compila sin errores ni warnings
- [ ] Todos los tests pasan (3/3)
- [ ] Valores simulados en rango 0-1023
- [ ] Valores varían entre lecturas

**Tiempo estimado:** 20 min
**Tiempo real:** ___ min

---

## ✅ Fase 4: Sensor Temperatura (25 min)

**Estado:** ⚪ Pendiente

### Archivos a Crear

- [ ] `src/infraestructura/sensores/sensor_temperatura.h`
- [ ] `src/infraestructura/sensores/sensor_temperatura.c`
- [ ] `test/unit/test_sensor_temperatura.c`

### Tareas - sensor_temperatura.h

- [ ] Crear header con include guards
- [ ] Incluir `tipos_comunes.h`
- [ ] Declarar `void sensor_temperatura_init(void);`
- [ ] Declarar `void sensor_temperatura_deinit(void);`
- [ ] Declarar `Temperatura sensor_temperatura_leer(void);`
- [ ] Agregar comentarios Doxygen

### Tareas - sensor_temperatura.c

- [ ] Incluir `sensor_temperatura.h`
- [ ] Incluir `hal_adc.h`
- [ ] Implementar `sensor_temperatura_init()`
  - [ ] Llamar a `hal_adc_init()`
- [ ] Implementar `sensor_temperatura_deinit()`
  - [ ] Llamar a `hal_adc_deinit()`
- [ ] Implementar `sensor_temperatura_leer()`
  - [ ] Leer ADC: `uint16_t adc = hal_adc_leer_simulado();`
  - [ ] Convertir: `Temperatura temp = adc * 0.0488f;`
  - [ ] Comentar fórmula de conversión
  - [ ] Retornar temperatura

### Tareas - test_sensor_temperatura.c

- [ ] Incluir `unity.h` y `sensor_temperatura.h`
- [ ] Test: `test_sensor_temperatura_init_no_falla()`
- [ ] Test: `test_sensor_temperatura_leer_retorna_valor_valido()`
  - [ ] Verificar temperatura > 0
- [ ] Test: `test_sensor_temperatura_leer_retorna_rango_esperado()`
  - [ ] Leer 10 veces
  - [ ] Verificar todas entre 5-40°C aproximadamente
- [ ] Test: `test_sensor_temperatura_conversion_correcta()`
  - [ ] Verificar matemática de conversión
- [ ] Agregar test a CMakeLists.txt
- [ ] Ejecutar: `./build.sh test`

### Criterios de Validación

- [ ] Compila sin errores ni warnings
- [ ] Todos los tests pasan (4/4)
- [ ] Temperaturas en rango esperado (~7-35°C)
- [ ] Conversión matemática verificada

**Tiempo estimado:** 25 min
**Tiempo real:** ___ min

---

## ✅ Fase 5: Gestor Termostato (30 min)

**Estado:** ⚪ Pendiente

### Archivos a Crear

- [ ] `src/aplicacion/gestores/gestor_termostato.h`
- [ ] `src/aplicacion/gestores/gestor_termostato.c`
- [ ] `test/unit/test_gestor_termostato.c`

### Tareas - gestor_termostato.h

- [ ] Crear header con include guards
- [ ] Incluir `ambiente.h` y `tipos_comunes.h`
- [ ] Declarar `typedef struct GestorTermostato GestorTermostato;`
- [ ] Declarar `GestorTermostato* gestor_termostato_crear(Ambiente* ambiente);`
- [ ] Declarar `void gestor_termostato_destruir(GestorTermostato* gestor);`
- [ ] Declarar `void gestor_termostato_actualizar_temperatura(GestorTermostato*);`
- [ ] Declarar `Temperatura gestor_termostato_obtener_temperatura_actual(const GestorTermostato*);`
- [ ] Agregar comentarios Doxygen

### Tareas - gestor_termostato.c

- [ ] Incluir `gestor_termostato.h`
- [ ] Incluir `sensor_temperatura.h`
- [ ] Incluir `<stdlib.h>`
- [ ] Definir `struct GestorTermostato { Ambiente* ambiente; }`
- [ ] Implementar `gestor_termostato_crear()`
  - [ ] Validar ambiente no NULL
  - [ ] Alocar memoria
  - [ ] Guardar referencia a ambiente
  - [ ] Retornar puntero
- [ ] Implementar `gestor_termostato_destruir()`
  - [ ] Validar puntero no NULL
  - [ ] Liberar memoria (NO liberar ambiente, es referencia)
- [ ] Implementar `gestor_termostato_actualizar_temperatura()`
  - [ ] Leer temperatura: `sensor_temperatura_leer()`
  - [ ] Actualizar ambiente: `ambiente_establecer_temperatura()`
- [ ] Implementar `gestor_termostato_obtener_temperatura_actual()`
  - [ ] Delegar a `ambiente_obtener_temperatura()`

### Tareas - test_gestor_termostato.c

- [ ] Incluir `unity.h`, `gestor_termostato.h`, `ambiente.h`, `sensor_temperatura.h`
- [ ] Implementar `setUp()`: inicializar sensor y crear ambiente
- [ ] Implementar `tearDown()`: destruir ambiente
- [ ] Test: `test_gestor_crear_retorna_puntero_valido()`
- [ ] Test: `test_gestor_destruir_no_falla()`
- [ ] Test: `test_gestor_actualizar_temperatura_modifica_ambiente()`
  - [ ] Crear gestor
  - [ ] Actualizar temperatura
  - [ ] Verificar ambiente tiene valor != inicial
- [ ] Test: `test_gestor_obtener_temperatura_retorna_valor_actual()`
- [ ] Test: `test_integracion_lectura_completa()`
  - [ ] Flujo completo: crear → actualizar → obtener
- [ ] Agregar test a CMakeLists.txt
- [ ] Ejecutar: `./build.sh test`

### Criterios de Validación

- [ ] Compila sin errores ni warnings
- [ ] Todos los tests pasan (5/5)
- [ ] Test de integración pasa
- [ ] Flujo completo funciona correctamente

**Tiempo estimado:** 30 min
**Tiempo real:** ___ min

---

## ✅ Fase 6: Integración en main.c (15 min)

**Estado:** ⚪ Pendiente

### Archivos a Modificar

- [ ] `src/main.c`

### Tareas

- [ ] Incluir headers necesarios:
  - [ ] `ambiente.h`
  - [ ] `gestor_termostato.h`
  - [ ] `sensor_temperatura.h`
  - [ ] `hal_adc.h`
- [ ] Buscar sección "FASE 4: Inicialización de Capa de Dominio"
- [ ] Reemplazar TODOs con código real:
  - [ ] `Ambiente* ambiente = ambiente_crear();`
  - [ ] `printf("  ✓ Entidad Ambiente creada (temp inicial: %.1f°C)\n", ...);`
- [ ] Buscar sección "FASE 5: Inicialización de Capa de Aplicación"
- [ ] Reemplazar TODOs con código real:
  - [ ] `GestorTermostato* gestor = gestor_termostato_crear(ambiente);`
  - [ ] Validar gestor no NULL
  - [ ] `printf("  ✓ Gestor de Termostato creado\n");`
- [ ] Buscar sección "FASE 3: Inicialización de Capa de Infraestructura"
- [ ] Agregar inicialización:
  - [ ] `sensor_temperatura_init();`
  - [ ] `printf("  ✓ Sensor de temperatura inicializado\n");`
- [ ] Buscar sección "FASE 2: Inicialización de Capa de Dispositivos (HAL)"
- [ ] Agregar inicialización:
  - [ ] `hal_adc_init();`
  - [ ] `printf("  ✓ HAL ADC inicializado\n");`
- [ ] Buscar comentario "TODO: Implementar ciclo principal de control"
- [ ] Agregar después del printf de "[ INFO ] Iniciando ciclo de control...":
  ```c
  // Simulación: 1 lectura de temperatura
  gestor_termostato_actualizar_temperatura(gestor);
  Temperatura temp_actual = gestor_termostato_obtener_temperatura_actual(gestor);
  printf("\n[ TEMP  ] Temperatura leída: %.1f°C\n", temp_actual);
  ```
- [ ] Buscar sección "FASE 7: Limpieza y Finalización"
- [ ] Agregar limpieza:
  - [ ] `gestor_termostato_destruir(gestor);`
  - [ ] `ambiente_destruir(ambiente);`
  - [ ] `sensor_temperatura_deinit();`
  - [ ] `hal_adc_deinit();`

### Criterios de Validación

- [ ] Compila sin errores ni warnings
- [ ] Ejecuta correctamente: `./build/bin/termostato`
- [ ] Muestra temperatura leída en consola
- [ ] Temperatura en rango esperado (~7-35°C)
- [ ] Programa finaliza correctamente (EXIT_SUCCESS)

**Tiempo estimado:** 15 min
**Tiempo real:** ___ min

---

## ✅ Fase 7: Tests Completos (45 min)

**Estado:** ⚪ Pendiente

### Tareas

- [ ] Revisar y completar test_ambiente.c
  - [ ] Agregar edge cases
  - [ ] Agregar test de valores extremos
- [ ] Revisar y completar test_hal_adc.c
  - [ ] Verificar múltiples inicializaciones
- [ ] Revisar y completar test_sensor_temperatura.c
  - [ ] Agregar test de inicialización múltiple
- [ ] Revisar y completar test_gestor_termostato.c
  - [ ] Agregar test de manejo de NULL
- [ ] Crear `test/integration/test_lectura_temperatura_completa.c`
  - [ ] Test de integración end-to-end
  - [ ] Verificar flujo completo
- [ ] Actualizar CMakeLists.txt con todos los tests
- [ ] Ejecutar suite completa: `./build.sh test`
- [ ] Verificar todos los tests pasan
- [ ] Ejecutar con verbose: `cd build && ctest -V`
- [ ] Verificar cobertura (si herramienta disponible)

### Criterios de Validación

- [ ] Todos los tests unitarios pasan
- [ ] Test de integración pasa
- [ ] `./build.sh test` retorna éxito (exit code 0)
- [ ] No hay memory leaks (valgrind si disponible)
- [ ] Cobertura ≥ 80% (si herramienta disponible)

**Tiempo estimado:** 45 min
**Tiempo real:** ___ min

---

## 🎯 Validación Final Completa

### Compilación

- [ ] `./build.sh` ejecuta sin errores
- [ ] Sin warnings con `-Wall -Wextra -Wpedantic`
- [ ] Genera ejecutable `build/bin/termostato`
- [ ] Genera tests en `build/test/`

### Ejecución

- [ ] `./build/bin/termostato` corre correctamente
- [ ] Muestra salida formateada
- [ ] Muestra temperatura en rango esperado
- [ ] Finaliza sin errores

### Tests

- [ ] `./build.sh test` pasa al 100%
- [ ] Mínimo 20+ tests ejecutados
- [ ] Todos los tests pasan
- [ ] CTest report muestra éxito

### Calidad de Código

- [ ] Nomenclatura snake_case consistente
- [ ] Todas las funciones públicas documentadas (Doxygen)
- [ ] Include guards en todos los headers
- [ ] Sin code smells obvios
- [ ] Sin memory leaks (verificar con valgrind)

### Arquitectura

- [ ] Respeta separación de capas
- [ ] Dependencias unidireccionales
- [ ] Capas aisladas (sin acoplamiento directo)
- [ ] Entidades de dominio puras (sin dependencias externas)

### Git y Control de Versiones

- [ ] Branch correctamente nombrado
- [ ] Commits con mensajes claros
- [ ] Push realizado a GitHub
- [ ] Sin archivos binarios commiteados

---

## 📊 Métricas Finales

### Archivos Creados

- Archivos .h: ____ (esperado: 5)
- Archivos .c: ____ (esperado: 5)
- Archivos test: ____ (esperado: 6)
- **Total:** ____ archivos (esperado: 16)

### Líneas de Código

- Código productivo: ____ líneas
- Código de tests: ____ líneas
- Comentarios: ____ líneas
- **Total:** ____ líneas

### Tests

- Tests unitarios: ____ (esperado: ~20)
- Tests de integración: ____ (esperado: 1-2)
- **Total de tests:** ____
- **Tests pasados:** ____
- **Porcentaje éxito:** ____%

### Tiempo

- Tiempo estimado total: 160 min (~2.5 horas)
- Tiempo real total: ____ min
- Diferencia: ____ min

---

## 🚀 Próximos Pasos (Post Sprint 1)

Una vez completada esta implementación:

### Sprint 2: Manejo de Errores
- [ ] Implementar códigos de error
- [ ] Agregar reintentos en lectura de sensor
- [ ] Tests de manejo de fallos

### Sprint 3: Validación y Límites
- [ ] Validar rango de temperatura (-10 a +50°C)
- [ ] Agregar constantes de límites
- [ ] Tests de valores fuera de rango

### Sprint 4: Logging
- [ ] Implementar aspecto transversal de logging
- [ ] Registrar eventos de lectura
- [ ] Registrar errores

### Sprint 5: Lectura Periódica
- [ ] Implementar ciclo de control (100ms)
- [ ] Timer para lectura periódica
- [ ] Tests de periodicidad

---

**Última actualización:** 2025-11-10
**Estado:** 🔴 No iniciado
**Progreso:** 0/7 fases completadas (0%)
