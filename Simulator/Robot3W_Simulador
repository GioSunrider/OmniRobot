// Robot Omnidireccional 3W - Simulador
// Processing 3.5.4 o superior

import processing.serial.*;

Serial puerto;
PFont fuente;
boolean[] teclas = new boolean[256];
boolean robotActivo = false;
int vidas = 10;
float anguloServo = 90;
boolean laserEncendido = false;

// Variables del robot
float posX, posY;
float anguloRobot = 0;
float velocidad = 2;

// Variables de control (simulación de joystick)
float joystickX = 0;
float joystickY = 0;
float joystickW = 0;

// Configuración de motores
float[] velocidadesMotor = new float[3];
float[] angulosRuedas = {0, 120, 240}; // Ángulos en grados

void setup() {
  size(1000, 700);
  fuente = createFont("Arial", 16);
  textFont(fuente);
  
  // Listar puertos seriales disponibles
  println("Puertos seriales disponibles:");
  printArray(Serial.list());
  
  // Conectar al puerto serial (ajusta el índice según tu configuración)
  try {
    puerto = new Serial(this, Serial.list()[0], 9600);
    println("Conectado a: " + Serial.list()[0]);
  } catch (Exception e) {
    println("No se pudo conectar al puerto serial. Modo simulación pura.");
  }
  
  // Posición inicial del robot
  posX = width/2;
  posY = height/2;
  
  // Inicializar teclas
  for (int i = 0; i < teclas.length; i++) {
    teclas[i] = false;
  }
}

void draw() {
  background(30);
  
  // Actualizar controles
  actualizarControles();
  
  // Dibujar área de simulación
  dibujarAreaSimulacion();
  
  // Dibujar robot
  dibujarRobot();
  
  // Dibujar panel de control
  dibujarPanelControl();
  
  // Dibujar información del estado
  dibujarInfoEstado();
  
  // Dibujar representación de motores
  dibujarMotores();
  
  // Enviar comandos por serial si está conectado
  enviarComandosSerial();
}

void actualizarControles() {
  // Control con teclas (WASD + QE para rotación)
  joystickX = 0;
  joystickY = 0;
  joystickW = 0;
  
  if (teclas['w'] || teclas['W']) joystickY = 100;
  if (teclas['s'] || teclas['S']) joystickY = -100;
  if (teclas['a'] || teclas['A']) joystickX = -100;
  if (teclas['d'] || teclas['D']) joystickX = 100;
  if (teclas['q'] || teclas['Q']) joystickW = -10;
  if (teclas['e'] || teclas['E']) joystickW = 10;
  
  // Tecla espacio para activar/desactivar
  if (teclas[' ']) {
    robotActivo = true;
  } else {
    // Desactivar con otra tecla, por ejemplo 'x'
    if (teclas['x'] || teclas['X']) robotActivo = false;
  }
  
  // Tecla 'l' para láser
  if (teclas['l'] || teclas['L']) {
    laserEncendido = true;
  } else {
    laserEncendido = false;
  }
  
  // Tecla 'r' para reset
  if (teclas['r'] || teclas['R']) {
    vidas = 10;
    posX = width/2;
    posY = height/2;
    anguloRobot = 0;
  }
  
  // Calcular velocidades de los motores (fórmula omnidireccional)
  calcularVelocidadesMotores();
  
  // Actualizar posición del robot (simulación)
  if (robotActivo) {
    // Convertir joystick a movimiento
    float movX = joystickX * 0.01 * velocidad;
    float movY = joystickY * 0.01 * velocidad;
    
    // Rotar el vector de movimiento según el ángulo del robot
    float cosA = cos(radians(anguloRobot));
    float sinA = sin(radians(anguloRobot));
    float movXRotado = movX * cosA - movY * sinA;
    float movYRotado = movX * sinA + movY * cosA;
    
    posX += movXRotado;
    posY += movYRotado;
    
    // Aplicar rotación
    anguloRobot += joystickW * 0.1;
  }
  
  // Limitar posición a los bordes
  posX = constrain(posX, 50, width - 50);
  posY = constrain(posY, 50, height - 200);
}

