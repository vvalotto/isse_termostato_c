#!/bin/bash
# ==============================================================================
# Script de Compilación Rápida - ISSE Termostato
# ==============================================================================

# Colores para output
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Detectar CMake
CMAKE=""
if command -v cmake &> /dev/null; then
    CMAKE="cmake"
elif [ -f "/Applications/CLion.app/Contents/bin/cmake/mac/x64/bin/cmake" ]; then
    CMAKE="/Applications/CLion.app/Contents/bin/cmake/mac/x64/bin/cmake"
else
    echo -e "${RED}Error: CMake no encontrado${NC}"
    exit 1
fi

echo -e "${BLUE}═══════════════════════════════════════════════════════════════${NC}"
echo -e "${BLUE}  ISSE Termostato - Build Script${NC}"
echo -e "${BLUE}═══════════════════════════════════════════════════════════════${NC}"
echo ""

# Opción de limpieza
if [ "$1" == "clean" ]; then
    echo -e "${YELLOW}Limpiando directorio build...${NC}"
    rm -rf build
    echo -e "${GREEN}✓ Build limpiado${NC}"
    echo ""
fi

# Determinar si compilar con tests
BUILD_TESTS_FLAG="OFF"
if [ "$1" == "test" ] || [ "$2" == "test" ] || [ "$1" == "tests" ] || [ "$2" == "tests" ]; then
    BUILD_TESTS_FLAG="ON"
fi

# Configurar si es necesario
if [ ! -d "build" ] || [ "$1" == "clean" ]; then
    echo -e "${YELLOW}Configurando proyecto con CMake...${NC}"
    $CMAKE -B build -DBUILD_TESTS=$BUILD_TESTS_FLAG -DTARGET_PLATFORM=sim
    if [ $? -ne 0 ]; then
        echo -e "${RED}✗ Error en configuración${NC}"
        exit 1
    fi
    echo -e "${GREEN}✓ Configuración exitosa${NC}"
    echo ""
fi

# Compilar
echo -e "${YELLOW}Compilando proyecto...${NC}"
$CMAKE --build build
if [ $? -ne 0 ]; then
    echo -e "${RED}✗ Error en compilación${NC}"
    exit 1
fi

echo -e "${GREEN}✓ Compilación exitosa${NC}"
echo ""

# Ejecutar si se solicita
if [ "$1" == "run" ] || [ "$2" == "run" ]; then
    echo -e "${BLUE}═══════════════════════════════════════════════════════════════${NC}"
    echo -e "${BLUE}  Ejecutando Termostato${NC}"
    echo -e "${BLUE}═══════════════════════════════════════════════════════════════${NC}"
    echo ""
    ./build/bin/termostato
fi

# Ejecutar tests si se solicita
if [ "$1" == "test" ] || [ "$2" == "test" ] || [ "$1" == "tests" ] || [ "$2" == "tests" ]; then
    echo -e "${BLUE}═══════════════════════════════════════════════════════════════${NC}"
    echo -e "${BLUE}  Ejecutando Tests${NC}"
    echo -e "${BLUE}═══════════════════════════════════════════════════════════════${NC}"
    echo ""
    cd build
    /Applications/CLion.app/Contents/bin/cmake/mac/x64/bin/ctest --output-on-failure
    TEST_RESULT=$?
    cd ..
    echo ""
    if [ $TEST_RESULT -eq 0 ]; then
        echo -e "${GREEN}✓ Todos los tests pasaron${NC}"
    else
        echo -e "${RED}✗ Algunos tests fallaron${NC}"
        exit 1
    fi
fi

# Resumen
echo ""
echo -e "${GREEN}═══════════════════════════════════════════════════════════════${NC}"
echo -e "${GREEN}  Build completado exitosamente${NC}"
echo -e "${GREEN}═══════════════════════════════════════════════════════════════${NC}"
echo -e "  Ejecutable: ${YELLOW}./build/bin/termostato${NC}"
echo -e "  Librerías:  ${YELLOW}./build/lib/${NC}"
echo ""
echo -e "  Uso:"
echo -e "    ${BLUE}./build.sh${NC}                - Compilar"
echo -e "    ${BLUE}./build.sh run${NC}            - Compilar y ejecutar"
echo -e "    ${BLUE}./build.sh test${NC}           - Compilar con tests y ejecutarlos"
echo -e "    ${BLUE}./build.sh clean${NC}          - Limpiar y recompilar"
echo -e "    ${BLUE}./build.sh clean run${NC}      - Limpiar, compilar y ejecutar"
echo -e "    ${BLUE}./build.sh clean test${NC}     - Limpiar, compilar y ejecutar tests"
echo ""
