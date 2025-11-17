# Guía Rápida de Implementación - ITE-57

**Historia:** HU-007 - Obtener el nivel de carga de la batería
**Para:** Empezar rápidamente

---

## 🚀 Inicio Rápido (5 minutos)

### 1. Preparación

```bash
cd /Users/victor/CLionProjects/isse_termostato_c

# Verificar que estás en el branch correcto
git branch
# Debe mostrar: * ITE-57-HU-007-Obtener-el-nivel-de-carga-de-la-batería

# Compilar para verificar estado actual
./build.sh
```

### 2. Orden de Implementación (Bottom-Up)

Implementar en este orden estricto:

```
1️⃣ src/common/tipos_comunes.h (extensión)       (5 min)
2️⃣ src/dominio/entidades/bateria.h/.c          (20 min) ← + test
3️⃣ src/dispositivos/hal/hal_bateria.h/.c       (20 min) ← + test
4️⃣ src/infraestructura/sensores/sensor_bateria.h/.c (25 min) ← + test
5️⃣ src/aplicacion/gestores/gestor_termostato.h/.c (extensión) (25 min) ← + test
6️⃣ src/main.c (integración)                     (15 min)
7️⃣ Tests completos                               (45 min)
```

**Total:** ~2.5 horas

---

## 📝 Template de Proceso por Componente

Para cada componente, seguir estos pasos:

### A. Crear Header (.h)

```bash
# Crear archivo
touch src/[capa]/[categoria]/[nombre].h

# Abrir en editor y agregar:
# 1. Include guards
# 2. Comentarios Doxygen
# 3. Includes necesarios
# 4. Declaraciones de tipos
# 5. Declaraciones de funciones
```

**Ejemplo:**
```c
#ifndef BATERIA_H
#define BATERIA_H

/**
 * @file bateria.h
 * @brief Entidad de dominio que modela la batería del sistema
 */

#include "tipos_comunes.h"

typedef struct Bateria Bateria;

Bateria* bateria_crear(void);
void bateria_destruir(Bateria* bateria);
void bateria_establecer_nivel(Bateria* bateria, NivelCarga nivel);
NivelCarga bateria_obtener_nivel(const Bateria* bateria);

#endif // BATERIA_H
```

### B. Crear Implementación (.c)

```bash
# Crear archivo
touch src/[capa]/[categoria]/[nombre].c

# Agregar:
# 1. Includes
# 2. Definición de struct
# 3. Implementación de funciones
```

### C. Crear Test

```bash
# Crear archivo
touch test/unit/test_[nombre].c

# Agregar:
# 1. Includes (unity.h + módulo a testear)
# 2. setUp() y tearDown()
# 3. Tests individuales
# 4. main() con UNITY_BEGIN/END
```

**Ejemplo:**
```c
#include "unity.h"
#include "bateria.h"

void setUp(void) {}
void tearDown(void) {}

void test_bateria_crear_retorna_valido(void) {
    Bateria* bat = bateria_crear();
    TEST_ASSERT_NOT_NULL(bat);
    bateria_destruir(bat);
}

void test_bateria_nivel_inicial_es_100(void) {
    Bateria* bat = bateria_crear();
    TEST_ASSERT_EQUAL(100, bateria_obtener_nivel(bat));
    bateria_destruir(bat);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_bateria_crear_retorna_valido);
    RUN_TEST(test_bateria_nivel_inicial_es_100);
    return UNITY_END();
}
```

### D. Agregar Test a CMakeLists.txt

```cmake
# En CMakeLists.txt, sección "# Tests de Dominio" o similar
add_unit_test(test_bateria test/unit/test_bateria.c)
```

### E. Compilar y Probar

```bash
# Reconfigurar CMake (detecta nuevos archivos)
cmake -B build -DBUILD_TESTS=ON -DTARGET_PLATFORM=sim

# Compilar
cmake --build build

# Ejecutar test específico
cd build && ctest -R test_bateria -V

# Ejecutar todos los tests
cd .. && ./build.sh test
```

### F. Commit