void calcularVelocidadesMotores() {
  // Fórmula simplificada para 3 ruedas
  float arms_size = 110;
  float wheel_radius = 20.5;
  
  // Conversión a radianes
  float rad120 = radians(120);
  float rad240 = radians(240);
  
  // Cálculo de velocidades
  velocidadesMotor[0] = (-sin(0) * joystickX + cos(0) * joystickY) + (arms_size * joystickW);
  velocidadesMotor[1] = (-sin(rad120) * joystickX + cos(rad120) * joystickY) + (arms_size * joystickW);
  velocidadesMotor[2] = (-sin(rad240) * joystickX + cos(rad240) * joystickY) + (arms_size * joystickW);
  
  // Aplicar factor de escala
  for (int i = 0; i < 3; i++) {
    velocidadesMotor[i] /= wheel_radius;
    // Limitar a rango [-200, 200]
    velocidadesMotor[i] = constrain(velocidadesMotor[i], -200, 200);
  }
}

void dibujarAreaSimulacion() {
  // Fondo del área de juego
  fill(50);
  stroke(100);
  strokeWeight(2);
  rect(20, 20, width - 40, height - 240, 10);
  
  // Líneas de guía
  stroke(80, 80, 80, 100);
  strokeWeight(1);
  // Línea horizontal central
  line(20, height/2 - 120, width - 20, height/2 - 120);
  // Línea vertical central
  line(width/2, 20, width/2, height - 220);
  
  // Obstáculos (para simular golpes)
  fill(200, 100, 100, 150);
  noStroke();
  ellipse(150, 150, 60, 60);
  ellipse(width - 150, 150, 60, 60);
  ellipse(width/2, height - 300, 60, 60);
  
  // Detectar colisiones con obstáculos
  detectarColisiones();
}

void detectarColisiones() {
  float[] obstaculosX = {150, width - 150, width/2};
  float[] obstaculosY = {150, 150, height - 300};
  float radioObstaculo = 30;
  float radioRobot = 40;
  
  for (int i = 0; i < 3; i++) {
    float distancia = dist(posX, posY, obstaculosX[i], obstaculosY[i]);
    if (distancia < (radioRobot + radioObstaculo)) {
      // Simular golpe
      if (frameCount % 60 == 0 && robotActivo) { // Una vez por segundo aproximadamente
        vidas = max(0, vidas - 1);
      }
      
      // Empujar al robot fuera del obstáculo
      float angulo = atan2(posY - obstaculosY[i], posX - obstaculosX[i]);
      posX = obstaculosX[i] + cos(angulo) * (radioRobot + radioObstaculo + 5);
      posY = obstaculosY[i] + sin(angulo) * (radioRobot + radioObstaculo + 5);
    }
  }
}

