# Plan de Implementación - ISSE Termostato

Este directorio contiene los planes de implementación detallados para cada historia de usuario del proyecto.

---

## 📂 Estructura

Cada historia de usuario tiene su propio subdirectorio con documentación completa:

```
plan_implementacion/
├── README.md (este archivo)
└── ITE-56/                          ← HU-014: Obtener temperatura ambiente
    ├── README.md                    ← Plan completo y detallado
    ├── CHECKLIST.md                 ← Checklist paso a paso
    ├── ESPECIFICACIONES_TECNICAS.md ← Código completo de referencia
    └── GUIA_RAPIDA.md              ← Guía de inicio rápido
```

---

## 🎯 Historias de Usuario

### Sprint 1 (10-14 Nov 2025)

| Historia | Epic | Estado | Documentación |
|----------|------|--------|---------------|
| **ITE-56:** HU-014 - Obtener temperatura ambiente | CU-006 | 🔴 No iniciado | [Ver plan](ITE-56/) |

### Próximos Sprints

Pendiente de planificación:
- ITE-57: HU-007 - Obtener nivel de carga de batería
- ITE-51: HU-001 - Apagar el climatizador
- ITE-52: HU-006 - Establecer temperatura deseada
- ITE-55: HU-015 - Accionar climatizador
- Etc.

---

## 📋 Cómo Usar Este Directorio

### Para Implementar una Historia

1. **Navegar al directorio de la historia:**
   ```bash
   cd docs/plan_implementacion/ITE-56/
   ```

2. **Leer primero:**
   - **README.md** - Contexto completo, arquitectura, estimaciones
   - **GUIA_RAPIDA.md** - Si quieres empezar rápido

3. **Durante implementación:**
   - **CHECKLIST.md** - Seguir paso a paso, marcar progreso
   - **ESPECIFICACIONES_TECNICAS.md** - Referencia de código completo

4. **Usar checklist:**
   ```bash
   # Ir marcando con 'x' conforme avanzas
   - [x] Tarea completada
   - [ ] Tarea pendiente
   ```

---

## 🎨 Template para Nuevas Historias

Cuando se planifique una nueva historia, crear:

```
docs/plan_implementacion/ITE-XXX/
├── README.md                    ← Basado en template
├── CHECKLIST.md                 ← Lista de tareas específicas
├── ESPECIFICACIONES_TECNICAS.md ← Código de referencia
└── GUIA_RAPIDA.md              ← Quick start
```

**Template disponible:** (Usar ITE-56 como referencia)

---

## 📊 Metodología de Planificación

### Principios

1. **Bottom-Up Implementation:**
   - Implementar desde capas inferiores hacia superiores
   - Permite probar cada componente aisladamente
   - Respeta dependencias arquitectónicas

2. **Test-Driven:**
   - Crear test junto (o antes) que la implementación
   - Cada componente tiene su suite de tests
   - Test de integración al final

3. **Incremental:**
   - Commits pequeños y frecuentes
   - Cada fase es validable independientemente
   - Fácil revertir si algo sale mal

4. **Documentado:**
   - Código completo de referencia disponible
   - Decisiones de diseño explicadas
   - Patrones identificados

### Fases Típicas

Todo plan de implementación sigue estas fases:

1. **Tipos comunes** - Tipos base y constantes
2. **Dominio** - Entidades puras de negocio
3. **Dispositivos** - HAL y drivers
4. **Infraestructura** - Proxies y adaptadores
5. **Aplicación** - Coordinadores y gestores
6. **Integración** - main.c y ciclo principal
7. **Tests completos** - Suite exhaustiva

---

## 🔗 Referencias

- **Jira:** https://vvalotto.atlassian.net/jira/software/projects/ITE/boards/2
- **Confluence:** https://vvalotto.atlassian.net/wiki/spaces/I2A/overview
- **GitHub:** Branch pattern: `ITE-XXX-HU-YYY-descripcion`
- **Commits:** Pattern: `[ITE-XXX] Descripción del cambio`

---

## 📈 Métricas de Planificación

### ITE-56 (Referencia)

- **Archivos a crear:** 16 (5 .h + 5 .c + 6 tests)
- **Líneas estimadas:** ~1000 (incluyendo tests)
- **Tiempo estimado:** 2.5 horas
- **Fases:** 7

---

## ✨ Mejores Prácticas

### Al Crear un Plan

1. ✅ Identificar componentes y dependencias
2. ✅ Definir interfaces antes que implementaciones
3. ✅ Planificar tests para cada componente
4. ✅ Estimar tiempos realistamente
5. ✅ Documentar decisiones de diseño
6. ✅ Proporcionar código de referencia completo

### Durante Implementación

1. ✅ Seguir el orden del plan (bottom-up)
2. ✅ Marcar checklist conforme avanzas
3. ✅ Compilar frecuentemente
4. ✅ Ejecutar tests después de cada componente
5. ✅ Commits pequeños y descriptivos
6. ✅ Actualizar métricas reales vs estimadas

### Al Finalizar

1. ✅ Validar que todos los criterios de aceptación se cumplen
2. ✅ Ejecutar suite completa de tests
3. ✅ Verificar sin memory leaks
4. ✅ Push a GitHub
5. ✅ Actualizar estado en Jira
6. ✅ Documentar lecciones aprendidas

---

## 📝 Notas

- Los planes son documentos vivos - actualizar según necesidad
- Las estimaciones mejoran con la experiencia
- Las especificaciones técnicas son referencias, no son ley
- Adaptar según contexto específico del sprint

---

**Última actualización:** 2025-11-10
**Historias planificadas:** 1 (ITE-56)
**Estado:** Listo para Sprint 1
