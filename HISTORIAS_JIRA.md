# Historias de Usuario desde Jira - Proyecto ITE

**Fecha de extracción:** 2025-11-09
**Proyecto:** ITE - ISSE Termostato
**Total de Historias:** 16
**Total de Epics:** 8

---

## 📊 Resumen Ejecutivo

### Epics (Casos de Uso)

| Key | Epic | Estado |
|-----|------|--------|
| ITE-39 | CU-001: Encender Termostato | Tareas por hacer |
| ITE-40 | CU-002: Apagar Termostato | Tareas por hacer |
| ITE-41 | CU-003: Establecer Temperatura Deseada | Tareas por hacer |
| ITE-42 | CU-004: Controlar Termostato | Tareas por hacer |
| ITE-43 | CU-005: Accionar Climatizador | Tareas por hacer |
| ITE-44 | CU-006: Obtener Temperatura Ambiente | Tareas por hacer |
| ITE-45 | CU-007: Verificar Nivel de Carga de la Batería | Tareas por hacer |
| ITE-46 | CU-008: Mostrar Estado del Termostato | Tareas por hacer |

### Historias de Usuario

| Key | Historia | Estado | Epic Relacionado |
|-----|----------|--------|------------------|
| ITE-47 | HU-002: Encender el Termostato | Tareas por hacer | CU-001 |
| ITE-48 | HU-003: Encender y no está disponible el climatizador | Tareas por hacer | CU-001 |
| ITE-49 | HU-004: Encender y no se lee la carga de la batería | Tareas por hacer | CU-001 |
| ITE-50 | HU-005: Encender y no sensa la temperatura | Tareas por hacer | CU-001 |
| ITE-51 | HU-001: Apagar el climatizador | Tareas por hacer | CU-002 |
| ITE-52 | HU-006: Establecer Temperatura deseada | Tareas por hacer | CU-003 |
| ITE-53 | HU-009: Lanzar las funciones de control | Tareas por hacer | CU-004 |
| ITE-54 | HU-016: Compara Temperaturas | Tareas por hacer | CU-004 |
| ITE-55 | HU-015: Accionar Climatizador | Tareas por hacer | CU-005 |
| ITE-56 | HU-014: Obtener la temperatura ambiente | Tareas por hacer | CU-006 |
| ITE-57 | HU-007: Obtener el nivel de carga de la batería | Tareas por hacer | CU-007 |
| ITE-58 | HU-008: Mostrar Temperatura Ambiente | Tareas por hacer | CU-008 |
| ITE-59 | HU-010: Mostrar Estado del Climatizador | Tareas por hacer | CU-008 |
| ITE-60 | HU-011: Mostrar Temperatura Deseada | Tareas por hacer | CU-008 |
| ITE-61 | HU-012: Indicar Nivel de Carga | Tareas por hacer | CU-008 |
| ITE-62 | HU-013: Indicar no hay sensor de temperatura | Tareas por hacer | CU-008 |

---

## 📦 Historias por Caso de Uso

### CU-001: Encender Termostato (Epic ITE-39)

#### ITE-47: HU-002 - Encender el Termostato (Principal)

**Como** dueño de casa
**Quiero** encender el termostato
**Para** ver que está preparado para configurarlo (tiene carga de batería y muestra la temperatura ambiente)

**Preguntas de Validación:**
- ¿El termostato se enciende correctamente cuando se presiona el botón de encendido?
- ¿Se muestra el nivel de carga de la batería en pantalla?
- ¿Se muestra la temperatura ambiente actual en pantalla?
- ¿El estado del climatizador se muestra como "Apagado"?
- ¿Todos los componentes (batería, sensor de temperatura) están funcionando correctamente?

**Relación:** Implementa el flujo principal completo del CU-001, pasos 1-7

**Enlace Confluence:** https://vvalotto.atlassian.net/wiki/spaces/I2A/pages/169345051

---

#### ITE-48: HU-003 - Encender y no está disponible el climatizador (Alternativa)

**Como** dueño de casa
**Quiero** que el termostato me informe cuando intente encenderlo y el climatizador no esté disponible
**Para** saber que no puedo controlar la temperatura hasta que el problema se solucione

**Preguntas de Validación:**
- ¿El termostato detecta correctamente cuando el climatizador no está disponible?
- ¿Se muestra un mensaje de error claro en pantalla?
- ¿El termostato no intenta accionar el climatizador?
- ¿El termostato se puede encender pero sin control de temperatura?
- ¿Se registra el error en el log?

**Relación:** Flujo alternativo del CU-001 (paso 5a)

