/*
 * Designed by @GioSunrider
 * TFG OmniRobot V0.1
 * CC-BY-SA
 *
 */

 // ------------------------------------------------------------------------------------------
 //    Esquema Coche
 // ------------------------------------------------------------------------------------------
 //      ___________
 // A | |           | |B
 //     |           |
 //     |           |
 // D | |___________| | C


// ------------------------------------------------------------------------------------------
// Librerias
// ------------------------------------------------------------------------------------------

#include <FlySkyIBus.h>

// ------------------------------------------------------------------------------------------
// DEFINICION PINES Pololu Dual MC33926 Motor Driver Shield
//              ARDUINO PRO MICRO
// ------------------------------------------------------------------------------------------

#define ENABLE  2

#define MotorA1 8
#define MotorA2 16

#define MotorB1 4
#define MotorB2 7

#define MotorC1 14
#define MotorC2 15

#define MotorD1 18  //A0
#define MotorD2 19  //A1

#define PWM_A    6
#define PWM_B    5
#define PWM_C    10
#define PWM_D    9

// ------------------------------------------------------------------------------------------
// DEFINICION PINES Emision y Recepcion Laser
// ------------------------------------------------------------------------------------------

#define analogInPin A3       // Pin analogico para la lectura del Fototransistor
#define Servo_Laser 3        // Pin para el servo laser
#define LED_Golpe A2         // Led que ilumina cuando ha recibido un golpe
#define Laser 1              // Control del Laser

// ------------------------------------------------------------------------------------------
// DEFINICION PINES Entrada iBus
//       Emisora Flysky FS-i6
//      Receptor Flysky FS-X6B
// ------------------------------------------------------------------------------------------

float Channel_1;  // Ahead and back pin (Y)
float Channel_2;  // Left and Right pin (X)
float Channel_3;  // Shooter Up-Down
float Channel_4;  // Rotation pin (W)
float Channel_5;  // ENABLE
float Channel_6;  // Laser

// ------------------------------------------------------------------------------------------
// Constantes y Variables
// ------------------------------------------------------------------------------------------

const float arms_size = 120.0; // mm
const float wheel_radius = 37.0; // mm (74/2)

float speed_A = 0;
float speed_B = 0;
float speed_C = 0;
float speed_D = 0;

const int thresholdMIN = 90;   // Valor Umbral Minimo de Recepcion del fotodiodo
const int thresholdMAX = 120;  // Valor Umbral Maximo de Recepcion del fotodiodo

int sensorValue = 0;        // Valor del sensor
int lives = 10;             // Numero de "vidas" que tiene
unsigned long lastHitTime = 0;  // Para evitar múltiples detecciones rápidas
const unsigned long hitDebounceTime = 500; // ms

// Variables para el servo PWM
unsigned long servoPulseStart = 0;
int servoAngle = 90;
bool servoPulseActive = false;
const unsigned long servoPeriod = 20000; // 20ms para servo (50Hz)

// ------------------------------------------------------------------------------------------
// Setup
// -----------------------------------------------------------------------------------------

void setup(){
  Serial1.begin(115200); // Necesario para la comunicación IBus
  IBus.begin(Serial1);

  pinMode(ENABLE, OUTPUT);

  // Configurar pines de motor
  pinMode(PWM_A, OUTPUT);
  pinMode(PWM_B, OUTPUT);
  pinMode(PWM_C, OUTPUT);
  pinMode(PWM_D, OUTPUT);

  pinMode(MotorA1, OUTPUT);
  pinMode(MotorA2, OUTPUT);
  pinMode(MotorB1, OUTPUT);
  pinMode(MotorB2, OUTPUT);
  pinMode(MotorC1, OUTPUT);
  pinMode(MotorC2, OUTPUT);
  pinMode(MotorD1, OUTPUT);
  pinMode(MotorD2, OUTPUT);
  
  // Configurar pines de sensores y actuadores
  pinMode(analogInPin, INPUT);
  pinMode(LED_Golpe, OUTPUT);
  pinMode(Laser, OUTPUT);
  pinMode(Servo_Laser, OUTPUT);
  
  // Inicializar todos los pines en estado seguro
  digitalWrite(ENABLE, LOW);
  digitalWrite(Laser, LOW);
  digitalWrite(LED_Golpe, LOW);
  
  // Configurar PWM a 0
  analogWrite(PWM_A, 0);
  analogWrite(PWM_B, 0);
  analogWrite(PWM_C, 0);
  analogWrite(PWM_D, 0);
  
  // Configurar servo a posición central
  servoAngle = 90;
}

