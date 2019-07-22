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

const float sqrt3o2 = sqrt(3)/2;
const float arms_size = 110; //mm
const float wheel_radius = 41/2; //mm

float speed_0 = 0;
float speed_1 = 0;
float speed_2 = 0;

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

vector_movement(Channel_1, Channel_2, Channel_4){
  if (Channel_2 > 150) {
   moveSidewaysLeft();
 }
 else if (Channel_2 < 100) {
   moveSidewaysRight();
 }
 else if (Channel_1 > 160) {
   moveForward();
 }
 else if (Channel_1 < 100) {
   moveBackward();
 }
 else if (Channel_4 < 100 & data.j2PotY > 160) {
   moveRightForward();
 }
 else if (Channel_4 > 160 & data.j2PotY > 160) {
   moveLeftForward();
 }
 else if (Channel_4 < 100 & data.j2PotY < 100) {
   moveRightBackward();
 }
 else if (Channel_4 > 160 & data.j2PotY < 100) {
   moveLeftBackward();
 }
 else if (Channel_4 < 100) {
   rotateRight();
 }
 else if (Channel_4 > 150) {
   rotateLeft();
 }
 else {
   stopMoving();
 }
}

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
