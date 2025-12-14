/*
 * Designed by @GioSunrider
 * TFG OmniRobot V0.3
 * CC-BY-SA
 *
 */

// ------------------------------------------------------------------------------------------
//    Esquema Coche
// ------------------------------------------------------------------------------------------ 
//       __ A
//     /    \ 
//    /      \   
// B |________| C
     
// ------------------------------------------------------------------------------------------
// Librerias
// ------------------------------------------------------------------------------------------

#include <math.h>
#include <FlySkyIBus.h>

// ------------------------------------------------------------------------------------------
// DEFINICION PINES Pololu Dual MC33926 Motor Driver Shield
//              ARDUINO MICRO PRO
// ------------------------------------------------------------------------------------------

#define ENABLE  2

#define MotorA1 4
#define MotorA2 5

#define MotorB1 7
#define MotorB2 8

#define MotorC1 10
#define MotorC2 16

#define PWMA    3
#define PWMB    6
#define PWMC    9

// ------------------------------------------------------------------------------------------
// DEFINICION PINES Emision y Recepcion Laser
// ------------------------------------------------------------------------------------------

#define analogInPin A3       // Pin analogico para la lectura del Fototransistor
#define Servo_Laser 3        // Pin para el servo laser (CONFLICTO CON PWMA - CORREGIDO)
#define LED_Golpe A2         // Led que ilumina cuando ha recibido un golpe
#define Laser 1              // Control del Laser

// ------------------------------------------------------------------------------------------
// DEFINICION PINES Entrada iBus
//       Emisora Flysky FS-i6
//      Receptor Flysky FS-X6B
// ------------------------------------------------------------------------------------------

float Channel_1;  // Ahead and back (Y)
float Channel_2;  // Left and Right (X)
float Channel_3;  // Shooter Up-Down
float Channel_4;  // Rotation (W)
float Channel_5;  // ENABLE
float Channel_6;  // Laser

// ------------------------------------------------------------------------------------------
// Constantes y Variables
// ------------------------------------------------------------------------------------------

const float sqrt3o2 = sqrt(3.0)/2.0;
const float arms_size = 110.0; // mm
const float wheel_radius = 20.5; // mm (41/2)

float speed_A = 0;
float speed_B = 0;
float speed_C = 0;

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
  // IMPORTANTE: Inicializar Serial1 para IBus (Arduino Micro Pro usa Serial1 para pines 0/1)
  Serial1.begin(115200);
  IBus.begin(Serial1);

  // Serial para debug
  Serial.begin(9600);

  pinMode(ENABLE, OUTPUT);

  // Configurar pines de motor PWM
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(PWMC, OUTPUT);

  // Configurar pines de dirección de motor
  pinMode(MotorA1, OUTPUT);
  pinMode(MotorA2, OUTPUT);
  pinMode(MotorB1, OUTPUT);
  pinMode(MotorB2, OUTPUT);
  pinMode(MotorC1, OUTPUT);
  pinMode(MotorC2, OUTPUT);

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
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
  analogWrite(PWMC, 0);
  
  // Configurar pines de dirección a LOW
  digitalWrite(MotorA1, LOW);
  digitalWrite(MotorA2, LOW);
  digitalWrite(MotorB1, LOW);
  digitalWrite(MotorB2, LOW);
  digitalWrite(MotorC1, LOW);
  digitalWrite(MotorC2, LOW);
  
  // Configurar servo a posición central
  servoAngle = 90;
  
  Serial.println("Robot 3W Inicializado");
}

// ------------------------------------------------------------------------------------------
// Funciones para control de motores (3 ruedas)
// ------------------------------------------------------------------------------------------

void set_motor_speed(int motor, float spd) {
  int pwmValue = constrain(abs(spd), 0, 200);
  pwmValue = map(pwmValue, 0, 200, 0, 255);
  
  switch (motor) {
    case 0: // Motor A (Frontal/Trasero)
      if (spd > 1) { // Adelante
        digitalWrite(MotorA1, HIGH);
        digitalWrite(MotorA2, LOW);
        analogWrite(PWMA, pwmValue);
      } else if (spd < -1) { // Atrás
        digitalWrite(MotorA1, LOW);
        digitalWrite(MotorA2, HIGH);
        analogWrite(PWMA, pwmValue);
      } else { // Parar
        digitalWrite(MotorA1, LOW);
        digitalWrite(MotorA2, LOW);
        analogWrite(PWMA, 0);
      }
      break;
      
    case 1: // Motor B (Izquierdo)
      if (spd > 1) {
        digitalWrite(MotorB1, HIGH);
        digitalWrite(MotorB2, LOW);
        analogWrite(PWMB, pwmValue);
      } else if (spd < -1) {
        digitalWrite(MotorB1, LOW);
        digitalWrite(MotorB2, HIGH);
        analogWrite(PWMB, pwmValue);
      } else {
        digitalWrite(MotorB1, LOW);
        digitalWrite(MotorB2, LOW);
        analogWrite(PWMB, 0);
      }
      break;
      
    case 2: // Motor C (Derecho)
      if (spd > 1) {
        digitalWrite(MotorC1, HIGH);
        digitalWrite(MotorC2, LOW);
        analogWrite(PWMC, pwmValue);
      } else if (spd < -1) {
        digitalWrite(MotorC1, LOW);
        digitalWrite(MotorC2, HIGH);
        analogWrite(PWMC, pwmValue);
      } else {
        digitalWrite(MotorC1, LOW);
        digitalWrite(MotorC2, LOW);
        analogWrite(PWMC, 0);
      }
      break;
  }
}