```bash
git add .
git commit -m "[ITE-57] Implementar [nombre_componente]"
git push origin ITE-57-HU-007-Obtener-el-nivel-de-carga-de-la-batería
```

---

## 📦 Implementación Fase por Fase

### Fase 1: tipos_comunes.h - Extensión (5 min)

```bash
# 1. Abrir archivo existente
# src/common/tipos_comunes.h

# 2. Agregar al final (antes del #endif):
```

**Contenido a agregar:**
```c
// ============================================
// TIPOS PARA BATERÍA (ITE-57)
// ============================================

// Nivel de carga de batería (porcentaje 0-100)
typedef uint8_t NivelCarga;

// Voltaje en milivolts (mV)
typedef uint16_t Voltaje;

// Estados de batería según nivel de carga
typedef enum {
    BATERIA_ESTADO_NORMAL,    // > 20%
    BATERIA_ESTADO_BAJO,      // 10-20%
    BATERIA_ESTADO_CRITICO    // < 10%
} EstadoBateria;

// Constantes de voltaje (batería Li-ion 3.7V nominal)
#define VOLTAJE_BATERIA_MAX_MV   4200  // 4.2V completamente cargada
#define VOLTAJE_BATERIA_MIN_MV   3000  // 3.0V descargada
#define VOLTAJE_BATERIA_NOM_MV   3700  // 3.7V nominal

// Constantes de nivel de carga
#define NIVEL_CARGA_INICIAL      100   // 100% inicial
#define UMBRAL_BATERIA_BAJA      20    // 20%
#define UMBRAL_BATERIA_CRITICA   10    // 10%
```

```bash
# 3. Verificar compilación
./build.sh

# 4. Commit
git add src/common/tipos_comunes.h
git commit -m "[ITE-57] Agregar tipos para batería"
```

---

### Fase 2: bateria (20 min)

```bash
# 1. Crear archivos
touch src/dominio/entidades/bateria.h
touch src/dominio/entidades/bateria.c
touch test/unit/test_bateria.c

# 2. Implementar según README.md (sección Componentes a Implementar)

# 3. Agregar test a CMakeLists.txt
# Abrir CMakeLists.txt, buscar sección "Tests de Dominio" y agregar:
# add_unit_test(test_bateria test/unit/test_bateria.c)

# 4. Compilar y probar
cmake -B build -DBUILD_TESTS=ON
cmake --build build
cd build && ctest -R test_bateria -V

# 5. Si pasa, commit
git add src/dominio/entidades/bateria.*
git add test/unit/test_bateria.c
git commit -m "[ITE-57] Implementar entidad Bateria"
```

**Estructura mínima de bateria.h:**
```c
#ifndef BATERIA_H
#define BATERIA_H

#include "tipos_comunes.h"
#include <stdbool.h>

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

#endif // BATERIA_H
```

**Estructura mínima de bateria.c:**
```c
#include "bateria.h"
#include <stdlib.h>

struct Bateria {
    NivelCarga nivel_carga;
    EstadoBateria estado;
};

Bateria* bateria_crear(void) {
    Bateria* bateria = (Bateria*)malloc(sizeof(Bateria));
    if (bateria != NULL) {
        bateria->nivel_carga = NIVEL_CARGA_INICIAL;
        bateria->estado = BATERIA_ESTADO_NORMAL;
    }
    return bateria;
}

void bateria_destruir(Bateria* bateria) {
    free(bateria);
}

void bateria_establecer_nivel(Bateria* bateria, NivelCarga nivel) {
    if (bateria == NULL) return;

    bateria->nivel_carga = nivel;

    // Actualizar estado según nivel
    if (nivel < UMBRAL_BATERIA_CRITICA) {
        bateria->estado = BATERIA_ESTADO_CRITICO;
    } else if (nivel < UMBRAL_BATERIA_BAJA) {
        bateria->estado = BATERIA_ESTADO_BAJO;
    } else {
        bateria->estado = BATERIA_ESTADO_NORMAL;
    }
}

NivelCarga bateria_obtener_nivel(const Bateria* bateria) {
    return (bateria != NULL) ? bateria->nivel_carga : 0;
}

EstadoBateria bateria_obtener_estado(const Bateria* bateria) {
    return (bateria != NULL) ? bateria->estado : BATERIA_ESTADO_CRITICO;
}

bool bateria_esta_baja(const Bateria* bateria) {
    return (bateria != NULL) && (bateria->nivel_carga < UMBRAL_BATERIA_BAJA);
}

bool bateria_esta_critica(const Bateria* bateria) {
    return (bateria != NULL) && (bateria->nivel_carga < UMBRAL_BATERIA_CRITICA);
}
```