**Tipo:** ⚠️ Excepción/Error

---

#### ITE-49: HU-004 - Encender y no se lee la carga de la batería (Alternativa)

**Como** dueño de casa
**Quiero** que el termostato me informe cuando no pueda leer el nivel de batería
**Para** tomar medidas preventivas sobre el suministro de energía

**Preguntas de Validación:**
- ¿El termostato detecta fallas en el sensor de batería?
- ¿Se muestra un mensaje de advertencia sobre la batería?
- ¿El termostato puede continuar operando?
- ¿Se asume un nivel de batería predeterminado seguro?
- ¿Se registra el error en el log?

**Relación:** Flujo alternativo del CU-001 (paso 6a)

**Tipo:** ⚠️ Excepción/Error

---

#### ITE-50: HU-005 - Encender y no sensa la temperatura (Alternativa)

**Como** dueño de casa
**Quiero** que el termostato me informe cuando no pueda leer la temperatura ambiente
**Para** saber que no puede controlar correctamente el clima

**Preguntas de Validación:**
- ¿El termostato detecta fallas en el sensor de temperatura?
- ¿Se muestra un mensaje de error claro?
- ¿El termostato no intenta controlar el clima sin datos de temperatura?
- ¿Se puede configurar pero no operar?
- ¿Se registra el error en el log?

**Relación:** Flujo alternativo del CU-001 (paso 4a)

**Tipo:** ⚠️ Excepción/Error

---

### CU-002: Apagar Termostato (Epic ITE-40)

#### ITE-51: HU-001 - Apagar el climatizador

**Como** dueño de casa
**Quiero** apagar el termostato cuando ya no necesite controlar la temperatura
**Para** ahorrar energía de la batería

**Preguntas de Validación:**
- ¿El termostato se apaga correctamente al presionar el botón?
- ¿El climatizador se apaga antes de apagar el termostato?
- ¿Se guarda la configuración actual antes de apagar?
- ¿El display se apaga completamente?
- ¿Se detiene el ciclo de control?

**Relación:** Implementa el CU-002 completo

**Tipo:** 🎯 Principal

---

### CU-003: Establecer Temperatura Deseada (Epic ITE-41)

#### ITE-52: HU-006 - Establecer Temperatura deseada

**Como** dueño de casa
**Quiero** configurar la temperatura deseada usando botones arriba/abajo
**Para** ajustar el nivel de confort según mis preferencias

**Preguntas de Validación:**
- ¿Los botones arriba/abajo ajustan la temperatura correctamente?
- ¿El incremento es de 0.5°C por pulsación?
- ¿El rango está limitado entre 10°C y 35°C?
- ¿Se muestra la nueva temperatura en el display inmediatamente?
- ¿La configuración se guarda automáticamente?
- ¿El sistema ajusta el control para alcanzar la nueva temperatura?

**Relación:** Implementa el CU-003 completo

**Tipo:** 🎯 Principal

---

### CU-004: Controlar Termostato (Epic ITE-42)

#### ITE-53: HU-009 - Lanzar las funciones de control

**Como** sistema termostato
**Quiero** ejecutar el ciclo de control automático periódicamente
**Para** mantener la temperatura ambiente en el nivel deseado

**Preguntas de Validación:**
- ¿El ciclo de control se ejecuta cada 100ms?
- ¿Se coordinan correctamente todas las funciones (leer sensor, comparar, actuar)?
- ¿El sistema responde a cambios en la temperatura deseada?
- ¿Se verifica la batería antes de cada acción?
- ¿El ciclo es determinista y predecible?

**Relación:** Orquestador principal del CU-004

**Tipo:** 🎯 Principal - Core del sistema

---

#### ITE-54: HU-016 - Compara Temperaturas

**Como** sistema termostato
**Quiero** comparar la temperatura actual con la deseada aplicando histéresis
**Para** decidir si debo activar calefacción, refrigeración o mantener apagado

**Preguntas de Validación:**
- ¿La comparación aplica correctamente la histéresis de ±0.5°C?
- ¿Se evitan oscilaciones continuas del climatizador?
- ¿La lógica de decisión es clara y predecible?
- ¿Se consideran los límites de temperatura permitidos?
- ¿La decisión se toma en menos de 10ms?

**Relación:** Parte del algoritmo de control del CU-004

**Tipo:** 🔧 Técnica - Algoritmo de control

---

### CU-005: Accionar Climatizador (Epic ITE-43)

#### ITE-55: HU-015 - Accionar Climatizador

