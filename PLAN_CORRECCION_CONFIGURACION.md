# Plan de Corrección de Configuración - ISSE Termostato

**Fecha:** 2025-11-09
**Proyecto:** isse_termostato_c
**Autor:** Análisis automatizado con Claude Code

---

## Resumen Ejecutivo

Este documento presenta un plan estructurado para corregir las **14 inconsistencias** detectadas en la configuración del proyecto. Las correcciones están organizadas por **prioridad crítica** para asegurar que el proyecto compile y funcione correctamente.

---

## Clasificación de Problemas

| Prioridad | Cantidad | Descripción |
|-----------|----------|-------------|
| 🔴 **Crítica** | 3 | Impiden compilación del proyecto |
| 🟡 **Alta** | 2 | Inconsistencias de nomenclatura |
| 🟠 **Media** | 3 | Estructura de directorios y scripts |
| 🔵 **Baja** | 4 | Documentación y placeholders |
| ⚪ **Opcional** | 2 | Mejoras recomendadas |

---

## FASE 1: Correcciones Críticas (OBLIGATORIO)

### 1.1 Crear archivo `main.c`

**Problema:**
CMakeLists.txt:193 referencia `src/main.c` que no existe.

**Solución:**
```bash
# Crear archivo main.c básico
touch src/main.c
```

**Contenido sugerido para `src/main.c`:**
```c
/**
 * @file main.c
 * @brief Punto de entrada del sistema Termostato ISSE
 * @version 1.0.0
 * @date 2025-11-09
 */

#include <stdio.h>

int main(void) {
    printf("ISSE Termostato - Sistema de Control de Temperatura\n");
    printf("Versión: 1.0.0\n");
    printf("Estado: Inicializando...\n");

    // TODO: Inicializar aspectos transversales
    // TODO: Inicializar capa de dispositivos
    // TODO: Inicializar capa de infraestructura
    // TODO: Inicializar capa de dominio
    // TODO: Inicializar capa de aplicación
    // TODO: Iniciar ciclo principal de control

    return 0;
}
```

**Verificación:**
```bash
cmake -B build -DBUILD_TESTS=OFF
cmake --build build
```

---

### 1.2 Instalar Unity Test Framework

**Problema:**
CMakeLists.txt:213 referencia `test/unity/unity.c` pero el framework no está instalado.

**Opciones de Solución:**

#### Opción A: Descarga Manual (RECOMENDADO)
```bash
# Descargar Unity desde GitHub
cd test/unity
curl -L -O https://raw.githubusercontent.com/ThrowTheSwitch/Unity/master/src/unity.c
curl -L -O https://raw.githubusercontent.com/ThrowTheSwitch/Unity/master/src/unity.h
curl -L -O https://raw.githubusercontent.com/ThrowTheSwitch/Unity/master/src/unity_internals.h
cd ../..

# Eliminar .gitkeep
rm test/unity/.gitkeep
```

#### Opción B: Git Submodule
```bash
# Agregar Unity como submódulo
git submodule add https://github.com/ThrowTheSwitch/Unity.git test/unity_framework

# Actualizar CMakeLists.txt para apuntar a test/unity_framework/src/
```

#### Opción C: Desactivar Tests Temporalmente
```bash
# Compilar sin tests
cmake -B build -DBUILD_TESTS=OFF
```

**Recomendación:** Usar Opción A para simplicidad y control de versiones.

**Verificación:**
```bash
# Verificar archivos
ls -la test/unity/

# Compilar con tests
cmake -B build -DBUILD_TESTS=ON
cmake --build build
```

---

### 1.3 Poblar Variables de Fuentes en CMakeLists.txt

**Problema:**
Variables `*_SOURCES` vacías (líneas 137-158). Las librerías se compilan sin código.

**Solución Temporal:**
Crear archivos `.c` placeholder en cada capa para validar compilación.

