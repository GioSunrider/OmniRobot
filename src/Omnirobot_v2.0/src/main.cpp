/*
 * Designed by @GioSunrider
 * TFG OmniRobot V0.1
 * CC-BY-SA
 *
 */

// ------------------------------------------------------------------------------------------
// Librerias
// ------------------------------------------------------------------------------------------

#include <FlySkyIBus.h>
#include <Arduino.h>

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
// DEFINICION de VARIABLES AUXILIARES
// Distancia entre es extremo de los ejes y el centro
// ------------------------------------------------------------------------------------------

float radio_ruedas = 50;
float distacia_eje = 200;

float speed_0 = 0;
float speed_1 = 0;
float speed_2 = 0;

// ------------------------------------------------------------------------------------------
// Convert the movement in (X, Y, W) to wheels speed.
// ------------------------------------------------------------------------------------------
void set_speed(int motor, float spd, float norm) {
  if (spd > 0){
    digitalWrite(MotorA1, HIGH);
    digitalWrite(MotorA2, LOW);

    digitalWrite(MotorB1, HIGH);
    digitalWrite(MotorB2, LOW);

    digitalWrite(MotorC1, HIGH);
    digitalWrite(MotorC2, LOW);

    spd = map(abs(spd / norm) * 100, 0.0, 100.0, 0, 255);
    switch (motor) {
      case 0:
        analogWrite(PWMA, spd);
        break;
      case 1:
        analogWrite(PWMB, spd);
        break;
      case 2:
        analogWrite(PWMC, spd);
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

  spd = map(abs(spd / norm) * 100, 0.0, 100.0, 0, 255);
    switch (motor) {
      case 0:
        analogWrite(PWMA, spd);
        break;
      case 1:
        analogWrite(PWMB, spd);
        break;
      case 2:
        analogWrite(PWMC, spd);
        break;
    }
  }
}

void combine_movements(float speed_X, float speed_Y, float speed_W) {
  speed_0 = (-speed_X * sin(PI / 3)) + (speed_Y * cos(PI / 3)) + speed_W;
  speed_1 = -speed_Y + speed_W;
  speed_2 = (speed_X * sin(PI / 3)) + (speed_Y * cos(PI / 3)) + speed_W;
  float norm = 1.0;
  if (abs(speed_0) > 1 || abs(speed_1) > 1 || abs(speed_2) > 1) {
    if (abs(speed_0) >= abs(speed_1)) {
      norm = abs(speed_0);
      if (abs(speed_2) > abs(speed_0)) norm = abs(speed_2);

    }
    else {
      norm = abs(speed_1);
      if (abs(speed_2) > abs(speed_1)) norm = abs(speed_2);
    }
  }
  set_speed(0, speed_0, norm);
  set_speed(1, speed_1, norm);
  set_speed(2, speed_2, norm);
}

void setup(){

  IBus.begin(Serial1);

  pinMode(ENABLE,OUTPUT);

  pinMode(PWMA,OUTPUT);
  pinMode(PWMB,OUTPUT);
  pinMode(PWMC,OUTPUT);

  pinMode(MotorA1,OUTPUT);
  pinMode(MotorA2,OUTPUT);

  pinMode(MotorB1,OUTPUT);
  pinMode(MotorB2,OUTPUT);

  pinMode(MotorC1,OUTPUT);
  pinMode(MotorC2,OUTPUT);

  Serial.begin(9600);
}

void loop() {

// ------------------------------------------------------------------------------------------
// Se lee cada canal via Ibus, se establece valores entre 0 y 1000 y en funcion de la
// posicion del joystick se establece un valor de -1 a 1. Para ello se mapea el valor entre
// 0 y 100 y posteriormente se divice entre 100 y -100 para dar el valor en el rango deseado.
// ------------------------------------------------------------------------------------------

  IBus.loop();

  Channel_3 = IBus.readChannel(2) - 1000; //Valores entre 0 y 1000
  Channel_5 = IBus.readChannel(4) - 1000; //Valores entre 0 y 1000

  Serial.print("5: ");
  Serial.println(Channel_5);

  if (Channel_5 > 500){ //"ARMADO" de los motores

    digitalWrite (ENABLE, HIGH);

    Channel_1 = IBus.readChannel(0) - 1000; //Valores entre 0 y 1000

    if(Channel_1>500){

      Channel_1 = map(Channel_1, 500, 1000, 0, 255);
      Channel_1 = Channel_1/1;

    } else {

      Channel_1 = map(Channel_1, 500, 0, 0, 255);
      Channel_1 = Channel_1/-1;

    }

    Channel_2 = IBus.readChannel(1) - 1000; //Valores entre 0 y 1000

    if(Channel_2>500){

      Channel_2 = map(Channel_2, 500, 1000, 0, 255);
      Channel_2 = Channel_2/1;

    } else {

      Channel_2 = map(Channel_2, 500, 0, 0, 255);
      Channel_2 = Channel_2/-1;

    }

    Channel_4 = IBus.readChannel(3) - 1000; //Valores entre 0 y 1000

    if(Channel_4>500){

      Channel_4 = map(Channel_4, 500, 1000, 0, 255);
      Channel_4 = Channel_4/1;

    } else {

      Channel_4 = map(Channel_4, 500, 0, 0, 255);
      Channel_4 = Channel_4/-100;

    }

  //Escritura
  combine_movements(Channel_1, Channel_2, Channel_4);

  }
}
