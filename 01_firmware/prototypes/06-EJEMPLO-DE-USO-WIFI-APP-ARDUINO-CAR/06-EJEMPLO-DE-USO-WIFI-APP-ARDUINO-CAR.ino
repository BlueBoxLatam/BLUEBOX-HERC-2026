/**
 * @file      06-CONTROLADOR_ROVER_WIFI_CLIENT.INO
 * @author    One Day Of Code (Documentado por Blue Box Team)
 * @brief     Firmware para un robot móvil (coche) controlado por WiFi. 🤖
 * @version   1.0
 * @date      2025-08-04
 *
 * @project   Rover para Competencia NASA (Blue Box)
 *
 * @note      💡 Este código configura el ESP32 para actuar como un cliente WiFi. Se conecta
 * a una red y a un servidor (host) específico (ej. una app móvil) en una IP y puerto
 * fijos. Recibe comandos de un solo carácter para controlar motores, servos y LEDs,
 * y envía datos del sensor de distancia de vuelta al host.
 *
 * @hardware  - ESP32 Dev Kit
 * - Driver de Motores (L298N o similar)
 * - 2x Servomotores
 * - 2x LEDs
 * - 1x Sensor de distancia ultrasónico HC-SR04
 *
 * @usage     1. ⚠️ **IMPORTANTE:** Modificar `ssid`, `password` y la `ipAddress` del host.
 * 2. Cargar el código en el ESP32.
 * 3. Asegurarse de que el host (app móvil) esté en la misma red y escuchando
 * en el puerto especificado.
 */

//==============================================================================
// INCLUSIÓN DE LIBRERÍAS 📚
//==============================================================================
#include <ESP32Servo.h>
#include <WiFi.h>

//==============================================================================
// DEFINICIONES Y VARIABLES GLOBALES 🌍
//==============================================================================

// --- Configuración de Red 🌐 ---
const char *ssid = "LAMED";                  ///< @brief Nombre de la red WiFi.
const char *password = "Bolfuturo2020.";     ///< @brief Contraseña de la red WiFi.
IPAddress ipAddress(192, 168, 88, 28); ///< @brief IP del host/servidor (app móvil).
#define PORT 50123                           ///< @brief Puerto de conexión del host.
WiFiClient client;

// --- Pinout del Hardware 📌 ---
// Servos
#define PIN_SERVO_A 18
#define PIN_SERVO_B 19
// Motor Izquierdo (Llantas izquierdas)
#define PIN_IN1 27
#define PIN_IN2 26
// Motor Derecho (Llantas derechas)
#define PIN_IN3 25
#define PIN_IN4 33
// LEDs / Luces
#define PIN_LED 12
#define PIN_LED_2 14
// Sensor de Distancia HC-SR04
#define PIN_ECHO 22
#define PIN_TRIG 23

// --- Objetos de Control ---
Servo servoA;
Servo servoB;

// --- Variables Auxiliares ---
String numericPart = ""; // Almacena la parte numérica de un comando (ej. para servos).
char codeReceived;       // Almacena el carácter de comando recibido.

//==============================================================================
// PROTOTIPOS DE FUNCIONES 🕹️
//==============================================================================
void connect();
long measureDistance();
long microsecondsToCentimeters(long microseconds);
void turnOnLed();
void turnOffLed();
void turnOnLed2();
void turnOffLed2();
void rotateServoA(int grados);
void rotateServoB(int grados);
void goForward();
void goBack();
void stop();
void turnLeft();
void turnRight();

//==============================================================================
// CONFIGURACIÓN INICIAL (SETUP) ⚙️
//==============================================================================
void setup() {
    Serial.begin(115200);

    // Asignar temporizadores para los servos en ESP32
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);

    // --- Inicializar WiFi ---
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.println("❌ Fallo en conexión WiFi. Reiniciando...");
        while (1) { delay(1000); }
    }
    Serial.print("✅ WiFi conectado. IP: ");
    Serial.println(WiFi.localIP());

    // --- Configurar Pines ---
    pinMode(PIN_IN1, OUTPUT);
    pinMode(PIN_IN2, OUTPUT);
    pinMode(PIN_IN3, OUTPUT);
    pinMode(PIN_IN4, OUTPUT);
    pinMode(PIN_LED, OUTPUT);
    pinMode(PIN_LED_2, OUTPUT);
    pinMode(PIN_TRIG, OUTPUT);
    pinMode(PIN_ECHO, INPUT);

    // --- Inicializar Servos ---
    servoA.setPeriodHertz(50); // Estándar para servos
    servoA.attach(PIN_SERVO_A, 500, 2400); // Rango de pulso típico
    servoB.setPeriodHertz(50);
    servoB.attach(PIN_SERVO_B, 500, 2400);

    // Conectar al host
    connect();
}

