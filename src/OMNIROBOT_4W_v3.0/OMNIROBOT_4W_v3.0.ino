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

#define analogInPin A3       // Pin analogico 1 para la lectura del Fototransistor
#define Servo_Laser 3        // Pin para el servo laser
#define LED_Golpe A2         //Led que ilumina cuando ha recibido un golpe
#define Laser 1              // Control del Laser

// ------------------------------------------------------------------------------------------
// DEFINICION PINES Entrada iBus
//       Emisora Flysky FS-i6
//      Receptor Flysky FS-X6B
// ------------------------------------------------------------------------------------------

float Channel_1;  //Ahead and back pin
float Channel_2;  //Left and Right pin
float Channel_3;  //Shooter Up-Down
float Channel_4;  //Rotation pin
float Channel_5;  //ENABLE
float Channel_6;  //Laser

// ------------------------------------------------------------------------------------------
// Distancia entre es extremo de los ejes y el centro
// Declaracion de Variable Auxiliares
// ------------------------------------------------------------------------------------------

const float sqrt3o2 = sqrt(3)/2;
const float arms_size = 120; //mm
const float wheel_radius = 74/2; //mm

float speed_A = 0;
float speed_B = 0;
float speed_C = 0;
float speed_D = 0;

const int thresholdMIN = 90;  //Valor Umbral Minimo de Recepcion del fotodiodo
const int thresholdMAX = 120; //Valor Umbral Maximo de Recepcion del fotodiodo

int sensorValue = 0;        // Inicializamos el valor del sensor

int lives = 10;             //umero de "vidas" que tiene

// ------------------------------------------------------------------------------------------
// Setup
// -----------------------------------------------------------------------------------------

void setup(){

  IBus.begin(Serial1);

  pinMode(ENABLE,OUTPUT);

  pinMode(PWM_A,OUTPUT);
  pinMode(PWM_B,OUTPUT);
  pinMode(PWM_C,OUTPUT);
  pinMode(PWM_D,OUTPUT);

  pinMode(MotorA1,OUTPUT);
  pinMode(MotorA2,OUTPUT);

  pinMode(MotorB1,OUTPUT);
  pinMode(MotorB2,OUTPUT);

  pinMode(MotorC1,OUTPUT);
  pinMode(MotorC2,OUTPUT);

  pinMode(MotorD1,OUTPUT);
  pinMode(MotorD2,OUTPUT);
  
  pinMode(analogInPin, INPUT);
  pinMode(LED_Golpe,OUTPUT);
  pinMode(Laser,OUTPUT);
}

// ------------------------------------------------------------------------------------------
// Convert the movement in (X, Y, W) to wheels speed.
// ------------------------------------------------------------------------------------------

void set_speed(int motor, float spd) {
  if (spd > 0){
    spd = map(spd, 0, 200, 0, 100);
    switch (motor) {
      case 0:
        digitalWrite(MotorA1, HIGH);
        digitalWrite(MotorA2, LOW);
        analogWrite(PWM_A, spd);
        break;
      case 1:
        digitalWrite(MotorB1, HIGH);
        digitalWrite(MotorB2, LOW);
        analogWrite(PWM_B, spd);
        break;
      case 2:
        digitalWrite(MotorC1, HIGH);
        digitalWrite(MotorC2, LOW);
        analogWrite(PWM_C, spd);
        break;
      case 3:
        digitalWrite(MotorD1, HIGH);
        digitalWrite(MotorD2, LOW);
        analogWrite(PWM_D, spd);
        break;
     }
  }

  if (spd < 0){
  spd = map(spd, 0, -200, 0, 100);
    switch (motor) {
      case 0:
        digitalWrite(MotorA1, LOW);
        digitalWrite(MotorA2, HIGH);
        analogWrite(PWM_A, spd);
        break;
      case 1:
        digitalWrite(MotorB1, LOW);
        digitalWrite(MotorB2, HIGH);
        analogWrite(PWM_B, spd);
        break;
      case 2:
        digitalWrite(MotorC1, LOW);
        digitalWrite(MotorC2, HIGH);
        analogWrite(PWM_C, spd);
        break;
      case 3:
        digitalWrite(MotorD1, LOW);
        digitalWrite(MotorD2, HIGH);
        analogWrite(PWM_D, spd);
        break;
    }
  }
}