void dibujarRobot() {
  pushMatrix();
  translate(posX, posY);
  rotate(radians(anguloRobot));
  
  // Cuerpo del robot
  fill(robotActivo ? color(0, 150, 255) : color(100, 100, 100));
  stroke(robotActivo ? color(0, 200, 255) : color(150, 150, 150));
  strokeWeight(2);
  ellipse(0, 0, 80, 80);
  
  // Indicador frontal
  fill(255, 200, 0);
  noStroke();
  triangle(0, -30, 20, 0, -20, 0);
  
  // Ruedas
  for (int i = 0; i < 3; i++) {
    float ang = radians(angulosRuedas[i]);
    float xRueda = cos(ang) * 45;
    float yRueda = sin(ang) * 45;
    
    // Dibujar rueda
    fill(50);
    stroke(100);
    strokeWeight(2);
    ellipse(xRueda, yRueda, 30, 30);
    
    // Indicador de velocidad de la rueda
    float velocidadRel = abs(velocidadesMotor[i]) / 200.0;
    fill(velocidadesMotor[i] > 0 ? color(0, 255, 0) : color(255, 0, 0));
    noStroke();
    float anchoFlecha = map(velocidadRel, 0, 1, 0, 15);
    if (velocidadesMotor[i] > 0) {
      triangle(xRueda, yRueda - 20, 
               xRueda - anchoFlecha, yRueda - 5, 
               xRueda + anchoFlecha, yRueda - 5);
    } else if (velocidadesMotor[i] < 0) {
      triangle(xRueda, yRueda + 20, 
               xRueda - anchoFlecha, yRueda + 5, 
               xRueda + anchoFlecha, yRueda + 5);
    }
  }
  
  // Servo y láser
  pushMatrix();
  rotate(radians(anguloServo));
  
  // Brazo del servo
  fill(150);
  stroke(200);
  strokeWeight(2);
  rect(-5, -10, 10, 40);
  
  // Láser
  if (laserEncendido) {
    stroke(255, 0, 0, 150);
    strokeWeight(3);
    line(0, 30, 0, 200);
    // Efecto de partículas
    for (int i = 0; i < 5; i++) {
      float offset = random(-5, 5);
      stroke(255, 100, 100, random(100, 200));
      strokeWeight(random(1, 3));
      line(offset, 30 + random(20), offset, 200);
    }
  }
  popMatrix();
  
  popMatrix();
}

void dibujarPanelControl() {
  // Panel de control
  fill(40, 40, 50, 200);
  stroke(100);
  strokeWeight(2);
  rect(20, height - 200, width - 40, 180, 10);
  
  // Título
  fill(255);
  textAlign(CENTER);
  textSize(20);
  text("PANEL DE CONTROL - ROBOT 3W OMNIDIRECCIONAL", width/2, height - 170);
  
  textSize(14);
  textAlign(LEFT);
  
  // Controles de teclado
  fill(200);
  text("CONTROLES DE TECLADO:", 40, height - 140);
  fill(150);
  text("W/S: Adelante/Atrás", 60, height - 120);
  text("A/D: Izquierda/Derecha", 60, height - 100);
  text("Q/E: Rotar Izquierda/Derecha", 60, height - 80);
  text("ESPACIO: Activar motores", 60, height - 60);
  text("X: Desactivar motores", 60, height - 40);
  text("L: Encender/Apagar láser", 60, height - 20);
  text("R: Resetear simulación", 60, height);
  
  // Joystick virtual
  float joyX = map(joystickX, -100, 100, -50, 50);
  float joyY = map(joystickY, -100, 100, -50, 50);
  float joyW = map(joystickW, -10, 10, -25, 25);
  
  pushMatrix();
  translate(width - 200, height - 130);
  
  // Área del joystick XY
  fill(60);
  stroke(100);
  ellipse(0, 0, 100, 100);
  
  // Punto del joystick
  fill(0, 200, 0);
  noStroke();
  ellipse(joyX, -joyY, 20, 20);
  
  // Joystick de rotación
  translate(120, 0);
  fill(60);
  stroke(100);
  rect(-30, -30, 60, 60, 5);
  
  fill(200, 0, 0);
  noStroke();
  ellipse(0, -joyW, 15, 15);
  
  popMatrix();
}

