# Blue Box Rover - NASA Human Exploration Rover Challenge

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
  - **prototypes/:** Pruebas de concepto y validación de componentes individuales. Cada prototipo ahora contiene su propio `README.md` con su descripción detallada.
- **/02_documentation/:** Documentación relevante, hojas de datos (datasheets) y reglas de la competencia.
- **/03_hardware/:** Diseños de PCB y modelos 3D para componentes físicos del rover.

---

## Equipo

- **Equipo:** Blue Box
- **Miembros:** [Lista de miembros]

---

## Hardware Necesario

A continuación se lista el hardware principal utilizado en este proyecto:

- **Microcontroladores:**
  - 2 x ESP32 (se usó el módulo `ESP32-WROOM-DA`)
- **Actuadores:**
  - 6 x Motor N20 con reductora
  - 4 x Servomotor SG90
  - 3 x Driver de motor dual (Puente H) TB6612FNG
- **Sensores:**
  - 1 x Sensor de distancia por ultrasonido HC-SR04

---

## Guía de Inicio Rápido (Setup)

Para compilar y cargar el firmware en los microcontroladores, necesitarás configurar el entorno de desarrollo de Arduino.

1.  **Instalar Arduino IDE:** Descarga e instala la última versión del [Arduino IDE](https://www.arduino.cc/en/software) desde su web oficial.

2.  **Añadir Soporte para ESP32:**
    - Abre el Arduino IDE y ve a `Archivo > Preferencias`.
    - En el campo "Gestor de URLs Adicionales de Tarjetas", pega la siguiente URL:
      ```
      https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
      ```
    - Haz clic en `OK`.

3.  **Instalar las Placas ESP32:**
    - Ve a `Herramientas > Placa > Gestor de Tarjetas...`.
    - Busca "esp32" y espera a que aparezca el paquete de Espressif Systems.
    - Haz clic en `Instalar` para añadir el soporte de las placas a tu IDE.

4.  **Seleccionar la Placa Correcta:**
    - Una vez instalado, ve a `Herramientas > Placa > ESP32 Arduino` y selecciona la placa **ESP32-WROOM-DA Module**.
    - Asegúrate de seleccionar también el puerto COM correcto en `Herramientas > Puerto` una vez que conectes el ESP32 a tu ordenador.

---

## Instrucciones de Uso

El sistema se basa en la comunicación directa entre un ESP32 Maestro (control) y un ESP32 Esclavo (actuadores) mediante ESP-NOW. Para que puedan comunicarse, el Maestro debe conocer la dirección MAC del Esclavo.

El flujo de trabajo recomendado es el siguiente:

1.  **Obtener la MAC del Esclavo:**
    - Abre el sketch del `01-RECEPTOR_ESCLAVO_ESP-NOW`.
    - Compila y cárgalo en uno de los ESP32.
    - Abre el Monitor Serie (Baudrate: 115200). Al arrancar, el ESP32 imprimirá su dirección MAC.
    - Copia esta dirección. Por ejemplo: `A1:B2:C3:D4:E5:F6`.

2.  **Configurar el Maestro:**
    - Abre el sketch del `00-EMISOR_MAESTRO_ESP-NOW`.
    - Busca la línea que define la variable `broadcastAddress`.
    - Reemplaza la dirección MAC de ejemplo con la que copiaste del Esclavo.

3.  **Cargar el Firmware al Maestro:**
    - Conecta el otro ESP32 (que será el Maestro).
    - Compila y carga el código ya modificado.

4.  **Ejecución:**
    - Alimenta ambos ESP32. Deberían enlazarse automáticamente.
    - Puedes enviar comandos numéricos ('1'-'6') a través del Monitor Serie del Maestro para ver cómo el Esclavo reacciona (activando los LEDs correspondientes).
