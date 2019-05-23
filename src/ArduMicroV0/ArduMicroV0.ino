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

// ------------------------------------------------------------------------------------------
// Distancia entre es extremo de los ejes y el centro
// ------------------------------------------------------------------------------------------

float arms_size = 140;

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

int Channel_1;  //Ahead and back pin
int Channel_2;  //Left and Right pin
int Channel_3;  //Rotation pin
//int Channel_4;  //Shooter Up-Down
int Channel_5;  //ENABLE

// ------------------------------------------------------------------------------------------
// DEFINICION de VARIABLES AUXILIARES
// ------------------------------------------------------------------------------------------

float radio_ruedas = 50;
float distacia_eje = 200;

float speed_0 = 0;
float speed_1 = 0;
float speed_2 = 0;

float theta = 0;


void setup(){

  Serial.begin(115200);
  IBus.begin(Serial1);

  pinMode(ENABLE,OUTPUT);
  digitalWrite (ENABLE, HIGH);

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
  //Lectura
  
  IBus.loop();
  
  Channel_1 = IBus.readChannel(0) - 1000; //Valores entre 0 y 1000
  Channel_2 = IBus.readChannel(1) - 1000; //Valores entre 0 y 1000
  Channel_3 = IBus.readChannel(2) - 1000; //Valores entre 0 y 1000
  //Channel_4 = IBus.readChannel(3) - 1000; //Valores entre 0 y 1000
  Channel_5 = IBus.readChannel(4) - 1000; //Valores entre 0 y 1000

  Serial.println(Channel_5);

  if (Channel_5 > 500){ //"ARMADO" de los motores
  
    if(Channel_1>500){
      //Channel_1 = map(Channel_1, 500, 1000, 0, 500);
      Channel_1 = map(Channel_1, 500, 1000, 0, 1.00);
      digitalWrite(MotorA1, HIGH);
      digitalWrite(MotorA2, LOW);
    } else {
      //Channel_1 = map(Channel_1, 500, 0, 0, 500);
      Channel_1 = map(Channel_1, 500, 0, 0, -1.00);
      digitalWrite(MotorA1, LOW);
      digitalWrite(MotorA2, HIGH);
    }

    if(Channel_2>500){
      //Channel_2 = map(Channel_2, 500, 1000, 0, 500);
      Channel_2 = map(Channel_2, 500, 1000, 0, 1.00);
      digitalWrite(MotorB1, HIGH);
      digitalWrite(MotorB2, LOW);
    } else {
      //Channel_2 = map(Channel_2, 500, 0, 0, 500);
      Channel_2 = map(Channel_2, 500, 0, 0, -1.00);
      digitalWrite(MotorB1, LOW);
      digitalWrite(MotorB2, HIGH);
    }

    if(Channel_3>500){
      //Channel_3 = map(Channel_3, 500, 1000, 0, 360);
      Channel_3 = map(Channel_3, 500, 1000, 0, 1.00);
      digitalWrite(MotorC1, HIGH);
      digitalWrite(MotorC2, LOW);
    } else {
      //Channel_3 = map(Channel_3, 500, 0, 0, 360);
      Channel_3 = map(Channel_3, 500, 0, 0, -1.00);
      digitalWrite(MotorC1, LOW);
      digitalWrite(MotorC2, HIGH);
    }

  Serial.println(Channel_1);
  Serial.println(Channel_2);
  Serial.println(Channel_3);

  //Escritura
  //vector_movement(Channel_1, Channel_2, Channel_3);
  combine_movements(Channel_1, Channel_2, Channel_3);
  }
}
  

// ------------------------------------------------------------------------------------------
// Convert the movement in (X, Y, W) to wheels speed.
// ------------------------------------------------------------------------------------------

/*
void vector_movement(float X, float Y, float W){
  speed_0 = ((-sin(theta)*X) + (cos(theta)*Y) + (distacia_eje*W))/ radio_ruedas;                //Frontal
  speed_1 = ((-sin((PI/3)-theta)*X) + (-cos((PI/3)-theta)*Y) + (distacia_eje*W))/ radio_ruedas; //Izquierda
  speed_2 = ((sin((PI/3)+theta)*X) + (-cos((PI/3)+theta)*Y) + (distacia_eje*W))/ radio_ruedas;  //Derecha

  set_speed(0, speed_0);
  set_speed(1, speed_1);
  set_speed(2, speed_2);
  }

void set_speed(int motor, float spd) {
  spd = map(spd, 0, 1500, 0, 255);
  switch (motor) {
    case 0:
      analogWrite(PWMA, spd);
    case 1:
      analogWrite(PWMB, spd);
    case 2:
      analogWrite(PWMC, spd);
  }
}
*/

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

void set_speed(int motor, float spd, float norm) {
  int dir = 1;
  if (spd < 0) dir = 0;
  spd = map(abs(spd / norm) * 100, 0.0, 100.0, 0, 255);
    switch (motor) {
    case 0:
      analogWrite(PWMA, spd);
    case 1:
      analogWrite(PWMB, spd);
    case 2:
      analogWrite(PWMC, spd);
    }
}
