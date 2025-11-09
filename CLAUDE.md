# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

**ISSE Termostato** is an embedded temperature control system implementing a clean, layered architecture for residential climate control. This is an academic project demonstrating software engineering best practices for embedded systems.

- **Language:** C11
- **Build System:** CMake 3.20+
- **Testing Framework:** Unity
- **Target Platforms:** Simulator (development), STM32, AVR, ESP32
- **Current Phase:** Phase 3 - Implementation (active development)

## Build Commands

### Basic Build
```bash
# Configure and build for simulator (default)
cmake -B build -DBUILD_TESTS=OFF -DTARGET_PLATFORM=sim
cmake --build build

# Run the application
./build/bin/termostato
```

### Using build.sh Script
```bash
# Quick build
./build.sh

# Build and run
./build.sh run

# Build with tests and run them
./build.sh test

# Clean build
./build.sh clean

# Clean build and run tests
./build.sh clean test
```

### Testing
```bash
# Build with tests enabled
cmake -B build -DBUILD_TESTS=ON -DTARGET_PLATFORM=sim
cmake --build build

# Run all tests
cd build && ctest --verbose

# Run specific test
cd build && ctest -R test_ejemplo --verbose
```

### Platform-Specific Builds
```bash
# For STM32
cmake -B build -DTARGET_PLATFORM=stm32 -DBUILD_TESTS=OFF

# For AVR
cmake -B build -DTARGET_PLATFORM=avr -DBUILD_TESTS=OFF

# For ESP32
cmake -B build -DTARGET_PLATFORM=esp32 -DBUILD_TESTS=OFF
```

### Clean Build
```bash
# Remove build directory
rm -rf build

# Or use CMake target
cd build && cmake --build . --target clean-all
```

## Architecture Overview

The project implements a **5-layer functional architecture** with cross-cutting concerns:

```
┌─────────────────────────────────────────────────┐
│         CROSS-CUTTING CONCERNS                  │
│   (Logging, Audit, Configuration)               │
└─────────────────────────────────────────────────┘
                    ↓
┌─────────────────────────────────────────────────┐
│    1. APPLICATION LAYER (aplicacion/)           │
│    Orchestration and use case coordination      │
│    • gestores/ - Entity managers                │
│    • servicios/ - Application services          │
└─────────────────────────────────────────────────┘
                    ↓
┌─────────────────────────────────────────────────┐
│    2. DOMAIN LAYER (dominio/)                   │
│    Business logic and domain rules              │
│    • entidades/ - Domain entities               │
│    • servicios/ - Domain services               │
└─────────────────────────────────────────────────┘
                    ↓
┌─────────────────────────────────────────────────┐
│    3. INFRASTRUCTURE LAYER (infraestructura/)   │
│    External resource access                     │
│    • sensores/ - Sensor proxies                 │
│    • actuadores/ - Actuators                    │
│    • visualizadores/ - Display components       │
└─────────────────────────────────────────────────┘
                    ↓
┌─────────────────────────────────────────────────┐
│    4. DEVICE LAYER (dispositivos/)              │
│    Hardware abstraction layer                   │
│    • hal/ - Hardware abstraction (GPIO, ADC)    │
│    • drivers/ - Specific device drivers         │
└─────────────────────────────────────────────────┘
                    ↓
┌─────────────────────────────────────────────────┐
│    5. PHYSICAL HARDWARE                         │
│    Microcontroller, sensors, actuators          │
└─────────────────────────────────────────────────┘
```

### Key Architectural Principles

1. **Unidirectional Dependency:** Layers depend only on layers below them
2. **Dependency Inversion:** Upper layers define interfaces, lower layers implement them
3. **Separation of Concerns:** Each layer has a single, well-defined responsibility
4. **Testability:** Dependency injection enables testing without hardware
5. **Portability:** Hardware changes affect only lower layers

### Domain Entities

The system models three core domain entities:

- **Ambiente (Environment):** Tracks ambient temperature
- **Batería (Battery):** Manages power state and charge level
- **Climatizador (Climate Controller):** Controls heating/cooling actuator

### Use Cases Implemented

- **CU-001:** Turn On Thermostat
- **CU-002:** Turn Off Thermostat
- **CU-003:** Set Desired Temperature
- **CU-004:** Control Thermostat (main orchestrator)
- **CU-005:** Actuate Climate Controller
- **CU-006:** Get Ambient Temperature
- **CU-007:** Check Battery Level
- **CU-008:** Display Thermostat Status

