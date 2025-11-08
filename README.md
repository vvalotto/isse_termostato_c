# Sistema Termostato Inteligente - ISSE

[![License](https://img.shields.io/badge/license-Academic-blue.svg)](LICENSE)
[![Language](https://img.shields.io/badge/language-C11-brightgreen.svg)](https://en.wikipedia.org/wiki/C11_(C_standard_revision))
[![Platform](https://img.shields.io/badge/platform-Embedded-orange.svg)](https://www.embedded.com/)
[![Architecture](https://img.shields.io/badge/architecture-Layered-purple.svg)](docs/arquitectura)

> **Sistema de control automático de temperatura para climatización residencial**  
> Caso de estudio para curso de especialización en Ingeniería de Software para Sistemas Embebidos (ISSE)

---

## 📖 Descripción

Sistema embebido que mantiene automáticamente la temperatura ambiente en un nivel confortable mediante el control inteligente de un climatizador (calefacción/refrigeración). El dispositivo opera de forma autónoma con alimentación por batería, proporcionando una interfaz simple mediante display y botones.

Este proyecto constituye un caso de estudio completo que ejemplifica el **proceso de desarrollo de software embebido** aplicando técnicas y prácticas de ingeniería de software, desde la elicitación de requerimientos hasta la implementación en microcontrolador.

---

## 🎯 Contexto del Proyecto

### Origen
- **Versión previa:** Implementación funcional en Python sobre Raspberry Pi
- **Objetivo actual:** Migración a lenguaje C para microcontrolador genérico
- **Enfoque:** Proceso de desarrollo, no conceptos de programación embebida

### Propósito Académico
Este proyecto sirve como **material didáctico** para enseñar:
- Arquitecturas limpias y por capas en sistemas embebidos
- Aplicación de patrones de diseño (State, Strategy, Facade, etc.)
- Ingeniería de requerimientos completa (funcionales y no funcionales)
- Trazabilidad desde requerimientos hasta código
- Domain-Driven Design (DDD) en contexto embebido
- Testing y simulación sin hardware real

---

## 🏗️ Arquitectura

El sistema implementa una **arquitectura funcional por capas** con 5 niveles de abstracción más aspectos transversales:

```
┌─────────────────────────────────────────────────────────────┐
│                    ASPECTOS TRANSVERSALES                    │
│          (Logging, Auditoría, Configuración)                │
└─────────────────────────────────────────────────────────────┘
         ↓                    ↓                    ↓
┌─────────────────────────────────────────────────────────────┐
│               1. CAPA DE APLICACIÓN                          │
│      (Orquestación de casos de uso y coordinación)          │
│   • Gestores de Entidades  • Servicios de Aplicación        │
└─────────────────────────────────────────────────────────────┘
         ↓                    ↓                    ↓
┌─────────────────────────────────────────────────────────────┐
│                2. CAPA DE DOMINIO                            │
│          (Lógica de negocio y reglas del dominio)           │
│   • Entidades (Ambiente, Batería, Climatizador)             │
│   • Servicios de Dominio  • Máquinas de Estado              │
└─────────────────────────────────────────────────────────────┘
         ↓                    ↓                    ↓
┌─────────────────────────────────────────────────────────────┐
│             3. CAPA DE INFRAESTRUCTURA                       │
│         (Acceso a recursos externos y servicios)            │
│   • Proxies Sensores  • Actuadores  • Visualizadores        │
└─────────────────────────────────────────────────────────────┘
         ↓                    ↓                    ↓
┌─────────────────────────────────────────────────────────────┐
│            4. CAPA DE DISPOSITIVOS FÍSICOS                   │
│              (HAL y Drivers de Hardware)                     │
│   • HAL (GPIO, ADC, I2C, SPI)  • Drivers Específicos        │
└─────────────────────────────────────────────────────────────┘
         ↓                    ↓                    ↓
┌─────────────────────────────────────────────────────────────┐
│                  5. HARDWARE FÍSICO                          │
│        (Microcontrolador, Sensores, Actuadores)             │
└─────────────────────────────────────────────────────────────┘
```

**Principios Arquitectónicos Clave:**
- ✅ Dependencia unidireccional (de arriba hacia abajo)
- ✅ Inversión de dependencias (interfaces definen contratos)
- ✅ Separación de responsabilidades por capa
- ✅ Testabilidad mediante inyección de dependencias
- ✅ Portabilidad (cambio de hardware afecta solo capas inferiores)

📚 **Documentación completa:** [docs/arquitectura/Arquitectura_Funcional.pdf](docs/arquitectura)

---

## ⚙️ Funcionalidades Principales

### Casos de Uso Implementados

#### 📦 Paquete: Manipular Termostato (Usuario)
- **CU-001:** Encender Termostato
- **CU-002:** Apagar Termostato
- **CU-003:** Establecer Temperatura Deseada

#### 📦 Paquete: Controlar Termostato (Automático)
- **CU-004:** Controlar Termostato (orquestador principal)
- **CU-005:** Accionar Climatizador
- **CU-006:** Obtener Temperatura Ambiente
- **CU-007:** Verificar Nivel de Carga de Batería
- **CU-008:** Mostrar Estado del Termostato

📋 **Especificaciones completas:** [docs/casos_uso/](docs/casos_uso)

---

## 🎯 Atributos de Calidad

El sistema cumple con **17 escenarios de calidad** priorizados:

| Atributo | Escenarios | Métricas Clave |
|----------|------------|----------------|
| **Confiabilidad** | 4 | MTBF ≥ 720h, Recuperación ante fallos |
| **Performance** | 3 | Ciclo control < 100ms, Latencia sensores < 50ms |
| **Eficiencia Energética** | 4 | Consumo ≤ 5mA, Batería > 30 días |
| **Disponibilidad** | 1 | Operación continua ante fallos parciales |
| **Mantenibilidad** | 1 | Cambios localizados, bajo acoplamiento |
| **Observabilidad** | 1 | Diagnóstico remoto sin acceso físico |
| **Otros** | 3 | Capacidad, Integridad, Determinismo temporal |

🎭 **Matriz de trazabilidad:** [docs/escenarios_calidad/Matriz_Trazabilidad.docx](docs/escenarios_calidad)

---

## 🚀 Quick Start

### Prerrequisitos

- **IDE:** CLion 2023.x o superior
- **Compilador:** GCC 11.x o superior (ARM GCC para microcontroladores)
- **Build System:** CMake 3.20+
- **Testing:** Unity Test Framework (incluido)
- **Git:** Para control de versiones

### Instalación

```bash
# Clonar el repositorio
git clone https://github.com/vvalotto/ISSE_Termostato.git
cd ISSE_Termostato

# Abrir con CLion
# File → Open → Seleccionar carpeta del proyecto

# CLion detectará automáticamente CMakeLists.txt
```

### Configuración en CLion

1. **Configurar Toolchain:**
   - `Settings → Build, Execution, Deployment → Toolchains`
   - Verificar que el compilador esté detectado

2. **Configurar CMake:**
   - `Settings → Build, Execution, Deployment → CMake`
   - Perfiles recomendados:
     - `Debug`: Para desarrollo con simuladores
     - `Release`: Para deployment en hardware
     - `Test`: Para ejecución de tests

3. **Build Options:**
   ```
   -DBUILD_TESTS=ON        # Incluir tests unitarios
   -DBUILD_SIMULATOR=ON    # Incluir simuladores
   -DTARGET_PLATFORM=sim   # sim | stm32 | avr | esp32
   ```

### Compilar

```bash
# Desde CLion: Build → Build Project (Ctrl+F9)

# O desde terminal:
mkdir build && cd build
cmake .. -DBUILD_TESTS=ON
make
```

### Ejecutar Tests

```bash
# Desde CLion: Run → Run 'All Tests'

# O desde terminal:
cd build
ctest --verbose
```

---

## 📁 Estructura del Proyecto

```
ISSE_Termostato/
├── docs/                   # Documentación completa
│   ├── arquitectura/       # Arquitectura funcional por capas
│   ├── casos_uso/          # Especificación de casos de uso
│   ├── escenarios_calidad/ # Atributos de calidad y métricas
│   └── diagramas/          # C4, UML, máquinas de estado
│
├── src/                    # Código fuente
│   ├── aplicacion/         # Capa de aplicación
│   ├── dominio/            # Capa de dominio (lógica de negocio)
│   ├── infraestructura/    # Capa de infraestructura
│   ├── dispositivos/       # Capa de dispositivos (HAL/Drivers)
│   ├── aspectos/           # Aspectos transversales
│   ├── common/             # Código compartido
│   └── main.c              # Punto de entrada
│
├── test/                   # Tests unitarios e integración
│   ├── unit/               # Tests unitarios por capa
│   ├── integration/        # Tests de integración
│   └── mocks/              # Mocks para testing
│
├── config/                 # Configuración del sistema
├── scripts/                # Scripts de build y deploy
├── tools/                  # Herramientas auxiliares y simuladores
│
├── CMakeLists.txt          # Configuración CMake principal
├── .gitignore              # Archivos ignorados por Git
└── README.md               # Este archivo
```

---

## 🧪 Testing

El proyecto incluye cobertura completa de tests:

### Tests Unitarios
- **Dominio:** Entidades y servicios de dominio
- **Aplicación:** Gestores y servicios de aplicación
- **Infraestructura:** Proxies y actuadores

### Tests de Integración
- Ciclo completo de control
- Interacción entre capas
- Escenarios de error y recuperación

### Simuladores
Para desarrollo sin hardware:
- Simulador de sensor de temperatura
- Simulador de climatizador
- Simulador de batería

```bash
# Ejecutar suite completa de tests
ctest --verbose

# Ejecutar tests específicos
ctest -R test_ambiente
ctest -R test_ciclo_control
```

---

## 📊 Gestión del Proyecto

### Jira
Las historias de usuario y tareas están gestionadas en:
- **Proyecto Jira:** [Link a tu proyecto Jira]
- **Convención de branches:** `feature/ISSE-XXX-descripcion`
- **Convención de commits:** `[ISSE-XXX] Descripción del commit`

### GitHub Issues
Usamos issues para:
- 🐛 Bugs detectados
- ✨ Features adicionales
- 📚 Mejoras de documentación
- ❓ Preguntas técnicas

### Contribución
Ver [CONTRIBUTING.md](CONTRIBUTING.md) para guía de contribución.

---

## 📚 Documentación Adicional

| Documento | Descripción |
|-----------|-------------|
| [Arquitectura Funcional](docs/arquitectura/Arquitectura_Funcional.pdf) | Descripción completa de las 5 capas + aspectos |
| [Casos de Uso](docs/casos_uso/Modelado_Casos_de_Uso.pdf) | 8 casos de uso con especificación detallada |
| [Escenarios de Calidad](docs/escenarios_calidad/Escenarios_de_Calidad.pdf) | 17 escenarios con métricas medibles |
| [Diagramas C4](docs/diagramas/C4.docx) | Contexto, Contenedores, Componentes, Código |
| [Historias de Usuario](docs/casos_uso/Casos_de_Uso_y_sus_Historias_de_Usuario__Sistema_Termostato.pdf) | 15 HU mapeadas a casos de uso |
| [Matriz de Trazabilidad](docs/escenarios_calidad/Matriz_de_Trazabilidad_Simplificada.docx) | Trazabilidad CU ↔ Escenarios |

---

## 🎓 Uso Académico

Este proyecto está diseñado como **material educativo** para cursos de:
- Ingeniería de Software
- Sistemas Embebidos
- Arquitectura de Software
- Diseño de Software

**Profesor:** Victor Valotto  
**Institución:** Facultad de Ingeniería - Universidad Nacional de Entre Ríos (FIUNER)

### Objetivos de Aprendizaje
- ✅ Aplicar arquitecturas limpias en sistemas con restricciones
- ✅ Implementar separación de responsabilidades mediante capas
- ✅ Gestionar requerimientos funcionales y no funcionales
- ✅ Aplicar patrones de diseño en contexto real
- ✅ Desarrollar sistemas testables sin hardware
- ✅ Documentar trazabilidad completa

---

## 🔧 Tecnologías y Herramientas

- **Lenguaje:** C11
- **Build System:** CMake
- **IDE Principal:** CLion
- **Testing Framework:** Unity
- **Control de Versiones:** Git + GitHub
- **Gestión de Proyecto:** Jira
- **Documentación:** Markdown + Doxygen
- **CI/CD:** GitHub Actions (próximamente)

---

## 📝 Licencia

Este proyecto es material académico. Ver [LICENSE](LICENSE) para más detalles.

---

## 👥 Autor

**Victor Valotto**
- 🏫 Docente Universitario - FIUNER
- 📧 Email: [tu email]
- 🔗 LinkedIn: [tu perfil]
- 🌐 GitHub: [@vvalotto](https://github.com/vvalotto)

---

## 🙏 Agradecimientos

- Estudiantes del curso ISSE por feedback y validación
- Comunidad de sistemas embebidos por recursos compartidos
- Autores de frameworks y herramientas utilizadas

---

## 📌 Estado del Proyecto

```
🚧 EN DESARROLLO ACTIVO 🚧

✅ Fase 1: Especificación de Requerimientos - COMPLETADA
✅ Fase 2: Diseño Arquitectónico - COMPLETADA
🔄 Fase 3: Implementación en C - EN PROGRESO
⏳ Fase 4: Testing y Validación - PENDIENTE
⏳ Fase 5: Deployment en Hardware - PENDIENTE
```

**Última actualización:** Noviembre 2025

---

<div align="center">

**⭐ Si este proyecto te resulta útil, considerá darle una estrella en GitHub ⭐**

[Reportar Bug](https://github.com/vvalotto/ISSE_Termostato/issues) • 
[Solicitar Feature](https://github.com/vvalotto/ISSE_Termostato/issues) • 
[Ver Documentación](docs/)

</div>
