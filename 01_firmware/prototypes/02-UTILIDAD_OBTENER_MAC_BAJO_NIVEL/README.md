## 02 - Utilidad para Obtener MAC (Bajo Nivel)

Este es un script de **diagnóstico y utilidad** diseñado para un propósito específico: obtener y mostrar la dirección MAC de un ESP32 utilizando funciones de bajo nivel del **ESP-IDF** (el framework de desarrollo de Espressif).

### Funcionalidad Principal

A diferencia del método estándar que utiliza la librería `WiFi.h` de Arduino, este código llama directamente a la función `esp_efuse_mac_get_default()`. Su objetivo es verificar la capacidad del hardware para reportar su MAC sin pasar por la capa de abstracción de Arduino.

Este enfoque es ideal para **solucionar problemas (troubleshooting)**. Si un ESP32 no muestra su MAC con los métodos habituales, este script ayuda a determinar si el problema reside en el hardware/firmware base o en la configuración del entorno de Arduino.
