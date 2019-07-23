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

// ------------------------------------------------------------------------------------------
//    Esquema Coche
// ------------------------------------------------------------------------------------------
//      ____________
// A | |           | |B
//     |           |
//     |           |
// C | |___________| | D



 //Definir LED Real Time
 #define RT 53

 //Definir número de datos a transmitir por Bluetooth
 #define nn 4

 //Definir minimo y máximo ponderado de datos
 #define MIN -1.0
 #define MAX 1.0
 #define VMIN -10.0
 #define VMAX 10.0

 //Definir sentidos de giro
 #define CW 0
 #define CCW 1

 // Arduino pins for the shift register
 #define MOTORLATCH 12
 #define MOTORCLK 4
 #define MOTORENABLE 7
 #define MOTORDATA 8

 // 8-bit bus after the 74HC595 shift register
 // (not Arduino pins)
 // These are used to set the direction of the bridge driver.
 #define MOTOR1_A 2
 #define MOTOR1_B 3
 #define MOTOR2_A 1
 #define MOTOR2_B 4
 #define MOTOR3_A 5
 #define MOTOR3_B 7
 #define MOTOR4_A 0
 #define MOTOR4_B 6

 // Arduino pins for the PWM signals.
 #define MOTOR1_PWM 11
 #define MOTOR2_PWM 3
 #define MOTOR3_PWM 6
 #define MOTOR4_PWM 5


 // Codes for the motor function.
 #define FORWARD 1
 #define BACKWARD 2
 #define BRAKE 3
 #define RELEASE 4


 //Definir máximos y mínimos de acc y potencia
 #define MINU 0
 #define MAXU 255
 #define MINW -15.0
 #define MAXW 15.0

 //Variables de tiempo
 long t1 = 0 ;
 long t2 = 0 ;
 int T = 10 ; // Periodo de muestreo milisegundos
 float Tm = T/1000.0; // Periodo de muestreo segundos
 int N=10 ;
 int cont=0 ;

 //Variables Bluetooth
 bool flag = 0;

 //Variables datos serie
 float dato1 = 0.0 ;
 float dato2 = 0.0 ;
 float dato3 = 0.0 ;
 float dato4 = 0.0 ;
 byte leodato1 = 127.5;
 byte leodato2 = 127.5;
 byte leodato3 = 127.5;
 byte leodato4 = 127.5;

 float C2D = 360.0/4480.0; // Conversión cuentas a deg
 float C2RS = (2*3.14/4480.0)/Tm; // Conversión cuentas a rad/seg

 //Variables PI
 float err1 = 0.0; // error de la señal
 float err2 = 0.0; // error de la señal
 float err3 = 0.0; // error de la señal
 float err4 = 0.0; // error de la señal
 float Ik1 = 0.0; // Integral del error
 float Ik2 = 0.0; // Integral del error
 float Ik3 = 0.0; // Integral del error
 float Ik4 = 0.0; // Integral del error

 float Kp = 18.0; // Constante proporcional
 float Ki = 5.0; // Constante integral


 //Variables
 float accA = 0.0; //Acción de control motor A
 float accB = 0.0; //Acción de control motor B
 float accC = 0.0; //Acción de control motor C
 float accD = 0.0; //Acción de control motor D
 float velA = 0.0; //Velocidad motor A
 float velB = 0.0; //Velocidad motor B
 float velC = 0.0; //Velocidad motor C
 float velD = 0.0; //Velocidad motor D
 byte velAbyte = 0; //Velocidad motor A en bytes
 byte velBbyte = 0; //Velocidad motor B en bytes
 byte velCbyte = 0; //Velocidad motor C en bytes
 byte velDbyte = 0; //Velocidad motor D en bytes
 float posA = 0.0; //Posición motor A
 float posB = 0.0; //Posición motor B
 float posC = 0.0; //Posición motor C
 float posD = 0.0; //Posición motor D
 long quadA = 2241; //
 long quadB = 2241; //
 long quadC = 2241; //
 long quadD = 2241; //
 long enc_antA = quadA; //Encoder anterior A
 long enc_antB = quadB; //Encoder anterior B
 long enc_antC = quadC; //Encoder anterior C
 long enc_antD = quadD; //Encoder anterior D
 long enc_actA = quadA; //Encoder actual A
 long enc_actB = quadB; //Encoder actual B
 long enc_actC = quadC; //Encoder actual C
 long enc_actD = quadD; //Encoder actual D
 int enc_difA = 0; //Diferencia encoders A
 int enc_difB = 0; //Diferencia encoders B
 int enc_difC = 0; //Diferencia encoders C
 int enc_difD = 0; //Diferencia encoders D
 int dirA; //Dirección motor A
 int dirB; //Dirección motor B
 int dirC; //Dirección motor C
 int dirD; //Dirección motor D

