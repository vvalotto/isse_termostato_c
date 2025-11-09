# Guía de Contribución - ISSE Termostato

Gracias por tu interés en contribuir al proyecto **ISSE Termostato**. Este documento establece las pautas y convenciones para mantener la calidad y consistencia del código.

---

## 📋 Tabla de Contenidos

- [Convenciones de Código](#convenciones-de-código)
- [Convenciones de Git](#convenciones-de-git)
- [Proceso de Desarrollo](#proceso-de-desarrollo)
- [Testing](#testing)
- [Documentación](#documentación)
- [Arquitectura y Diseño](#arquitectura-y-diseño)

---

## 💻 Convenciones de Código

### Estándar del Lenguaje
- **Estándar:** C11
- **Compilador:** GCC 11.x o superior
- **Extensiones:** Deshabilitadas (`CMAKE_C_EXTENSIONS OFF`)

### Estilo de Código

#### Indentación y Formato
- **Indentación:** 4 espacios (no tabs)
- **Ancho de línea:** Máximo 100 caracteres (recomendado 80)
- **Llaves:** Estilo K&R (llave de apertura en la misma línea)
  ```c
  if (condicion) {
      // código
  } else {
      // código
  }
  ```

#### Nombres

**Archivos:**
```c
// Nombres en snake_case
sensor_temperatura.c
sensor_temperatura.h
gestor_termostato.c
```

**Funciones:**
```c
// Patrón: modulo_accion()
ambiente_crear();
bateria_verificar_carga();
climatizador_cambiar_modo();
```

**Tipos y Estructuras:**
```c
// PascalCase para tipos
typedef struct Ambiente Ambiente;
typedef struct Bateria Bateria;
typedef enum ModoClimatizador ModoClimatizador;
```

**Constantes:**
```c
// UPPER_SNAKE_CASE
#define MAX_TEMPERATURA 35.0f
#define MIN_TEMPERATURA 10.0f
#define CICLO_CONTROL_MS 100
```

**Variables:**
```c
// snake_case
float temperatura_actual;
int nivel_bateria;
bool termostato_encendido;
```

**Funciones Privadas:**
```c
// Prefijo static
static void calcular_consumo_interno(void);
static float obtener_temperatura_promedio(void);
```

### Comentarios

**Doxygen para funciones públicas:**
```c
/**
 * @brief Crea una nueva instancia de Ambiente
 *
 * Inicializa una entidad Ambiente con la temperatura inicial especificada.
 * Reserva memoria dinámica que debe ser liberada con ambiente_destruir().
 *
 * @param temperatura_inicial Temperatura en grados Celsius
 * @return Puntero a la nueva instancia, NULL si falla la asignación
 *
 * @note La temperatura debe estar en el rango [MIN_TEMP, MAX_TEMP]
 * @see ambiente_destruir()
 */
Ambiente* ambiente_crear(float temperatura_inicial);
```

**Comentarios de línea:**
```c
// Comentarios cortos en español
// Usar para explicar "por qué", no "qué"
```

### Headers Guards

```c
#ifndef MODULO_NOMBRE_H
#define MODULO_NOMBRE_H

// Contenido del header

#endif // MODULO_NOMBRE_H
```

### Includes

```c
// 1. Headers del sistema
#include <stdio.h>
#include <stdlib.h>

// 2. Headers de librerías externas
// (si aplica)

// 3. Headers del proyecto
#include "ambiente.h"
#include "bateria.h"
```

---

## 🔀 Convenciones de Git

### Estructura de Branches

```
main                        # Producción estable
├── develop                 # Integración de features
│   ├── feature/ITE-XXX-descripcion   # Nuevas funcionalidades
│   ├── fix/ITE-XXX-descripcion       # Correcciones de bugs
│   └── docs/ITE-XXX-descripcion      # Actualización de docs
```

**Convenciones:**
- `feature/ITE-XXX-descripcion` - Nuevas funcionalidades
- `fix/ITE-XXX-descripcion` - Correcciones de bugs
- `docs/ITE-XXX-descripcion` - Actualización de documentación
- `refactor/ITE-XXX-descripcion` - Refactoring sin cambio funcional
- `test/ITE-XXX-descripcion` - Agregado o mejora de tests

### Mensajes de Commit

**Formato:**
```
[ITE-XXX] Título descriptivo en imperativo (máx 50 chars)

Descripción detallada del cambio si es necesario (máx 72 chars por línea).
Explicar el "por qué" del cambio, no el "qué" (eso está en el diff).

Referencias a issues relacionados:
- Resuelve #123
- Relacionado con #456
```

**Ejemplos:**
```bash
[ITE-042] Implementar entidad Ambiente con validación de temperatura

[ITE-015] Corregir fuga de memoria en gestor_termostato_destruir

[SETUP] Actualizar CMakeLists.txt para soportar tests de integración

[DOCS] Agregar documentación de arquitectura por capas

[REFACTOR] Extraer lógica de validación a módulo común
```

**Prefijos especiales (sin número de issue):**
- `[SETUP]` - Configuración inicial o cambios en build system
- `[DOCS]` - Documentación general no ligada a un issue
- `[REFACTOR]` - Refactoring general
- `[FIX]` - Corrección urgente sin issue

### Pull Requests

**Título:** Similar al commit principal
```
[ITE-XXX] Descripción clara de la funcionalidad o corrección
```

**Descripción debe incluir:**
1. **Resumen:** Breve descripción del cambio
2. **Motivación:** Por qué es necesario este cambio
3. **Cambios realizados:** Lista de cambios principales
4. **Testing:** Cómo se probó (tests unitarios, integración, manual)
5. **Checklist:**
   - [ ] Código compilado sin warnings
   - [ ] Tests pasando
   - [ ] Documentación actualizada
   - [ ] Respeta la arquitectura por capas

---

## 🔄 Proceso de Desarrollo

### 1. Crear Branch desde Develop

```bash
git checkout develop
git pull origin develop
git checkout -b feature/ITE-XXX-nombre-descriptivo
```

### 2. Implementar Cambios

- Seguir las convenciones de código
- Respetar la arquitectura por capas
- Escribir tests para el código nuevo
- Documentar funciones públicas con Doxygen

### 3. Ejecutar Tests

```bash
# Compilar con tests
cmake -B build -DBUILD_TESTS=ON
cmake --build build

# Ejecutar tests
cd build && ctest --verbose
```

**Los tests deben pasar sin errores antes de hacer commit.**

### 4. Verificar Calidad

```bash
# Verificar que compila sin warnings
./build.sh

# Verificar que no hay memory leaks (si aplica)
valgrind --leak-check=full ./build/bin/termostato
```

### 5. Hacer Commit

```bash
git add .
git commit -m "[ITE-XXX] Descripción del cambio"
```

### 6. Push y Crear Pull Request

```bash
git push origin feature/ITE-XXX-nombre-descriptivo
```

Luego crear Pull Request en GitHub hacia `develop`.

### 7. Code Review

- Al menos un revisor debe aprobar
- Responder a comentarios y hacer ajustes necesarios
- Mantener el branch actualizado con develop si hay conflictos

### 8. Merge

Una vez aprobado, el revisor o maintainer hará el merge a `develop`.

---

## 🧪 Testing

Todo código nuevo debe incluir tests apropiados.

### Tests Unitarios

**Ubicación:** `test/unit/test_<modulo>.c`

**Estructura:**
```c
#include "unity.h"
#include "modulo_a_probar.h"

void setUp(void) {
    // Inicialización antes de cada test
}

void tearDown(void) {
    // Limpieza después de cada test
}

void test_modulo_crear_retorna_no_nulo(void) {
    Modulo* m = modulo_crear();
    TEST_ASSERT_NOT_NULL(m);
    modulo_destruir(m);
}

void test_modulo_validacion_temperatura(void) {
    TEST_ASSERT_TRUE(modulo_validar_temperatura(20.0f));
    TEST_ASSERT_FALSE(modulo_validar_temperatura(100.0f));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_modulo_crear_retorna_no_nulo);
    RUN_TEST(test_modulo_validacion_temperatura);
    return UNITY_END();
}
```

### Tests de Integración

**Ubicación:** `test/integration/test_<flujo>.c`

Probar interacciones entre capas y flujos completos de casos de uso.

### Cobertura de Tests

**Objetivo:** Mínimo 80% de cobertura para código de dominio y aplicación.

**Qué testear:**
- ✅ Funciones públicas de todas las capas
- ✅ Casos de uso completos
- ✅ Manejo de errores
- ✅ Validaciones de entrada
- ✅ Flujos alternativos

**Qué no requiere tests:**
- Código trivial (getters/setters simples)
- Placeholders vacíos
- Código de HAL simulado (hasta tener hardware real)

### Agregar Nuevo Test

1. Crear archivo `test/unit/test_<modulo>.c`
2. Agregar al `CMakeLists.txt`:
   ```cmake
   add_unit_test(test_modulo test/unit/test_modulo.c)
   ```
3. Ejecutar: `./build.sh test`

---

## 📚 Documentación

### Código

- **Funciones públicas:** Comentarios Doxygen completos
- **Funciones privadas:** Comentario breve si no es obvio
- **Decisiones de diseño:** Comentarios explicando "por qué"
- **TODOs:** Usar formato `// TODO: descripción` solo para código temporal

### README y CLAUDE.md

- **README.md:** Actualizar si cambia funcionalidad visible al usuario
- **CLAUDE.md:** Actualizar si cambia arquitectura o comandos de build
- **docs/:** Actualizar diagramas y documentos si cambia la arquitectura

### Confluence

La documentación completa del proyecto está en Confluence:
- **Espacio:** [Ingeniería de Software para Sistemas Embebidos (I2A)](https://vvalotto.atlassian.net/wiki/spaces/I2A/overview)
- **Contenido:** Arquitectura detallada, casos de uso, escenarios de calidad, diagramas C4/UML

**Cuándo actualizar Confluence:**
- Cambios en arquitectura o diseño de alto nivel
- Nuevos casos de uso o modificaciones significativas
- Actualización de escenarios de calidad o métricas
- Nuevos diagramas o modificación de diagramas existentes

**Sincronización:** Mantener consistencia entre código (GitHub), documentación técnica (Confluence) y tareas (Jira).

### Commits y PRs

- Mensajes de commit descriptivos
- PRs con descripción clara del cambio y su motivación

---

## 🏗️ Arquitectura y Diseño

### Principios Arquitectónicos

**1. Dependencias Unidireccionales**
```
Aplicación → Dominio → (nada)
Infraestructura → Dominio
Dispositivos → (nada)
```

**No permitido:**
- Dominio no puede depender de Infraestructura
- Dominio no puede depender de Aplicación
- Capas inferiores no pueden conocer capas superiores

**2. Inversión de Dependencias**

Definir interfaces en capas superiores, implementar en capas inferiores:
```c
// dominio/interfaces/sensor_temperatura.h
typedef struct ISensorTemperatura ISensorTemperatura;
float isensor_temperatura_leer(ISensorTemperatura* sensor);

// infraestructura/sensores/sensor_temperatura_impl.c
// Implementa la interfaz definida en dominio
```

**3. Separación de Responsabilidades**

Cada capa tiene responsabilidades específicas:
- **Dominio:** Lógica de negocio pura
- **Aplicación:** Orquestación de casos de uso
- **Infraestructura:** Acceso a recursos externos
- **Dispositivos:** Abstracción de hardware

### Patrones de Diseño Aplicados

**En el proyecto se usan:**
- **State Pattern:** Para máquinas de estado (Termostato, Climatizador)
- **Strategy Pattern:** Para algoritmos de control intercambiables
- **Facade Pattern:** Para simplificar interfaces complejas
- **Factory Pattern:** Para creación de entidades
- **Dependency Injection:** Para testabilidad

### Antes de Agregar Código

**Pregúntate:**
1. ¿En qué capa pertenece este código?
2. ¿Respeta las dependencias unidireccionales?
3. ¿Es testeable sin hardware?
4. ¿Sigue los patrones existentes?
5. ¿Cumple con los atributos de calidad (performance, energía)?

---

## ❌ Anti-Patrones a Evitar

- ❌ God objects (objetos que hacen todo)
- ❌ Dependencias circulares entre módulos
- ❌ Lógica de negocio en capas de infraestructura
- ❌ Acceso directo a hardware desde capas superiores
- ❌ Variables globales (usar Singleton pattern si es necesario)
- ❌ Magic numbers (usar constantes con nombres descriptivos)
- ❌ Funciones de más de 50 líneas (refactorizar)
- ❌ Malloc sin free correspondiente
- ❌ Código duplicado (aplicar DRY)

---

## 🔒 Seguridad y Buenas Prácticas

- ✅ Validar todas las entradas externas
- ✅ Verificar retornos de malloc antes de usar
- ✅ Liberar toda memoria asignada dinámicamente
- ✅ Inicializar todas las variables
- ✅ Evitar buffer overflows (usar strncpy, no strcpy)
- ✅ Proteger contra divisiones por cero
- ✅ Manejar errores de forma consistente

---

## 📞 Contacto

**Preguntas o dudas sobre contribuciones:**
- Abrir un issue en GitHub con la etiqueta `question`
- Contactar a Victor Valotto: vvalotto@fiuner.edu.ar

**Reporte de bugs:**
- Usar GitHub Issues con plantilla de bug report
- Incluir: pasos para reproducir, comportamiento esperado, comportamiento actual

---

## 📄 Licencia

Al contribuir a este proyecto, aceptas que tus contribuciones serán licenciadas bajo la misma licencia del proyecto (ver [LICENSE](LICENSE)).

---

¡Gracias por contribuir al proyecto ISSE Termostato! 🎉