//==============================================================================
// BUCLE PRINCIPAL (LOOP) 🔄
//==============================================================================
void loop() {
    if (!client.connected()) { // Reconectar si se pierde la conexión
        connect();
    }

    // --- Procesamiento de Comandos ---
    while (client.available() > 0) {
        String line = client.readStringUntil('\n');
        Serial.printf("Comando recibido: %s\n", line.c_str());
        numericPart = "";
        
        // Parsea la línea para separar el carácter de comando y la parte numérica
        for (unsigned int i = 0; i < line.length(); i++) {
            if (isDigit(line[i])) {
                numericPart += line[i];
            } else if (line[i] != '\n') {
                codeReceived = line[i];
            }
        }
    }

    // --- Ejecución de Comandos ---
    switch (codeReceived) {
        case 'F': goForward(); break;    // Adelante
        case 'B': goBack(); break;       // Atrás
        case 'L': turnLeft(); break;     // Izquierda
        case 'R': turnRight(); break;    // Derecha
        case 'S': stop(); break;         // Detener
        case 'M': turnOnLed(); break;    // Luz 1 ON
        case 'm': turnOffLed(); break;   // Luz 1 OFF
        case 'N': turnOnLed2(); break;   // Luz 2 ON
        case 'n': turnOffLed2(); break;  // Luz 2 OFF
        case 'J': if (numericPart != "") rotateServoA(numericPart.toInt()); break;
        case 'K': if (numericPart != "") rotateServoB(numericPart.toInt()); break;
    }
    codeReceived = ' '; // Limpiar comando para evitar repetición

    // --- Envío de Datos de Sensores ---
    client.printf("D%d cm\n", measureDistance());
    delay(20); // Pequeña pausa para estabilidad
}

//==============================================================================
// IMPLEMENTACIÓN DE FUNCIONES 🕹️
//==============================================================================

/** @brief Intenta conectar con el servidor host. */
void connect() {
    if (!client.connect(ipAddress, PORT)) {
        Serial.println("Fallo al conectar con el host.");
        delay(1000);
        return;
    }
    Serial.printf("Conectado al host: %s\n", client.remoteIP().toString().c_str());
}

/** @brief Mide la distancia usando el sensor HC-SR04. @return Distancia en cm. */
long measureDistance() {
    digitalWrite(PIN_TRIG, LOW);
    delayMicroseconds(5);
    digitalWrite(PIN_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_TRIG, LOW);
    long duration = pulseIn(PIN_ECHO, HIGH);
    return microsecondsToCentimeters(duration);
}

/** @brief Convierte una duración en microsegundos a centímetros. @return Distancia en cm. */
long microsecondsToCentimeters(long microseconds) { return microseconds / 29 / 2; }

/** @brief Enciende el LED 1. */
void turnOnLed() { digitalWrite(PIN_LED, HIGH); }
/** @brief Apaga el LED 1. */
void turnOffLed() { digitalWrite(PIN_LED, LOW); }
/** @brief Enciende el LED 2. */
void turnOnLed2() { digitalWrite(PIN_LED_2, HIGH); }
/** @brief Apaga el LED 2. */
void turnOffLed2() { digitalWrite(PIN_LED_2, LOW); }

/** @brief Rota el servo A a un ángulo específico. @param grados Ángulo en grados. */
void rotateServoA(int grados) { servoA.write(grados); }
/** @brief Rota el servo B a un ángulo específico. @param grados Ángulo en grados. */
void rotateServoB(int grados) { servoB.write(grados); }

/** @brief Mueve ambas llantas hacia adelante. 💨 */
void goForward() {
    digitalWrite(PIN_IN1, HIGH); digitalWrite(PIN_IN2, LOW);
    digitalWrite(PIN_IN3, HIGH); digitalWrite(PIN_IN4, LOW);
}
/** @brief Mueve ambas llantas hacia atrás. */
void goBack() {
    digitalWrite(PIN_IN1, LOW); digitalWrite(PIN_IN2, HIGH);
    digitalWrite(PIN_IN3, LOW); digitalWrite(PIN_IN4, HIGH);
}
/** @brief Detiene ambos motores. */
void stop() {
    digitalWrite(PIN_IN1, LOW); digitalWrite(PIN_IN2, LOW);
    digitalWrite(PIN_IN3, LOW); digitalWrite(PIN_IN4, LOW);
}
/** @brief Gira a la izquierda (movimiento tipo tanque). */
void turnLeft() {
    digitalWrite(PIN_IN1, LOW); digitalWrite(PIN_IN2, HIGH);  // Rueda izquierda atrás
    digitalWrite(PIN_IN3, HIGH); digitalWrite(PIN_IN4, LOW); // Rueda derecha adelante
}
/** @brief Gira a la derecha (movimiento tipo tanque). */
void turnRight() {
    digitalWrite(PIN_IN1, HIGH); digitalWrite(PIN_IN2, LOW); // Rueda izquierda adelante
    digitalWrite(PIN_IN3, LOW); digitalWrite(PIN_IN4, HIGH);  // Rueda derecha atrás
}