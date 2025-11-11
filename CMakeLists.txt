# ==============================================================================
# CMakeLists.txt - ISSE Termostato
# Sistema de Control de Temperatura - Arquitectura por Capas
# ==============================================================================

cmake_minimum_required(VERSION 3.20)

# ------------------------------------------------------------------------------
# Información del Proyecto
# ------------------------------------------------------------------------------
project(isse_termostato_c
    VERSION 1.0.0
    DESCRIPTION "Sistema embebido de control de temperatura con arquitectura por capas"
    LANGUAGES C
)

# ------------------------------------------------------------------------------
# Estándar de C
# ------------------------------------------------------------------------------
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# ------------------------------------------------------------------------------
# Opciones de Configuración
# ------------------------------------------------------------------------------
option(BUILD_TESTS "Compilar tests unitarios e integración" ON)
option(BUILD_SIMULATOR "Compilar con soporte para simuladores" ON)
option(ENABLE_WARNINGS "Habilitar warnings del compilador" ON)
option(WARNINGS_AS_ERRORS "Tratar warnings como errores" OFF)

# Plataforma objetivo: sim (simulador), stm32, avr, esp32
set(TARGET_PLATFORM "sim" CACHE STRING "Plataforma de hardware objetivo")
set_property(CACHE TARGET_PLATFORM PROPERTY STRINGS sim stm32 avr esp32)

# ------------------------------------------------------------------------------
# Configuración de Output
# ------------------------------------------------------------------------------
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# ------------------------------------------------------------------------------
# Flags de Compilación según Plataforma
# ------------------------------------------------------------------------------
if(TARGET_PLATFORM STREQUAL "sim")
    message(STATUS "Configurando para SIMULADOR")
    add_definitions(-DTARGET_SIMULATOR)
    
    # Flags para desarrollo en host
    if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options(
            -Wall
            -Wextra
            -Wpedantic
            -Wno-unused-parameter
            -Wno-unused-function
        )
        
        if(WARNINGS_AS_ERRORS)
            add_compile_options(-Werror)
        endif()
        
        # Debug symbols
        add_compile_options(-g)
    endif()
    
elseif(TARGET_PLATFORM STREQUAL "stm32")
    message(STATUS "Configurando para STM32")
    add_definitions(-DTARGET_STM32)
    
    # MCU específico (ajustar según tu STM32)
    set(MCU "STM32F103C8")
    add_definitions(-D${MCU})
    
    # Flags para ARM Cortex-M3
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-m3 -mthumb")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffunction-sections -fdata-sections")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-sections")
    
elseif(TARGET_PLATFORM STREQUAL "avr")
    message(STATUS "Configurando para AVR")
    add_definitions(-DTARGET_AVR)
    
    # MCU específico (ajustar según tu AVR)
    set(MCU "atmega328p")
    set(F_CPU "16000000UL")
    add_definitions(-DF_CPU=${F_CPU})
    
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mmcu=${MCU}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Os -ffunction-sections -fdata-sections")
    
elseif(TARGET_PLATFORM STREQUAL "esp32")
    message(STATUS "Configurando para ESP32")
    add_definitions(-DTARGET_ESP32)
    
    # Flags específicos de ESP32 (requiere ESP-IDF)
    # Este es un placeholder - en producción usarías el toolchain de ESP-IDF
    
else()
    message(FATAL_ERROR "Plataforma no soportada: ${TARGET_PLATFORM}")
endif()

# ------------------------------------------------------------------------------
# Directorios de Include (Arquitectura por Capas)
# ------------------------------------------------------------------------------
include_directories(
    # Capa de Aplicación
    ${CMAKE_SOURCE_DIR}/src/aplicacion/gestores
    ${CMAKE_SOURCE_DIR}/src/aplicacion/servicios
    
    # Capa de Dominio
    ${CMAKE_SOURCE_DIR}/src/dominio/entidades
    ${CMAKE_SOURCE_DIR}/src/dominio/servicios
    
    # Capa de Infraestructura
    ${CMAKE_SOURCE_DIR}/src/infraestructura/sensores
    ${CMAKE_SOURCE_DIR}/src/infraestructura/actuadores
    ${CMAKE_SOURCE_DIR}/src/infraestructura/visualizadores
    
    # Capa de Dispositivos
    ${CMAKE_SOURCE_DIR}/src/dispositivos/hal
    ${CMAKE_SOURCE_DIR}/src/dispositivos/drivers
    
    # Aspectos Transversales
    ${CMAKE_SOURCE_DIR}/src/aspectos
    
    # Common
    ${CMAKE_SOURCE_DIR}/src/common
)

# ------------------------------------------------------------------------------
# Fuentes por Capa
# ------------------------------------------------------------------------------

# Capa de Dominio (Lógica de Negocio)
file(GLOB_RECURSE DOMINIO_SOURCES
    ${CMAKE_SOURCE_DIR}/src/dominio/entidades/*.c
    ${CMAKE_SOURCE_DIR}/src/dominio/servicios/*.c
)

# Capa de Aplicación (Orquestación)
file(GLOB_RECURSE APLICACION_SOURCES
    ${CMAKE_SOURCE_DIR}/src/aplicacion/gestores/*.c
    ${CMAKE_SOURCE_DIR}/src/aplicacion/servicios/*.c
)

# Capa de Infraestructura (Acceso a Recursos)
file(GLOB_RECURSE INFRAESTRUCTURA_SOURCES
    ${CMAKE_SOURCE_DIR}/src/infraestructura/sensores/*.c
    ${CMAKE_SOURCE_DIR}/src/infraestructura/actuadores/*.c
    ${CMAKE_SOURCE_DIR}/src/infraestructura/visualizadores/*.c
)

# Capa de Dispositivos (HAL y Drivers)
file(GLOB_RECURSE DISPOSITIVOS_SOURCES
    ${CMAKE_SOURCE_DIR}/src/dispositivos/hal/*.c
    ${CMAKE_SOURCE_DIR}/src/dispositivos/drivers/*.c
)

# Aspectos Transversales
file(GLOB_RECURSE ASPECTOS_SOURCES
    ${CMAKE_SOURCE_DIR}/src/aspectos/*/*.c
)

