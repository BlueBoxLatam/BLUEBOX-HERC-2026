/**
 * @file      03-DIAGNOSTICO_CONEXION_WIFI.INO
 * @author    Blue Box Team
 * @brief     Script de diagnóstico para probar la conexión a una red WiFi. 📡
 * @version   1.0
 * @date      2025-07-29
 *
 * @project   Rover para Competencia NASA (Blue Box)
 *
 * @note      💡 Este código es una prueba fundamental para validar que el hardware
 * del ESP32 y la librería `WiFi.h` funcionan correctamente. Intenta conectarse
 * a una red WiFi específica y obtener una dirección IP. El éxito en esta
 * operación confirma que el stack de red del dispositivo está operativo.
 *
 * @usage     1. ⚠️ **IMPORTANTE:** Reemplaza el valor de `ssid` y `password` con las
 * credenciales de tu red WiFi.
 * 2. Carga este código en el ESP32.
 * 3. Abre el Monitor Serie a 115200 baudios.
 * 4. Observa si el ESP32 logra conectarse y obtener una IP.
 */

//==============================================================================
// INCLUSIÓN DE LIBRERÍAS 📚
//==============================================================================
#include "WiFi.h"

//==============================================================================
// CONFIGURACIÓN DE RED 🌐
//==============================================================================

// --- ⚠️ ¡DEBES MODIFICAR ESTAS LÍNEAS! ---
const char* ssid = "ANGEL";         ///< @brief Nombre de tu red WiFi (SSID).
const char* password = "12345678";  ///< @brief Contraseña de tu red WiFi.
// -----------------------------------------

//==============================================================================
// CONFIGURACIÓN INICIAL (SETUP) ⚙️
//==============================================================================
void setup() {
  // Inicia la comunicación serial para mostrar el estado.
  Serial.begin(115200);
  delay(100);

  Serial.println("\nIniciando prueba de conexión WiFi...");
  Serial.print("Conectando a la red: ");
  Serial.println(ssid);

  // Inicia el proceso de conexión WiFi.
  WiFi.begin(ssid, password);

  // Bucle de espera para la conexión con un tiempo límite.
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) { // Intenta por ~10 segundos
    delay(500);
    Serial.print(".");
    attempts++;
  }

  Serial.println(); // Salto de línea estético.

  // Verifica el resultado final de la conexión.
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("✅ ¡Conexión exitosa!");
    Serial.print("   Dirección IP obtenida: ");
    Serial.println(WiFi.localIP()); // Imprime la IP asignada por el router.
  } else {
    Serial.println("❌ Fallo en la conexión.");
    Serial.println("   Revisa el SSID y la contraseña.");
    Serial.print("   Código de estado WiFi: ");
    Serial.println(WiFi.status()); // Muestra el código de error para depuración.
  }
}

//==============================================================================
// BUCLE PRINCIPAL (LOOP) 🔄
//==============================================================================
void loop() {
  // El loop se puede dejar vacío, ya que la conexión se realiza una sola vez.
  // Se añade una pausa larga para evitar que el loop se ejecute innecesariamente.
  delay(10000);
}