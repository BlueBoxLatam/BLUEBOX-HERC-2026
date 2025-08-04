/**
 * @file      05-TEST_DRIVER_TB6612FNG_ANALOGWRITE.INO
 * @author    Blue Box Team
 * @brief     Código de prueba para un driver de motores TB6612FNG con ESP32. 💨
 * @version   1.0
 * @date      2025-07-29
 *
 * @project   Rover para Competencia NASA (Blue Box)
 *
 * @note      💡 VERSIÓN ALTERNATIVA: Este código utiliza la función `analogWrite` en
 * lugar de `ledc` para generar la señal PWM. Se usa como alternativa
 * para solucionar problemas de compilación en entornos donde `ledcSetup`
 * no es reconocido.
 *
 * @hardware  - ESP32 WROOM (38 pines)
 * - Driver de Motores TB6612FNG
 * - STBY: GPIO 21
 * - Motor A: AIN1=19, AIN2=22, PWMA=23
 * - Motor B: BIN1=5, BIN2=17, PWMB=18
 *
 * @sequence  1. Motor A gira ADELANTE por 3s.
 * 2. Motor B gira ADELANTE por 3s.
 * 3. Motor A gira ATRÁS por 3s.
 * 4. Motor B gira ATRÁS por 3s.
 * 5. Pausa de 5s y el ciclo se repite.
 */

//==============================================================================
// INCLUSIÓN DE LIBRERÍAS 📚
//==============================================================================
#include <Arduino.h>

//==============================================================================
// DEFINICIONES Y VARIABLES GLOBALES 🌍
//==============================================================================

// --- Definición de Pines ---
#define STBY_PIN 21   ///< @brief Pin Standby del driver. HIGH para operar.
#define AIN1_PIN 19   ///< @brief Pin de Dirección 1 del Motor A.
#define AIN2_PIN 22   ///< @brief Pin de Dirección 2 del Motor A.
#define PWMA_PIN 23   ///< @brief Pin PWM de Velocidad del Motor A.
#define BIN1_PIN 5    ///< @brief Pin de Dirección 1 del Motor B.
#define BIN2_PIN 17   ///< @brief Pin de Dirección 2 del Motor B.
#define PWMB_PIN 18   ///< @brief Pin PWM de Velocidad del Motor B.

// --- Configuración de Velocidad ---
// Con analogWrite, la resolución por defecto es de 8 bits (0-255).
const int MOTOR_SPEED = 200; ///< @brief Velocidad del motor (0-255).

//==============================================================================
// FUNCIONES 🕹️
//==============================================================================

/**
 * @brief     Controla un motor específico usando `analogWrite`.
 * @param motor     El motor a mover (0 para A, 1 para B).
 * @param direccion La dirección (1 para adelante, -1 para atrás, 0 para frenar).
 * @param speed     La velocidad del motor (0-255).
 */
void moveMotor(int motor, int direccion, int speed) {
  bool in1, in2;

  if (direccion == 1) {       // Adelante
    in1 = HIGH;
    in2 = LOW;
  } else if (direccion == -1) { // Atrás
    in1 = LOW;
    in2 = HIGH;
  } else {                      // Freno (Brake)
    in1 = HIGH;
    in2 = HIGH;
    speed = 0; // La velocidad es 0 al frenar.
  }

  if (motor == 0) { // Controlar Motor A
    digitalWrite(AIN1_PIN, in1);
    digitalWrite(AIN2_PIN, in2);
    analogWrite(PWMA_PIN, speed); // Usa analogWrite directamente.
  } else {          // Controlar Motor B
    digitalWrite(BIN1_PIN, in1);
    digitalWrite(BIN2_PIN, in2);
    analogWrite(PWMB_PIN, speed); // Usa analogWrite directamente.
  }
}

//==============================================================================
// CONFIGURACIÓN INICIAL (SETUP) ⚙️
//==============================================================================
void setup() {
  // Configurar todos los pines de control como salidas.
  pinMode(STBY_PIN, OUTPUT);
  pinMode(AIN1_PIN, OUTPUT);
  pinMode(AIN2_PIN, OUTPUT);
  pinMode(PWMA_PIN, OUTPUT);
  pinMode(BIN1_PIN, OUTPUT);
  pinMode(BIN2_PIN, OUTPUT);
  pinMode(PWMB_PIN, OUTPUT);

  // Iniciar con el driver en standby (motores apagados y sin consumir).
  digitalWrite(STBY_PIN, LOW);
}

//==============================================================================
// BUCLE PRINCIPAL (LOOP) 🔄
//==============================================================================
void loop() {
  // Sacar el driver del modo standby para empezar a mover los motores.
  digitalWrite(STBY_PIN, HIGH);
  delay(10); // Pequeña pausa para asegurar que el driver esté listo.

  // --- Inicia la secuencia de prueba ---

  // 1. Motor A gira ADELANTE por 3 segundos
  moveMotor(0, 1, MOTOR_SPEED);
  delay(3000);
  moveMotor(0, 0, 0); // Detener Motor A (freno).
  delay(500);         // Pausa entre movimientos.

  // 2. Motor B gira ADELANTE por 3 segundos
  moveMotor(1, 1, MOTOR_SPEED);
  delay(3000);
  moveMotor(1, 0, 0); // Detener Motor B (freno).
  delay(500);

  // 3. Motor A gira ATRÁS por 3 segundos
  moveMotor(0, -1, MOTOR_SPEED);
  delay(3000);
  moveMotor(0, 0, 0); // Detener Motor A (freno).
  delay(500);

  // 4. Motor B gira ATRÁS por 3 segundos
  moveMotor(1, -1, MOTOR_SPEED);
  delay(3000);
  moveMotor(1, 0, 0); // Detener Motor B (freno).

  // 5. Pausa general con los motores detenidos.
  delay(5000);
}