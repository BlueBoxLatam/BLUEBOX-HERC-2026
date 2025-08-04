/**
 * @file      01-RECEPTOR_ESCLAVO_ESP-NOW.INO
 * @author    Blue Box Team
 * @brief     ESP32 Receptor (Actuador Esclavo) para comunicación vía ESP-NOW. 📡
 * @version   1.1
 * @date      2025-07-29
 *
 * @project   Rover para Competencia NASA (Blue Box)
 *
 * @note      💡 Este firmware implementa el rol de receptor en una comunicación ESP-NOW.
 * Su función es recibir comandos de un emisor maestro y actuar sobre ellos,
 * en este caso, controlando tres LEDs conectados a sus GPIOs.
 * Utiliza un método alternativo para mostrar su dirección MAC al inicio.
 *
 * @hardware  - ESP32 Dev Kit
 * - LED 1: Conectado a GPIO 33
 * - LED 2: Conectado a GPIO 26
 * - LED 3: Conectado a GPIO 27
 */

//==============================================================================
// INCLUSIÓN DE LIBRERÍAS 📚
//==============================================================================
#include <esp_now.h>
#include <WiFi.h>
#include "esp_mac.h" // Librería para obtener la MAC de bajo nivel

//==============================================================================
// DEFINICIONES Y VARIABLES GLOBALES 🌍
//==============================================================================

// Define los pines para los 3 LEDs
const int LED1_PIN = 33; ///< @brief Pin para el LED 1.
const int LED2_PIN = 26; ///< @brief Pin para el LED 2.
const int LED3_PIN = 27; ///< @brief Pin para el LED 3.

/**
 * @struct struct_message
 * @brief  Define la estructura de los datos a recibir.
 * @note   Esta estructura DEBE ser idéntica a la del emisor.
 */
typedef struct struct_message {
  int id;      // ID del actuador a controlar.
  bool state;  // Estado deseado (ON/OFF).
} struct_message;

// Instancia de la estructura para almacenar los datos recibidos. 📥
struct_message myData;

//==============================================================================
// CALLBACK DE RECEPCIÓN 📥
//==============================================================================

/**
 * @brief     Función de Callback que se ejecuta al recibir datos vía ESP-NOW.
 * @param info Puntero a la estructura con información del remitente.
 * @param incomingData Puntero a los datos recibidos.
 * @param len Longitud de los datos recibidos.
 */
void OnDataRecv(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Mensaje recibido. ID: ");
  Serial.print(myData.id);
  Serial.print(", Estado: ");
  Serial.println(myData.state ? "ON" : "OFF");

  // Controla el LED correspondiente según el ID recibido
  switch (myData.id) {
    case 1:
      digitalWrite(LED1_PIN, myData.state);
      break;
    case 2:
      digitalWrite(LED2_PIN, myData.state);
      break;
    case 3:
      digitalWrite(LED3_PIN, myData.state);
      break;
  }
}

//==============================================================================
// CONFIGURACIÓN INICIAL (SETUP) ⚙️
//==============================================================================
void setup() {
  Serial.begin(115200);

  // Configura los pines de los LEDs como salida y los apaga.
  pinMode(LED1_PIN, OUTPUT);
  digitalWrite(LED1_PIN, LOW);
  pinMode(LED2_PIN, OUTPUT);
  digitalWrite(LED2_PIN, LOW);
  pinMode(LED3_PIN, OUTPUT);
  digitalWrite(LED3_PIN, LOW);

  // MODO ALTERNATIVO PARA MOSTRAR LA DIRECCIÓN MAC
  uint8_t mac_addr[6];
  esp_efuse_mac_get_default(mac_addr);
  Serial.print("Dirección MAC de este Receptor: ");
  for (int i = 0; i < 6; ++i) {
    if (mac_addr[i] < 0x10) Serial.print("0");
    Serial.print(mac_addr[i], HEX);
    if (i < 5) Serial.print(":");
  }
  Serial.println();

  // Inicia WiFi en modo Station e inicializa ESP-NOW.
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error al inicializar ESP-NOW");
    return;
  }

  // Registra el callback de recepción de datos.
  esp_now_register_recv_cb(OnDataRecv);
  Serial.println("✅ Receptor ESP-NOW listo