## Directory Structure

```
src/
├── aplicacion/          # Application layer (orchestration)
│   ├── gestores/        # Entity managers
│   └── servicios/       # Application services
├── dominio/             # Domain layer (business logic)
│   ├── entidades/       # Domain entities
│   └── servicios/       # Domain services
├── infraestructura/     # Infrastructure layer
│   ├── sensores/        # Sensor proxies
│   ├── actuadores/      # Actuators
│   └── visualizadores/  # Display components
├── dispositivos/        # Device layer (HAL)
│   ├── hal/             # Hardware abstraction
│   └── drivers/         # Device drivers
├── aspectos/            # Cross-cutting concerns
│   ├── logging/         # Logging system
│   ├── auditoria/       # Audit system
│   └── configuracion/   # Configuration management
├── common/              # Shared utilities
└── main.c               # Entry point

test/
├── unity/               # Unity test framework
├── unit/                # Unit tests by layer
├── integration/         # Integration tests
└── mocks/               # Test mocks and stubs
```

## Code Organization Patterns

### Naming Conventions

- **Files:** `snake_case.c` and `snake_case.h`
- **Functions:** `modulo_accion()` format (e.g., `ambiente_crear()`, `bateria_verificar_carga()`)
- **Types:** `PascalCase` (e.g., `Ambiente`, `Bateria`, `Climatizador`)
- **Constants:** `UPPER_SNAKE_CASE`
- **Private functions:** Prefix with `static`

### Header Guard Pattern
```c
#ifndef MODULO_NOMBRE_H
#define MODULO_NOMBRE_H

// Header content

#endif // MODULO_NOMBRE_H
```

### Module Structure Pattern

Each module follows this structure:
```c
// 1. File header with Doxygen comments
// 2. Includes (system, then project)
// 3. Type definitions
// 4. Public function declarations (in .h)
// 5. Public function implementations (in .c)
// 6. Static helper functions (in .c)
```

### Entity Pattern

Domain entities follow this pattern:
```c
// Header file (.h)
typedef struct Entidad Entidad;

Entidad* entidad_crear(parametros...);
void entidad_destruir(Entidad* entidad);
// Getters and setters
// Business logic methods

// Implementation file (.c)
struct Entidad {
    // Private fields
};
```

## CMake Configuration

The project uses CMake with modular library organization:

- **dominio** - Static library (domain layer, no external dependencies)
- **aplicacion** - Static library (depends on dominio)
- **infraestructura** - Static library (depends on dominio)
- **dispositivos** - Static library (hardware abstraction)
- **aspectos** - Static library (cross-cutting concerns)
- **common** - Static library (shared utilities)
- **termostato** - Main executable (links all libraries)

### CMake Options

- `BUILD_TESTS` - Enable/disable test compilation (default: ON)
- `BUILD_SIMULATOR` - Enable simulator support (default: ON)
- `TARGET_PLATFORM` - Target platform: sim, stm32, avr, esp32 (default: sim)
- `WARNINGS_AS_ERRORS` - Treat warnings as errors (default: OFF)

### Adding New Source Files

When adding new `.c` files, place them in the appropriate layer directory. CMake uses `file(GLOB_RECURSE)` to automatically discover them, but you need to reconfigure:

```bash
cmake -B build  # Reconfigure to pick up new files
cmake --build build
```

### Adding New Tests

Create test files in `test/unit/` and add them to CMakeLists.txt:

```cmake
add_unit_test(test_mi_modulo test/unit/test_mi_modulo.c)
```

## Quality Attributes

The system implements 17 quality scenarios prioritized across:

- **Reliability:** MTBF ≥ 720h, fault recovery
- **Performance:** Control cycle < 100ms, sensor latency < 50ms
- **Energy Efficiency:** Power consumption ≤ 5mA, battery life > 30 days
- **Availability:** Continuous operation under partial failures
- **Maintainability:** Localized changes, low coupling
- **Observability:** Remote diagnostics without physical access

## Development Workflow

### Before Starting Implementation

1. **Read use case specifications** in `docs/casos_uso/`
2. **Understand layer responsibilities** - implement from bottom-up (HAL → Domain → Application)
3. **Check architectural constraints** - respect unidirectional dependencies
4. **Review quality attributes** - ensure implementation meets performance/reliability requirements

### Adding New Features

