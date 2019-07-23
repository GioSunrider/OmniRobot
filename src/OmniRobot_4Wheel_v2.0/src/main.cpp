/*
 * Designed by @GioSunrider
 * TFG OmniRobot V0.1
 * CC-BY-SA
 *
 */

// ------------------------------------------------------------------------------------------
// Librerias
// ------------------------------------------------------------------------------------------

#include <Arduino.h>
#include <math.h>
#include <FlySkyIBus.h>
#include <AFMotor.h>

// ------------------------------------------------------------------------------------------
// Distancia entre es extremo de los ejes y el centro
// Declaracion de Variable Auxiliares
// ------------------------------------------------------------------------------------------

const float arms_size = 70; //mm

float speed_1 = 0;
float speed_2 = 0;
float speed_3 = 0;
float speed_4 = 0;

// ------------------------------------------------------------------------------------------
//       Emisora Flysky FS-i6
//      Receptor Flysky FS-X6B
// ------------------------------------------------------------------------------------------

float Channel_1;  //Ahead and back pin
float Channel_2;  //Left and Right pin
float Channel_3;  //Shooter Up-Down
float Channel_4;  //Rotation pin
float Channel_5;  //ENABLE

// ------------------------------------------------------------------------------------------
// Declaracion de los motores
// ------------------------------------------------------------------------------------------

AF_DCMotor LeftFrontWheel(1, MOTOR12_64KHZ);
AF_DCMotor RightFrontWheel(2, MOTOR12_64KHZ);
AF_DCMotor LeftBackWheel(3, MOTOR34_64KHZ);
AF_DCMotor RightBackWheel(4, MOTOR34_64KHZ);

// ------------------------------------------------------------------------------------------
// Habilitacion de los Pines
// ------------------------------------------------------------------------------------------

void setup(){

  IBus.begin(Serial3);


  Serial.begin(9600);
}
// ------------------------------------------------------------------------------------------
// Convert the movement in (X, Y, W) to wheels speed.
// ------------------------------------------------------------------------------------------

void set_speed(int motor, float spd, float norm) {
  if (spd > 0){
    spd = map(spd, 0, 30, 0, 255);
    switch (motor) {
      case 0:
        LeftFrontWheel.speed(spd);
        LeftFrontWheel.run(FORWARD);
        break;
      case 1:
        RightFrontWheel.speed(spd);
        RightFrontWheel.run(FORWARD);
        break;
      case 2:
        RightBackWheel.speed(spd);
        RightBackWheel.run(FORWARD);
        break;
      case 3:
        LeftBackWheel.speed(spd);
        LeftBackWheel.run(FORWARD);
        break;
     }
  }

  if (spd < 0){
  spd = map(abs(spd / norm) * 100, 0.0, 100.0, 0, 255);
    switch (motor) {
      case 0:
        LeftFrontWheel.speed(spd);
        LeftFrontWheel.run(BACKWARD);
        break;
      case 1:
        RightFrontWheel.speed(spd);
        RightFrontWheel.run(BACKWARD);
        break;
      case 2:
        RightBackWheel.speed(spd);
        RightBackWheel.run(BACKWARD);
        break;
      case 3:
        LeftBackWheel.speed(spd);
        LeftBackWheel.run(BACKWARD);
        break;

    }
  }
}

void vector_movement(float X, float Y, float W) {
  speed_0 = ((Y)+(arms_size*W));
  speed_1 = ((-X)+(arms_size*W));
  speed_2 = ((-Y)+(arms_size*W));
  speed_3 = ((X)+(arms_size*W)

  set_speed(0, speed_0, norm);
  set_speed(1, speed_1, norm);
  set_speed(2, speed_2, norm);
  set_speed(3, speed_2, norm);
}
/*
  void moveForward() {
    LeftFrontWheel.run(FORWARD);
    LeftBackWheel.run(FORWARD);
    RightFrontWheel.run(FORWARD);
    RightBackWheel.run(FORWARD);
  }
  void moveBackward() {
    LeftFrontWheel.run(BACKWARD);
    LeftBackWheel.run(BACKWARD);
    RightFrontWheel.run(BACKWARD);
    RightBackWheel.run(BACKWARD);
  }
  void moveSidewaysRight() {
    LeftFrontWheel.run(FORWARD);
    LeftBackWheel.run(BACKWARD);
    RightFrontWheel.run(BACKWARD);
    RightBackWheel.run(FORWARD);
  }
  void moveSidewaysLeft() {
    LeftFrontWheel.run(BACKWARD);
    LeftBackWheel.run(FORWARD);
    RightFrontWheel.run(FORWARD);
    RightBackWheel.run(BACKWARD);
  }
  void rotateLeft() {
    LeftFrontWheel.run(BACKWARD);
    LeftBackWheel.run(BACKWARD);
    RightFrontWheel.run(FORWARD);
    RightBackWheel.run(FORWARD);
  }
  void rotateRight() {
    LeftFrontWheel.run(FORWARD);
    LeftBackWheel.run(FORWARD);
    RightFrontWheel.run(BACKWARD);
    RightBackWheel.run(BACKWARD);
  }
  void moveRightForward() {
    LeftFrontWheel.run(FORWARD);
    LeftBackWheel.run(RELEASE);
    RightFrontWheel.run(RELEASE);
    RightBackWheel.run(FORWARD);
  }
  void moveRightBackward() {
    LeftFrontWheel.run(RELEASE);
    LeftBackWheel.run(BACKWARD);
    RightFrontWheel.run(BACKWARD);
    RightBackWheel.run(RELEASE);
  }
  void moveLeftForward() {
    LeftFrontWheel.run(RELEASE);
    LeftBackWheel.run(FORWARD);
    RightFrontWheel.run(FORWARD);
    RightBackWheel.run(RELEASE);
  }
  void moveLeftBackward() {
    LeftFrontWheel.run(BACKWARD);
    LeftBackWheel.run(RELEASE);
    RightFrontWheel.run(RELEASE);
    RightBackWheel.run(BACKWARD);
  }
  void stopMoving() {
    LeftFrontWheel.run(RELEASE);
    LeftBackWheel.run(RELEASE);
    RightFrontWheel.run(RELEASE);
    RightBackWheel.run(RELEASE);
  }
*/

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
