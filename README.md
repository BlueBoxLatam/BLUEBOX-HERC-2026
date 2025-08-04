# Blue Box Rover - NASA Human Exploration Rover Challenge

![Logo del Equipo Blue Box](URL_A_TU_LOGO)

## Descripción del Proyecto

Este repositorio contiene todo el firmware, documentación y diseños de hardware para el vehículo rover desarrollado por el equipo **Blue Box** para la competencia **Human Exploration Rover Challenge (HERC)** de la NASA.

El objetivo es diseñar y construir un rover de propulsión humana capaz de superar un circuito de obstáculos y completar una serie de misiones.

---

## Arquitectura del Firmware

El control del rover se distribuye en dos microcontroladores **ESP32** que se comunican mediante el protocolo **ESP-NOW** para garantizar una respuesta rápida y sin necesidad de una red Wi-Fi externa.

- **ESP32 Maestro (Controlador):** Se encarga de la lógica principal, leer los comandos del piloto y enviar instrucciones.
- **ESP32 Esclavo (Actuadores):** Dedicado exclusivamente a recibir instrucciones y gestionar en tiempo real los 6 motores N20 y 4 servomotores SG90 a través de 3 puentes H TB6612FNG.

---

## Estructura del Repositorio

- **/01_firmware/:** Contiene todo el código fuente para los microcontroladores.
  - **main/:** Código final y estable del proyecto.
  - **prototypes/:** Pruebas de concepto y validación de componentes individuales.
- **/02_documentation/:** Documentación relevante, hojas de datos (datasheets) y reglas de la competencia.
- **/03_hardware/:** Diseños de PCB y modelos 3D para componentes físicos del rover.

---

## Equipo

- **Equipo:** Blue Box
- **Miembros:** [Lista de miembros]

## 00 - Emisor Maestro ESP-NOW

Este firmware configura un ESP32 como un **dispositivo emisor (maestro)** utilizando el protocolo de comunicación inalámbrica **ESP-NOW**.

### Funcionalidad Principal

El código inicializa el ESP32 y espera la entrada de comandos a través del Monitor Serie (baudrate: 115200). Al recibir un número del '1' al '6', empaqueta un comando en una estructura de datos y lo transmite de forma inalámbrica al ESP32 receptor (esclavo) predefinido.

- **Comandos '1', '2', '3'**: Activan los actuadores correspondientes (ID 1, 2, 3).
- **Comandos '4', '5', '6'**: Desactivan los actuadores correspondientes (ID 1, 2, 3).

El estado de cada envío (éxito o fallo) se reporta de forma asíncrona en el Monitor Serie. Este script es una pieza clave para el sistema de control del Rover, actuando como el cerebro que envía órdenes a otros módulos.

## 01 - Receptor Esclavo ESP-NOW

Este firmware configura un ESP32 como un **dispositivo receptor (esclavo)** que actúa como un módulo de actuadores, respondiendo a comandos recibidos vía **ESP-NOW**.

### Funcionalidad Principal

El código inicializa el ESP32 en modo receptor y espera pasivamente los mensajes del controlador maestro. Su principal función es:

1.  **Mostrar su Dirección MAC**: Al arrancar, imprime su dirección MAC en el Monitor Serie para facilitar la configuración del emisor.
2.  **Recibir Comandos**: Escucha continuamente los mensajes enviados por el emisor maestro.
3.  **Controlar LEDs**: Al recibir un mensaje, decodifica el `ID` del actuador y su `estado` (ON/OFF) para encender o apagar el LED correspondiente conectado a los pines GPIO.

Este script está diseñado para ser altamente eficiente, ya que toda la lógica de control se maneja a través de interrupciones (`callback`), dejando el bucle principal (`loop`) libre.

## 02 - Utilidad para Obtener MAC (Bajo Nivel)

Este es un script de **diagnóstico y utilidad** diseñado para un propósito específico: obtener y mostrar la dirección MAC de un ESP32 utilizando funciones de bajo nivel del **ESP-IDF** (el framework de desarrollo de Espressif).

### Funcionalidad Principal

A diferencia del método estándar que utiliza la librería `WiFi.h` de Arduino, este código llama directamente a la función `esp_efuse_mac_get_default()`. Su objetivo es verificar la capacidad del hardware para reportar su MAC sin pasar por la capa de abstracción de Arduino.

Este enfoque es ideal para **solucionar problemas (troubleshooting)**. Si un ESP32 no muestra su MAC con los métodos habituales, este script ayuda a determinar si el problema reside en el hardware/firmware base o en la configuración del entorno de Arduino.

## 03 - Diagnóstico de Conexión WiFi