**Script de Inicialización:**
```bash
#!/bin/bash
# Crear archivos placeholder para cada capa

# Dominio
touch src/dominio/entidades/placeholder_entidades.c
touch src/dominio/servicios/placeholder_servicios.c

# Aplicación
touch src/aplicacion/gestores/placeholder_gestores.c
touch src/aplicacion/servicios/placeholder_servicios.c

# Infraestructura
touch src/infraestructura/sensores/placeholder_sensores.c
touch src/infraestructura/actuadores/placeholder_actuadores.c
touch src/infraestructura/visualizadores/placeholder_visualizadores.c

# Dispositivos
touch src/dispositivos/hal/placeholder_hal.c
touch src/dispositivos/drivers/placeholder_drivers.c

# Aspectos
touch src/aspectos/logging/placeholder_logging.c
touch src/aspectos/auditoria/placeholder_auditoria.c
touch src/aspectos/configuracion/placeholder_configuracion.c

# Common
touch src/common/placeholder_common.c
```

**Actualizar CMakeLists.txt:**
```cmake
# Líneas 137-158, reemplazar por:

# Capa de Dominio (Lógica de Negocio)
file(GLOB_RECURSE DOMINIO_SOURCES
    src/dominio/entidades/*.c
    src/dominio/servicios/*.c
)

# Capa de Aplicación (Orquestación)
file(GLOB_RECURSE APLICACION_SOURCES
    src/aplicacion/gestores/*.c
    src/aplicacion/servicios/*.c
)

# Capa de Infraestructura (Acceso a Recursos)
file(GLOB_RECURSE INFRAESTRUCTURA_SOURCES
    src/infraestructura/sensores/*.c
    src/infraestructura/actuadores/*.c
    src/infraestructura/visualizadores/*.c
)

# Capa de Dispositivos (HAL y Drivers)
file(GLOB_RECURSE DISPOSITIVOS_SOURCES
    src/dispositivos/hal/*.c
    src/dispositivos/drivers/*.c
)

# Aspectos Transversales
file(GLOB_RECURSE ASPECTOS_SOURCES
    src/aspectos/*/*.c
)

# Common
file(GLOB_RECURSE COMMON_SOURCES
    src/common/*.c
)
```

**Verificación:**
```bash
cmake -B build
cmake --build build
./build/bin/termostato
```

---

## FASE 2: Correcciones de Nomenclatura (ALTA PRIORIDAD)

### 2.1 Unificar Nombre del Proyecto

**Problema:**
Inconsistencia entre directorio real (`isse_termostato_c`) y nombres en documentación.

**Decisión Requerida:**
Elegir UNA de estas opciones:

- **Opción A:** `isse_termostato_c` (minúsculas, snake_case) ✅ RECOMENDADO
- **Opción B:** `ISSE_Termostato` (PascalCase con guión bajo)
- **Opción C:** `isse-termostato-c` (kebab-case)

**Asumiendo Opción A, actualizar:**

1. **CMakeLists.txt línea 11:**
   ```cmake
   project(isse_termostato_c
       VERSION 1.0.0
       ...
   )
   ```

2. **README.md línea 144:**
   ```markdown
   cd isse_termostato_c
   ```

3. **README.md línea 198:**
   ```markdown
   isse_termostato_c/
   ├── docs/
   ```

4. **Todos los comentarios y referencias en el código**

**Script de Búsqueda y Reemplazo:**
```bash
# Buscar todas las referencias
grep -r "ISSE_Termostato" . --exclude-dir=.git --exclude-dir=build

# Confirmar cambios manualmente
```

---

### 2.2 Actualizar URLs del Repositorio

**Problema:**
README líneas 143-144 tienen path inconsistente.

**Solución:**
```markdown
# Líneas 142-144 del README.md
git clone https://github.com/vvalotto/isse_termostato_c.git
cd isse_termostato_c

# Abrir con CLion
```

**Verificación:**
```bash
# Confirmar que la URL es correcta
git remote -v
```

---