void setup(){
 Serial.begin(115200) ;
 pinMode(RT,OUTPUT) ;

 IBus.begin(Serial3);

 //Resolución de lectura y escritura
 analogWriteResolution(8);
 analogReadResolution(8);

 delay(1000);
 }


 ///////////////////////////////////////
 ///////////// LOOP ////////////////
 ///////////////////////////////////////
void loop(){
 flag = 1;
 if(cont == N)
 {
 if (Serial1.available() >= nn)
 {
 flag = 1;

 //Lectura del Serial
 leodato1 = Serial1.read();
 leodato2 = Serial1.read();
 leodato3 = Serial1.read();
 leodato4 = Serial1.read();

 //Escribir datos en el serial
 Serial1.write(velAbyte);
 Serial1.write(velBbyte);
 Serial1.write(velCbyte);
 Serial1.write(velDbyte);

 cont = 0;
 }
 }
 else
 {
 dato1 = (leodato1*(VMAX-VMIN)/255.0)+VMIN ; //lectura de las velocidades (rad/s) y escaladas [-10...+10]
dato2 = (leodato2*(VMAX-VMIN)/255.0)+VMIN ; //lectura de las velocidades (rad/s) y escaladas [-10...+10]
dato3 = (leodato3*(VMAX-VMIN)/255.0)+VMIN ; //lectura de las velocidades (rad/s) y escaladas [-10...+10]
dato4 = (leodato4*(VMAX-VMIN)/255.0)+VMIN ; //lectura de las velocidades (rad/s) y escaladas [-10...+10]

//Cálculo velocidad motores 58
enc_actA = quadA;
enc_actB = quadB;
enc_actC = quadC;
enc_actD = quadD;
enc_difA = enc_actA - enc_antA;
enc_difB = enc_actB - enc_antB;
enc_difC = enc_actC - enc_antC;
enc_difD = enc_actD - enc_antD;
velA = (enc_actA - enc_antA)*C2RS;
velB = (enc_actB - enc_antB)*C2RS;
velC = (enc_actC - enc_antC)*C2RS;
velD = (enc_actD - enc_antD)*C2RS;
enc_antA = enc_actA;
enc_antB = enc_actB;
enc_antC = enc_actC;
enc_antD = enc_actD;

err1 = dato1 - velA; //Calculo error motor A
err2 = dato2 - velB; //Calculo error motor B
err3 = dato3 - velC; //Calculo error motor C
err4 = dato4 - velD; //Calculo error motor D

Ik1 = err1*Tm + Ik1; //Calculo integral del error
Ik2 = err2*Tm + Ik2; //Calculo integral del error
Ik3 = err3*Tm + Ik3; //Calculo integral del error
Ik4 = err4*Tm + Ik4; //Calculo integral del error

//Cálculo Acciones de Control
accA = abs(Kp*err1 + Ki*Ik1);
accB = abs(Kp*err2 + Ki*Ik2);
accC = abs(Kp*err3 + Ki*Ik3);
accD = abs(Kp*err4 + Ki*Ik4);

//Saturar acción de control
if(accA > VMAX) {accA = VMAX; digitalWrite(RT,HIGH);} if (accA < VMIN) {accA = VMIN; digitalWrite(RT,HIGH);}
if(accB > VMAX) {accB = VMAX; digitalWrite(RT,HIGH);} if (accB < VMIN) {accB = VMIN; digitalWrite(RT,HIGH);}
if(accC > VMAX) {accC = VMAX; digitalWrite(RT,HIGH);} if (accC < VMIN) {accC = VMIN; digitalWrite(RT,HIGH);}
if(accD > VMAX) {accD = VMAX; digitalWrite(RT,HIGH);} if (accD < VMIN) {accD = VMIN; digitalWrite(RT,HIGH);}

//Transformar acciones de control de [-10...10] a [0...255]
accA = (accA + VMAX)*255/(VMAX - VMIN);
accB = (accB + VMAX)*255/(VMAX - VMIN);
accC = (accC + VMAX)*255/(VMAX - VMIN);
accD = (accD + VMAX)*255/(VMAX - VMIN);

//Definir sentido de giro
if(int(dato1) > 0) dirA = FORWARD; else if(int(dato1) == 0) dirA = RELEASE ; else dirA = BACKWARD;
if(int(dato2) > 0) dirB = FORWARD; else if(int(dato2) == 0) dirB = RELEASE ; else dirB = BACKWARD;
if(int(dato3) > 0) dirC = FORWARD; else if(int(dato3) == 0) dirC = RELEASE ; else dirC = BACKWARD;

if(int(dato4) > 0) dirD = FORWARD; else if(int(dato4) == 0) dirD = RELEASE ; else dirD = BACKWARD;

//Escribir acción de control
motor(1,dirA,accA);
motor(2,dirB,accB);
motor(3,dirC,accC);
motor(4,dirD,accD);

//Transformar velocidad en bytes
velAbyte = (velA + VMAX)*255/(VMAX-VMIN); // de [-10...+10] a [0...255]
velBbyte = (velB + VMAX)*255/(VMAX-VMIN); // de [-10...+10] a [0...255]
velCbyte = (velC + VMAX)*255/(VMAX-VMIN); // de [-10...+10] a [0...255]
velDbyte = (velD + VMAX)*255/(VMAX-VMIN); // de [-10...+10] a [0...255]

cont++;

Serial.print(cont); Serial.print(" ");
Serial.print(velA); Serial.print(" ");
Serial.print(err1); Serial.print(" ");
Serial.print(accA); Serial.print(" ");
//Serial.print(velA); Serial.print(" ");
//Serial.print(velB); Serial.print(" ");
//Serial.print(velC); Serial.print(" ");
//Serial.print(velD); Serial.print(" ");
Serial.println("");
}
Serial1.flush() ;

if (flag)
{
for (int ii=1 ; ii<=nn ; ii++){}
//Serial1.flush() ;
} else Serial.println("No conectado") ;

//Acabar ciclo
t2=millis() ;
if (t2-t1>T) {digitalWrite(RT,HIGH) ;}
else {digitalWrite(RT,LOW) ;}
while (t2-t1<T) {t2=millis() ;}
t1=millis() ;
}