Este script es una herramienta de **diagnóstico** simple pero crucial. Su única función es intentar conectar el ESP32 a una red WiFi utilizando las credenciales (SSID y contraseña) especificadas en el código.

### Funcionalidad Principal

Al ejecutarse, el programa inicia el módulo WiFi y trata de establecer una conexión. El progreso se muestra en el Monitor Serie con una serie de puntos.

- **Si la conexión es exitosa**, el programa imprime un mensaje de confirmación y la **dirección IP** que le fue asignada por el router.
- **Si la conexión falla**, informa del error y muestra un código de estado para ayudar a identificar la causa (ej: credenciales incorrectas, señal débil, etc.).

Este código es el paso fundamental para verificar la funcionalidad de la radio WiFi del ESP32 antes de implementar aplicaciones más complejas como ESP-NOW o clientes/servidores web.

## 04 - Diagnóstico de Escaneo WiFi

Este es un script de **diagnóstico** diseñado para verificar el funcionamiento básico del hardware de radio del ESP32. Su única tarea es poner el dispositivo en modo `WIFI_STA` (Estación) y realizar un escaneo continuo de las redes WiFi cercanas.

### Funcionalidad Principal

El programa inicializa el módulo WiFi y, en un bucle infinito, busca redes en el espectro de 2.4 GHz. Los resultados se imprimen en el Monitor Serie.

- **Si encuentra redes**: Lista el nombre (SSID) de cada una, junto con su potencia de señal (RSSI) y si es una red abierta o segura.
- **Si no encuentra redes**: Muestra un mensaje indicando que el escaneo no tuvo éxito.

Un escaneo exitoso es una fuerte indicación de que el hardware de radio del ESP32 está alimentado y funcionando correctamente, lo que permite centrar la depuración de problemas de conectividad en otras áreas como las credenciales de red o la configuración del software.

## 05 - Test para Driver de Motores TB6612FNG (con analogWrite)

Este script es un código de prueba para controlar dos motores de corriente continua (DC) a través de un driver **TB6612FNG** conectado a un ESP32.

### Funcionalidad Principal

El programa ejecuta una secuencia predefinida para verificar el correcto funcionamiento tanto del hardware (motores, driver, conexiones) como del software. La secuencia es la siguiente:

1.  Mueve el Motor A hacia adelante.
2.  Mueve el Motor B hacia adelante.
3.  Mueve el Motor A hacia atrás.
4.  Mueve el Motor B hacia atrás.

Cada movimiento dura 3 segundos y es seguido por una breve pausa. Al final del ciclo completo, hay una pausa más larga antes de que la secuencia se repita.

### Característica Clave: `analogWrite`

Esta versión del código utiliza la función estándar de Arduino `analogWrite()` para generar las señales PWM que controlan la velocidad de los motores. Este método es una **alternativa más compatible** al uso directo del periférico `LEDC` del ESP32, evitando posibles errores de compilación en algunas configuraciones del IDE de Arduino.

## 06 - Controlador de Rover WiFi (Modo Cliente)

Este firmware transforma un ESP32 en el cerebro de un robot móvil completo, controlado remotamente a través de una conexión WiFi. El ESP32 actúa como un **cliente**, conectándose a un servidor (como una aplicación en un teléfono móvil) para recibir órdenes y enviar telemetría.

### Funcionalidad y Características

El código gestiona múltiples periféricos y ejecuta una lógica de control basada en un protocolo de comunicación simple:

- **Conectividad WiFi**: Se conecta a una red WiFi y a un host con IP y puerto predefinidos. Incluye lógica de reconexión automática.
- **Control de Movimiento**: Implementa funciones para mover el robot hacia adelante, atrás, girar a la izquierda, derecha y detenerse por completo (control tipo tanque).
- **Control de Servos**: Maneja dos servomotores independientes, permitiendo su posicionamiento en grados.
- **Actuadores Adicionales**: Controla dos LEDs que pueden ser usados como luces.
- **Sensor de Distancia**: Utiliza un sensor ultrasónico HC-SR04 para medir continuamente la distancia al objeto más cercano y la reporta de vuelta al host.

### Protocolo de Comunicación

El control se realiza mediante comandos de un solo carácter enviados desde el host al ESP32:

- `F`, `B`, `L`, `R`, `S`: Comandos de movimiento.
- `M`/`m`, `N`/`n`: Comandos para encender/apagar los LEDs.
- `J<grados>`, `K<grados>`: Comandos para posicionar los servos A y B.
- `D<distancia> cm`: Formato de mensaje que el ESP32 envía al host con la telemetría.
