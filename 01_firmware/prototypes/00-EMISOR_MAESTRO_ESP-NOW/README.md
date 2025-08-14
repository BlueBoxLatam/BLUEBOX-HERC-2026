## 00 - Emisor Maestro ESP-NOW

Este firmware configura un ESP32 como un **dispositivo emisor (maestro)** utilizando el protocolo de comunicación inalámbrica **ESP-NOW**.

### Funcionalidad Principal

El código inicializa el ESP32 y espera la entrada de comandos a través del Monitor Serie (baudrate: 115200). Al recibir un número del '1' al '6', empaqueta un comando en una estructura de datos y lo transmite de forma inalámbrica al ESP32 receptor (esclavo) predefinido.

- **Comandos '1', '2', '3'**: Activan los actuadores correspondientes (ID 1, 2, 3).
- **Comandos '4', '5', '6'**: Desactivan los actuadores correspondientes (ID 1, 2, 3).

El estado de cada envío (éxito o fallo) se reporta de forma asíncrona en el Monitor Serie. Este script es una pieza clave para el sistema de control del Rover, actuando como el cerebro que envía órdenes a otros módulos.
