# Historias de Usuario - ISSE Termostato

**Proyecto:** isse_termostato_c
**Fecha:** 2025-11-09
**Versión:** 1.0

---

## 📋 Índice

- [Resumen Ejecutivo](#resumen-ejecutivo)
- [Paquete 1: Manipular Termostato (Usuario)](#paquete-1-manipular-termostato-usuario)
- [Paquete 2: Controlar Termostato (Sistema Automático)](#paquete-2-controlar-termostato-sistema-automático)
- [Paquete 3: Gestión de Energía](#paquete-3-gestión-de-energía)
- [Paquete 4: Observabilidad y Monitoreo](#paquete-4-observabilidad-y-monitoreo)
- [Matriz de Trazabilidad HU ↔ CU](#matriz-de-trazabilidad-hu--cu)

---

## 📊 Resumen Ejecutivo

### Total de Historias de Usuario: 15

| Paquete | Cantidad | Estado |
|---------|----------|--------|
| **Manipular Termostato (Usuario)** | 3 | 📝 Por implementar |
| **Controlar Termostato (Sistema)** | 5 | 📝 Por implementar |
| **Gestión de Energía** | 3 | 📝 Por implementar |
| **Observabilidad y Monitoreo** | 4 | 📝 Por implementar |

### Priorización

- **Prioridad Alta (MUST HAVE):** 8 HU - Funcionalidad core del termostato
- **Prioridad Media (SHOULD HAVE):** 5 HU - Mejoras y gestión de energía
- **Prioridad Baja (COULD HAVE):** 2 HU - Observabilidad avanzada

---

## 📦 Paquete 1: Manipular Termostato (Usuario)

### HU-001: Encender el Termostato

**Como** usuario del termostato
**Quiero** encender el dispositivo presionando el botón de encendido
**Para** comenzar a controlar la temperatura de mi hogar

#### Criterios de Aceptación

- [ ] Al presionar el botón de encendido, el termostato se enciende
- [ ] El display muestra la temperatura actual del ambiente
- [ ] El sistema carga la última temperatura deseada configurada (o 22°C por defecto)
- [ ] El sistema inicia el ciclo de control automático
- [ ] El tiempo de encendido es menor a 2 segundos
- [ ] Se registra el evento de encendido en el log de auditoría

#### Detalles Técnicos

- **Caso de Uso:** CU-001 - Encender Termostato
- **Prioridad:** Alta (MUST HAVE)
- **Estimación:** 5 Story Points
- **Epic:** Funcionalidad Básica del Termostato
- **Labels:** `user-interaction`, `core-feature`, `power-management`

#### Dependencias

- Sistema de logging funcional
- Entidad Termostato con máquina de estados
- Capa de infraestructura para botón físico

#### Notas de Implementación

```c
// Máquina de estados: APAGADO → ENCENDIENDO → OPERANDO
estado_termostato_transicion(termostato, EVENTO_BOTON_ENCENDIDO);
```

---

### HU-002: Apagar el Termostato

**Como** usuario del termostato
**Quiero** apagar el dispositivo presionando el botón de encendido
**Para** detener el control de temperatura y ahorrar energía

#### Criterios de Aceptación

- [ ] Al presionar el botón de encendido (estando encendido), el termostato se apaga
- [ ] El sistema detiene el ciclo de control automático
- [ ] El climatizador se apaga antes de apagar el termostato
- [ ] Se guarda la última temperatura deseada para el próximo encendido
- [ ] El display se apaga o muestra mensaje "OFF"
- [ ] El tiempo de apagado completo es menor a 3 segundos
- [ ] Se registra el evento de apagado en el log de auditoría

#### Detalles Técnicos

- **Caso de Uso:** CU-002 - Apagar Termostato
- **Prioridad:** Alta (MUST HAVE)
- **Estimación:** 5 Story Points
- **Epic:** Funcionalidad Básica del Termostato
- **Labels:** `user-interaction`, `core-feature`, `power-management`

#### Dependencias

- HU-001 (Encender termostato)
- Sistema de persistencia de configuración
- Control del climatizador

#### Notas de Implementación

```c
// Secuencia de apagado seguro
1. Detener ciclo de control
2. Apagar climatizador
3. Guardar configuración
4. Transición a estado APAGADO
```

---

### HU-003: Establecer Temperatura Deseada

**Como** usuario del termostato
**Quiero** establecer la temperatura deseada usando botones arriba/abajo
**Para** ajustar el nivel de confort térmico según mis preferencias

#### Criterios de Aceptación

- [ ] El usuario puede incrementar la temperatura presionando botón "arriba" (+)
- [ ] El usuario puede decrementar la temperatura presionando botón "abajo" (-)
- [ ] La temperatura se ajusta en incrementos de 0.5°C
- [ ] El rango válido es de 10°C a 35°C
- [ ] Al alcanzar los límites, el sistema no permite más cambios en esa dirección
- [ ] El display muestra la nueva temperatura deseada inmediatamente
- [ ] El sistema ajusta el control para alcanzar la nueva temperatura
- [ ] Los cambios se guardan automáticamente
- [ ] Feedback visual cuando se alcanza un límite

#### Detalles Técnicos

- **Caso de Uso:** CU-003 - Establecer Temperatura Deseada
- **Prioridad:** Alta (MUST HAVE)
- **Estimación:** 8 Story Points
- **Epic:** Funcionalidad Básica del Termostato
- **Labels:** `user-interaction`, `core-feature`, `temperature-control`

#### Dependencias

- HU-001 (Termostato debe estar encendido)
- Sistema de validación de temperatura
- Interfaz de usuario (botones + display)

#### Notas de Implementación

```c
// Constantes de configuración
#define TEMP_MIN 10.0f
#define TEMP_MAX 35.0f
#define TEMP_STEP 0.5f

// Validación
if (nueva_temp >= TEMP_MIN && nueva_temp <= TEMP_MAX) {
    termostato_establecer_temperatura_deseada(termostato, nueva_temp);
}
```

---

## 📦 Paquete 2: Controlar Termostato (Sistema Automático)

### HU-004: Control Automático de Temperatura

**Como** sistema termostato
**Quiero** ejecutar un ciclo de control automático cada 100ms
**Para** mantener la temperatura ambiente en el nivel deseado

#### Criterios de Aceptación

- [ ] El sistema ejecuta el ciclo de control cada 100ms (±10ms de tolerancia)
- [ ] En cada ciclo se lee la temperatura ambiente actual
- [ ] Se compara la temperatura actual con la temperatura deseada
- [ ] Se decide si activar calefacción, refrigeración o mantener apagado
- [ ] Se verifica el nivel de batería antes de cada acción
- [ ] Se actualiza el display con la información actual
- [ ] El sistema implementa histéresis de 0.5°C para evitar oscilaciones
- [ ] Si la batería está crítica, el sistema entra en modo de ahorro

#### Detalles Técnicos

- **Caso de Uso:** CU-004 - Controlar Termostato
- **Prioridad:** Alta (MUST HAVE)
- **Estimación:** 13 Story Points
- **Epic:** Control Automático
- **Labels:** `core-feature`, `automation`, `real-time`, `control-loop`

#### Dependencias

- HU-006 (Lectura de temperatura)
- HU-005 (Control del climatizador)
- HU-007 (Verificación de batería)
- HU-008 (Actualización del display)

#### Atributos de Calidad

- **Performance:** Ciclo < 100ms
- **Determinismo:** Ejecución periódica garantizada
- **Confiabilidad:** Sin pérdida de ciclos

#### Notas de Implementación

```c
// Pseudo-código del ciclo principal
while (termostato_esta_encendido()) {
    inicio_ciclo = obtener_tiempo();

    temp_actual = obtener_temperatura_ambiente();
    temp_deseada = termostato_obtener_temperatura_deseada();

    modo = calcular_modo_climatizador(temp_actual, temp_deseada);
    accionar_climatizador(modo);

    actualizar_display();

    esperar_hasta_completar_100ms(inicio_ciclo);
}
```

---

### HU-005: Accionar Climatizador

**Como** sistema termostato
**Quiero** controlar el climatizador (calefacción/refrigeración)
**Para** ajustar la temperatura ambiente según sea necesario

#### Criterios de Aceptación

- [ ] El sistema puede encender el climatizador en modo CALEFACCION
- [ ] El sistema puede encender el climatizador en modo REFRIGERACION
- [ ] El sistema puede apagar el climatizador (modo APAGADO)
- [ ] No se permite cambiar directamente de CALEFACCION a REFRIGERACION sin pasar por APAGADO
- [ ] El cambio de modo tiene un retardo de seguridad de 5 segundos
- [ ] Se registra cada cambio de estado del climatizador
- [ ] Si hay error en el climatizador, se notifica al usuario

#### Detalles Técnicos

- **Caso de Uso:** CU-005 - Accionar Climatizador
- **Prioridad:** Alta (MUST HAVE)
- **Estimación:** 8 Story Points
- **Epic:** Control Automático
- **Labels:** `core-feature`, `actuator`, `hvac`

#### Dependencias

- Entidad Climatizador con máquina de estados
- Driver de actuador (simulado o real)

#### Máquina de Estados

```
APAGADO ←→ CALEFACCION
APAGADO ←→ REFRIGERACION

Transición CALEFACCION ↔ REFRIGERACION requiere:
1. CALEFACCION → APAGADO (esperar 5s)
2. APAGADO → REFRIGERACION
```

#### Notas de Implementación

```c
// Estrategia de control con histéresis
if (temp_actual < temp_deseada - HISTERESIS) {
    modo_requerido = CALEFACCION;
} else if (temp_actual > temp_deseada + HISTERESIS) {
    modo_requerido = REFRIGERACION;
} else {
    modo_requerido = APAGADO;
}
```

---

### HU-006: Obtener Temperatura Ambiente

**Como** sistema termostato
**Quiero** leer la temperatura ambiente del sensor cada ciclo de control
**Para** tomar decisiones basadas en la temperatura real

#### Criterios de Aceptación

- [ ] El sistema lee la temperatura del sensor en cada ciclo (cada 100ms)
- [ ] El tiempo de lectura es menor a 50ms
- [ ] Se valida que la temperatura esté en un rango físicamente posible (-20°C a 60°C)
- [ ] Si la lectura falla, se reintenta hasta 3 veces
- [ ] Si todas las lecturas fallan, se usa el último valor válido y se notifica error
- [ ] La temperatura se filtra para eliminar ruido (promedio móvil de 5 muestras)
- [ ] La precisión de lectura es de ±0.5°C

#### Detalles Técnicos

- **Caso de Uso:** CU-006 - Obtener Temperatura Ambiente
- **Prioridad:** Alta (MUST HAVE)
- **Estimación:** 5 Story Points
- **Epic:** Sensores y Adquisición de Datos
- **Labels:** `sensor`, `input`, `data-acquisition`

#### Dependencias

- HAL ADC configurado
- Driver de sensor de temperatura
- Proxy de sensor en capa de infraestructura

#### Atributos de Calidad

- **Performance:** Latencia < 50ms
- **Confiabilidad:** Manejo de fallos de sensor
- **Precisión:** ±0.5°C

#### Notas de Implementación

```c
// Lectura con validación y reintentos
float temperatura = TEMP_INVALIDA;
for (int intento = 0; intento < 3; intento++) {
    temperatura = sensor_leer_temperatura();
    if (temperatura >= TEMP_FISICA_MIN && temperatura <= TEMP_FISICA_MAX) {
        break; // Lectura válida
    }
}

if (temperatura == TEMP_INVALIDA) {
    temperatura = ultima_temperatura_valida;
    logger_error("Fallo de sensor de temperatura");
}

// Filtrado
temperatura_filtrada = filtro_promedio_movil(temperatura);
```

---

### HU-007: Verificar Nivel de Batería

**Como** sistema termostato
**Quiero** verificar el nivel de carga de la batería periódicamente
**Para** alertar al usuario y prevenir apagado inesperado

#### Criterios de Aceptación

- [ ] El sistema verifica el nivel de batería cada 10 segundos
- [ ] Se detectan tres niveles: NORMAL (>20%), BAJO (10-20%), CRITICO (<10%)
- [ ] Cuando la batería está BAJA, se muestra ícono parpadeante en display
- [ ] Cuando la batería está CRITICA, el sistema entra en modo de ahorro de energía
- [ ] En modo de ahorro, el display se atenúa y el ciclo de control se reduce a 500ms
- [ ] Si la batería llega a 5%, el sistema se apaga automáticamente de forma segura
- [ ] Se registra cada cambio de nivel de batería

#### Detalles Técnicos

- **Caso de Uso:** CU-007 - Verificar Nivel de Carga de Batería
- **Prioridad:** Alta (MUST HAVE)
- **Estimación:** 8 Story Points
- **Epic:** Gestión de Energía
- **Labels:** `power-management`, `battery`, `energy-efficiency`

#### Dependencias

- HAL ADC para lectura de batería
- Entidad Batería
- Sistema de notificaciones al usuario

#### Atributos de Calidad

- **Eficiencia Energética:** Detección temprana de batería baja
- **Disponibilidad:** Operación continua con batería baja
- **Confiabilidad:** Apagado seguro antes de agotamiento total

#### Notas de Implementación

```c
// Umbrales de batería
#define BATERIA_NORMAL 20.0f
#define BATERIA_BAJO 10.0f
#define BATERIA_CRITICO 5.0f

// Estrategia de ahorro de energía
if (nivel_bateria <= BATERIA_CRITICO) {
    sistema_apagar_seguro();
} else if (nivel_bateria <= BATERIA_BAJO) {
    sistema_modo_ahorro_energia();
}
```

---

### HU-008: Mostrar Estado del Termostato

**Como** usuario del termostato
**Quiero** ver en el display la temperatura actual, deseada y estado del sistema
**Para** conocer el estado de funcionamiento en todo momento

#### Criterios de Aceptación

- [ ] El display muestra la temperatura actual con 1 decimal (ej: 21.5°C)
- [ ] Se muestra la temperatura deseada configurada
- [ ] Se muestra el modo actual del climatizador (calor/frío/apagado) con ícono
- [ ] Se muestra el nivel de batería con ícono
- [ ] El display se actualiza cada ciclo de control (100ms) sin parpadeos
- [ ] En caso de error, se muestra mensaje de error claro
- [ ] El brillo del display se ajusta según nivel de batería

#### Detalles Técnicos

- **Caso de Uso:** CU-008 - Mostrar Estado del Termostato
- **Prioridad:** Media (SHOULD HAVE)
- **Estimación:** 5 Story Points
- **Epic:** Interfaz de Usuario
- **Labels:** `display`, `ui`, `user-feedback`

#### Dependencias

- HAL para control de display
- Estado de todas las entidades del sistema

#### Diseño del Display

```
┌─────────────────────┐
│ 🔋 80%   [🔥]/[❄️] │  ← Batería y modo
│                     │
│  ACTUAL: 21.5°C     │  ← Temperatura actual
│  DESEADA: 22.0°C    │  ← Temperatura deseada
│                     │
│  ▮▮▮▮▮▮▮▯▯▯ 70%    │  ← Barra de progreso (opcional)
└─────────────────────┘
```

---

### HU-009: Ciclo de Control con Histéresis

**Como** sistema termostato
**Quiero** implementar control con histéresis de ±0.5°C
**Para** evitar oscilaciones continuas del climatizador (relay cycling)

#### Criterios de Aceptación

- [ ] El climatizador no se enciende/apaga continuamente
- [ ] Banda de histéresis configurada en ±0.5°C
- [ ] Si temp actual < temp_deseada - 0.5°C → Activar calefacción
- [ ] Si temp actual > temp_deseada + 0.5°C → Activar refrigeración
- [ ] Dentro de la banda de histéresis → Mantener estado actual
- [ ] El número de conmutaciones del climatizador es menor a 10 por hora
- [ ] El sistema mantiene la temperatura dentro del rango deseado ±1°C el 95% del tiempo

#### Detalles Técnicos

- **Caso de Uso:** CU-004 - Controlar Termostato (refinamiento)
- **Prioridad:** Media (SHOULD HAVE)
- **Estimación:** 5 Story Points
- **Epic:** Control Automático
- **Labels:** `algorithm`, `control-theory`, `optimization`

#### Dependencias

- HU-004 (Control automático básico)

#### Atributos de Calidad

- **Confiabilidad:** Vida útil extendida del relay
- **Eficiencia Energética:** Menos conmutaciones = menos consumo
- **Performance:** Control estable sin oscilaciones

#### Notas de Implementación

```c
// Control con histéresis
#define HISTERESIS 0.5f

float error = temp_deseada - temp_actual;

if (error > HISTERESIS) {
    // Hace frío, necesita calefacción
    modo = CALEFACCION;
} else if (error < -HISTERESIS) {
    // Hace calor, necesita refrigeración
    modo = REFRIGERACION;
} else {
    // Dentro de banda muerta, mantener estado
    modo = climatizador_obtener_modo_actual();
}
```

---

## 📦 Paquete 3: Gestión de Energía

### HU-010: Modo de Ahorro de Energía Automático

**Como** sistema termostato
**Quiero** entrar automáticamente en modo de ahorro cuando la batería esté baja
**Para** maximizar el tiempo de operación con batería limitada

#### Criterios de Aceptación

- [ ] Cuando batería < 20%, el sistema activa modo de ahorro automáticamente
- [ ] En modo ahorro, el ciclo de control se reduce a 500ms (en lugar de 100ms)
- [ ] El brillo del display se reduce al 50%
- [ ] Se deshabilitan funciones no críticas (logging extendido)
- [ ] Se muestra ícono de "modo ahorro" en el display
- [ ] El usuario puede desactivar temporalmente el modo ahorro (15 minutos)
- [ ] Al conectar alimentación externa, el modo ahorro se desactiva

#### Detalles Técnicos

- **Prioridad:** Media (SHOULD HAVE)
- **Estimación:** 8 Story Points
- **Epic:** Gestión de Energía
- **Labels:** `energy-efficiency`, `battery`, `power-management`

#### Dependencias

- HU-007 (Verificación de batería)
- Sistema de configuración

#### Atributos de Calidad

- **Eficiencia Energética:** Extender vida de batería 50%
- **Disponibilidad:** Mantener funcionalidad core con batería baja

---

### HU-011: Apagado Seguro por Batería Crítica

**Como** sistema termostato
**Quiero** apagar el sistema de forma segura cuando la batería esté crítica
**Para** evitar corrupción de datos y daño al hardware

#### Criterios de Aceptación

- [ ] Cuando batería < 5%, se inicia secuencia de apagado automático
- [ ] El usuario recibe notificación visual 30 segundos antes del apagado
- [ ] Se detiene el ciclo de control
- [ ] Se apaga el climatizador de forma segura
- [ ] Se guarda el estado actual (temperatura deseada, configuración)
- [ ] Se genera log de apagado por batería crítica
- [ ] El display muestra "BATERIA AGOTADA - APAGANDO"
- [ ] El apagado completo ocurre en menos de 5 segundos

#### Detalles Técnicos

- **Prioridad:** Alta (MUST HAVE)
- **Estimación:** 8 Story Points
- **Epic:** Gestión de Energía
- **Labels:** `power-management`, `safety`, `data-integrity`

#### Dependencias

- HU-007 (Verificación de batería)
- Sistema de persistencia
- HU-002 (Apagado normal)

#### Atributos de Calidad

- **Confiabilidad:** No pérdida de datos
- **Seguridad:** Protección del hardware

#### Notas de Implementación

```c
// Secuencia de apagado seguro
void sistema_apagado_seguro_bateria() {
    display_mostrar("BATERIA BAJA - APAGANDO");

    // 1. Detener ciclo de control
    detener_ciclo_control();

    // 2. Apagar climatizador
    climatizador_apagar();

    // 3. Guardar estado
    configuracion_guardar(estado_actual);

    // 4. Log de auditoría
    logger_warning("Apagado automático por batería crítica");

    // 5. Apagar display
    display_apagar();

    // 6. Modo ultra bajo consumo
    hal_modo_bajo_consumo();
}
```

---

### HU-012: Estimación de Tiempo de Batería Restante

**Como** usuario del termostato
**Quiero** ver una estimación del tiempo de batería restante
**Para** planificar cuándo necesito recargar o reemplazar la batería

#### Criterios de Aceptación

- [ ] El sistema calcula el tiempo estimado de batería restante en horas
- [ ] El cálculo se basa en el consumo promedio de las últimas 24 horas
- [ ] Se muestra en el display: "BATERIA: XX horas restantes"
- [ ] La estimación se actualiza cada 10 minutos
- [ ] Si la batería es superior al 80%, se muestra "> 24h"
- [ ] El margen de error es ±20%

#### Detalles Técnicos

- **Prioridad:** Baja (COULD HAVE)
- **Estimación:** 5 Story Points
- **Epic:** Gestión de Energía
- **Labels:** `battery`, `ux`, `analytics`

#### Dependencias

- HU-007 (Monitoreo de batería)
- Sistema de logging de consumo

---

## 📦 Paquete 4: Observabilidad y Monitoreo

### HU-013: Registro de Eventos del Sistema (Logging)

**Como** desarrollador/mantenedor del sistema
**Quiero** que el sistema registre eventos importantes en un log
**Para** diagnosticar problemas y analizar el comportamiento

#### Criterios de Aceptación

- [ ] Se registran eventos con timestamp, nivel y mensaje
- [ ] Niveles de log: DEBUG, INFO, WARNING, ERROR, CRITICAL
- [ ] Se registran: encendido/apagado, cambios de temperatura, errores de sensores
- [ ] Los logs se pueden enviar a stdout (simulador) o UART (hardware)
- [ ] El nivel de log es configurable (por defecto: INFO)
- [ ] Los logs no impactan el tiempo del ciclo de control (< 1ms por log)
- [ ] En modo producción, DEBUG está deshabilitado

#### Detalles Técnicos

- **Prioridad:** Media (SHOULD HAVE)
- **Estimación:** 8 Story Points
- **Epic:** Aspectos Transversales
- **Labels:** `logging`, `observability`, `debugging`

#### Dependencias

- Aspecto transversal: `src/aspectos/logging/`

#### Ejemplo de Log

```
[2025-11-09 15:30:00.123] [INFO] Termostato encendido
[2025-11-09 15:30:00.150] [INFO] Temperatura actual: 20.5°C, Deseada: 22.0°C
[2025-11-09 15:30:05.230] [INFO] Climatizador activado: CALEFACCION
[2025-11-09 15:45:00.500] [WARNING] Batería baja: 18%
[2025-11-09 16:00:00.012] [ERROR] Fallo de lectura del sensor (reintentando)
```

---

### HU-014: Auditoría de Acciones del Usuario

**Como** administrador del sistema
**Quiero** que se registren todas las acciones del usuario
**Para** tener trazabilidad de cambios de configuración

#### Criterios de Aceptación

- [ ] Se registra cada acción del usuario: encendido, apagado, cambio de temperatura
- [ ] Cada registro incluye: timestamp, acción, valor anterior, valor nuevo
- [ ] Los registros de auditoría son persistentes (sobreviven al apagado)
- [ ] Se pueden exportar/consultar para análisis
- [ ] Los registros no se pueden modificar (append-only)

#### Detalles Técnicos

- **Prioridad:** Media (SHOULD HAVE)
- **Estimación:** 8 Story Points
- **Epic:** Aspectos Transversales
- **Labels:** `audit`, `security`, `compliance`

#### Dependencias

- Aspecto transversal: `src/aspectos/auditoria/`
- Sistema de persistencia

---

### HU-015: Reporte de Estado del Sistema

**Como** desarrollador/mantenedor
**Quiero** poder obtener un reporte completo del estado del sistema
**Para** diagnóstico remoto sin acceso físico al dispositivo

#### Criterios de Aceptación

- [ ] Se puede solicitar un reporte de estado (por UART o interfaz)
- [ ] El reporte incluye:
  - Versión del firmware
  - Tiempo de operación (uptime)
  - Temperatura actual y deseada
  - Estado del climatizador
  - Nivel de batería
  - Estadísticas: ciclos ejecutados, errores, conmutaciones
- [ ] El reporte se genera en formato JSON o texto plano
- [ ] La generación del reporte no interrumpe la operación normal

#### Detalles Técnicos

- **Prioridad:** Baja (COULD HAVE)
- **Estimación:** 5 Story Points
- **Epic:** Observabilidad y Monitoreo
- **Labels:** `observability`, `diagnostics`, `remote-support`

#### Ejemplo de Reporte

```json
{
  "version": "1.0.0",
  "uptime_seconds": 86400,
  "temperatura": {
    "actual": 21.5,
    "deseada": 22.0,
    "sensor_ok": true
  },
  "climatizador": {
    "modo": "CALEFACCION",
    "conmutaciones_total": 42
  },
  "bateria": {
    "nivel_porcentaje": 75.0,
    "estado": "NORMAL"
  },
  "estadisticas": {
    "ciclos_ejecutados": 864000,
    "errores_sensor": 3,
    "tiempo_en_calefaccion_segundos": 32400
  }
}
```

---

### HU-016: Monitoreo de Performance del Sistema

**Como** desarrollador
**Quiero** monitorear las métricas de performance del sistema en tiempo real
**Para** validar que se cumplen los atributos de calidad

#### Criterios de Aceptación

- [ ] Se mide el tiempo de ejecución de cada ciclo de control
- [ ] Se detecta si un ciclo excede los 100ms (violación de requisito)
- [ ] Se mide el tiempo de lectura del sensor
- [ ] Se cuenta el número de ciclos perdidos (si los hay)
- [ ] Las métricas se pueden consultar via comando
- [ ] Se genera alerta si performance degrada

#### Detalles Técnicos

- **Prioridad:** Baja (COULD HAVE)
- **Estimación:** 5 Story Points
- **Epic:** Observabilidad y Monitoreo
- **Labels:** `performance`, `monitoring`, `quality-attributes`

#### Dependencias

- Sistema de timing preciso (HAL timer)
- Sistema de logging

---

## 📊 Matriz de Trazabilidad HU ↔ CU

| Historia de Usuario | Caso de Uso | Prioridad | Story Points |
|---------------------|-------------|-----------|--------------|
| **HU-001:** Encender Termostato | CU-001 | Alta | 5 |
| **HU-002:** Apagar Termostato | CU-002 | Alta | 5 |
| **HU-003:** Establecer Temperatura Deseada | CU-003 | Alta | 8 |
| **HU-004:** Control Automático de Temperatura | CU-004 | Alta | 13 |
| **HU-005:** Accionar Climatizador | CU-005 | Alta | 8 |
| **HU-006:** Obtener Temperatura Ambiente | CU-006 | Alta | 5 |
| **HU-007:** Verificar Nivel de Batería | CU-007 | Alta | 8 |
| **HU-008:** Mostrar Estado del Termostato | CU-008 | Media | 5 |
| **HU-009:** Control con Histéresis | CU-004 (refinamiento) | Media | 5 |
| **HU-010:** Modo Ahorro de Energía | - | Media | 8 |
| **HU-011:** Apagado Seguro por Batería | - | Alta | 8 |
| **HU-012:** Estimación Tiempo de Batería | - | Baja | 5 |
| **HU-013:** Logging de Eventos | - | Media | 8 |
| **HU-014:** Auditoría de Acciones | - | Media | 8 |
| **HU-015:** Reporte de Estado | - | Baja | 5 |
| **HU-016:** Monitoreo de Performance | - | Baja | 5 |

### Totales por Prioridad

- **Alta (MUST HAVE):** 8 HU - 60 Story Points
- **Media (SHOULD HAVE):** 5 HU - 39 Story Points
- **Baja (COULD HAVE):** 3 HU - 15 Story Points

**Total:** 16 HU - 114 Story Points

---

## 🎯 Plan de Iteraciones Sugerido

### Sprint 1: Fundamentos (2 semanas) - 30 SP

- HU-001: Encender Termostato (5 SP)
- HU-002: Apagar Termostato (5 SP)
- HU-006: Obtener Temperatura Ambiente (5 SP)
- HU-007: Verificar Nivel de Batería (8 SP)
- HU-013: Logging de Eventos (8 SP) - Para debugging de los demás

### Sprint 2: Control Básico (2 semanas) - 26 SP

- HU-003: Establecer Temperatura Deseada (8 SP)
- HU-005: Accionar Climatizador (8 SP)
- HU-008: Mostrar Estado (5 SP)
- HU-009: Control con Histéresis (5 SP)

### Sprint 3: Control Automático (2 semanas) - 21 SP

- HU-004: Control Automático de Temperatura (13 SP)
- HU-014: Auditoría de Acciones (8 SP)

### Sprint 4: Gestión de Energía (2 semanas) - 21 SP

- HU-010: Modo Ahorro de Energía (8 SP)
- HU-011: Apagado Seguro por Batería (8 SP)
- HU-012: Estimación Tiempo de Batería (5 SP)

### Sprint 5: Observabilidad (1 semana) - 10 SP

- HU-015: Reporte de Estado (5 SP)
- HU-016: Monitoreo de Performance (5 SP)

**Duración Total Estimada:** 9 semanas (2.25 meses)

---

## 📝 Notas para Implementación en Jira

### Creación de Issues

Para cada Historia de Usuario, crear un issue tipo **Story** en Jira:

```
Summary: [HU-XXX] Título de la Historia
Description:
  Como [rol]
  Quiero [funcionalidad]
  Para [beneficio]

  Criterios de Aceptación:
  - [ ] Criterio 1
  - [ ] Criterio 2
  ...

Story Points: X
Priority: High/Medium/Low
Epic Link: Epic correspondiente
Labels: labels, separados, por, comas
Components: Capa correspondiente (Dominio/Aplicación/Infraestructura)
```

### Epics Sugeridos

1. **Funcionalidad Básica del Termostato** (HU-001, HU-002, HU-003)
2. **Control Automático** (HU-004, HU-005, HU-009)
3. **Sensores y Adquisición** (HU-006)
4. **Gestión de Energía** (HU-007, HU-010, HU-011, HU-012)
5. **Interfaz de Usuario** (HU-008)
6. **Aspectos Transversales** (HU-013, HU-014)
7. **Observabilidad** (HU-015, HU-016)

---

**Documento generado:** 2025-11-09
**Versión:** 1.0
**Estado:** Listo para carga en Jira