**Como** sistema termostato
**Quiero** controlar el climatizador en modo calefacción, refrigeración o apagado
**Para** ajustar la temperatura ambiente según sea necesario

**Preguntas de Validación:**
- ¿Se puede cambiar entre modos CALEFACCION, REFRIGERACION y APAGADO?
- ¿Se respeta el retardo de seguridad entre cambios de modo?
- ¿No se cambia directamente de CALEFACCION a REFRIGERACION sin pasar por APAGADO?
- ¿Se registra cada cambio de estado?
- ¿Se maneja correctamente un fallo en el actuador?

**Relación:** Implementa el CU-005 completo

**Tipo:** 🎯 Principal - Actuador crítico

---

### CU-006: Obtener Temperatura Ambiente (Epic ITE-44)

#### ITE-56: HU-014 - Obtener la temperatura ambiente

**Como** sistema termostato
**Quiero** leer la temperatura del sensor cada ciclo de control
**Para** tomar decisiones basadas en datos reales

**Preguntas de Validación:**
- ¿La lectura se completa en menos de 50ms?
- ¿Se valida que la temperatura esté en un rango físicamente posible?
- ¿Se reintenta hasta 3 veces en caso de fallo?
- ¿Se aplica filtrado para eliminar ruido?
- ¿La precisión es de ±0.5°C?

**Relación:** Implementa el CU-006 completo

**Tipo:** 🎯 Principal - Sensor crítico

---

### CU-007: Verificar Nivel de Carga de la Batería (Epic ITE-45)

#### ITE-57: HU-007 - Obtener el nivel de carga de la batería

**Como** sistema termostato
**Quiero** verificar el nivel de batería periódicamente
**Para** alertar al usuario y prevenir apagado inesperado

**Preguntas de Validación:**
- ¿Se verifica la batería cada 10 segundos?
- ¿Se detectan correctamente los niveles: NORMAL (>20%), BAJO (10-20%), CRITICO (<10%)?
- ¿Se muestra indicador en pantalla cuando está baja?
- ¿El sistema entra en modo ahorro con batería crítica?
- ¿Se apaga automáticamente al llegar a 5%?

**Relación:** Implementa el CU-007 completo

**Tipo:** 🎯 Principal - Gestión de energía

---

### CU-008: Mostrar Estado del Termostato (Epic ITE-46)

#### ITE-58: HU-008 - Mostrar Temperatura Ambiente

**Como** dueño de casa
**Quiero** ver la temperatura ambiente actual en el display
**Para** conocer la temperatura real del ambiente

**Preguntas de Validación:**
- ¿La temperatura se muestra con 1 decimal?
- ¿El valor se actualiza cada ciclo sin parpadeos?
- ¿Es claramente legible?
- ¿Se muestra unidad (°C)?

**Relación:** Parte del CU-008

**Tipo:** 🖥️ Interfaz - Display

---

#### ITE-59: HU-010 - Mostrar Estado del Climatizador

**Como** dueño de casa
**Quiero** ver el estado actual del climatizador en el display
**Para** saber si está en calefacción, refrigeración o apagado

**Preguntas de Validación:**
- ¿Se muestra un ícono claro del modo actual (🔥/❄️/⚫)?
- ¿El estado se actualiza inmediatamente al cambiar de modo?
- ¿Es fácilmente distinguible entre los tres estados?

**Relación:** Parte del CU-008

**Tipo:** 🖥️ Interfaz - Display

---

#### ITE-60: HU-011 - Mostrar Temperatura Deseada

**Como** dueño de casa
**Quiero** ver la temperatura deseada configurada en el display
**Para** saber qué temperatura estoy solicitando al sistema

**Preguntas de Validación:**
- ¿La temperatura deseada se muestra claramente diferenciada de la actual?
- ¿Se actualiza inmediatamente al cambiar la configuración?
- ¿Se muestra con 1 decimal?

**Relación:** Parte del CU-008

**Tipo:** 🖥️ Interfaz - Display

---

#### ITE-61: HU-012 - Indicar Nivel de Carga

**Como** dueño de casa
**Quiero** ver el nivel de batería en el display
**Para** saber cuándo necesito recargar

**Preguntas de Validación:**
- ¿Se muestra un ícono de batería con porcentaje?
- ¿El ícono cambia según el nivel (NORMAL/BAJO/CRITICO)?
- ¿Parpadea o alerta cuando está bajo?
- ¿Es visible en todo momento?

**Relación:** Parte del CU-008

**Tipo:** 🖥️ Interfaz - Display

---

