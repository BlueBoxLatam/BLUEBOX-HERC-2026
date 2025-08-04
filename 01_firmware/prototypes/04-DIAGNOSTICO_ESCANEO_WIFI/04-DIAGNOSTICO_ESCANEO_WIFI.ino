/**
 * @file      04-DIAGNOSTICO_ESCANEO_WIFI.INO
 * @author    Blue Box Team
 * @brief     Script de diagnóstico que escanea redes WiFi cercanas. 📶
 * @version   1.0
 * @date      2025-07-29
 *
 * @project   Rover para Competencia NASA (Blue Box)
 *
 * @note      💡 Este código es una prueba clave para verificar que el hardware de radio
 * del ESP32 se inicializa y funciona en modo Estación (`WIFI_STA`). Si el escaneo
 * tiene éxito y muestra una lista de redes, confirma que la radio está
 * operativa, lo cual es un paso fundamental para depurar problemas de conexión
 * o de obtención de la MAC.
 *
 * @usage     1. Cargar este código en el ESP32.
 * 2. Abrir el Monitor Serie a 115200 baudios.
 * 3. Observar si el dispositivo lista las redes WiFi de su entorno.
 */

//==============================================================================
// INCLUSIÓN DE LIBRERÍAS 📚
//==============================================================================
#include "WiFi.h"

//==============================================================================
// CONFIGURACIÓN INICIAL (SETUP) ⚙️
//==============================================================================
void setup() {
  // Inicia la comunicación serial para mostrar los resultados.
  Serial.begin(115200);
  delay(100);

  Serial.println("\nIniciando prueba de escaneo WiFi...");

  // 1. Configura el WiFi en modo Station (estación).
  //    Este modo es necesario para poder escanear redes.
  WiFi.mode(WIFI_STA);

  // 2. Nos aseguramos de no estar conectados a ninguna red.
  WiFi.disconnect();
  delay(100);

  Serial.println("Modo WiFi Station activado. Comenzando escaneo...");
}

//==============================================================================
// BUCLE PRINCIPAL (LOOP) 🔄
//==============================================================================
void loop() {
  // WiFi.scanNetworks() inicia el escaneo y devuelve el número de redes encontradas.
  int n = WiFi.scanNetworks();

  Serial.println("--- Escaneo Finalizado ---");
  if (n == 0) {
    Serial.println("❌ No se encontraron redes. El módulo WiFi podría tener problemas.");
  } else {
    Serial.print("✅ ");
    Serial.print(n);
    Serial.println(" redes encontradas:");
    
    // Imprime la lista de redes encontradas con su SSID, potencia (RSSI) y seguridad.
    for (int i = 0; i < n; ++i) {
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i)); // Nombre de la red (SSID)
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i)); // Potencia de la señal en dBm
      Serial.print("dBm)");
      // Muestra si la red es abierta o segura.
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " [Abierta]" : " [Segura]");
      delay(10);
    }
  }
  Serial.println("\nVolviendo a escanear en 10 segundos...");
  
  // Espera 10 segundos antes de iniciar el próximo escaneo.
  delay(10000);
}