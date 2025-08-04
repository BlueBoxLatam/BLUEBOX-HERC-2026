/**
 * @file      02-UTILIDAD_OBTENER_MAC_BAJO_NIVEL.INO
 * @author    Blue Box Team
 * @brief     Script de diagnóstico para obtener la dirección MAC de un ESP32. 💻
 * @version   1.0
 * @date      2025-07-29
 *
 * @project   Rover para Competencia NASA (Blue Box)
 *
 * @note      💡 Este código es una herramienta de prueba. Su propósito es obtener la
 * dirección MAC llamando a una función de bajo nivel del ESP-IDF (`esp_efuse_mac_get_default`),
 * sin depender de la librería `WiFi.h` de Arduino. Es útil para diagnosticar
 * si un problema de red podría estar en la capa de abstracción de Arduino o
 * en un nivel más profundo del hardware/firmware.
 *
 * @usage     1. Cargar este código en el ESP32.
 * 2. Abrir el Monitor Serie a 115200 baudios.
 * 3. Observar la dirección MAC que se imprime en la consola.
 */

//==============================================================================
// INCLUSIÓN DE LIBRERÍAS 📚
//==============================================================================
#include "esp_system.h" // Funciones del sistema base de ESP-IDF
#include "esp_mac.h"    // Funciones específicas para la gestión de la MAC

//==============================================================================
// CONFIGURACIÓN INICIAL (SETUP) ⚙️
//==============================================================================
void setup() {
  // Inicia la comunicación serial para mostrar resultados.
  Serial.begin(115200);
  delay(500); // Pequeña pausa para estabilizar la comunicación.

  // Array de 6 bytes para almacenar la dirección MAC.
  uint8_t mac_addr[6];

  // Llama a la función de bajo nivel para obtener la MAC por defecto.
  esp_err_t result = esp_efuse_mac_get_default(mac_addr);
  
  // 💡 Alternativa: Si la función anterior falla, se podría probar esta otra.
  // esp_err_t result = esp_read_mac(mac_addr, ESP_MAC_WIFI_STA);

  Serial.println("Intentando leer la MAC con un método de bajo nivel...");

  if (result == ESP_OK) {
    Serial.print("✅ La dirección MAC de este ESP32 es: ");
    
    // Imprime cada byte de la dirección en formato hexadecimal.
    for (int i = 0; i < 6; ++i) {
      if (mac_addr[i] < 0x10) {
        Serial.print("0"); // Añade un cero inicial para formato (e.g., 0A en vez de A).
      }
      Serial.print(mac_addr[i], HEX);
      if (i < 5) {
        Serial.print(":"); // Separador.
      }
    }
    Serial.println();
  } else {
    Serial.println("❌ Fallo al leer la dirección MAC.");
    Serial.print("Código de error: ");
    Serial.println(result); // Imprime el código de error para depuración.
  }
}

//==============================================================================
// BUCLE PRINCIPAL (LOOP) 🔄
//==============================================================================
void loop() {
  // El loop se deja vacío, ya que el programa solo se ejecuta una vez.
}