---

### Fase 3: hal_bateria (20 min)

```bash
# 1. Crear archivos
touch src/dispositivos/hal/hal_bateria.h
touch src/dispositivos/hal/hal_bateria.c
touch test/unit/test_hal_bateria.c

# 2. Implementar según README.md

# 3. Agregar test a CMakeLists.txt
# add_unit_test(test_hal_bateria test/unit/test_hal_bateria.c)

# 4. Compilar y probar
cmake --build build
cd build && ctest -R test_hal_bateria -V

# 5. Commit
git add src/dispositivos/hal/hal_bateria.*
git add test/unit/test_hal_bateria.c
git commit -m "[ITE-57] Implementar HAL Batería simulado"
```

**Estructura mínima de hal_bateria.h:**
```c
#ifndef HAL_BATERIA_H
#define HAL_BATERIA_H

#include "tipos_comunes.h"

void hal_bateria_init(void);
void hal_bateria_deinit(void);
Voltaje hal_bateria_leer_voltaje(void);

#endif // HAL_BATERIA_H
```

**Estructura mínima de hal_bateria.c:**
```c
#include "hal_bateria.h"
#include <stdlib.h>
#include <time.h>

void hal_bateria_init(void) {
    srand((unsigned int)time(NULL));
}

void hal_bateria_deinit(void) {
    // Nada que hacer en simulador
}

Voltaje hal_bateria_leer_voltaje(void) {
    // Simular voltaje entre 3000-4200 mV
    uint16_t rango = VOLTAJE_BATERIA_MAX_MV - VOLTAJE_BATERIA_MIN_MV;
    return VOLTAJE_BATERIA_MIN_MV + (rand() % rango);
}
```

---

### Fase 4: sensor_bateria (25 min)

```bash
# 1. Crear archivos
touch src/infraestructura/sensores/sensor_bateria.h
touch src/infraestructura/sensores/sensor_bateria.c
touch test/unit/test_sensor_bateria.c

# 2. Implementar según README.md

# 3. Agregar test a CMakeLists.txt

# 4. Compilar y probar
cmake --build build
cd build && ctest -R test_sensor_bateria -V

# 5. Commit
git add src/infraestructura/sensores/sensor_bateria.*
git add test/unit/test_sensor_bateria.c
git commit -m "[ITE-57] Implementar sensor de batería"
```

**Estructura mínima de sensor_bateria.h:**
```c
#ifndef SENSOR_BATERIA_H
#define SENSOR_BATERIA_H

#include "tipos_comunes.h"

void sensor_bateria_init(void);
void sensor_bateria_deinit(void);
NivelCarga sensor_bateria_leer(void);

#endif // SENSOR_BATERIA_H
```

**Estructura mínima de sensor_bateria.c:**
```c
#include "sensor_bateria.h"
#include "hal_bateria.h"

void sensor_bateria_init(void) {
    hal_bateria_init();
}

void sensor_bateria_deinit(void) {
    hal_bateria_deinit();
}

NivelCarga sensor_bateria_leer(void) {
    Voltaje voltaje_mv = hal_bateria_leer_voltaje();

    // Limitar al rango válido
    if (voltaje_mv < VOLTAJE_BATERIA_MIN_MV) {
        voltaje_mv = VOLTAJE_BATERIA_MIN_MV;
    }
    if (voltaje_mv > VOLTAJE_BATERIA_MAX_MV) {
        voltaje_mv = VOLTAJE_BATERIA_MAX_MV;
    }

    // Conversión lineal: (V - Vmin) * 100 / (Vmax - Vmin)
    uint16_t rango = VOLTAJE_BATERIA_MAX_MV - VOLTAJE_BATERIA_MIN_MV;
    uint16_t delta = voltaje_mv - VOLTAJE_BATERIA_MIN_MV;
    NivelCarga porcentaje = (delta * 100) / rango;

    return porcentaje;
}
```