#### ITE-62: HU-013 - Indicar no hay sensor de temperatura

**Como** dueño de casa
**Quiero** que se muestre un mensaje de error cuando el sensor de temperatura falla
**Para** saber que el sistema no puede operar correctamente

**Preguntas de Validación:**
- ¿Se muestra un mensaje de error claro cuando el sensor falla?
- ¿Se diferencia de otros errores?
- ¿Indica qué acción debe tomar el usuario?
- ¿Permanece visible hasta que se solucione?

**Relación:** Parte del CU-008 - Manejo de errores

**Tipo:** ⚠️ Interfaz - Error handling

---

## 📊 Análisis de las Historias

### Distribución por Tipo

| Tipo | Cantidad | Keys |
|------|----------|------|
| 🎯 **Principal (Flujo feliz)** | 7 | ITE-47, 51, 52, 53, 55, 56, 57 |
| 🖥️ **Interfaz (Display)** | 5 | ITE-58, 59, 60, 61, 62 |
| ⚠️ **Excepciones/Errores** | 3 | ITE-48, 49, 50 |
| 🔧 **Técnica (Algoritmo)** | 1 | ITE-54 |

### Distribución por Caso de Uso

| Caso de Uso | Historias | Keys |
|-------------|-----------|------|
| CU-001: Encender | 4 HU | ITE-47, 48, 49, 50 |
| CU-002: Apagar | 1 HU | ITE-51 |
| CU-003: Configurar Temp | 1 HU | ITE-52 |
| CU-004: Control Auto | 2 HU | ITE-53, 54 |
| CU-005: Accionar | 1 HU | ITE-55 |
| CU-006: Leer Temp | 1 HU | ITE-56 |
| CU-007: Verificar Batería | 1 HU | ITE-57 |
| CU-008: Mostrar Estado | 5 HU | ITE-58, 59, 60, 61, 62 |

### Observaciones

1. **CU-008 (Mostrar Estado)** tiene la mayor cantidad de historias (5), divididas por cada elemento del display
2. **CU-001 (Encender)** incluye 3 flujos alternativos para manejo de errores
3. **Todas las historias están en estado "Tareas por hacer"** - Listas para comenzar implementación
4. **No hay Story Points asignados** en Jira (a diferencia del documento HISTORIAS_USUARIO.md)
5. **No hay asignaciones** todavía

---

## 🔄 Comparación con HISTORIAS_USUARIO.md

### Historias Presentes en Jira pero no en mi documento:

- ITE-54: HU-016 - Compara Temperaturas (esta es técnica, yo la incluí como parte de HU-004)
- ITE-62: HU-013 - Indicar no hay sensor de temperatura (manejo de errores del display)

### Historias en mi documento pero no como HU separadas en Jira:

- Mi documento trata "Control con Histéresis" como HU-009 independiente
- En Jira esto está implícito en ITE-54 (HU-016 Compara Temperaturas)

### Diferencias en numeración:

- **Jira:** Usa HU-001 a HU-016 (con números no consecutivos)
- **Mi documento:** Usa HU-001 a HU-016 consecutivos

---

## 💡 Recomendaciones

### Para Comenzar la Implementación:

1. **Prioridad 1 - Fundamentos:**
   - ITE-56 (HU-014): Obtener temperatura ambiente
   - ITE-57 (HU-007): Obtener nivel de batería
   - ITE-58-61 (HU-008-012): Display básico

2. **Prioridad 2 - Control Básico:**
   - ITE-47 (HU-002): Encender termostato
   - ITE-51 (HU-001): Apagar termostato
   - ITE-52 (HU-006): Configurar temperatura
   - ITE-55 (HU-015): Accionar climatizador

3. **Prioridad 3 - Control Automático:**
   - ITE-53 (HU-009): Ciclo de control
   - ITE-54 (HU-016): Comparación con histéresis

4. **Prioridad 4 - Manejo de Errores:**
   - ITE-48, 49, 50 (HU-003, 004, 005): Flujos alternativos
   - ITE-62 (HU-013): Indicación de errores

### Sugerencias para Jira:

1. **Asignar Story Points** a cada historia
2. **Crear Sprints** siguiendo el plan de HISTORIAS_USUARIO.md
3. **Asignar issues** a desarrolladores
4. **Agregar labels:** `core`, `ui`, `error-handling`, `sensor`, etc.
5. **Vincular issues** con sus Epics correspondientes

---

**Documento generado desde:** Jira API
**Fecha:** 2025-11-09
**Issues totales:** 24 (8 Epics + 16 Historias)
