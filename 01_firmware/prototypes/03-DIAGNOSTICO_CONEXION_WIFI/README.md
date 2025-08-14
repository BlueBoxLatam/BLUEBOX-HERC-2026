## 03 - Diagnóstico de Conexión WiFi

Este script es una herramienta de **diagnóstico** simple pero crucial. Su única función es intentar conectar el ESP32 a una red WiFi utilizando las credenciales (SSID y contraseña) especificadas en el código.

### Funcionalidad Principal

Al ejecutarse, el programa inicia el módulo WiFi y trata de establecer una conexión. El progreso se muestra en el Monitor Serie con una serie de puntos.

- **Si la conexión es exitosa**, el programa imprime un mensaje de confirmación y la **dirección IP** que le fue asignada por el router.
- **Si la conexión falla**, informa del error y muestra un código de estado para ayudar a identificar la causa (ej: credenciales incorrectas, señal débil, etc.).

Este código es el paso fundamental para verificar la funcionalidad de la radio WiFi del ESP32 antes de implementar aplicaciones más complejas como ESP-NOW o clientes/servidores web.
