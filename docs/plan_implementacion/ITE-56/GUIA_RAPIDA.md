# Guía Rápida de Implementación - ITE-56

**Historia:** HU-014 - Obtener la temperatura ambiente
**Para:** Empezar rápidamente

---

## 🚀 Inicio Rápido (5 minutos)

### 1. Preparación

```bash
cd /Users/victor/CLionProjects/isse_termostato_c

# Verificar que estás en el branch correcto
git branch
# Debe mostrar: * ITE-56-HU-014-Obtener-la-temperatura-ambiente

# Compilar para verificar estado actual
./build.sh
```

### 2. Orden de Implementación (Bottom-Up)

Implementar en este orden estricto:

```
1️⃣ src/common/tipos_comunes.h              (5 min)
2️⃣ src/dominio/entidades/ambiente.h/.c     (20 min) ← + test
3️⃣ src/dispositivos/hal/hal_adc.h/.c       (20 min) ← + test
4️⃣ src/infraestructura/sensores/sensor_temperatura.h/.c (25 min) ← + test
5️⃣ src/aplicacion/gestores/gestor_termostato.h/.c (30 min) ← + test
6️⃣ src/main.c (integración)                (15 min)
7️⃣ Tests completos                          (45 min)
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
#ifndef NOMBRE_H
#define NOMBRE_H

/**
 * @file nombre.h
 * @brief Breve descripción
 */

#include "tipos_comunes.h"

typedef struct Nombre Nombre;

Nombre* nombre_crear(void);
void nombre_destruir(Nombre* nombre);

#endif // NOMBRE_H
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
#include "nombre.h"

void setUp(void) {}
void tearDown(void) {}

void test_nombre_crear_retorna_valido(void) {
    Nombre* n = nombre_crear();
    TEST_ASSERT_NOT_NULL(n);
    nombre_destruir(n);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_nombre_crear_retorna_valido);
    return UNITY_END();
}
```

### D. Agregar Test a CMakeLists.txt

```cmake
# En CMakeLists.txt, sección "# Tests de Dominio" o similar
add_unit_test(test_nombre test/unit/test_nombre.c)
```

### E. Compilar y Probar

```bash
# Reconfigurar CMake (detecta nuevos archivos)
cmake -B build -DBUILD_TESTS=ON -DTARGET_PLATFORM=sim

# Compilar
cmake --build build

# Ejecutar test específico
cd build && ctest -R test_nombre -V

# Ejecutar todos los tests
cd .. && ./build.sh test
```

### F. Commit

```bash
git add .
git commit -m "[ITE-56] Implementar [nombre_componente]"
git push origin ITE-56-HU-014-Obtener-la-temperatura-ambiente
```

---

## 📦 Implementación Fase por Fase

### Fase 1: tipos_comunes.h (5 min)

```bash
# 1. Crear archivo
touch src/common/tipos_comunes.h

# 2. Implementar según ESPECIFICACIONES_TECNICAS.md (sección 1)

# 3. Verificar compilación
./build.sh

# 4. Commit
git add src/common/tipos_comunes.h
git commit -m "[ITE-56] Agregar tipos comunes"
```

**Contenido mínimo:**
```c
#ifndef TIPOS_COMUNES_H
#define TIPOS_COMUNES_H

#include <stdint.h>
#include <stdbool.h>

typedef float Temperatura;
#define TEMP_INICIAL_DEFAULT 20.0f

#endif
```

---

### Fase 2: ambiente (20 min)

```bash
# 1. Crear archivos
touch src/dominio/entidades/ambiente.h
touch src/dominio/entidades/ambiente.c
touch test/unit/test_ambiente.c

# 2. Implementar según ESPECIFICACIONES_TECNICAS.md (sección 2)
#    Copiar código de las especificaciones técnicas

# 3. Agregar test a CMakeLists.txt
# Abrir CMakeLists.txt y agregar:
# add_unit_test(test_ambiente test/unit/test_ambiente.c)

# 4. Compilar y probar
cmake -B build -DBUILD_TESTS=ON
cmake --build build
cd build && ctest -R test_ambiente -V

# 5. Si pasa, commit
git add src/dominio/entidades/ambiente.*
git add test/unit/test_ambiente.c
git commit -m "[ITE-56] Implementar entidad Ambiente"
```

---

### Fase 3: hal_adc (20 min)

