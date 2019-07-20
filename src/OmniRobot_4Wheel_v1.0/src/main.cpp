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
// Habilitacion de los Pines
// ------------------------------------------------------------------------------------------

void setup(){

  IBus.begin(Serial3);


  Serial.begin(9600);
}
// ------------------------------------------------------------------------------------------
// Convert the movement in (X, Y, W) to wheels speed.
// ------------------------------------------------------------------------------------------

void set_speed(int motor, float spd) {
  if (spd > 0){

    spd = map(spd, 0, 30, 0, 255);
    switch (motor) {
      case 0:

      case 1:

      case 2:

     }
  }

  if (spd < 0){

  spd = map(spd, 0, 30, 0, 255);
    switch (motor) {
      case 0:

      case 1:

      case 2:

    }
  }
}

void vector_movement(float X, float Y, float W)
{

  speed_0 =                          //Frontal
  speed_1 =     //Izquierda
  speed_2 =  //Derecha

  Serial.print("speed_0: ");
  Serial.print(speed_0);
  Serial.print(" speed_1: ");
  Serial.print(speed_0);
  Serial.print(" speed_2: ");
  Serial.println(speed_0);

  set_speed(0, speed_0);
  set_speed(1, speed_1);
  set_speed(2, speed_2);
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
