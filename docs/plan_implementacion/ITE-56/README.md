# Plan de Implementación - ITE-56

**Historia de Usuario:** HU-014 - Obtener la temperatura ambiente
**Epic:** ITE-44 - CU-006: Obtener Temperatura Ambiente
**Sprint:** Sprint 1 (10-Nov a 14-Nov-2025)
**Asignado:** Victor Valotto
**Estado:** En curso

---

## 📋 Índice

1. [Resumen Ejecutivo](#resumen-ejecutivo)
2. [Contexto y Migración](va#contexto-y-migración)
3. [Arquitectura de la Solución](#arquitectura-de-la-solución)
4. [Componentes a Implementar](#componentes-a-implementar)
5. [Plan de Fases](#plan-de-fases)
6. [Orden de Implementación](#orden-de-implementación)
7. [Criterios de Aceptación](#criterios-de-aceptación)
8. [Checklist de Progreso](#checklist-de-progreso)

---

## 📊 Resumen Ejecutivo

### Objetivo

Implementar la funcionalidad de lectura de temperatura ambiente del sensor, migrar desde la versión Python/Raspberry Pi a C para microcontrolador, manteniendo la arquitectura por capas y el enfoque didáctico progresivo.

### Historia de Usuario

> **Como** sistema de control del termostato
> **Quiero** leer continuamente la temperatura ambiente a través del sensor de temperatura
> **Para** mantener actualizado el valor de temperatura y permitir el control automático del climatizador

### Alcance de Esta Iteración

**Incluido:**
- ✅ Lectura básica de temperatura desde sensor simulado
- ✅ Almacenamiento en entidad de dominio Ambiente
- ✅ Coordinación desde gestor de aplicación
- ✅ Capa HAL simulada para desarrollo sin hardware
- ✅ Tests unitarios básicos

**NO Incluido (Fases Futuras):**
- ❌ Manejo de errores robusto (reintentos, excepciones)
- ❌ Validación de rango de temperatura
- ❌ Sistema de logging/auditoría
- ❌ Filtrado de ruido (promedio móvil)
- ❌ Lectura periódica automática (ciclo de control)

### Estimación

| Fase | Componentes | Tiempo Estimado |
|------|-------------|-----------------|
| Fase 1 | Tipos comunes | 5 min |
| Fase 2 | Entidad Ambiente | 20 min |
| Fase 3 | HAL ADC simulado | 20 min |
| Fase 4 | Sensor Temperatura (Proxy) | 25 min |
| Fase 5 | Gestor Termostato | 30 min |
| Fase 6 | Integración en main.c | 15 min |
| Fase 7 | Tests unitarios | 45 min |
| **TOTAL** | | **~2.5 horas** |

---

## 🔄 Contexto y Migración

### Versión Anterior: Python + Raspberry Pi

La implementación anterior en Python sobre Raspberry Pi utilizaba:

- **Lenguaje:** Python 3.x
- **Hardware:** Raspberry Pi (ARM Cortex-A, Linux)
- **Librerías:** `RPi.GPIO`, `gpiozero`
- **Paradigma:** POO (Programación Orientada a Objetos)
- **Gestión de memoria:** Garbage Collector automático
- **Manejo de errores:** Excepciones Python

**Componentes Python:**
```python
class GestorAmbiente:
    def leer_temperatura_ambiente(self):
        temp = self.proxy_sensor.leer_temperatura()
        self.ambiente.temperatura = temp

class ProxySensorTemperatura:
    def leer_temperatura(self):
        valor_adc = GPIO.read(PIN_SENSOR)
        return convertir_a_celsius(valor_adc)

class Ambiente:
    def __init__(self):
        self.temperatura = 20.0
```

### Versión Nueva: C + Microcontrolador

**Objetivo de la migración:**
- **Lenguaje:** C11
- **Hardware objetivo:** Microcontrolador genérico (STM32/AVR/ESP32/Simulador)
- **Paradigma:** Programación estructurada con módulos
- **Gestión de memoria:** Manual (malloc/free)
- **Portabilidad:** Mediante capa HAL

**Adaptaciones clave:**
1. **POO → Estructuras + funciones:** Structs opacas con funciones como "métodos"
2. **RPi.GPIO → HAL ADC:** Abstracción propia de hardware
3. **Nomenclatura:** PascalCase → snake_case (convención C)
4. **Memoria:** GC automático → malloc/free explícito
5. **Nueva capa:** HAL para abstracción de hardware

---

## 🏗️ Arquitectura de la Solución

### Diagrama de Capas (5 niveles)

```
┌──────────────────────────────────────────────────────────┐
│          ASPECTOS TRANSVERSALES (Fase futura)            │
│                (Logging, Validación)                     │
└──────────────────────────────────────────────────────────┘
                            ↓
┌──────────────────────────────────────────────────────────┐
│          1. CAPA DE APLICACIÓN                           │
│     src/aplicacion/gestores/                             │
│                                                          │
│     • gestor_termostato.h/.c                             │
│       - Coordinador principal                            │
│       - Orquesta la lectura de temperatura               │
└──────────────────────────────────────────────────────────┘
                            ↓
┌──────────────────────────────────────────────────────────┐
│          2. CAPA DE DOMINIO                              │
│     src/dominio/entidades/                               │
│                                                          │
│     • ambiente.h/.c                                      │
│       - Entidad que modela el ambiente                   │
│       - Almacena temperatura actual                      │
└──────────────────────────────────────────────────────────┘
                            ↓
┌──────────────────────────────────────────────────────────┐
│          3. CAPA DE INFRAESTRUCTURA                      │
│     src/infraestructura/sensores/                        │
│                                                          │
│     • sensor_temperatura.h/.c                            │
│       - Proxy del sensor físico                          │
│       - Conversión ADC → °C                              │
└──────────────────────────────────────────────────────────┘
                            ↓
┌──────────────────────────────────────────────────────────┐
│          4. CAPA DE DISPOSITIVOS (HAL)                   │
│     src/dispositivos/hal/                                │
│                                                          │
│     • hal_adc.h/.c                                       │
│       - Abstracción del ADC                              │
│       - Versión simulada para desarrollo                 │
└──────────────────────────────────────────────────────────┘
                            ↓
┌──────────────────────────────────────────────────────────┐
│          5. HARDWARE FÍSICO                              │
│     (Simulado en esta fase)                              │
│                                                          │
│     • Sensor de temperatura (LM35, DHT22, etc.)          │
│     • ADC del microcontrolador                           │
└──────────────────────────────────────────────────────────┘
```

### Diagrama de Secuencia Adaptado

```
┌─────────────┐  ┌────────────┐  ┌──────────────┐  ┌─────────┐
│   main.c    │  │  Gestor    │  │SensorTemp.   │  │HAL_ADC  │  ┌─────────┐
│(Ciclo ctrl.)│  │ Termostato │  │              │  │         │  │Ambiente │
└──────┬──────┘  └─────┬──────┘  └──────┬───────┘  └────┬────┘  └────┬────┘
       │                │                │               │            │
       │ 1. gestor_termostato_actualizar_temperatura()  │            │
       │─────────────>  │                │               │            │
       │                │                │               │            │
       │                │ 2. sensor_temperatura_leer()   │            │
       │                │─────────────>  │               │            │
       │                │                │               │            │
       │                │                │ 3. hal_adc_leer_simulado() │
       │                │                │─────────────> │            │
       │                │                │               │            │
       │                │                │ 4. adc_value  │            │
       │                │                │<───────────── │            │
       │                │                │               │            │
       │                │                │ 5. convertir_adc_a_celsius │
       │                │                │               │            │
       │                │ 6. temperatura │               │            │
       │                │<─────────────  │               │            │
       │                │                │               │            │
       │                │ 7. ambiente_establecer_temperatura(temp)    │
       │                │────────────────────────────────────────────>│
       │                │                │               │            │
       │                │ 8. OK          │               │            │
       │                │<────────────────────────────────────────────│
       │                │                │               │            │
       │ 9. OK          │                │               │            │
       │<─────────────  │                │               │            │
```

### Dependencias entre Componentes

```
gestor_termostato (Aplicación)
    ├── depende de → sensor_temperatura (Infraestructura)
    └── depende de → ambiente (Dominio)

sensor_temperatura (Infraestructura)
    └── depende de → hal_adc (Dispositivos)

hal_adc (Dispositivos)
    └── NO depende de nada (hoja)

ambiente (Dominio)
    └── NO depende de nada (puro)
```

**Orden de implementación (bottom-up):**
1. Tipos comunes
2. Entidad Ambiente (Dominio)
3. HAL ADC (Dispositivos)
4. Sensor Temperatura (Infraestructura)
5. Gestor Termostato (Aplicación)

---

## 🔧 Componentes a Implementar

### 1. Tipos Comunes (src/common/)

**Archivos:**
- `tipos_comunes.h`

**Responsabilidad:**
- Definir tipos base del sistema
- Constantes globales
- Tipos para temperatura

**Contenido:**
```c
// Alias de tipo para temperatura
typedef float Temperatura;

// Constantes
#define TEMP_INICIAL_DEFAULT 20.0f
```

---

### 2. Entidad Ambiente (src/dominio/entidades/)

**Archivos:**
- `ambiente.h` (interfaz pública)
- `ambiente.c` (implementación)

**Responsabilidad:**
- Modelar el concepto de dominio "Ambiente"
- Almacenar temperatura actual
- Operaciones de negocio básicas

**Interfaz pública:**
```c
typedef struct Ambiente Ambiente;

// Ciclo de vida
Ambiente* ambiente_crear(void);
void ambiente_destruir(Ambiente* ambiente);

// Operaciones
void ambiente_establecer_temperatura(Ambiente* ambiente, Temperatura temp);
Temperatura ambiente_obtener_temperatura(const Ambiente* ambiente);
```

**Estructura interna:**
```c
struct Ambiente {
    Temperatura temperatura_actual;
};
```

**Tests:**
- `test/unit/test_ambiente.c`

---

### 3. HAL ADC (src/dispositivos/hal/)

**Archivos:**
- `hal_adc.h` (interfaz pública)
- `hal_adc.c` (implementación simulada)

**Responsabilidad:**
- Abstracción del ADC del microcontrolador
- Versión simulada para desarrollo sin hardware
- Interfaz única para todas las plataformas

**Interfaz pública:**
```c
// Inicialización
void hal_adc_init(void);
void hal_adc_deinit(void);

// Lectura simulada (retorna valor ADC de 0-1023)
uint16_t hal_adc_leer_simulado(void);
```

**Implementación simulada:**
```c
// Simula lectura de sensor:
// - Valor ADC: 0-1023
// - Correspondiente a ~15-30°C
uint16_t hal_adc_leer_simulado(void) {
    // Retorna valor aleatorio entre 150-300
    // Equivale a ~15-30°C después de conversión
    return 150 + (rand() % 150);
}
```

**Tests:**
- `test/unit/test_hal_adc.c`

---

### 4. Sensor Temperatura (src/infraestructura/sensores/)

**Archivos:**
- `sensor_temperatura.h` (interfaz pública)
- `sensor_temperatura.c` (implementación)

**Responsabilidad:**
- Proxy del sensor físico
- Delegar a HAL para lectura
- Conversión ADC → Temperatura en °C

**Interfaz pública:**
```c
// Inicialización
void sensor_temperatura_init(void);
void sensor_temperatura_deinit(void);

// Lectura (versión simple, retorna directamente)
Temperatura sensor_temperatura_leer(void);
```

**Lógica de conversión:**
```c
// Ejemplo para sensor LM35 simulado:
// ADC: 0-1023 → Voltaje: 0-5V
// LM35: 10mV/°C
// Temperatura = (adc_value * 5.0 / 1023.0) / 0.01
//
// Simplificado para esta fase:
Temperatura sensor_temperatura_leer(void) {
    uint16_t adc_value = hal_adc_leer_simulado();

    // Conversión simple: ADC 0-1023 → 0-50°C
    // Factor de conversión: 0.0488 (≈ 50/1023)
    Temperatura temp = adc_value * 0.0488f;

    return temp;
}
```

**Tests:**
- `test/unit/test_sensor_temperatura.c`

---

### 5. Gestor Termostato (src/aplicacion/gestores/)

**Archivos:**
- `gestor_termostato.h` (interfaz pública)
- `gestor_termostato.c` (implementación)

**Responsabilidad:**
- Coordinador principal de la aplicación
- Orquesta la lectura de temperatura
- Actualiza la entidad Ambiente

**Interfaz pública:**
```c
typedef struct GestorTermostato GestorTermostato;

// Ciclo de vida
GestorTermostato* gestor_termostato_crear(Ambiente* ambiente);
void gestor_termostato_destruir(GestorTermostato* gestor);

// Operaciones principales
void gestor_termostato_actualizar_temperatura(GestorTermostato* gestor);
Temperatura gestor_termostato_obtener_temperatura_actual(const GestorTermostato* gestor);
```

**Estructura interna:**
```c
struct GestorTermostato {
    Ambiente* ambiente;  // Referencia a la entidad de dominio
};
```

**Lógica principal:**
```c
void gestor_termostato_actualizar_temperatura(GestorTermostato* gestor) {
    // 1. Leer del sensor (capa infraestructura)
    Temperatura temp = sensor_temperatura_leer();

    // 2. Actualizar entidad de dominio
    ambiente_establecer_temperatura(gestor->ambiente, temp);
}
```

**Tests:**
- `test/unit/test_gestor_termostato.c`
- `test/integration/test_lectura_temperatura_completa.c`

---

## 📅 Plan de Fases

### Fase 1: Tipos Comunes (5 min)

**Objetivo:** Establecer tipos base del sistema

**Archivos a crear:**
- `src/common/tipos_comunes.h`

**Tareas:**
1. Crear header con guardas
2. Incluir headers estándar necesarios
3. Definir typedef para Temperatura
4. Definir constantes globales

**Criterio de validación:**
- ✅ Compila sin errores
- ✅ Puede incluirse desde otros módulos

---

### Fase 2: Entidad Ambiente (20 min)

**Objetivo:** Implementar entidad de dominio pura

**Archivos a crear:**
- `src/dominio/entidades/ambiente.h`
- `src/dominio/entidades/ambiente.c`
- `test/unit/test_ambiente.c`

**Tareas:**
1. Crear header con interfaz pública
2. Implementar struct opaca
3. Implementar funciones de ciclo de vida
4. Implementar getters/setters
5. Crear tests unitarios
6. Actualizar CMakeLists.txt para tests

**Criterios de validación:**
- ✅ Compila sin errores
- ✅ Tests pasan (crear/destruir, set/get)
- ✅ No hay memory leaks (valgrind si disponible)

---

### Fase 3: HAL ADC Simulado (20 min)

**Objetivo:** Abstracción de hardware para desarrollo sin HW físico

**Archivos a crear:**
- `src/dispositivos/hal/hal_adc.h`
- `src/dispositivos/hal/hal_adc.c`
- `test/unit/test_hal_adc.c`

**Tareas:**
1. Crear header con interfaz HAL
2. Implementar versión simulada
3. Agregar comentarios sobre versiones reales futuras
4. Crear tests unitarios
5. Verificar valores en rango esperado

**Criterios de validación:**
- ✅ Compila sin errores
- ✅ Tests pasan
- ✅ Valores simulados en rango 0-1023
- ✅ Múltiples lecturas retornan valores distintos (simulación de variabilidad)

---

### Fase 4: Sensor Temperatura (25 min)

**Objetivo:** Proxy de infraestructura que usa HAL

**Archivos a crear:**
- `src/infraestructura/sensores/sensor_temperatura.h`
- `src/infraestructura/sensores/sensor_temperatura.c`
- `test/unit/test_sensor_temperatura.c`

**Tareas:**
1. Crear header con interfaz pública
2. Implementar inicialización
3. Implementar lectura con conversión ADC→°C
4. Crear tests unitarios
5. Verificar conversión correcta

**Criterios de validación:**
- ✅ Compila sin errores
- ✅ Tests pasan
- ✅ Temperaturas en rango esperado (~15-30°C)
- ✅ Conversión matemática correcta

---

### Fase 5: Gestor Termostato (30 min)

**Objetivo:** Coordinador de capa de aplicación

**Archivos a crear:**
- `src/aplicacion/gestores/gestor_termostato.h`
- `src/aplicacion/gestores/gestor_termostato.c`
- `test/unit/test_gestor_termostato.c`

**Tareas:**
1. Crear header con interfaz pública
2. Implementar struct con referencia a Ambiente
3. Implementar ciclo de vida
4. Implementar actualización de temperatura
5. Crear tests unitarios
6. Crear test de integración básico

**Criterios de validación:**
- ✅ Compila sin errores
- ✅ Tests unitarios pasan
- ✅ Test de integración pasa (lectura → ambiente)
- ✅ Flujo completo funciona

---

### Fase 6: Integración en main.c (15 min)

**Objetivo:** Integrar componentes en programa principal

**Archivos a modificar:**
- `src/main.c`

**Tareas:**
1. Eliminar placeholders/TODOs
2. Incluir headers necesarios
3. Inicializar HAL
4. Crear instancias (Ambiente, Gestor)
5. Ejecutar lectura de prueba
6. Mostrar resultado en consola
7. Liberar recursos

**Criterios de validación:**
- ✅ Compila sin errores
- ✅ Ejecuta correctamente
- ✅ Muestra temperatura en consola
- ✅ No hay memory leaks

---

### Fase 7: Tests Unitarios Completos (45 min)

**Objetivo:** Cobertura de tests robusta

**Archivos a crear:**
- Tests unitarios para cada componente
- Test de integración completo

**Tareas:**
1. Completar test_ambiente.c
2. Completar test_hal_adc.c
3. Completar test_sensor_temperatura.c
4. Completar test_gestor_termostato.c
5. Crear test_integracion_lectura.c
6. Verificar todos los tests pasan
7. Verificar cobertura ≥80% (si herramienta disponible)

**Criterios de validación:**
- ✅ Todos los tests pasan
- ✅ CTest ejecuta exitosamente
- ✅ `./build.sh test` pasa sin errores

---

## 🔀 Orden de Implementación Detallado

### Orden Bottom-Up (Recomendado)

Este orden respeta las dependencias y permite probar cada componente aisladamente:

```
1. tipos_comunes.h
   └── Sin dependencias

2. ambiente.h/.c + test
   └── Depende: tipos_comunes.h

3. hal_adc.h/.c + test
   └── Sin dependencias de proyecto

4. sensor_temperatura.h/.c + test
   ├── Depende: hal_adc.h
   └── Depende: tipos_comunes.h

5. gestor_termostato.h/.c + test
   ├── Depende: sensor_temperatura.h
   ├── Depende: ambiente.h
   └── Depende: tipos_comunes.h

6. main.c (integración)
   └── Depende: todos los anteriores

7. test_integracion_completo.c
   └── Depende: todos los anteriores
```

### Checklist por Componente

Para cada componente, seguir este proceso:

1. **Diseño:**
   - [ ] Definir interfaz pública (header)
   - [ ] Documentar funciones con comentarios Doxygen
   - [ ] Identificar dependencias

2. **Implementación:**
   - [ ] Crear archivo .h con guardas
   - [ ] Crear archivo .c con implementación
   - [ ] Incluir headers necesarios
   - [ ] Implementar funciones
   - [ ] Agregar validación de parámetros (si aplica)

3. **Compilación:**
   - [ ] Agregar a CMakeLists.txt si necesario
   - [ ] Compilar: `cmake --build build`
   - [ ] Resolver errores de compilación
   - [ ] Resolver warnings

4. **Testing:**
   - [ ] Crear test_*.c
   - [ ] Agregar test a CMakeLists.txt
   - [ ] Implementar casos de prueba
   - [ ] Ejecutar: `cd build && ctest -R test_nombre -V`
   - [ ] Verificar que pasa

5. **Integración:**
   - [ ] Probar con componente consumidor
   - [ ] Verificar no rompe otros tests
   - [ ] Ejecutar suite completa: `./build.sh test`

6. **Commit:**
   - [ ] Revisar cambios: `git status`
   - [ ] Commit: `git commit -m "[ITE-56] Implementar [componente]"`
   - [ ] Push: `git push origin ITE-56-HU-014-Obtener-la-temperatura-ambiente`

---

## ✅ Criterios de Aceptación

### Criterios de ITE-56 (Simplificados para esta fase)

| # | Criterio Original | Implementado en esta Fase | Estado |
|---|-------------------|---------------------------|--------|
| 1 | ¿Lee correctamente del sensor? | ✅ Sí, mediante HAL simulado | Sprint 1 |
| 2 | ¿Almacena en entidad Ambiente? | ✅ Sí, mediante gestor | Sprint 1 |
| 3 | ¿Maneja sensor no disponible? | ❌ Fase futura | Sprint 2+ |
| 4 | ¿Registra eventos de error? | ❌ Fase futura (logging) | Sprint 2+ |
| 5 | ¿Lectura periódica automática? | ⚠️ Parcial (1 lectura manual) | Sprint 2+ |
| 6 | ¿Validación de rango (-10 a +50°C)? | ❌ Fase futura | Sprint 2+ |

### Criterios de Validación Técnica

**Compilación:**
- [ ] `./build.sh` ejecuta sin errores
- [ ] No hay warnings (con `-Wall -Wextra`)
- [ ] Genera ejecutable `build/bin/termostato`

**Ejecución:**
- [ ] `./build/bin/termostato` corre correctamente
- [ ] Muestra temperatura leída en consola
- [ ] Salida esperada: "Temperatura actual: XX.X°C"
- [ ] Temperatura en rango 15-30°C aproximadamente

**Tests:**
- [ ] `./build.sh test` pasa al 100%
- [ ] Mínimo 5 tests implementados (1 por componente)
- [ ] Tests de integración pasa

**Calidad de Código:**
- [ ] Nomenclatura snake_case consistente
- [ ] Funciones documentadas con comentarios
- [ ] Sin memory leaks (verificar con valgrind si disponible)
- [ ] Headers con include guards

**Arquitectura:**
- [ ] Respeta separación de capas
- [ ] Dependencias unidireccionales
- [ ] No hay acoplamiento entre capas no adyacentes

---

## 📊 Checklist de Progreso

### Componentes Implementados

- [ ] **tipos_comunes.h** (src/common/)
  - [ ] Typedef Temperatura
  - [ ] Constantes globales
  - [ ] Compila correctamente

- [ ] **ambiente** (src/dominio/entidades/)
  - [ ] ambiente.h (interfaz)
  - [ ] ambiente.c (implementación)
  - [ ] test_ambiente.c
  - [ ] Tests pasan

- [ ] **hal_adc** (src/dispositivos/hal/)
  - [ ] hal_adc.h (interfaz)
  - [ ] hal_adc.c (versión simulada)
  - [ ] test_hal_adc.c
  - [ ] Tests pasan

- [ ] **sensor_temperatura** (src/infraestructura/sensores/)
  - [ ] sensor_temperatura.h (interfaz)
  - [ ] sensor_temperatura.c (implementación)
  - [ ] test_sensor_temperatura.c
  - [ ] Tests pasan

- [ ] **gestor_termostato** (src/aplicacion/gestores/)
  - [ ] gestor_termostato.h (interfaz)
  - [ ] gestor_termostato.c (implementación)
  - [ ] test_gestor_termostato.c
  - [ ] Tests pasan

- [ ] **Integración en main.c**
  - [ ] Eliminar TODOs relacionados
  - [ ] Incluir headers
  - [ ] Inicializar componentes
  - [ ] Ejecutar lectura
  - [ ] Mostrar resultado
  - [ ] Liberar recursos

- [ ] **Tests de Integración**
  - [ ] test_integracion_lectura.c
  - [ ] Test pasa

### Validación Final

- [ ] Compilación completa sin errores ni warnings
- [ ] Todos los tests unitarios pasan
- [ ] Test de integración pasa
- [ ] Ejecutable funciona correctamente
- [ ] Memoria liberada correctamente
- [ ] Código documentado
- [ ] Commits realizados con mensajes claros

### Documentación

- [ ] Este plan de implementación completo
- [ ] Comentarios Doxygen en headers
- [ ] README actualizado si necesario
- [ ] Diagramas validados

### Control de Versiones

- [ ] Branch creado: `ITE-56-HU-014-Obtener-la-temperatura-ambiente`
- [ ] Commits incrementales realizados
- [ ] Push a GitHub realizado
- [ ] PR creado (al finalizar)

---

## 📝 Notas Adicionales

### Simplificaciones Didácticas

Este plan implementa una versión **simplificada** con fines didácticos:

1. **Sin manejo de errores robusto:** Las funciones retornan valores directamente sin códigos de error
2. **Sin validación de rango:** Se asume que el simulador retorna valores válidos
3. **Sin logging:** Se agregará en sprints futuros
4. **Sin filtrado:** No se implementa promedio móvil u otros filtros
5. **Sin lectura periódica:** Se ejecuta 1 sola lectura en main()

### Extensiones Futuras

Después de completar este sprint, se pueden agregar:

- **Sprint 2:** Manejo de errores (códigos de retorno, reintentos)
- **Sprint 3:** Validación de rango y límites
- **Sprint 4:** Sistema de logging
- **Sprint 5:** Lectura periódica automática (ciclo de control)
- **Sprint 6:** Filtrado de ruido (promedio móvil)
- **Sprint 7:** HAL real para hardware específico (STM32/AVR/ESP32)

### Comandos Útiles

```bash
# Compilar
./build.sh

# Compilar y ejecutar tests
./build.sh test

# Compilar y ejecutar aplicación
./build.sh run

# Limpiar y recompilar
./build.sh clean

# Ejecutar test específico
cd build && ctest -R test_ambiente -V

# Verificar memory leaks (si valgrind disponible)
valgrind --leak-check=full ./build/bin/termostato
```

---

## 📚 Referencias

- **Jira:** https://vvalotto.atlassian.net/browse/ITE-56
- **Confluence CU-006:** https://vvalotto.atlassian.net/wiki/spaces/I2A/pages/169639963
- **CLAUDE.md:** `/Users/victor/CLionProjects/isse_termostato_c/CLAUDE.md`
- **CONTRIBUTING.md:** Convenciones de código y nomenclatura
- **Diagramas:** `docs/diagramas/ITE-56/diagrama.docx`

---

**Documento generado:** 2025-11-10
**Versión:** 1.0
**Estado:** Listo para implementación
