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