// ---------------------------------
// motor
//
// Select the motor (1-4), the command,
// and the speed (0-255).
// The commands are: FORWARD, BACKWARD, BRAKE, RELEASE.
//
void motor(int nMotor, int command, int speed)
{
int motorA, motorB;

if (nMotor >= 1 && nMotor <= 4)
{
switch (nMotor)
{
case 1:
motorA = MOTOR1_A;
motorB = MOTOR1_B;
break;
case 2:
motorA = MOTOR2_A;
motorB = MOTOR2_B;
break;
case 3:
motorA = MOTOR3_A;
motorB = MOTOR3_B;
break;
case 4:
motorA = MOTOR4_A;
motorB = MOTOR4_B;
break;
default:
break;
}

switch (command)
{
case FORWARD:
motor_output (motorA, HIGH, speed);
motor_output (motorB, LOW, -1); // -1: no PWM set
break;
case BACKWARD:
motor_output (motorA, LOW, speed);
motor_output (motorB, HIGH, -1); // -1: no PWM set
break;
case BRAKE:
// The AdaFruit library didn't implement a brake.

// The L293D motor driver ic doesn't have a good
// brake anyway.
// It uses transistors inside, and not mosfets.
// Some use a software break, by using a short
// reverse voltage.
// This brake will try to brake, by enabling
// the output and by pulling both outputs to ground.
// But it isn't a good break.
motor_output(motorA, LOW, 255); // 255: fully on.
motor_output(motorB, LOW,-1); //-1: no PWM set
break;
case RELEASE:
motor_output(motorA, LOW, 0); // 0: output floating.
motor_output(motorB, LOW,-1); //-1: no PWM set
break;
default:
break;

}

}

}

 // ---------------------------------
 // motor_output
 //
 // The function motor_ouput uses the motor driver to
 // drive normal outputs like lights, relays, solenoids,
 // DC motors (but not in reverse).
 //
 // It is also used as an internal helper function
 // for the motor() function.
 //
 // The high_low variable should be set 'HIGH'
 // to drive lights, etc.
 // It can be set 'LOW', to switch it off,
 // but also a 'speed' of 0 will switch it off.
 //
 // The 'speed' sets the PWM for 0...255, and is for
 // both pins of the motor output.
 // For example, if motor 3 side 'A' is used to for a
 // dimmed light at 50% (speed is 128), also the
 // motor 3 side 'B' output will be dimmed for 50%.
 // Set to 0 for completelty off (high impedance).
 // Set to 255 for fully on.
 // Special settings for the PWM speed:
 // Set to-1 for not setting the PWM at all.
 //