---

### Fase 5: gestor_termostato - Extensión (25 min)

```bash
# 1. Modificar archivos existentes
# src/aplicacion/gestores/gestor_termostato.h
# src/aplicacion/gestores/gestor_termostato.c
# test/unit/test_gestor_termostato.c

# 2. Agregar funciones de batería según README.md

# 3. Compilar y probar
cmake --build build
cd build && ctest -R test_gestor_termostato -V

# 4. Commit
git add src/aplicacion/gestores/gestor_termostato.*
git add test/unit/test_gestor_termostato.c
git commit -m "[ITE-57] Extender gestor con batería"
```

**Cambios en gestor_termostato.h:**
```c
// Agregar include
#include "bateria.h"
#include "sensor_bateria.h"

// Agregar a las declaraciones de funciones:
void gestor_termostato_actualizar_bateria(GestorTermostato* gestor);
NivelCarga gestor_termostato_obtener_nivel_bateria(const GestorTermostato* gestor);
EstadoBateria gestor_termostato_obtener_estado_bateria(const GestorTermostato* gestor);
bool gestor_termostato_bateria_baja(const GestorTermostato* gestor);
```

**Cambios en gestor_termostato.c:**
```c
// Modificar struct:
struct GestorTermostato {
    Ambiente* ambiente;
    Bateria* bateria;  // NUEVO
};

// Modificar gestor_termostato_crear():
GestorTermostato* gestor_termostato_crear(Ambiente* ambiente, Bateria* bateria) {
    // ... validaciones ...
    gestor->ambiente = ambiente;
    gestor->bateria = bateria;  // NUEVO
    return gestor;
}

// Agregar nuevas funciones:
void gestor_termostato_actualizar_bateria(GestorTermostato* gestor) {
    if (gestor == NULL) return;
    NivelCarga nivel = sensor_bateria_leer();
    bateria_establecer_nivel(gestor->bateria, nivel);
}

NivelCarga gestor_termostato_obtener_nivel_bateria(const GestorTermostato* gestor) {
    if (gestor == NULL || gestor->bateria == NULL) return 0;
    return bateria_obtener_nivel(gestor->bateria);
}

// ... implementar otras funciones ...
```

---

### Fase 6: Integración en main.c (15 min)

```bash
# 1. Editar src/main.c
# 2. Agregar includes de batería
# 3. Inicializar HAL batería
# 4. Crear instancia Bateria
# 5. Modificar creación de GestorTermostato

# 6. Compilar
./build.sh

# 7. Ejecutar
./build/bin/termostato

# 8. Verificar salida (debe mostrar temperatura Y batería)

# 9. Commit
git add src/main.c
git commit -m "[ITE-57] Integrar batería en main"
```

**Cambios en main.c:**
```c
// Agregar includes:
#include "bateria.h"
#include "sensor_bateria.h"
#include "hal_bateria.h"

// En main(), agregar:

// Inicializar HAL batería
hal_bateria_init();

// Crear entidad Bateria
Bateria* bateria = bateria_crear();

// Modificar creación de gestor:
GestorTermostato* gestor = gestor_termostato_crear(ambiente, bateria);

// Agregar lectura de batería en el loop:
gestor_termostato_actualizar_bateria(gestor);
NivelCarga nivel = gestor_termostato_obtener_nivel_bateria(gestor);
EstadoBateria estado = gestor_termostato_obtener_estado_bateria(gestor);

printf("  Nivel batería: %u%% (", nivel);
switch(estado) {
    case BATERIA_ESTADO_NORMAL:   printf("Normal"); break;
    case BATERIA_ESTADO_BAJO:     printf("Bajo"); break;
    case BATERIA_ESTADO_CRITICO:  printf("Crítico"); break;
}
printf(")\n");

// Cleanup (antes de return):
bateria_destruir(bateria);
hal_bateria_deinit();
```

