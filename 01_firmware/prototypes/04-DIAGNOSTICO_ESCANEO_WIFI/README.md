## 04 - Diagnóstico de Escaneo WiFi

Este es un script de **diagnóstico** diseñado para verificar el funcionamiento básico del hardware de radio del ESP32. Su única tarea es poner el dispositivo en modo `WIFI_STA` (Estación) y realizar un escaneo continuo de las redes WiFi cercanas.

### Funcionalidad Principal

El programa inicializa el módulo WiFi y, en un bucle infinito, busca redes en el espectro de 2.4 GHz. Los resultados se imprimen en el Monitor Serie.

- **Si encuentra redes**: Lista el nombre (SSID) de cada una, junto con su potencia de señal (RSSI) y si es una red abierta o segura.
- **Si no encuentra redes**: Muestra un mensaje indicando que el escaneo no tuvo éxito.

Un escaneo exitoso es una fuerte indicación de que el hardware de radio del ESP32 está alimentado y funcionando correctamente, lo que permite centrar la depuración de problemas de conectividad en otras áreas como las credenciales de red o la configuración del software.