# Common
file(GLOB_RECURSE COMMON_SOURCES
    ${CMAKE_SOURCE_DIR}/src/common/*.c
)

# ------------------------------------------------------------------------------
# Librerías por Capa (Compilación Modular)
# ------------------------------------------------------------------------------

# Librería del Dominio (puro, sin dependencias externas)
add_library(dominio STATIC ${DOMINIO_SOURCES})
target_compile_definitions(dominio PRIVATE CAPA_DOMINIO)

# Librería de Aplicación (depende de Dominio)
add_library(aplicacion STATIC ${APLICACION_SOURCES})
target_link_libraries(aplicacion PRIVATE dominio)
target_compile_definitions(aplicacion PRIVATE CAPA_APLICACION)

# Librería de Infraestructura (depende de Dominio)
add_library(infraestructura STATIC ${INFRAESTRUCTURA_SOURCES})
target_link_libraries(infraestructura PRIVATE dominio)
target_compile_definitions(infraestructura PRIVATE CAPA_INFRAESTRUCTURA)

# Librería de Dispositivos (hardware abstraction)
add_library(dispositivos STATIC ${DISPOSITIVOS_SOURCES})
target_compile_definitions(dispositivos PRIVATE CAPA_DISPOSITIVOS)

# Librería de Aspectos Transversales
add_library(aspectos STATIC ${ASPECTOS_SOURCES})
target_compile_definitions(aspectos PRIVATE ASPECTOS_TRANSVERSALES)

# Librería Common
add_library(common STATIC ${COMMON_SOURCES})

# ------------------------------------------------------------------------------
# Ejecutable Principal
# ------------------------------------------------------------------------------
add_executable(termostato
    src/main.c
)

target_link_libraries(termostato PRIVATE
    aplicacion
    dominio
    infraestructura
    dispositivos
    aspectos
    common
)

# ------------------------------------------------------------------------------
# Tests Unitarios (Unity Framework)
# ------------------------------------------------------------------------------
if(BUILD_TESTS)
    message(STATUS "Habilitando tests unitarios")
    
    # Unity Test Framework (embebido en el proyecto)
    add_library(unity STATIC
        test/unity/unity.c
    )
    target_include_directories(unity PUBLIC
        test/unity
    )
    
    # Función helper para crear tests
    function(add_unit_test test_name test_source)
        add_executable(${test_name} ${test_source})
        target_link_libraries(${test_name} PRIVATE
            unity
            dominio
            aplicacion
            infraestructura
            dispositivos
            aspectos
            common
        )
        add_test(NAME ${test_name} COMMAND ${test_name})
    endfunction()
    
    # Habilitar CTest
    enable_testing()

    # Test de Ejemplo (verificar que Unity funciona)
    add_unit_test(test_ejemplo test/unit/test_ejemplo.c)

    # Tests de Dominio
    add_unit_test(test_ambiente test/unit/test_ambiente.c)

    # Tests de Aplicación
    
    # Tests de Integración
    
endif()

# ------------------------------------------------------------------------------
# Simuladores (para desarrollo sin hardware)
# ------------------------------------------------------------------------------
if(BUILD_SIMULATOR AND TARGET_PLATFORM STREQUAL "sim")
    message(STATUS "Habilitando simuladores")

endif()

# ------------------------------------------------------------------------------
# Instalación
# ------------------------------------------------------------------------------
install(TARGETS termostato
    RUNTIME DESTINATION bin
)

# ------------------------------------------------------------------------------
# Información de Configuración
# ------------------------------------------------------------------------------
message(STATUS "")
message(STATUS "════════════════════════════════════════════════════════════")
message(STATUS "  ISSE Termostato - Configuración de Build")
message(STATUS "════════════════════════════════════════════════════════════")
message(STATUS "  Versión:              ${PROJECT_VERSION}")
message(STATUS "  Plataforma objetivo:  ${TARGET_PLATFORM}")
message(STATUS "  Compilador:           ${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION}")
message(STATUS "  Build Type:           ${CMAKE_BUILD_TYPE}")
message(STATUS "  Tests:                ${BUILD_TESTS}")
message(STATUS "  Simuladores:          ${BUILD_SIMULATOR}")
message(STATUS "  Warnings as errors:   ${WARNINGS_AS_ERRORS}")
message(STATUS "  Binary output:        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "════════════════════════════════════════════════════════════")
message(STATUS "")

# ------------------------------------------------------------------------------
# Targets Personalizados
# ------------------------------------------------------------------------------

# Target para limpiar completamente
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}
    COMMENT "Limpiando todos los archivos de build..."
)

# Target para ver la configuración
add_custom_target(show-config
    COMMAND ${CMAKE_COMMAND} -E echo "Configuración actual:"
    COMMAND ${CMAKE_COMMAND} -E echo "  TARGET_PLATFORM = ${TARGET_PLATFORM}"
    COMMAND ${CMAKE_COMMAND} -E echo "  BUILD_TESTS = ${BUILD_TESTS}"
    COMMAND ${CMAKE_COMMAND} -E echo "  BUILD_SIMULATOR = ${BUILD_SIMULATOR}"
    COMMENT "Mostrando configuración..."
)

# ==============================================================================
# FIN DEL CMakeLists.txt
# ==============================================================================