void vector_movement(float X, float Y, float W)
{

  speed_A = ((Y)+(arms_size*W));
  speed_B = ((-X)+(arms_size*W));
  speed_C = ((-Y)+(arms_size*W));
  speed_D = ((X)+(arms_size*W));

  set_speed(0, speed_A);
  set_speed(1, speed_B);
  set_speed(2, speed_C);
  set_speed(3, speed_D);
}

void moverServo (int angulo)
{
   float pausa;
   pausa = angulo*2000.0/180.0 + 250;
   digitalWrite(Servo_Laser, HIGH);
   delayMicroseconds(pausa);
   digitalWrite(Servo_Laser, LOW);
   delayMicroseconds(2500-pausa);
}

// ------------------------------------------------------------------------------------------
// Comienzo de los ciclos del programa
// ------------------------------------------------------------------------------------------

void loop() {

// ------------------------------------------------------------------------------------------
// Se lee cada canal via Ibus, se establece valores entre 0 y 1000 y en funcion de la
// posicion del joystick se establece un valor de -1 a 1. Para ello se mapea el valor entre
// 0 y 100 y posteriormente se divice entre 100 y -100 para dar el valor en el rango deseado.
// ------------------------------------------------------------------------------------------

IBus.loop();

Channel_1 = IBus.readChannel(0) - 1000; //Valores entre 0 y 1000 //Y
Channel_2 = IBus.readChannel(1) - 1000; //Valores entre 0 y 1000 //X
Channel_3 = IBus.readChannel(2) - 1000; //Valores entre 0 y 1000 //Servo
Channel_4 = IBus.readChannel(3) - 1000; //Valores entre 0 y 1000 //W
Channel_5 = IBus.readChannel(4) - 1000; //Valores entre 0 y 1000 //ENABLE
Channel_6 = IBus.readChannel(5) - 1000; //Valores entre 0 y 1000 //Laser

//Encemos y apagamos laser
if (Channel_6 > 500){
  digitalWrite(Laser, HIGH);
} else {
  digitalWrite(Laser, LOW);
}

// leemos el pin para y asignamos el valor a la variable.
sensorValue = analogRead(analogInPin);

// Si el valor obtenido es mayor a 900 se activa el LED
if(sensorValue > thresholdMIN && sensorValue < thresholdMAX )
  {
    lives = lives - 1;
    digitalWrite(LED_Golpe, HIGH);
    delay (500);
    digitalWrite(LED_Golpe, LOW);
  }

if (lives>0){

    if (Channel_5 > 500) //"ARMADO" de los motores
    {

      digitalWrite (ENABLE, HIGH);

      if(Channel_1>500){

        Channel_1 = map(Channel_1, 500, 1000, 0, 100);

      } else {

        Channel_1 = map(Channel_1, 500, 0, 0, 100);
        Channel_1 = -Channel_1;

      }

      if(Channel_2>500){

        Channel_2 = map(Channel_2, 500, 1000, 0, 100);

      } else {

        Channel_2 = map(Channel_2, 500, 0, 0, 100);
        Channel_2 = -Channel_2;

      }

      if(Channel_4>500){

        Channel_4 = map(Channel_4, 500, 1000, 0, 10);

      } else {

        Channel_4 = map(Channel_4, 500, 0, 0, 10);
        Channel_4 = -Channel_4;

      }

    //Escritura
    vector_movement(Channel_2, Channel_1, Channel_4);
    Channel_3 = map(Channel_3, 0, 1000, 0, 180);
    moverServo(Channel_3);
  
    } else {
      digitalWrite (ENABLE, LOW);
    }
  } else{
    do{
    digitalWrite (ENABLE, LOW);
    }while (lives==0);
   }
}
