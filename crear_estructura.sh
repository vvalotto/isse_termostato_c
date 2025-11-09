#!/bin/bash
# ==============================================================================
# Script: crear_estructura.sh
# Proyecto: ISSE Termostato - Sistema de Control de Temperatura
# Descripción: Crea la estructura completa de directorios del proyecto
# Autor: Victor Valotto
# Fecha: 2025-11-08
# ==============================================================================

# Colores para output
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Función para crear directorio
create_dir() {
    if [ ! -d "$1" ]; then
        mkdir -p "$1"
        echo -e "${GREEN}✓${NC} Creado: $1"
    else
        echo -e "${YELLOW}→${NC} Ya existe: $1"
    fi
}

# Función para crear .gitkeep (para que Git trackee carpetas vacías)
create_gitkeep() {
    if [ ! -f "$1/.gitkeep" ]; then
        touch "$1/.gitkeep"
        echo -e "${BLUE}  ├─${NC} .gitkeep agregado"
    fi
}

echo ""
echo "╔════════════════════════════════════════════════════════════════╗"
echo "║   ISSE Termostato - Creación de Estructura del Proyecto       ║"
echo "╚════════════════════════════════════════════════════════════════╝"
echo ""

# Verificar que estamos en el directorio correcto
if [ ! -f "CMakeLists.txt" ]; then
    echo -e "${YELLOW}⚠ Advertencia:${NC} No se encontró CMakeLists.txt en el directorio actual"
    echo -e "  ¿Estás en la raíz del proyecto isse_termostato_c?"
    read -p "  ¿Deseas continuar de todas formas? (s/n): " -n 1 -r
    echo
    if [[ ! $REPLY =~ ^[Ss]$ ]]; then
        echo "Operación cancelada."
        exit 1
    fi
fi

echo "Creando estructura de directorios..."
echo ""

# ==============================================================================
# CAPA DE APLICACIÓN
# ==============================================================================
echo "📦 Capa de Aplicación"
create_dir "src/aplicacion/gestores"
create_gitkeep "src/aplicacion/gestores"

create_dir "src/aplicacion/servicios"
create_gitkeep "src/aplicacion/servicios"
echo ""

# ==============================================================================
# CAPA DE DOMINIO
# ==============================================================================
echo "🎯 Capa de Dominio"
create_dir "src/dominio/entidades"
create_gitkeep "src/dominio/entidades"

create_dir "src/dominio/servicios"
create_gitkeep "src/dominio/servicios"
echo ""

# ==============================================================================
# CAPA DE INFRAESTRUCTURA
# ==============================================================================
echo "🔧 Capa de Infraestructura"
create_dir "src/infraestructura/sensores"
create_gitkeep "src/infraestructura/sensores"

create_dir "src/infraestructura/actuadores"
create_gitkeep "src/infraestructura/actuadores"

create_dir "src/infraestructura/visualizadores"
create_gitkeep "src/infraestructura/visualizadores"
echo ""

# ==============================================================================
# CAPA DE DISPOSITIVOS
# ==============================================================================
echo "⚙️  Capa de Dispositivos"
create_dir "src/dispositivos/hal"
create_gitkeep "src/dispositivos/hal"

create_dir "src/dispositivos/drivers"
create_gitkeep "src/dispositivos/drivers"
echo ""

# ==============================================================================
# ASPECTOS TRANSVERSALES
# ==============================================================================
echo "🔀 Aspectos Transversales"
create_dir "src/aspectos"
create_gitkeep "src/aspectos"
echo ""

# ==============================================================================
# COMMON
# ==============================================================================
echo "📚 Common (Código Compartido)"
create_dir "src/common"
create_gitkeep "src/common"
echo ""

# ==============================================================================
# TESTS
# ==============================================================================
echo "🧪 Tests"
create_dir "test/unity"
create_gitkeep "test/unity"

create_dir "test/unit"
create_gitkeep "test/unit"

create_dir "test/integration"
create_gitkeep "test/integration"

create_dir "test/mocks"
create_gitkeep "test/mocks"
echo ""

# ==============================================================================
# DOCUMENTACIÓN
# ==============================================================================
echo "📖 Documentación"
create_dir "docs/arquitectura"
create_gitkeep "docs/arquitectura"

create_dir "docs/casos_uso"
create_gitkeep "docs/casos_uso"

create_dir "docs/escenarios_calidad"
create_gitkeep "docs/escenarios_calidad"

create_dir "docs/diagramas"
create_gitkeep "docs/diagramas"
echo ""

# ==============================================================================
# CONFIGURACIÓN
# ==============================================================================
echo "⚙️  Configuración"
create_dir "config"
create_gitkeep "config"
echo ""

# ==============================================================================
# SCRIPTS
# ==============================================================================
echo "📜 Scripts"
create_dir "scripts"
create_gitkeep "scripts"
echo ""

# ==============================================================================
# HERRAMIENTAS
# ==============================================================================
echo "🛠️  Herramientas"
create_dir "tools/simuladores"
create_gitkeep "tools/simuladores"
echo ""

# ==============================================================================
# BUILD (no se versiona, pero útil crear)
# ==============================================================================
echo "🏗️  Build (directorio local, no versionado)"
create_dir "build"
echo -e "${BLUE}  ├─${NC} Este directorio está en .gitignore"
echo ""

# ==============================================================================
# RESUMEN
# ==============================================================================
echo "╔════════════════════════════════════════════════════════════════╗"
echo "║                      ✅ ESTRUCTURA CREADA                       ║"
echo "╚════════════════════════════════════════════════════════════════╝"
echo ""
echo "Resumen de directorios creados:"
echo ""
echo "  src/"
echo "    ├── aplicacion/    (gestores, servicios)"
echo "    ├── dominio/       (entidades, servicios)"
echo "    ├── infraestructura/ (sensores, actuadores, visualizadores)"
echo "    ├── dispositivos/  (hal, drivers)"
echo "    ├── aspectos/"
echo "    └── common/"
echo ""
echo "  test/"
echo "    ├── unity/"
echo "    ├── unit/"
echo "    ├── integration/"
echo "    └── mocks/"
echo ""
echo "  docs/"
echo "    ├── arquitectura/"
echo "    ├── casos_uso/"
echo "    ├── escenarios_calidad/"
echo "    └── diagramas/"
echo ""
echo "  config/"
echo "  scripts/"
echo "  tools/simuladores/"
echo "  build/"
echo ""
echo "Próximos pasos:"
echo "  1. Verificar la estructura: tree -L 3 (o ls -R)"
echo "  2. Agregar archivos .gitignore y README.md si no existen"
echo "  3. Hacer commit inicial: git add . && git commit -m \"[SETUP] Estructura inicial del proyecto\""
echo ""