## FASE 3: Estructura de Directorios (PRIORIDAD MEDIA)

### 3.1 Actualizar Script `crear_estructura.sh`

**Problema:**
El script no crea los subdirectorios de `aspectos/` que existen actualmente.

**Solución:**
Actualizar sección de Aspectos Transversales (líneas 105-108):

```bash
# ==============================================================================
# ASPECTOS TRANSVERSALES
# ==============================================================================
echo "🔀 Aspectos Transversales"
create_dir "src/aspectos/logging"
create_gitkeep "src/aspectos/logging"

create_dir "src/aspectos/auditoria"
create_gitkeep "src/aspectos/auditoria"

create_dir "src/aspectos/configuracion"
create_gitkeep "src/aspectos/configuracion"

create_dir "src/aspectos/eventos"
create_gitkeep "src/aspectos/eventos"

create_dir "src/aspectos/persistencia"
create_gitkeep "src/aspectos/persistencia"

create_dir "src/aspectos/validacion"
create_gitkeep "src/aspectos/validacion"
echo ""
```

**Actualizar también el resumen final del script (líneas 199-200):**
```bash
echo "    ├── aspectos/      (logging, auditoria, configuracion, eventos, persistencia, validacion)"
```

---

### 3.2 Versionar el Script

**Problema:**
`crear_estructura.sh` no está en Git.

**Solución:**
```bash
git add crear_estructura.sh
git commit -m "[SETUP] Add project structure creation script"
```

---

### 3.3 Eliminar `.gitkeep` de Directorios Poblados

**Problema:**
Directorios que ya tienen contenido aún tienen `.gitkeep`.

**Solución:**
```bash
# Identificar directorios con más de solo .gitkeep
find . -name ".gitkeep" -type f | while read file; do
    dir=$(dirname "$file")
    count=$(ls -A "$dir" | wc -l)
    if [ "$count" -gt 1 ]; then
        echo "Eliminar: $file (directorio tiene $count archivos)"
        # rm "$file"  # Descomentar para ejecutar
    fi
done
```

**Ejecución segura:**
```bash
# Revisar primero qué se eliminaría
bash script_limpieza.sh

# Si está bien, eliminar
rm src/aspectos/.gitkeep
```

---

## FASE 4: Documentación (PRIORIDAD BAJA)

### 4.1 Resolver Enlaces Rotos en README

**Problema:**
Referencias a PDFs que no existen.

**Opciones:**

#### Opción A: Crear Placeholders
```bash
# Crear archivos README.md en cada directorio de docs
echo "# Arquitectura Funcional - Pendiente" > docs/arquitectura/README.md
echo "# Casos de Uso - Pendiente" > docs/casos_uso/README.md
echo "# Escenarios de Calidad - Pendiente" > docs/escenarios_calidad/README.md
echo "# Diagramas - Pendiente" > docs/diagramas/README.md
```

#### Opción B: Actualizar README con Estado Real
Modificar tabla de documentación (líneas 283-290):

```markdown
| Documento | Descripción | Estado |
|-----------|-------------|--------|
| [Arquitectura Funcional](docs/arquitectura/) | Descripción de las 5 capas + aspectos | 📝 En desarrollo |
| [Casos de Uso](docs/casos_uso/) | 8 casos de uso detallados | 📝 En desarrollo |
| [Escenarios de Calidad](docs/escenarios_calidad/) | 17 escenarios con métricas | 📝 En desarrollo |
| [Diagramas C4](docs/diagramas/) | Contexto, Contenedores, Componentes | 📝 En desarrollo |
```

#### Opción C: Comentar Temporalmente
```markdown
<!--
## 📚 Documentación Adicional

Esta sección estará disponible cuando la documentación esté completa.
-->
```

**Recomendación:** Opción B (transparencia sobre estado real).

---

### 4.2 Completar Placeholders en README

**Problema:**
Información personal incompleta.

**Solución:**
Editar líneas 265, 337-338 del README:

```markdown
# Línea 265
**Proyecto Jira:** [ISSE-Termostato](https://tuinstancia.atlassian.net/browse/ISSE)
# O si no usas Jira:
**Gestión de Tareas:** GitHub Projects

# Líneas 337-340
**Victor Valotto**
- 🏫 Docente Universitario - FIUNER
- 📧 Email: vvalotto@fiuner.edu.ar
- 🔗 LinkedIn: [Victor Valotto](https://linkedin.com/in/vvalotto)
- 🌐 GitHub: [@vvalotto](https://github.com/vvalotto)
```

---

### 4.3 Crear CONTRIBUTING.md

**Problema:**
README referencia archivo inexistente.

**Solución:**
```bash
# Crear archivo básico
touch CONTRIBUTING.md
```

**Contenido mínimo:**
```markdown
# Guía de Contribución - ISSE Termostato

## Convenciones de Código

- Estándar: C11
- Indentación: 4 espacios
- Nombres: snake_case para funciones y variables
- Comentarios: Doxygen style

## Convenciones de Git

### Branches
\`\`\`
main                    # Producción estable
develop                 # Integración de features
feature/ISSE-XXX-desc   # Nuevas funcionalidades
fix/ISSE-XXX-desc       # Correcciones de bugs
\`\`\`

### Commits
\`\`\`
[ISSE-XXX] Título descriptivo (max 50 chars)

Descripción detallada del cambio (si es necesario).
Referencias a issues relacionados.
\`\`\`

## Proceso de Desarrollo

1. Crear branch desde `develop`
2. Implementar cambios con tests
3. Ejecutar suite de tests: `ctest --verbose`
4. Crear Pull Request a `develop`
5. Code review requerido
6. Merge tras aprobación

## Tests

Todo nuevo código debe incluir:
- Tests unitarios para funciones públicas
- Tests de integración para interacciones entre capas
- Cobertura mínima: 80%

## Documentación

- Actualizar README si es necesario
- Comentarios Doxygen en funciones públicas
- Actualizar diagramas C4 si la arquitectura cambia
```

---

## FASE 5: Configuración de Git (OPCIONAL PERO RECOMENDADO)

### 5.1 Limpiar Directorios IDE del Repositorio

**Problema:**
`.idea/` y `cmake-build-debug/` podrían estar versionados.

**Verificación:**
```bash
git ls-files | grep -E "\.idea|cmake-build"
```

**Si encuentra archivos:**
```bash
# Eliminar del tracking de Git (mantiene archivos locales)
git rm -r --cached .idea
git rm -r --cached cmake-build-debug
git commit -m "[CONFIG] Remove IDE directories from version control"
```

**Confirmar .gitignore:**
```bash
# Verificar que están en .gitignore
grep -E "^\.idea|^cmake-build" .gitignore
```

---

### 5.2 Agregar .editorconfig (Opcional)

**Beneficio:**
Consistencia de estilo entre diferentes editores.

**Solución:**
```bash
touch .editorconfig
```

**Contenido:**
```ini
# EditorConfig para ISSE Termostato
root = true

[*]
charset = utf-8
end_of_line = lf
insert_final_newline = true
trim_trailing_whitespace = true

[*.{c,h}]
indent_style = space
indent_size = 4

[CMakeLists.txt]
indent_style = space
indent_size = 4

[*.md]
indent_style = space
indent_size = 2
trim_trailing_whitespace = false

[*.sh]
indent_style = space
indent_size = 2
```

---

## FASE 6: Validación Final

### Checklist de Verificación

Ejecutar después de completar todas las fases:

```bash
# 1. Compilación sin tests
cmake -B build -DBUILD_TESTS=OFF
cmake --build build
./build/bin/termostato

# 2. Compilación con tests
cmake -B build -DBUILD_TESTS=ON
cmake --build build
cd build && ctest --verbose

# 3. Verificar estructura
find src -type f -name "*.c" -o -name "*.h"

# 4. Verificar Git
git status
git diff

# 5. Verificar enlaces en README
# (Revisar manualmente todos los enlaces)

# 6. Verificar documentación
ls -la docs/*/
```

