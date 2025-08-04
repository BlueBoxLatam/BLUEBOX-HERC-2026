/**
 * @file      00-EMISOR_MAESTRO_ESP-NOW.INO
 * @author    Blue Box Team
 * @brief     ESP32 Emisor (Controlador Maestro) para comunicación vía ESP-NOW. 📡
 * @version   1.1
 * @date      2025-07-29
 *
 * @project   Rover para Competencia NASA (Blue Box)
 *
 * @note      💡 Este firmware implementa el rol de emisor en una comunicación ESP-NOW.
 * Su función es leer comandos enviados a través del Monitor Serie y
 * transmitirlos de forma inalámbrica a un ESP32 receptor (esclavo).
 * Este es un código fundamental para validar la comunicación entre los dos ESP32
 * del rover, donde este actuará como el cerebro principal (maestro) enviando
 * instrucciones al segundo ESP32 (esclavo) para la gestión de motores y servos.
 *
 * @usage     1. **IMPORTANTE:** Reemplazar el valor de `broadcastAddress` con la dirección MAC
 * física del ESP32 que funcionará como RECEPTOR.
 * 2. Cargar este código en el ESP32 que será el EMISOR.
 * 3. Abrir el Monitor Serie de Arduino IDE (o similar) a 115200 baudios.
 * 4. Enviar comandos numéricos ('1' al '6') para controlar el receptor.
 */

//==============================================================================
// INCLUSIÓN DE LIBRERÍAS 📚
//==============================================================================
#include <esp_now.h> // Librería principal para el protocolo ESP-NOW
#include <WiFi.h>    // Requerida por ESP-NOW para operar

//==============================================================================
// DEFINICIONES Y VARIABLES GLOBALES 🌍
//==============================================================================

/**
 * @brief Dirección MAC del dispositivo ESP32 RECEPTOR.
 * @note  Para obtener la dirección MAC de tu ESP32 receptor, puedes cargar un
 * código simple que incluya WiFi.h y ejecute `Serial.println(WiFi.macAddress());`
 * en el `setup()`.
 */
uint8_t broadcastAddress[] = {0x08, 0xD1, 0xF9, 0xE8, 0xDF, 0x18};

/**
 * @struct struct_message
 * @brief  Define la estructura de los datos a enviar.
 * @note   Esta estructura DEBE ser idéntica en el código del emisor y del receptor
 * para garantizar una deserialización correcta de los datos.
 */
typedef struct struct_message {
  int id;      // Identificador único para la acción o actuador (e.g., motor 1, servo 2).
  bool state;  // Estado deseado (e.g., true para encendido/activado, false para apagado).
} struct_message;

// Instancia de la estructura que contendrá los datos del mensaje a enviar. 📤
struct_message myData;

// Instancia para la información del dispositivo par (peer).
esp_now_peer_info_t peerInfo;


//==============================================================================
// CALLBACK DE ENVÍO 🚀
//==============================================================================

/**
 * @brief     Función de Callback que se ejecuta de forma asíncrona tras un envío.
 * Permite conocer el resultado de la transmisión.
 * @param mac_addr La dirección MAC del peer al que se le envió el mensaje.
 * @param status   El estado del envío (e.g., ESP_NOW_SEND_SUCCESS o ESP_NOW_SEND_FAIL).
 */
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print(">> Estado del último envío: ");
  if (status == ESP_NOW_SEND_SUCCESS) {
    Serial.println("Éxito ✅");
  } else {
    Serial.println("Fallo ❌");
  }
}

//==============================================================================
// CONFIGURACIÓN INICIAL (SETUP) ⚙️
//==============================================================================
void setup() {
  // Iniciar la comunicación serie para depuración y entrada de comandos del usuario.
  Serial.begin(115200);
  Serial.println("\n--- INICIALIZANDO EMISOR ESP-NOW (CONTROLADOR ROVER) ---");

  // 1. Establecer el modo Wi-Fi del dispositivo en Station (WIFI_STA).
  //    ESP-NOW requiere que el Wi-Fi esté activo para funcionar.
  WiFi.mode(WIFI_STA);
  Serial.println("Modo Wi-Fi: Station");

  // 2. Inicializar el protocolo ESP-NOW.
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error: No se pudo inicializar ESP-NOW.");
    return;
  }
  Serial.println("Protocolo ESP-NOW inicializado.");

  // 3. Registrar la función de callback para el estado de los envíos.
  esp_now_register_send_cb(OnDataSent);

  // 4. Registrar el dispositivo par (peer) para la comunicación.
  //    Un "peer" es el otro dispositivo ESP32 con el que nos comunicaremos.
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;   // Canal WiFi (0-13). Ambos dispositivos deben usar el mismo.
  peerInfo.encrypt = false; // Deshabilitar encriptación para esta prueba básica.

  // Añadir el peer a la lista de ESP-NOW.
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Error: No se pudo añadir el peer receptor.");
    return;
  }
  Serial.println("Peer receptor añadido correctamente.");
  
  Serial.println("\n✅ Sistema listo. Esperando comandos...");
  Serial.println("   (Envía un número del '1' al '6' vía Monitor Serie)");
}


//==============================================================================
// BUCLE PRINCIPAL (LOOP) 🔄
//==============================================================================
void loop() {
  // Verificar si hay datos disponibles en el buffer del puerto serie.
  if (Serial.available() > 0) {
    char input = Serial.read(); // Leer el carácter entrante.
    bool shouldSend = true;     // Flag para determinar si se debe enviar un mensaje.

    // Mapea la entrada del usuario a la estructura de datos `myData`.
    // Esto traduce un simple carácter en un comando estructurado para el receptor.
    switch (input) {
      case '1': myData = {1, true};  break; // Ejemplo: Activar actuador #1
      case '2': myData = {2, true};  break; // Ejemplo: Activar actuador #2
      case '3': myData = {3, true};  break; // Ejemplo: Activar actuador #3
      case '4': myData = {1, false}; break; // Ejemplo: Desactivar actuador #1
      case '5': myData = {2, false}; break; // Ejemplo: Desactivar actuador #2
      case '6': myData = {3, false}; break; // Ejemplo: Desactivar actuador #3
      default:
        // Si la entrada no es válida, no se envía nada y se notifica al usuario.
        shouldSend = false;
        if (isprint(input)){ // Solo imprimir si es un caracter legible
             Serial.printf("Comando no válido: '%c'. Use números del 1 al 6.\n", input);
        }
        break;
    }

    // Si el comando fue válido, proceder con el envío de los datos. 💾
    if (shouldSend) {
      Serial.printf("Enviando comando -> ID: %d, Estado: %s\n", myData.id, myData.state ? "ON" : "OFF");
      esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
      
      // La validación del envío se realiza de forma asíncrona en OnDataSent.
      // Esta verificación inmediata solo confirma si el mensaje se pudo encolar.
      if (result != ESP_OK) {
          Serial.println("Error al encolar el mensaje para envío.");
      }
    }
  }

  // Pequeña pausa para no sobrecargar el procesador y permitir que otras tareas se ejecuten.
  delay(50);
}