---

### Fase 7: Tests Completos (45 min)

```bash
# 1. Revisar todos los tests existentes
# 2. Agregar casos edge
# 3. Crear test de integración completo

# 4. Ejecutar suite completa
./build.sh test

# 5. Verificar que todos pasan
# 6. Commit
git add test/
git commit -m "[ITE-57] Completar suite de tests"
```

---

## ✅ Validación Final

```bash
# 1. Compilación limpia
./build.sh clean
./build.sh

# 2. Todos los tests pasan
./build.sh test

# 3. Ejecución correcta
./build/bin/termostato
# Debe mostrar:
# - Temperatura leída: XX.X°C
# - Nivel batería: XX% (Normal/Bajo/Crítico)

# 4. Sin warnings
cmake --build build 2>&1 | grep -i warning
# No debe mostrar nada

# 5. Push final
git push origin ITE-57-HU-007-Obtener-el-nivel-de-carga-de-la-batería
```

---

## 🐛 Troubleshooting

### Error: "No such file or directory"
```bash
# Reconfigura CMake para detectar nuevos archivos
cmake -B build -DBUILD_TESTS=ON
cmake --build build
```

### Error: "Undefined reference to..."
```bash
# Verifica que el .c está en el directorio correcto
# CMake usa GLOB_RECURSE, así que debe detectarlo automáticamente
# Si no, reconfigura:
cmake -B build
```

### Tests fallan
```bash
# Ejecutar con verbose para ver detalles
cd build && ctest -R test_bateria -V

# Verificar lógica del test
# Verificar implementación del componente
```

### Tests de temperatura se rompieron
```bash
# Verificar que gestor_termostato_crear() fue modificado correctamente
# Verificar que todos los tests llaman con parámetro Bateria*
# Puede ser NULL en tests antiguos si no se usa
```

### Memory leaks (si valgrind disponible)
```bash
valgrind --leak-check=full ./build/bin/termostato
# Verificar que no hay "definitely lost"
```

---

## 💡 Tips

### Copiar código del README
- El README.md tiene ejemplos completos de código
- Puedes copiar y adaptar
- Asegúrate de entender lo que copias

### Compilar frecuentemente
- Compila después de cada cambio pequeño
- Es más fácil encontrar errores así

### Un test a la vez
- Implementa un test, hazlo pasar
- Luego el siguiente

### Commits pequeños
- Commit después de cada fase
- Facilita revertir si algo sale mal

### Usar CLion
- Autocompletado ayuda con nombres
- Ctrl+Click para navegar a definiciones
- Shift+F6 para renombrar

### Mantener compatibilidad
- Los tests de temperatura deben seguir pasando
- Agregar NULL como segundo parámetro si no usan batería

---

## ⏱️ Checkpoint de Tiempo

Si vas retrasado:

**Prioridad 1 (Mínimo viable):**
- ✅ tipos_comunes.h (extensión)
- ✅ bateria.h/.c (sin tests extensivos)
- ✅ hal_bateria.h/.c (sin tests)
- ✅ sensor_bateria.h/.c (básico)
- ✅ gestor_termostato (extensión básica)
- ✅ main.c integración
- ✅ 1 test de integración que funcione

**Prioridad 2 (Deseable):**
- Tests unitarios completos
- Edge cases
- Validación exhaustiva
- Estados de batería (bajo/crítico)

---

## 📚 Puntos Clave de Batería

### Rangos importantes

- **Voltaje:** 3000-4200 mV (batería Li-ion)
- **Nivel:** 0-100%
- **Normal:** > 20%
- **Bajo:** 10-20%
- **Crítico:** < 10%

### Conversión V→%

```
Porcentaje = (Voltaje - 3000) * 100 / (4200 - 3000)
Porcentaje = (Voltaje - 3000) * 100 / 1200
```

**Ejemplos:**
- 3000 mV → 0%
- 3600 mV → 50%
- 4200 mV → 100%

---

**Última actualización:** 2025-11-17
**Listo para comenzar** ✅