// ------------------------------------------------------------------------------------------
// Funciones para control de motores
// ------------------------------------------------------------------------------------------

void set_motor_speed(int motor, float spd) {
  int pwmValue = constrain(abs(spd), 0, 200);
  pwmValue = map(pwmValue, 0, 200, 0, 255);
  
  switch (motor) {
    case 0: // Motor A
      if (spd > 1) {
        digitalWrite(MotorA1, HIGH);
        digitalWrite(MotorA2, LOW);
        analogWrite(PWM_A, pwmValue);
      } else if (spd < -1) {
        digitalWrite(MotorA1, LOW);
        digitalWrite(MotorA2, HIGH);
        analogWrite(PWM_A, pwmValue);
      } else {
        digitalWrite(MotorA1, LOW);
        digitalWrite(MotorA2, LOW);
        analogWrite(PWM_A, 0);
      }
      break;
      
    case 1: // Motor B
      if (spd > 1) {
        digitalWrite(MotorB1, HIGH);
        digitalWrite(MotorB2, LOW);
        analogWrite(PWM_B, pwmValue);
      } else if (spd < -1) {
        digitalWrite(MotorB1, LOW);
        digitalWrite(MotorB2, HIGH);
        analogWrite(PWM_B, pwmValue);
      } else {
        digitalWrite(MotorB1, LOW);
        digitalWrite(MotorB2, LOW);
        analogWrite(PWM_B, 0);
      }
      break;
      
    case 2: // Motor C
      if (spd > 1) {
        digitalWrite(MotorC1, HIGH);
        digitalWrite(MotorC2, LOW);
        analogWrite(PWM_C, pwmValue);
      } else if (spd < -1) {
        digitalWrite(MotorC1, LOW);
        digitalWrite(MotorC2, HIGH);
        analogWrite(PWM_C, pwmValue);
      } else {
        digitalWrite(MotorC1, LOW);
        digitalWrite(MotorC2, LOW);
        analogWrite(PWM_C, 0);
      }
      break;
      
    case 3: // Motor D
      if (spd > 1) {
        digitalWrite(MotorD1, HIGH);
        digitalWrite(MotorD2, LOW);
        analogWrite(PWM_D, pwmValue);
      } else if (spd < -1) {
        digitalWrite(MotorD1, LOW);
        digitalWrite(MotorD2, HIGH);
        analogWrite(PWM_D, pwmValue);
      } else {
        digitalWrite(MotorD1, LOW);
        digitalWrite(MotorD2, LOW);
        analogWrite(PWM_D, 0);
      }
      break;
  }
}

void vector_movement(float X, float Y, float W) {
  // Calcular velocidades para cada rueda (Omnidireccional)
  // Ajustado para movimiento correcto de robot omnidireccional
  speed_A = Y + X + (arms_size * W);
  speed_B = -Y + X + (arms_size * W);
  speed_C = -Y - X + (arms_size * W);
  speed_D = Y - X + (arms_size * W);
  
  // Normalizar velocidades si alguna excede el máximo
  float max_speed = max(max(abs(speed_A), abs(speed_B)), max(abs(speed_C), abs(speed_D)));
  if (max_speed > 200) {
    float scale = 200.0 / max_speed;
    speed_A *= scale;
    speed_B *= scale;
    speed_C *= scale;
    speed_D *= scale;
  }
  
  // Aplicar velocidades a los motores
  set_motor_speed(0, speed_A);
  set_motor_speed(1, speed_B);
  set_motor_speed(2, speed_C);
  set_motor_speed(3, speed_D);
}

// ------------------------------------------------------------------------------------------
// Funciones para servo (generación de señal PWM manual)
// ------------------------------------------------------------------------------------------