void motor_output(int output, int high_low, int speed)
{
int motorPWM;
switch(output){
case MOTOR1_A:
case MOTOR1_B:
motorPWM = MOTOR1_PWM;
break;
case MOTOR2_A:
case MOTOR2_B:
motorPWM = MOTOR2_PWM;
break;
case MOTOR3_A:
case MOTOR3_B:
motorPWM = MOTOR3_PWM;
break;
case MOTOR4_A:
case MOTOR4_B:
motorPWM = MOTOR4_PWM;
break;
default:
// Use speed as error flag,-3333 = invalid output.
speed =-3333;
break;

}
if(speed !=-3333)
{
 // Set the direction with the shift register
 // on the MotorShield, even if the speed =-1.
 // In that case the direction will be set, but
 // not the PWM.
 shiftWrite(output, high_low);

 // set PWM only if it is valid
 if(speed >= 0 && speed <= 255)
  {analogWrite(motorPWM, speed);}
}
}

 // ---------------------------------
 // shiftWrite
 //
 // The parameters are just like digitalWrite().
 //
 // The output is the pin 0...7 (the pin behind
 // the shift register).
 // The second parameter is HIGH or LOW.
 //
 // There is no initialization function.
 // Initialization is automatically done at the first
 // time it is used.
 //
 void shiftWrite(int output, int high_low)
{
static int latch_copy;
static int shift_register_initialized = false;
// Do the initialization on the fly,
// at the first time it is used.
if (!shift_register_initialized)
{

 // Set pins for shift register to output
 pinMode(MOTORLATCH, OUTPUT);
 pinMode(MOTORENABLE, OUTPUT);
 pinMode(MOTORDATA, OUTPUT);
 pinMode(MOTORCLK, OUTPUT);

 // Set pins for shift register to default value (low);
 digitalWrite(MOTORDATA, LOW);
 digitalWrite(MOTORLATCH, LOW);
 digitalWrite(MOTORCLK, LOW);
 // Enable the shift register, set Enable pin Low.
 digitalWrite(MOTORENABLE, LOW);

 // start with all outputs (of the shift register) low
 latch_copy = 0;

 shift_register_initialized = true;
 }

 // The defines HIGH and LOW are 1 and 0.
 // So this is valid.
 bitWrite(latch_copy, output, high_low);

 // Use the default Arduino 'shiftOut()' function to
 // shift the bits with the MOTORCLK as clock pulse.
 // The 74HC595 shiftregister wants the MSB first.
 // After that, generate a latch pulse with MOTORLATCH.
 shiftOut(MOTORDATA, MOTORCLK, MSBFIRST, latch_copy);
 delayMicroseconds(5); // For safety, not really needed.
 digitalWrite(MOTORLATCH, HIGH);
 delayMicroseconds(5); // For safety, not really needed.
 digitalWrite(MOTORLATCH, LOW);
 }
