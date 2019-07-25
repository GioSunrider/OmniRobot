/*
 * Designed by @GioSunrider
 * TFG OmniRobot V0.1
 * CC-BY-SA
 *
 */

 // ------------------------------------------------------------------------------------------
 //    Esquema Coche
 // ------------------------------------------------------------------------------------------
 //      ____________
 // A | |           | |B
 //     |           |
 //     |           |
 // C | |___________| | D

// ------------------------------------------------------------------------------------------
// Librerias
// ------------------------------------------------------------------------------------------

#include <Arduino.h>
#include <math.h>
#include <FlySkyIBus.h>

// ------------------------------------------------------------------------------------------
// Distancia entre es extremo de los ejes y el centro
// Declaracion de Variable Auxiliares
// ------------------------------------------------------------------------------------------

const float arms_size = 110; //mm

float speed_A = 0;
float speed_B = 0;
float speed_C = 0;
float speed_D = 0;

// ------------------------------------------------------------------------------------------
// DEFINICION PINES Pololu Dual MC33926 Motor Driver Shield
//              ARDUINO PRO MICRO
// ------------------------------------------------------------------------------------------

#define ENABLE  2

#define MotorA1 4
#define MotorA2 7

#define MotorB1 8
#define MotorB2 16

#define MotorC1 14
#define MotorC2 15

#define MotorD1 18 //A0
#define MotorD2 19 //A1

#define PWM_A    10
#define PWM_B    9
#define PWM_C    5
#define PWM_D    6

// ------------------------------------------------------------------------------------------
//            DEFINICION PINES EXTRAS
//              ARDUINO PRO MICRO
// ------------------------------------------------------------------------------------------

//#define PWM_Servo    3
//#define DISPARO_IN   21 //A3

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

// ------------------------------------------------------------------------------------------
// Habilitacion de los Pines
// ------------------------------------------------------------------------------------------

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

  //pinMode(PWM_Servo,OUTPUT);
  //pinMode(DISPARO_IN,INPUT);

  Serial.begin(9600);
}

// ------------------------------------------------------------------------------------------
// Convert the movement in (X, Y, W) to wheels speed.
// ------------------------------------------------------------------------------------------

void set_speed(int motor, float spd) {
  if (spd > 0){
    digitalWrite(MotorA1, HIGH);
    digitalWrite(MotorA2, LOW);

    digitalWrite(MotorB1, HIGH);
    digitalWrite(MotorB2, LOW);

    digitalWrite(MotorC1, HIGH);
    digitalWrite(MotorC2, LOW);

    digitalWrite(MotorD1, HIGH);
    digitalWrite(MotorD2, LOW);

    spd = map(spd, 0, 30, 0, 255);
    switch (motor) {
      case 0:
        analogWrite(PWM_A, spd);
        break;
      case 1:
        analogWrite(PWM_B, spd);
        break;
      case 2:
        analogWrite(PWM_C, spd);
        break;
      case 3:
        analogWrite(PWM_D, spd);
        break;
     }
  }

  if (spd < 0){
    digitalWrite(MotorA1, LOW);
    digitalWrite(MotorA2, HIGH);

    digitalWrite(MotorB1, LOW);
    digitalWrite(MotorB2, HIGH);

    digitalWrite(MotorC1, LOW);
    digitalWrite(MotorC2, HIGH);

    digitalWrite(MotorD1, LOW);
    digitalWrite(MotorD2, HIGH);

  spd = map(spd, 0, 30, 0, 255);
    switch (motor) {
      case 0:
        analogWrite(PWM_A, spd);
        break;
      case 1:
        analogWrite(PWM_B, spd);
        break;
      case 2:
        analogWrite(PWM_C, spd);
        break;
      case 3:
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

  Serial.print("speed_A: ");
  Serial.print(speed_A);
  Serial.print(" speed_B: ");
  Serial.print(speed_B);
  Serial.print(" speed_C: ");
  Serial.print(speed_C);
  Serial.print(" speed_D: ");
  Serial.println(speed_D);

  set_speed(0, speed_A);
  set_speed(1, speed_B);
  set_speed(2, speed_D);
  set_speed(3, speed_C);
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

Channel_1 = IBus.readChannel(0) - 1000; //Valores entre 0 y 1000
Channel_2 = IBus.readChannel(1) - 1000; //Valores entre 0 y 1000
Channel_3 = IBus.readChannel(2) - 1000; //Valores entre 0 y 1000
Channel_4 = IBus.readChannel(3) - 1000; //Valores entre 0 y 1000
Channel_5 = IBus.readChannel(4) - 1000; //Valores entre 0 y 1000

Serial.print("Channel_5: ");
Serial.println(Channel_5);

  if (Channel_5 > 500) //"ARMADO" de los motores
  {

    digitalWrite (ENABLE, HIGH);

    if(Channel_1>500){

      Channel_1 = map(Channel_1, 500, 1000, 0, 500);

    } else {

      Channel_1 = map(Channel_1, 500, 0, 0, 500);
      Channel_1 = -Channel_1;

    }

    if(Channel_2>500){

      Channel_2 = map(Channel_2, 500, 1000, 0, 500);

    } else {

      Channel_2 = map(Channel_2, 500, 0, 0, 500);
      Channel_2 = -Channel_2;

    }

    if(Channel_4>500){

      Channel_4 = map(Channel_4, 500, 1000, 0, 5);

    } else {

      Channel_4 = map(Channel_4, 500, 0, 0, 5);
      Channel_4 = -Channel_4;

    }

    Serial.print("Channel_1: ");
    Serial.print(Channel_1);
    Serial.print(" Channel_2: ");
    Serial.print(Channel_2);
    Serial.print(" Channel_4: ");
    Serial.println(Channel_4);

  //Escritura
  vector_movement(Channel_1, Channel_2, Channel_4);

  }
}
