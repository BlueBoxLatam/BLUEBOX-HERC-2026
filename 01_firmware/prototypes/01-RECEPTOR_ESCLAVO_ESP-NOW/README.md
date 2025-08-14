## 01 - Receptor Esclavo ESP-NOW

Este firmware configura un ESP32 como un **dispositivo receptor (esclavo)** que actúa como un módulo de actuadores, respondiendo a comandos recibidos vía **ESP-NOW**.

### Funcionalidad Principal

El código inicializa el ESP32 en modo receptor y espera pasivamente los mensajes del controlador maestro. Su principal función es:

1.  **Mostrar su Dirección MAC**: Al arrancar, imprime su dirección MAC en el Monitor Serie para facilitar la configuración del emisor.
2.  **Recibir Comandos**: Escucha continuamente los mensajes enviados por el emisor maestro.
3.  **Controlar LEDs**: Al recibir un mensaje, decodifica el `ID` del actuador y su `estado` (ON/OFF) para encender o apagar el LED correspondiente conectado a los pines GPIO.

Este script está diseñado para ser altamente eficiente, ya que toda la lógica de control se maneja a través de interrupciones (`callback`), dejando el bucle principal (`loop`) libre.