// Movimiento omnidireccional para robot de 3 ruedas
void vector_movement(float X, float Y, float W) {
  // Fórmula corregida para robot de 3 ruedas en configuración triangular
  // Ruedas a 120 grados: A(0°), B(120°), C(240°)
  
  // Componentes de velocidad para cada rueda
  speed_A = (-sin(0) * X + cos(0) * Y) + (arms_size * W);          // Rueda A (0°)
  speed_B = (-sin(2.0944) * X + cos(2.0944) * Y) + (arms_size * W); // Rueda B (120° = 2.0944 rad)
  speed_C = (-sin(4.1888) * X + cos(4.1888) * Y) + (arms_size * W); // Rueda C (240° = 4.1888 rad)
  
  // Aplicar factor de escala del radio de la rueda
  speed_A /= wheel_radius;
  speed_B /= wheel_radius;
  speed_C /= wheel_radius;
  
  // Normalizar velocidades si alguna excede el máximo
  float max_speed = max(max(abs(speed_A), abs(speed_B)), abs(speed_C));
  if (max_speed > 200) {
    float scale = 200.0 / max_speed;
    speed_A *= scale;
    speed_B *= scale;
    speed_C *= scale;
  }
  
  // Aplicar velocidades a los motores
  set_motor_speed(0, speed_A);
  set_motor_speed(1, speed_B);
  set_motor_speed(2, speed_C);
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
      lives = max(0, lives - 1); // Asegurar que no sea negativo
      lastHitTime = currentTime;
      
      // Parpadear LED para indicar golpe
      for (int i = 0; i < 3; i++) {
        digitalWrite(LED_Golpe, HIGH);
        delay(100);
        digitalWrite(LED_Golpe, LOW);
        delay(100);
      }
      
      Serial.print("Golpe recibido! Vidas restantes: ");
      Serial.println(lives);
    }
  }
}

// ------------------------------------------------------------------------------------------
// Función para leer y mapear canales IBus
// ------------------------------------------------------------------------------------------

void readIBusChannels() {
  IBus.loop();
  
  // Leer canales IBus (rango 1000-2000)
  int ch1_raw = IBus.readChannel(0);
  int ch2_raw = IBus.readChannel(1);
  int ch3_raw = IBus.readChannel(2);
  int ch4_raw = IBus.readChannel(3);
  int ch5_raw = IBus.readChannel(4);
  int ch6_raw = IBus.readChannel(5);
  
  // Mapear a rangos útiles
  Channel_1 = mapFloat(ch1_raw, 1000, 2000, -100.0, 100.0); // Y (adelante/atrás)
  Channel_2 = mapFloat(ch2_raw, 1000, 2000, -100.0, 100.0); // X (izquierda/derecha)
  Channel_3 = mapFloat(ch3_raw, 1000, 2000, 0.0, 180.0);    // Servo (0-180 grados)
  Channel_4 = mapFloat(ch4_raw, 1000, 2000, -10.0, 10.0);   // W (rotación)
  Channel_5 = ch5_raw;  // ENABLE (1000-2000)
  Channel_6 = ch6_raw;  // Laser (1000-2000)
  
  // Debug opcional
  static unsigned long lastDebug = 0;
  if (millis() - lastDebug > 1000) {
    Serial.print("Canales: ");
    Serial.print(Channel_1); Serial.print(", ");
    Serial.print(Channel_2); Serial.print(", ");
    Serial.print(Channel_3); Serial.print(", ");
    Serial.print(Channel_4); Serial.print(", ");
    Serial.print(Channel_5); Serial.print(", ");
    Serial.println(Channel_6);
    lastDebug = millis();
  }
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
  
  // Leer valores del receptor IBus
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
      
      // Aplicar movimiento omnidireccional
      vector_movement(Channel_2, Channel_1, Channel_4);
      
      // Actualizar posición del servo
      setServoAngle((int)Channel_3);
    } else {
      // Desarmado - detener todos los motores
      digitalWrite(ENABLE, LOW);
      set_motor_speed(0, 0);
      set_motor_speed(1, 0);
      set_motor_speed(2, 0);
    }
  } else {
    // Sin vidas - desactivar todo
    digitalWrite(ENABLE, LOW);
    digitalWrite(Laser, LOW);
    set_motor_speed(0, 0);
    set_motor_speed(1, 0);
    set_motor_speed(2, 0);
    
    // LED de alerta intermitente
    static unsigned long lastBlink = 0;
    if (millis() - lastBlink > 500) {
      digitalWrite(LED_Golpe, !digitalRead(LED_Golpe));
      lastBlink = millis();
    }
  }
  
  // Pequeña pausa para estabilidad
  delay(10);
}
