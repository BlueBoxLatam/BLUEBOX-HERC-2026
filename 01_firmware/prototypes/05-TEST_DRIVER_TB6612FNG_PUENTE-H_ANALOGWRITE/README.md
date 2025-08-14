## 05 - Test para Driver de Motores TB6612FNG (con analogWrite)

Este script es un código de prueba para controlar dos motores de corriente continua (DC) a través de un driver **TB6612FNG** conectado a un ESP32.

### Funcionalidad Principal

El programa ejecuta una secuencia predefinida para verificar el correcto funcionamiento tanto del hardware (motores, driver, conexiones) como del software. La secuencia es la siguiente:

1.  Mueve el Motor A hacia adelante.
2.  Mueve el Motor B hacia adelante.
3.  Mueve el Motor A hacia atrás.
4.  Mueve el Motor B hacia atrás.

Cada movimiento dura 3 segundos y es seguido por una breve pausa. Al final del ciclo completo, hay una pausa más larga antes de que la secuencia se repita.

### Característica Clave: `analogWrite`

Esta versión del código utiliza la función estándar de Arduino `analogWrite()` para generar las señales PWM que controlan la velocidad de los motores. Este método es una **alternativa más compatible** al uso directo del periférico `LEDC` del ESP32, evitando posibles errores de compilación en algunas configuraciones del IDE de Arduino.