---

## Priorización de Ejecución

### Orden Recomendado (Mínimo Viable)

1. ✅ **Fase 1.1:** Crear `main.c` (5 min)
2. ✅ **Fase 1.2:** Instalar Unity (10 min)
3. ✅ **Fase 1.3:** Poblar fuentes en CMake (15 min)
4. ✅ **Fase 2.1:** Unificar nomenclatura (20 min)
5. ⏳ **Fase 3.2:** Versionar script (2 min)

**Total MVP:** ~52 minutos

### Orden Completo (Proyecto Production-Ready)

1. Fases 1.1 - 1.3 (Críticas)
2. Fases 2.1 - 2.2 (Nomenclatura)
3. Fases 3.1 - 3.3 (Estructura)
4. Fases 4.1 - 4.3 (Documentación)
5. Fases 5.1 - 5.2 (Git config)
6. Fase 6 (Validación)

**Total completo:** ~3-4 horas

---

## Scripts de Automatización

### Script All-in-One para Fase 1 (Críticas)

```bash
#!/bin/bash
# fix_critical_issues.sh

echo "=== Corrigiendo problemas críticos del proyecto ==="

# 1.1 Crear main.c
echo "1. Creando src/main.c..."
cat > src/main.c << 'EOF'
#include <stdio.h>

int main(void) {
    printf("ISSE Termostato v1.0.0\n");
    return 0;
}
EOF

# 1.2 Instalar Unity
echo "2. Descargando Unity Test Framework..."
cd test/unity
curl -L -O https://raw.githubusercontent.com/ThrowTheSwitch/Unity/master/src/unity.c
curl -L -O https://raw.githubusercontent.com/ThrowTheSwitch/Unity/master/src/unity.h
curl -L -O https://raw.githubusercontent.com/ThrowTheSwitch/Unity/master/src/unity_internals.h
rm .gitkeep
cd ../..

# 1.3 Crear archivos placeholder
echo "3. Creando archivos placeholder..."
touch src/dominio/entidades/placeholder.c
touch src/dominio/servicios/placeholder.c
touch src/aplicacion/gestores/placeholder.c
touch src/aplicacion/servicios/placeholder.c
touch src/infraestructura/sensores/placeholder.c
touch src/infraestructura/actuadores/placeholder.c
touch src/infraestructura/visualizadores/placeholder.c
touch src/dispositivos/hal/placeholder.c
touch src/dispositivos/drivers/placeholder.c
touch src/aspectos/logging/placeholder.c
touch src/common/placeholder.c

echo ""
echo "=== Problemas críticos corregidos ==="
echo "Ejecuta: cmake -B build && cmake --build build"
```

---

## Notas Finales

### Recomendaciones

1. **No aplicar cambios masivos sin revisión:** Revisar cada sección antes de ejecutar
2. **Hacer commits atómicos:** Un commit por corrección, no todo junto
3. **Probar compilación incremental:** Después de cada fase crítica
4. **Mantener backup:** `git branch backup-$(date +%Y%m%d)` antes de cambios masivos

### Punto de Decisión

Antes de proceder, responder:

- ¿Qué nombre definitivo tendrá el proyecto? (`isse_termostato_c` recomendado)
- ¿Usarás Jira o GitHub Projects?
- ¿Prefieres Unity como submódulo o archivos directos?
- ¿Completarás documentación ahora o después?

### Siguiente Paso

Una vez corregida la configuración, el siguiente hito será:
- **Implementar entidades del dominio** (Ambiente, Batería, Climatizador)
- **Crear tests unitarios básicos**
- **Implementar primer caso de uso:** CU-006 Obtener Temperatura Ambiente

---

**Documento generado automáticamente**
**Fecha:** 2025-11-09
**Versión:** 1.0