void dibujarInfoEstado() {
  textAlign(LEFT);
  textSize(16);
  
  // Estado del robot
  fill(robotActivo ? color(0, 255, 0) : color(255, 100, 0));
  text("ESTADO: " + (robotActivo ? "ACTIVO" : "INACTIVO"), 40, 40);
  
  // Vidas
  fill(vidas > 5 ? color(0, 255, 0) : (vidas > 2 ? color(255, 200, 0) : color(255, 0, 0)));
  text("VIDAS: " + vidas, 40, 70);
  
  // Láser
  fill(laserEncendido ? color(255, 0, 0) : color(150));
  text("LÁSER: " + (laserEncendido ? "ENCENDIDO" : "APAGADO"), 40, 100);
  
  // Ángulo servo
  fill(200);
  text("SERVO: " + nf(anguloServo, 0, 1) + "°", 40, 130);
  
  // Velocidades de motores
  text("MOTORES:", width - 200, 40);
  for (int i = 0; i < 3; i++) {
    fill(velocidadesMotor[i] > 0 ? color(0, 255, 0) : 
         (velocidadesMotor[i] < 0 ? color(255, 0, 0) : color(150)));
    text("Motor " + char('A' + i) + ": " + nf(velocidadesMotor[i], 0, 1), 
         width - 200, 70 + i * 20);
  }
  
  // Posición
  fill(200);
  text("POSICIÓN: X=" + nf(posX, 0, 1) + " Y=" + nf(posY, 0, 1), width - 200, 140);
  text("ÁNGULO: " + nf(anguloRobot, 0, 1) + "°", width - 200, 160);
}

void dibujarMotores() {
  // Visualización detallada de motores
  pushMatrix();
  translate(width/2, height - 350);
  
  for (int i = 0; i < 3; i++) {
    float ang = radians(angulosRuedas[i]);
    float x = cos(ang) * 80;
    float y = sin(ang) * 80;
    
    // Barra de velocidad
    float largoBarra = map(abs(velocidadesMotor[i]), 0, 200, 0, 50);
    fill(velocidadesMotor[i] > 0 ? color(0, 200, 0) : color(200, 0, 0));
    noStroke();
    
    if (velocidadesMotor[i] > 0) {
      rect(x - 10, y - 60, 20, -largoBarra);
    } else if (velocidadesMotor[i] < 0) {
      rect(x - 10, y + 10, 20, largoBarra);
    }
    
    // Etiqueta del motor
    fill(255);
    textAlign(CENTER);
    text("M" + char('A' + i), x, y - 80);
    text(nf(velocidadesMotor[i], 0, 0), x, y + 40);
  }
  popMatrix();
}

void enviarComandosSerial() {
  if (puerto != null && puerto.active()) {
    // Formato: A,B,C,D,E,F
    // A: joystickX (-100 a 100)
    // B: joystickY (-100 a 100)
    // C: anguloServo (0-180)
    // D: joystickW (-10 a 10)
    // E: robotActivo (0 o 1)
    // F: laserEncendido (0 o 1)
    
    String datos = 
      int(joystickX) + "," +
      int(joystickY) + "," +
      int(anguloServo) + "," +
      int(joystickW) + "," +
      (robotActivo ? "1" : "0") + "," +
      (laserEncendido ? "1" : "0") + "\n";
    
    puerto.write(datos);
    
    // Leer respuesta del Arduino (si hay)
    while (puerto.available() > 0) {
      String respuesta = puerto.readStringUntil('\n');
      if (respuesta != null) {
        println("Arduino: " + respuesta.trim());
      }
    }
  }
}

void keyPressed() {
  if (keyCode < teclas.length) {
    teclas[keyCode] = true;
  }
  
  // Controles adicionales con teclas regulares
  if (key == '=' || key == '+') {
    velocidad = min(velocidad + 0.5, 5);
  }
  if (key == '-' || key == '_') {
    velocidad = max(velocidad - 0.5, 0.5);
  }
  if (key == '[') {
    anguloServo = max(anguloServo - 5, 0);
  }
  if (key == ']') {
    anguloServo = min(anguloServo + 5, 180);
  }
}

void keyReleased() {
  if (keyCode < teclas.length) {
    teclas[keyCode] = false;
  }
}

void mouseDragged() {
  // Control del servo con el ratón
  if (mouseY > height - 200 && mouseX > width - 250 && mouseX < width - 130) {
    anguloServo = int(map(mouseX, width - 250, width - 130, 0, 180));
  }
}