1. **Start with domain layer:** Define entities and business logic
2. **Add infrastructure layer:** Implement proxies and actuators
3. **Implement application layer:** Orchestrate use cases
4. **Write tests:** Unit tests for each layer
5. **Update documentation:** Keep README and diagrams in sync

### Testing Strategy

- **Domain layer:** Pure unit tests (no dependencies)
- **Application layer:** Test with mocked infrastructure
- **Infrastructure layer:** Test with mocked HAL
- **Integration tests:** Test complete use case flows
- **Simulator mode:** Full system testing without hardware

## Important Constraints

### Performance Requirements

- Control cycle frequency: 100ms maximum
- Sensor read latency: 50ms maximum
- Display update: As needed (event-driven)

### Power Constraints

- Idle current: ≤ 5mA
- Active current: ≤ 50mA
- Battery life: > 30 days continuous operation

### Memory Constraints

- ROM: Target for microcontrollers (typically 32-128KB)
- RAM: Minimize dynamic allocations
- Stack: Keep recursion minimal or avoid

## Platform-Specific Considerations

### Simulator (TARGET_SIMULATOR)

- Uses standard C library
- Full printf debugging available
- No hardware timing constraints
- Default for development and testing

### STM32 (TARGET_STM32)

- ARM Cortex-M architecture
- Requires STM32 HAL
- Real-time constraints apply
- Limited RAM/ROM

### AVR (TARGET_AVR)

- 8-bit architecture
- Very limited RAM (2KB typical)
- Requires avr-libc
- No floating-point hardware

### ESP32 (TARGET_ESP32)

- Dual-core Xtensa
- WiFi/BLE available
- FreeRTOS integration
- More resources available

## Common Tasks

### Adding a New Entity

1. Create header in `src/dominio/entidades/`
2. Create implementation in same directory
3. Define struct, constructor, destructor
4. Implement business logic methods
5. Create corresponding unit test in `test/unit/`

### Adding a New Use Case

1. Create service in `src/aplicacion/servicios/`
2. Coordinate domain entities and infrastructure
3. Implement state machine if needed
4. Add integration test
5. Update documentation in `docs/casos_uso/`

### Adding Hardware Support

1. Define interface in `src/dispositivos/hal/`
2. Create platform-specific implementation
3. Use conditional compilation (`#ifdef TARGET_*`)
4. Provide simulator stub for testing
5. Test on target hardware

## Troubleshooting

### Build Fails with "No such file or directory"

- Ensure all placeholder files exist or have real implementations
- Run `cmake -B build` to reconfigure
- Check that CMake version is 3.20+

### Tests Fail to Build

- Verify Unity framework is installed in `test/unity/`
- Check that `BUILD_TESTS=ON` is set
- Ensure test files follow naming convention `test_*.c`

### Linking Errors

- Check library dependencies in CMakeLists.txt
- Verify layer dependencies are unidirectional
- Ensure all required libraries are linked to executable

### Runtime Crashes on Target

- Check stack size configuration
- Verify memory allocations don't exceed available RAM
- Review interrupt priorities
- Use simulator for initial debugging

## Git Workflow

### Branch Naming

- `feature/ISSE-XXX-descripcion` - New features
- `fix/ISSE-XXX-descripcion` - Bug fixes
- `docs/ISSE-XXX-descripcion` - Documentation updates

### Commit Messages

```
[ISSE-XXX] Brief description (max 50 chars)

Detailed explanation if needed.
References to related issues or use cases.
```

### Before Committing

1. Build successfully: `./build.sh`
2. Run all tests: `./build.sh test`
3. Check for warnings
4. Update documentation if architecture changed

## Resources

- **README.md** - Project overview and quick start
- **PLAN_CORRECCION_CONFIGURACION.md** - Configuration correction history
- **docs/arquitectura/** - Detailed architecture documentation
- **docs/casos_uso/** - Use case specifications
- **docs/escenarios_calidad/** - Quality attribute scenarios

## Notes for AI Assistants

- **Respect layer boundaries:** Never make upper layers depend on lower implementation details
- **Follow naming conventions:** Use Spanish names for domain concepts (Ambiente, Batería, Climatizador)
- **Generate complete code:** Include Doxygen comments, error handling, and defensive programming
- **Consider resource constraints:** Avoid dynamic allocations in critical paths
- **Write testable code:** Use dependency injection and interface segregation
- **Maintain consistency:** Follow existing patterns for similar functionality
- **Document decisions:** Add comments explaining non-obvious architectural choices