void updateServo() {
  unsigned long currentMicros = micros();
  
  if (!servoPulseActive) {
    // Comenzar nuevo pulso
    digitalWrite(Servo_Laser, HIGH);
    servoPulseStart = currentMicros;
    servoPulseActive = true;
  } else {
    // Terminar pulso después del tiempo apropiado
    unsigned long pulseWidth = map(servoAngle, 0, 180, 500, 2500); // 0.5ms a 2.5ms
    
    if (currentMicros - servoPulseStart >= pulseWidth) {
      digitalWrite(Servo_Laser, LOW);
      
      // Si ha pasado un período completo, reiniciar
      if (currentMicros - servoPulseStart >= servoPeriod) {
        servoPulseActive = false;
      }
    }
  }
}

void setServoAngle(int angle) {
  servoAngle = constrain(angle, 0, 180);
}

// ------------------------------------------------------------------------------------------
// Función para detección de golpes
// ------------------------------------------------------------------------------------------

void checkHit() {
  sensorValue = analogRead(analogInPin);
  unsigned long currentTime = millis();
  
  if (sensorValue > thresholdMIN && sensorValue < thresholdMAX) {
    if (currentTime - lastHitTime > hitDebounceTime) {
      lives--;
      lastHitTime = currentTime;
      
      // Parpadear LED para indicar golpe
      for (int i = 0; i < 3; i++) {
        digitalWrite(LED_Golpe, HIGH);
        delay(100);
        digitalWrite(LED_Golpe, LOW);
        delay(100);
      }
    }
  }
}

// ------------------------------------------------------------------------------------------
// Función para leer y mapear canales IBus
// ------------------------------------------------------------------------------------------

void readIBusChannels() {
  IBus.loop();
  
  // Leer canales y mapear a rangos útiles
  Channel_1 = mapFloat(IBus.readChannel(0), 1000, 2000, -100.0, 100.0); // Y (adelante/atrás)
  Channel_2 = mapFloat(IBus.readChannel(1), 1000, 2000, -100.0, 100.0); // X (izquierda/derecha)
  Channel_3 = mapFloat(IBus.readChannel(2), 1000, 2000, 0.0, 180.0);   // Servo (0-180 grados)
  Channel_4 = mapFloat(IBus.readChannel(3), 1000, 2000, -10.0, 10.0);  // W (rotación)
  Channel_5 = IBus.readChannel(4);  // ENABLE (1000-2000)
  Channel_6 = IBus.readChannel(5);  // Laser (1000-2000)
}

float mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// ------------------------------------------------------------------------------------------
// Loop principal
// ------------------------------------------------------------------------------------------

void loop() {
  // Actualizar servo continuamente
  updateServo();
  
  // Leer valores del receptor
  readIBusChannels();
  
  // Controlar laser
  if (Channel_6 > 1500) { // Umbral en el centro del rango
    digitalWrite(Laser, HIGH);
  } else {
    digitalWrite(Laser, LOW);
  }
  
  // Verificar golpes
  checkHit();
  
  // Control principal si hay vidas
  if (lives > 0) {
    if (Channel_5 > 1500) { // ARMADO de los motores
      digitalWrite(ENABLE, HIGH);
      
      // Aplicar movimiento
      vector_movement(Channel_2, Channel_1, Channel_4);
      
      // Actualizar posición del servo
      setServoAngle((int)Channel_3);
    } else {
      digitalWrite(ENABLE, LOW);
      // Detener todos los motores
      set_motor_speed(0, 0);
      set_motor_speed(1, 0);
      set_motor_speed(2, 0);
      set_motor_speed(3, 0);
    }
  } else {
    // Sin vidas - desactivar todo
    digitalWrite(ENABLE, LOW);
    digitalWrite(Laser, LOW);
    set_motor_speed(0, 0);
    set_motor_speed(1, 0);
    set_motor_speed(2, 0);
    set_motor_speed(3, 0);
    
    // LED de alerta
    digitalWrite(LED_Golpe, HIGH);
    delay(1000);
    digitalWrite(LED_Golpe, LOW);
    delay(1000);
  }
}
