# Plan de Implementación - ITE-57

**Historia de Usuario:** HU-007 - Obtener el nivel de carga de la batería
**Epic:** ITE-44 - Monitoreo de Sistema
**Sprint:** Sprint 2 (17-Nov a 21-Nov-2025)
**Asignado:** Victor Valotto
**Estado:** Planificado

---

## 📋 Índice

1. [Resumen Ejecutivo](#resumen-ejecutivo)
2. [Contexto y Referencia](#contexto-y-referencia)
3. [Arquitectura de la Solución](#arquitectura-de-la-solución)
4. [Componentes a Implementar](#componentes-a-implementar)
5. [Plan de Fases](#plan-de-fases)
6. [Orden de Implementación](#orden-de-implementación)
7. [Criterios de Aceptación](#criterios-de-aceptación)

---

## 📊 Resumen Ejecutivo

### Objetivo

Implementar la funcionalidad de monitoreo del nivel de carga de la batería, siguiendo la arquitectura por capas establecida en ITE-56 y manteniendo el enfoque didáctico progresivo del proyecto.

### Historia de Usuario

> **Como** sistema de control del termostato
> **Quiero** monitorear continuamente el nivel de carga de la batería
> **Para** prevenir apagados inesperados y alertar sobre batería baja

### Alcance de Esta Iteración

**Incluido:**
- ✅ Lectura básica del nivel de batería desde sensor simulado
- ✅ Almacenamiento en entidad de dominio Bateria
- ✅ Conversión de voltaje a porcentaje de carga
- ✅ Capa HAL simulada para desarrollo sin hardware
- ✅ Tests unitarios básicos
- ✅ Integración en main.c

**NO Incluido (Fases Futuras):**
- ❌ Alertas de batería baja (umbral crítico)
- ❌ Sistema de logging/auditoría de eventos de batería
- ❌ Gestión de modos de ahorro de energía
- ❌ Estimación de tiempo restante de batería
- ❌ Monitoreo de salud de la batería (ciclos de carga)
- ❌ Detección de batería en carga

### Estimación

| Fase | Componentes | Tiempo Estimado |
|------|-------------|-----------------|
| Fase 1 | Tipos comunes (extensión) | 5 min |
| Fase 2 | Entidad Bateria | 20 min |
| Fase 3 | HAL Batería (ADC) | 20 min |
| Fase 4 | Sensor Batería (Proxy) | 25 min |
| Fase 5 | Extensión Gestor Termostato | 25 min |
| Fase 6 | Integración en main.c | 15 min |
| Fase 7 | Tests unitarios e integración | 45 min |
| **TOTAL** | | **~2.5 horas** |

---

## 🔄 Contexto y Referencia

### Patrón Establecido: ITE-56 (Temperatura)

La implementación de ITE-56 estableció el patrón arquitectónico que seguiremos:

**Capas implementadas en ITE-56:**
```
HAL ADC → Sensor Temperatura → Entidad Ambiente → Gestor Termostato → main.c
```

**Componentes existentes que reutilizaremos:**
- `tipos_comunes.h` - Tipos base del sistema
- `gestor_termostato.c` - Coordinador principal (extenderemos)
- Estructura de testing con Unity
- CMake build system configurado

### Patrón a Replicar: ITE-57 (Batería)

Seguiremos el mismo flujo arquitectónico:

```
HAL Batería → Sensor Batería → Entidad Bateria → Gestor Termostato → main.c
```

**Similaridades con ITE-56:**
- Misma estructura de capas (5 niveles)
- Mismo patrón de dependencias (bottom-up)
- Mismos patrones de código (structs opacas, memoria manual)
- Mismo framework de testing

**Diferencias clave:**
- **Medición:** Voltaje de batería (ADC) → Porcentaje de carga (%)
- **Entidad:** Bateria almacena nivel de carga y estado
- **Conversión:** Curva de descarga de batería Li-ion (no lineal)
- **Validación:** Rango 0-100% vs temperatura -10 a +50°C

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
│     • gestor_termostato.h/.c (EXTENDER)                  │
│       - Agregar coordinación de batería                  │
│       - Orquesta lectura de nivel de carga               │
└──────────────────────────────────────────────────────────┘
                            ↓
┌──────────────────────────────────────────────────────────┐
│          2. CAPA DE DOMINIO                              │
│     src/dominio/entidades/                               │
│                                                          │
│     • bateria.h/.c (NUEVO)                               │
│       - Entidad que modela la batería                    │
│       - Almacena nivel de carga (0-100%)                 │
│       - Estado de carga (bajo, normal, crítico)          │
└──────────────────────────────────────────────────────────┘
                            ↓
┌──────────────────────────────────────────────────────────┐
│          3. CAPA DE INFRAESTRUCTURA                      │
│     src/infraestructura/sensores/                        │
│                                                          │
│     • sensor_bateria.h/.c (NUEVO)                        │
│       - Proxy del sensor de batería                      │
│       - Conversión Voltaje → Porcentaje                  │
└──────────────────────────────────────────────────────────┘
                            ↓
┌──────────────────────────────────────────────────────────┐
│          4. CAPA DE DISPOSITIVOS (HAL)                   │
│     src/dispositivos/hal/                                │
│                                                          │
│     • hal_bateria.h/.c (NUEVO)                           │
│       - Abstracción del ADC para batería                 │
│       - Versión simulada para desarrollo                 │
└──────────────────────────────────────────────────────────┘
                            ↓
┌──────────────────────────────────────────────────────────┐
│          5. HARDWARE FÍSICO                              │
│     (Simulado en esta fase)                              │
│                                                          │
│     • Divisor de voltaje (batería → ADC)                 │
│     • ADC del microcontrolador                           │
│     • Batería Li-ion (3.0V - 4.2V típico)                │
└──────────────────────────────────────────────────────────┘
```

### Diagrama de Secuencia

```
┌─────────────┐  ┌────────────┐  ┌──────────────┐  ┌─────────────┐
│   main.c    │  │  Gestor    │  │SensorBateria │  │HAL_Bateria  │  ┌─────────┐
│(Ciclo ctrl.)│  │ Termostato │  │              │  │             │  │Bateria  │
└──────┬──────┘  └─────┬──────┘  └──────┬───────┘  └──────┬──────┘  └────┬────┘
       │                │                │                 │              │
       │ 1. gestor_termostato_actualizar_bateria()        │              │
       │─────────────>  │                │                 │              │
       │                │                │                 │              │
       │                │ 2. sensor_bateria_leer()         │              │
       │                │─────────────>  │                 │              │
       │                │                │                 │              │
       │                │                │ 3. hal_bateria_leer_voltaje()  │
       │                │                │─────────────>   │              │
       │                │                │                 │              │
       │                │                │ 4. voltaje_mv   │              │
       │                │                │<───────────────  │              │
       │                │                │                 │              │
       │                │                │ 5. convertir_voltaje_a_porcentaje│
       │                │                │                 │              │
       │                │ 6. nivel_carga (%)               │              │
       │                │<─────────────  │                 │              │
       │                │                │                 │              │
       │                │ 7. bateria_establecer_nivel(nivel)              │
       │                │─────────────────────────────────────────────────>│
       │                │                │                 │              │
       │                │ 8. OK          │                 │              │
       │                │<─────────────────────────────────────────────────│
       │                │                │                 │              │
       │ 9. OK          │                │                 │              │
       │<─────────────  │                │                 │              │
```

### Dependencias entre Componentes

```
gestor_termostato (Aplicación) [EXTENDER]
    ├── depende de → sensor_bateria (Infraestructura) [NUEVO]
    ├── depende de → bateria (Dominio) [NUEVO]
    └── depende de → sensor_temperatura (existente)

sensor_bateria (Infraestructura) [NUEVO]
    └── depende de → hal_bateria (Dispositivos) [NUEVO]

hal_bateria (Dispositivos) [NUEVO]
    └── NO depende de nada (hoja)

bateria (Dominio) [NUEVO]
    └── NO depende de nada (puro)
```

**Orden de implementación (bottom-up):**
1. Extensión de tipos comunes (si necesario)
2. Entidad Bateria (Dominio)
3. HAL Batería (Dispositivos)
4. Sensor Batería (Infraestructura)
5. Extensión de Gestor Termostato (Aplicación)
6. Integración en main.c

---

## 🔧 Componentes a Implementar

### 1. Tipos Comunes - Extensión (src/common/)

**Archivos a modificar:**
- `tipos_comunes.h` (agregar tipos para batería)

**Responsabilidad:**
- Definir tipos relacionados con batería
- Constantes de umbrales y rangos

**Contenido a agregar:**
```c
// Tipos para batería
typedef uint8_t NivelCarga;  // Porcentaje 0-100
typedef uint16_t Voltaje;     // Voltaje en mV (milivolts)

// Estados de batería
typedef enum {
    BATERIA_ESTADO_NORMAL,    // > 20%
    BATERIA_ESTADO_BAJO,      // 10-20%
    BATERIA_ESTADO_CRITICO    // < 10%
} EstadoBateria;

// Constantes de batería (Li-ion típica 3.7V nominal)
#define VOLTAJE_BATERIA_MAX_MV   4200  // 4.2V completamente cargada
#define VOLTAJE_BATERIA_MIN_MV   3000  // 3.0V descargada (protección)
#define VOLTAJE_BATERIA_NOM_MV   3700  // 3.7V nominal

#define NIVEL_CARGA_INICIAL      100   // 100% inicial
#define UMBRAL_BATERIA_BAJA      20    // 20%
#define UMBRAL_BATERIA_CRITICA   10    // 10%
```

---

### 2. Entidad Bateria (src/dominio/entidades/)

**Archivos a crear:**
- `bateria.h` (interfaz pública)
- `bateria.c` (implementación)

**Responsabilidad:**
- Modelar el concepto de dominio "Batería"
- Almacenar nivel de carga actual
- Determinar estado de batería (normal/bajo/crítico)
- Operaciones de negocio básicas

**Interfaz pública:**
```c
typedef struct Bateria Bateria;

// Ciclo de vida
Bateria* bateria_crear(void);
void bateria_destruir(Bateria* bateria);

// Operaciones
void bateria_establecer_nivel(Bateria* bateria, NivelCarga nivel);
NivelCarga bateria_obtener_nivel(const Bateria* bateria);
EstadoBateria bateria_obtener_estado(const Bateria* bateria);
bool bateria_esta_baja(const Bateria* bateria);
bool bateria_esta_critica(const Bateria* bateria);
```

**Estructura interna:**
```c
struct Bateria {
    NivelCarga nivel_carga;  // Porcentaje 0-100
    EstadoBateria estado;     // Normal/Bajo/Crítico
};
```

**Lógica de negocio:**
- Al establecer nivel, actualizar estado automáticamente
- `esta_baja()` retorna true si nivel < 20%
- `esta_critica()` retorna true si nivel < 10%

**Tests:**
- `test/unit/test_bateria.c`

---

### 3. HAL Batería (src/dispositivos/hal/)

**Archivos a crear:**
- `hal_bateria.h` (interfaz pública)
- `hal_bateria.c` (implementación simulada)

**Responsabilidad:**
- Abstracción de lectura de voltaje de batería
- Versión simulada para desarrollo sin hardware
- Interfaz única para todas las plataformas

**Interfaz pública:**
```c
// Inicialización
void hal_bateria_init(void);
void hal_bateria_deinit(void);

// Lectura de voltaje en mV (milivolts)
Voltaje hal_bateria_leer_voltaje(void);
```

**Implementación simulada:**
```c
// Simula lectura de batería:
// - Retorna voltaje entre 3000-4200 mV
// - Simula descarga gradual (opcional)
// - Variación aleatoria pequeña (±50mV)

Voltaje hal_bateria_leer_voltaje(void) {
    // Valor simulado: 3000-4200 mV
    // Para simplificar, retornar valor aleatorio en rango
    uint16_t rango = VOLTAJE_BATERIA_MAX_MV - VOLTAJE_BATERIA_MIN_MV;
    return VOLTAJE_BATERIA_MIN_MV + (rand() % rango);
}
```

**Futuras implementaciones:**
- STM32: Usar ADC con divisor de voltaje
- AVR: Usar ADC interno con AREF
- ESP32: Usar ADC con atenuación

**Tests:**
- `test/unit/test_hal_bateria.c`

---

### 4. Sensor Batería (src/infraestructura/sensores/)

**Archivos a crear:**
- `sensor_bateria.h` (interfaz pública)
- `sensor_bateria.c` (implementación)

**Responsabilidad:**
- Proxy del sensor de batería
- Delegar a HAL para lectura de voltaje
- Conversión Voltaje (mV) → Nivel de carga (%)

**Interfaz pública:**
```c
// Inicialización
void sensor_bateria_init(void);
void sensor_bateria_deinit(void);

// Lectura (retorna nivel de carga 0-100%)
NivelCarga sensor_bateria_leer(void);
```

**Lógica de conversión (simplificada):**
```c
// Conversión lineal simplificada:
// 3000 mV = 0%
// 4200 mV = 100%
//
// Fórmula: porcentaje = (voltaje - V_min) * 100 / (V_max - V_min)

NivelCarga sensor_bateria_leer(void) {
    Voltaje voltaje_mv = hal_bateria_leer_voltaje();

    // Limitar al rango válido
    if (voltaje_mv < VOLTAJE_BATERIA_MIN_MV) {
        voltaje_mv = VOLTAJE_BATERIA_MIN_MV;
    }
    if (voltaje_mv > VOLTAJE_BATERIA_MAX_MV) {
        voltaje_mv = VOLTAJE_BATERIA_MAX_MV;
    }

    // Conversión lineal (simplificada para esta fase)
    uint16_t rango = VOLTAJE_BATERIA_MAX_MV - VOLTAJE_BATERIA_MIN_MV;
    uint16_t delta = voltaje_mv - VOLTAJE_BATERIA_MIN_MV;
    NivelCarga porcentaje = (delta * 100) / rango;

    return porcentaje;
}
```

**Nota:** En producción real, usar curva de descarga de batería Li-ion (no lineal).

**Tests:**
- `test/unit/test_sensor_bateria.c`

---

### 5. Extensión Gestor Termostato (src/aplicacion/gestores/)

**Archivos a modificar:**
- `gestor_termostato.h` (agregar funciones de batería)
- `gestor_termostato.c` (implementar funciones)

**Responsabilidad:**
- Coordinar lectura de batería
- Actualizar entidad Bateria
- Mantener coherencia con gestión de temperatura existente

**Extensión de interfaz pública:**
```c
// Agregar a gestor_termostato.h:

// Operaciones de batería
void gestor_termostato_actualizar_bateria(GestorTermostato* gestor);
NivelCarga gestor_termostato_obtener_nivel_bateria(const GestorTermostato* gestor);
EstadoBateria gestor_termostato_obtener_estado_bateria(const GestorTermostato* gestor);
bool gestor_termostato_bateria_baja(const GestorTermostato* gestor);
```

**Extensión de estructura interna:**
```c
struct GestorTermostato {
    Ambiente* ambiente;   // Existente
    Bateria* bateria;     // NUEVO - agregar referencia
};
```

**Modificaciones necesarias:**
1. `gestor_termostato_crear()` - Agregar parámetro `Bateria*`
2. Implementar nuevas funciones de batería
3. Mantener compatibilidad con funciones existentes de temperatura

**Lógica de actualización:**
```c
void gestor_termostato_actualizar_bateria(GestorTermostato* gestor) {
    // 1. Leer del sensor (capa infraestructura)
    NivelCarga nivel = sensor_bateria_leer();

    // 2. Actualizar entidad de dominio
    bateria_establecer_nivel(gestor->bateria, nivel);
}
```

**Tests:**
- `test/unit/test_gestor_termostato.c` (agregar tests de batería)

---

## 📅 Plan de Fases

### Fase 1: Tipos Comunes - Extensión (5 min)

**Objetivo:** Agregar tipos relacionados con batería

**Archivos a modificar:**
- `src/common/tipos_comunes.h`

**Tareas:**
1. Agregar typedef para NivelCarga y Voltaje
2. Agregar enum EstadoBateria
3. Definir constantes de voltaje y umbrales
4. Mantener compatibilidad con tipos existentes

**Criterio de validación:**
- ✅ Compila sin errores
- ✅ No rompe compilación de componentes existentes
- ✅ Tipos disponibles para nuevos módulos

---

### Fase 2: Entidad Bateria (20 min)

**Objetivo:** Implementar entidad de dominio pura para batería

**Archivos a crear:**
- `src/dominio/entidades/bateria.h`
- `src/dominio/entidades/bateria.c`
- `test/unit/test_bateria.c`

**Tareas:**
1. Crear header con interfaz pública
2. Implementar struct opaca
3. Implementar funciones de ciclo de vida
4. Implementar setters/getters
5. Implementar lógica de estado (normal/bajo/crítico)
6. Crear tests unitarios
7. Actualizar CMakeLists.txt para tests

**Criterios de validación:**
- ✅ Compila sin errores
- ✅ Tests pasan (crear/destruir, set/get, estados)
- ✅ No hay memory leaks
- ✅ Lógica de umbrales funciona correctamente

---

### Fase 3: HAL Batería (20 min)

**Objetivo:** Abstracción de hardware para lectura de voltaje

**Archivos a crear:**
- `src/dispositivos/hal/hal_bateria.h`
- `src/dispositivos/hal/hal_bateria.c`
- `test/unit/test_hal_bateria.c`

**Tareas:**
1. Crear header con interfaz HAL
2. Implementar versión simulada
3. Agregar comentarios sobre versiones reales futuras
4. Simular voltajes en rango 3000-4200 mV
5. Crear tests unitarios
6. Verificar valores en rango esperado

**Criterios de validación:**
- ✅ Compila sin errores
- ✅ Tests pasan
- ✅ Voltajes simulados en rango 3000-4200 mV
- ✅ Múltiples lecturas retornan valores variados

---

### Fase 4: Sensor Batería (25 min)

**Objetivo:** Proxy de infraestructura con conversión V→%

**Archivos a crear:**
- `src/infraestructura/sensores/sensor_bateria.h`
- `src/infraestructura/sensores/sensor_bateria.c`
- `test/unit/test_sensor_bateria.c`

**Tareas:**
1. Crear header con interfaz pública
2. Implementar inicialización
3. Implementar lectura con conversión Voltaje→Porcentaje
4. Implementar validación de rango
5. Crear tests unitarios
6. Verificar conversión matemática correcta

**Criterios de validación:**
- ✅ Compila sin errores
- ✅ Tests pasan
- ✅ Niveles de carga en rango 0-100%
- ✅ Conversión correcta (3000mV→0%, 4200mV→100%)
- ✅ Valores intermedios calculados correctamente

---

### Fase 5: Extensión Gestor Termostato (25 min)

**Objetivo:** Agregar coordinación de batería al gestor existente

**Archivos a modificar:**
- `src/aplicacion/gestores/gestor_termostato.h`
- `src/aplicacion/gestores/gestor_termostato.c`
- `test/unit/test_gestor_termostato.c`

**Tareas:**
1. Agregar referencia Bateria* a struct
2. Modificar gestor_termostato_crear() para recibir Bateria*
3. Implementar funciones de actualización de batería
4. Implementar getters de nivel y estado
5. Agregar tests para nuevas funciones
6. Verificar compatibilidad con funcionalidad existente

**Criterios de validación:**
- ✅ Compila sin errores
- ✅ Tests nuevos pasan
- ✅ Tests existentes NO se rompen
- ✅ Flujo completo de batería funciona
- ✅ Integración con temperatura sigue funcionando

---

### Fase 6: Integración en main.c (15 min)

**Objetivo:** Integrar batería en programa principal

**Archivos a modificar:**
- `src/main.c`

**Tareas:**
1. Incluir headers de batería
2. Inicializar HAL batería
3. Crear instancia de Bateria
4. Modificar creación de GestorTermostato (pasar Bateria*)
5. Ejecutar lectura de batería de prueba
6. Mostrar resultado en consola junto con temperatura
7. Liberar recursos de batería

**Criterios de validación:**
- ✅ Compila sin errores
- ✅ Ejecuta correctamente
- ✅ Muestra nivel de batería en consola
- ✅ Muestra estado de batería
- ✅ No hay memory leaks
- ✅ Temperatura sigue funcionando

---

### Fase 7: Tests Unitarios e Integración (45 min)

**Objetivo:** Cobertura de tests completa

**Archivos a crear/modificar:**
- Tests unitarios para cada componente nuevo
- Test de integración completo de batería
- Test de integración combinado (temperatura + batería)

**Tareas:**
1. Completar test_bateria.c (casos edge)
2. Completar test_hal_bateria.c
3. Completar test_sensor_bateria.c
4. Extender test_gestor_termostato.c
5. Crear test_integracion_bateria.c
6. Crear test_integracion_completa.c (temp + batería)
7. Verificar todos los tests pasan
8. Verificar cobertura adecuada

**Criterios de validación:**
- ✅ Todos los tests pasan
- ✅ Tests existentes no se rompen
- ✅ CTest ejecuta exitosamente
- ✅ `./build.sh test` pasa sin errores
- ✅ Cobertura de casos edge (0%, 100%, umbrales)

---

## 🔀 Orden de Implementación Detallado

### Orden Bottom-Up (Recomendado)

Este orden respeta las dependencias y permite probar cada componente aisladamente:

```
1. tipos_comunes.h (extensión)
   └── Agregar tipos de batería a archivo existente

2. bateria.h/.c + test
   └── Depende: tipos_comunes.h (extendido)

3. hal_bateria.h/.c + test
   ├── Depende: tipos_comunes.h (para tipo Voltaje)
   └── Sin otras dependencias de proyecto

4. sensor_bateria.h/.c + test
   ├── Depende: hal_bateria.h
   └── Depende: tipos_comunes.h

5. gestor_termostato.h/.c (extensión) + test
   ├── Depende: sensor_bateria.h
   ├── Depende: bateria.h
   ├── Mantiene: sensor_temperatura.h (existente)
   └── Mantiene: ambiente.h (existente)

6. main.c (integración extendida)
   └── Depende: todos los anteriores

7. test_integracion_bateria.c
   └── Depende: flujo completo de batería

8. test_integracion_completa.c
   └── Depende: temperatura + batería
```

### Checklist por Componente

Para cada componente nuevo, seguir este proceso:

1. **Diseño:**
   - [ ] Definir interfaz pública (header)
   - [ ] Documentar funciones con comentarios Doxygen
   - [ ] Identificar dependencias

2. **Implementación:**
   - [ ] Crear archivo .h con guardas
   - [ ] Crear archivo .c con implementación
   - [ ] Incluir headers necesarios
   - [ ] Implementar funciones
   - [ ] Agregar validación de parámetros

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
   - [ ] Commit: `git commit -m "[ITE-57] Implementar [componente]"`
   - [ ] Push: `git push origin ITE-57-HU-007-Obtener-el-nivel-de-carga-de-la-batería`

---

## ✅ Criterios de Aceptación

### Criterios Funcionales

| # | Criterio | Verificación |
|---|----------|--------------|
| 1 | ¿Lee correctamente el nivel de batería? | HAL retorna voltajes válidos (3000-4200 mV) |
| 2 | ¿Convierte voltaje a porcentaje correctamente? | 3000mV→0%, 4200mV→100%, valores intermedios proporcionales |
| 3 | ¿Almacena en entidad Bateria? | Entidad actualiza nivel y estado |
| 4 | ¿Detecta batería baja correctamente? | Estado BAJO cuando nivel < 20% |
| 5 | ¿Detecta batería crítica correctamente? | Estado CRITICO cuando nivel < 10% |
| 6 | ¿Integra con temperatura sin conflictos? | Ambas funcionalidades conviven correctamente |

### Criterios de Validación Técnica

**Compilación:**
- [ ] `./build.sh` ejecuta sin errores
- [ ] No hay warnings (con `-Wall -Wextra`)
- [ ] Genera ejecutable `build/bin/termostato`
- [ ] Componentes existentes siguen compilando

**Ejecución:**
- [ ] `./build/bin/termostato` corre correctamente
- [ ] Muestra nivel de batería en consola
- [ ] Muestra estado de batería (Normal/Bajo/Crítico)
- [ ] Nivel en rango 0-100%
- [ ] Temperatura sigue funcionando correctamente

**Tests:**
- [ ] `./build.sh test` pasa al 100%
- [ ] Mínimo 5 tests nuevos (1 por componente)
- [ ] Tests de integración pasan
- [ ] Tests existentes NO se rompen

**Calidad de Código:**
- [ ] Nomenclatura snake_case consistente
- [ ] Funciones documentadas con Doxygen
- [ ] Sin memory leaks
- [ ] Headers con include guards
- [ ] Código sigue estilo del proyecto

**Arquitectura:**
- [ ] Respeta separación de capas
- [ ] Dependencias unidireccionales
- [ ] No hay acoplamiento entre capas no adyacentes
- [ ] Sigue patrón establecido en ITE-56

---

## 📝 Notas Adicionales

### Simplificaciones Didácticas

Esta implementación usa simplificaciones con fines didácticos:

1. **Conversión lineal:** Voltaje→Porcentaje es lineal (real: curva de descarga)
2. **Sin filtrado:** No se implementa promedio móvil para estabilizar lecturas
3. **Sin calibración:** No se calibra el ADC (offset, ganancia)
4. **Sin compensación de temperatura:** El voltaje de batería varía con temperatura
5. **Sin detección de carga:** No detecta si batería está cargando
6. **Umbrales fijos:** No considera diferentes químicas de batería

### Extensiones Futuras

Después de completar este sprint, se pueden agregar:

- **Sprint 3:** Alertas de batería baja (callback, eventos)
- **Sprint 4:** Curva de descarga real (lookup table)
- **Sprint 5:** Estimación de tiempo restante
- **Sprint 6:** Modos de ahorro de energía
- **Sprint 7:** Detección de batería en carga
- **Sprint 8:** Calibración y compensación de temperatura
- **Sprint 9:** Logging de eventos de batería
- **Sprint 10:** HAL real para hardware específico

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
cd build && ctest -R test_bateria -V

# Ejecutar todos los tests de batería
cd build && ctest -R bateria -V

# Verificar memory leaks (si valgrind disponible)
valgrind --leak-check=full ./build/bin/termostato
```

### Consideraciones de Hardware Real

Cuando se implemente en hardware real:

**Divisor de voltaje:**
```
Batería (4.2V max) ───[R1]───┬───[R2]───GND
                             │
                          ADC_PIN (3.3V max)
```

- Calcular R1/R2 para que 4.2V → 3.3V (o 5V según MCU)
- Ejemplo: R1=10kΩ, R2=27kΩ → Factor 0.73

**ADC:**
- Resolución típica: 10-12 bits
- Referencia: 3.3V o 5V
- Múltiples muestras y promedio para estabilidad

**Batería Li-ion:**
- Voltaje nominal: 3.7V
- Carga completa: 4.2V
- Descargada: 3.0V (protección)
- NO descargar por debajo de 3.0V (daño permanente)

---

## 📚 Referencias

- **Jira:** https://vvalotto.atlassian.net/browse/ITE-57
- **ITE-56:** Plan de implementación de temperatura (referencia)
- **CLAUDE.md:** `/Users/victor/CLionProjects/isse_termostato_c/CLAUDE.md`
- **CONTRIBUTING.md:** Convenciones de código y nomenclatura

---

**Documento generado:** 2025-11-17
**Versión:** 1.0
**Estado:** Listo para implementación