```bash
# 1. Crear archivos
touch src/dispositivos/hal/hal_adc.h
touch src/dispositivos/hal/hal_adc.c
touch test/unit/test_hal_adc.c

# 2. Implementar según ESPECIFICACIONES_TECNICAS.md (sección 3)

# 3. Agregar test a CMakeLists.txt
# add_unit_test(test_hal_adc test/unit/test_hal_adc.c)

# 4. Compilar y probar
cmake --build build
cd build && ctest -R test_hal_adc -V

# 5. Commit
git add src/dispositivos/hal/hal_adc.*
git add test/unit/test_hal_adc.c
git commit -m "[ITE-56] Implementar HAL ADC simulado"
```

---

### Fase 4: sensor_temperatura (25 min)

```bash
# 1. Crear archivos
touch src/infraestructura/sensores/sensor_temperatura.h
touch src/infraestructura/sensores/sensor_temperatura.c
touch test/unit/test_sensor_temperatura.c

# 2. Implementar según ESPECIFICACIONES_TECNICAS.md (sección 4)

# 3. Agregar test a CMakeLists.txt

# 4. Compilar y probar
cmake --build build
cd build && ctest -R test_sensor_temperatura -V

# 5. Commit
git add src/infraestructura/sensores/sensor_temperatura.*
git add test/unit/test_sensor_temperatura.c
git commit -m "[ITE-56] Implementar sensor de temperatura"
```

---

### Fase 5: gestor_termostato (30 min)

```bash
# 1. Crear archivos
touch src/aplicacion/gestores/gestor_termostato.h
touch src/aplicacion/gestores/gestor_termostato.c
touch test/unit/test_gestor_termostato.c

# 2. Implementar según ESPECIFICACIONES_TECNICAS.md (sección 5)

# 3. Agregar test a CMakeLists.txt

# 4. Compilar y probar
cmake --build build
cd build && ctest -R test_gestor_termostato -V

# 5. Commit
git add src/aplicacion/gestores/gestor_termostato.*
git add test/unit/test_gestor_termostato.c
git commit -m "[ITE-56] Implementar gestor de termostato"
```

---

### Fase 6: Integración en main.c (15 min)

```bash
# 1. Editar src/main.c
# 2. Buscar TODOs y reemplazar con código real según ESPECIFICACIONES_TECNICAS.md
# 3. Incluir headers necesarios

# 4. Compilar
./build.sh

# 5. Ejecutar
./build/bin/termostato

# 6. Verificar salida (debe mostrar temperatura)

# 7. Commit
git add src/main.c
git commit -m "[ITE-56] Integrar componentes en main"
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
git commit -m "[ITE-56] Completar suite de tests"
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
# Debe mostrar: "Temperatura leída: XX.X°C"

# 4. Sin warnings
cmake --build build 2>&1 | grep -i warning
# No debe mostrar nada

# 5. Push final
git push origin ITE-56-HU-014-Obtener-la-temperatura-ambiente
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
cd build && ctest -R test_nombre -V

# Verificar lógica del test
# Verificar implementación del componente
```

### Memory leaks (si valgrind disponible)
```bash
valgrind --leak-check=full ./build/bin/termostato
# Verificar que no hay "definitely lost"
```

---

## 📚 Documentos de Referencia

Durante la implementación, consultar:

1. **ESPECIFICACIONES_TECNICAS.md** - Código completo de cada componente
2. **CHECKLIST.md** - Seguimiento de progreso paso a paso
3. **README.md** - Plan completo y contexto
4. **CONTRIBUTING.md** (raíz del proyecto) - Convenciones de código

---

## 💡 Tips

### Copiar código de las especificaciones
- Las ESPECIFICACIONES_TECNICAS.md tienen el código completo
- Puedes copiar y pegar directamente
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

---

## ⏱️ Checkpoint de Tiempo

Si vas retrasado:

**Prioridad 1 (Mínimo viable):**
- ✅ tipos_comunes.h
- ✅ ambiente.h/.c (sin tests extensivos)
- ✅ hal_adc.h/.c (sin tests)
- ✅ sensor_temperatura.h/.c (básico)
- ✅ gestor_termostato.h/.c (básico)
- ✅ main.c integración
- ✅ 1 test de integración que funcione

**Prioridad 2 (Deseable):**
- Tests unitarios completos
- Edge cases
- Validación exhaustiva

---

**Última actualización:** 2025-11-10
**Listo para comenzar** ✅
