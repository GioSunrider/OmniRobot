// Robot Omnidireccional 4W - Simulador
// Processing 3.5.4 o superior
// Compatible con código OMNIROBOT_4W_v3.0.ino

import processing.serial.*;

Serial puerto;
PFont fuente;
boolean[] teclas = new boolean[256];
boolean robotActivo = false;
int vidas = 10;
float anguloServo = 90;
boolean laserEncendido = false;
boolean modoDebug = false;

// Variables del robot (configuración 4 ruedas)
float posX, posY;
float anguloRobot = 0;
float velocidad = 2.5;

// Variables de control
float joystickX = 0;
float joystickY = 0;
float joystickW = 0;

// Configuración de motores (4 ruedas en X)
float[] velocidadesMotor = new float[4];
// Posiciones de ruedas: A(arriba-izq), B(arriba-der), C(abajo-der), D(abajo-izq)
PVector[] posicionesRuedas = {
  new PVector(-1, -1),  // A
  new PVector(1, -1),   // B  
  new PVector(1, 1),    // C
  new PVector(-1, 1)    // D
};

// Configuración específica del robot 4W
float arms_size = 120; // mm (del código original)
float wheel_radius = 37; // mm (74/2 del código original)

void setup() {
  size(1100, 800);
  fuente = createFont("Arial", 16);
  textFont(fuente);
  
  // Configuración de puerto serial
  println("=== SIMULADOR ROBOT 4W OMNIDIRECCIONAL ===");
  println("Puertos seriales disponibles:");
  printArray(Serial.list());
  
  // Intentar conectar automáticamente
  for (int i = 0; i < Serial.list().length; i++) {
    if (Serial.list()[i].contains("usbmodem") || 
        Serial.list()[i].contains("COM") || 
        Serial.list()[i].contains("ttyACM") ||
        Serial.list()[i].contains("ttyUSB")) {
      try {
        puerto = new Serial(this, Serial.list()[i], 9600);
        puerto.bufferUntil('\n');
        println("Conectado a: " + Serial.list()[i]);
        break;
      } catch (Exception e) {
        println("Error conectando a " + Serial.list()[i]);
      }
    }
  }
  
  if (puerto == null) {
    println("Modo simulación pura - Sin conexión serial");
  }
  
  // Posición inicial
  posX = width/2;
  posY = height/2 - 100;
  
  // Inicializar teclas
  for (int i = 0; i < teclas.length; i++) {
    teclas[i] = false;
  }
  
  // Configuración de gráficos
  smooth();
  frameRate(60);
}

void draw() {
  background(25, 25, 35);
  
  // Actualizar controles
  actualizarControles();
  
  // Dibujar área de simulación principal
  dibujarAreaSimulacion();
  
  // Dibujar robot
  dibujarRobot4W();
  
  // Dibujar paneles de información
  dibujarPanelControl();
  dibujarPanelEstado();
  dibujarPanelMotores();
  
  // Dibujar vista superior (diagrama de omni)
  dibujarVistaSuperior();
  
  // Enviar comandos si está conectado
  if (puerto != null && puerto.active()) {
    enviarComandosSerial();
  }
  
  // Actualizar simulación de movimiento
  actualizarSimulacion();
}

void actualizarControles() {
  // Resetear controles
  joystickX = joystickY = joystickW = 0;
  
  // Control de movimiento (WASD + QE para rotación)
  if (teclas['w'] || teclas['W']) joystickY = 100;
  if (teclas['s'] || teclas['S']) joystickY = -100;
  if (teclas['a'] || teclas['A']) joystickX = -100;
  if (teclas['d'] || teclas['D']) joystickX = 100;
  if (teclas['q'] || teclas['Q']) joystickW = -10;
  if (teclas['e'] || teclas['E']) joystickW = 10;
  
  // Control de activación (ESPACIO para activar, X para desactivar)
  if (teclas[' ']) {
    robotActivo = true;
  }
  if (teclas['x'] || teclas['X']) {
    robotActivo = false;
  }
  
  // Control de láser
  laserEncendido = teclas['l'] || teclas['L'];
  
  // Control de servo con teclas
  if (teclas['z'] || teclas['Z']) {
    anguloServo = max(anguloServo - 2, 0);
  }
  if (teclas['c'] || teclas['C']) {
    anguloServo = min(anguloServo + 2, 180);
  }
  
  // Reset con R
  if (teclas['r'] || teclas['R']) {
    resetSimulacion();
  }
  
  // Modo debug con F1
  if (teclas[112]) { // F1
    modoDebug = !modoDebug;
    teclas[112] = false; // Evitar toggle rápido
  }
  
  // Calcular velocidades de los motores (fórmula 4W)
  calcularVelocidades4W();
}

void calcularVelocidades4W() {
  // Fórmula original del código Arduino para 4 ruedas:
  // speed_A = ((Y)+(arms_size*W));
  // speed_B = ((-X)+(arms_size*W));
  // speed_C = ((-Y)+(arms_size*W));
  // speed_D = ((X)+(arms_size*W));
  
  // Aplicamos la fórmula
  velocidadesMotor[0] = (joystickY) + (arms_size * joystickW);  // A
  velocidadesMotor[1] = (-joystickX) + (arms_size * joystickW); // B
  velocidadesMotor[2] = (-joystickY) + (arms_size * joystickW); // C
  velocidadesMotor[3] = (joystickX) + (arms_size * joystickW);  // D
  
  // Normalizar a rango [-200, 200] como en el código Arduino
  float maxSpeed = 0;
  for (int i = 0; i < 4; i++) {
    maxSpeed = max(maxSpeed, abs(velocidadesMotor[i]));
  }
  
  if (maxSpeed > 200) {
    float factor = 200.0 / maxSpeed;
    for (int i = 0; i < 4; i++) {
      velocidadesMotor[i] *= factor;
    }
  }
}

void actualizarSimulacion() {
  if (robotActivo && vidas > 0) {
    // Movimiento omnidireccional puro (sin rotación del chasis)
    float movX = joystickX * 0.01 * velocidad;
    float movY = joystickY * 0.01 * velocidad;
    
    posX += movX;
    posY -= movY; // Invertir Y para coordenadas de pantalla
    
    // Rotación independiente
    anguloRobot += joystickW * 0.5;
    
    // Mantener dentro de límites
    posX = constrain(posX, 100, width - 100);
    posY = constrain(posY, 100, height - 300);
    
    // Detectar colisiones con obstáculos
    detectarColisiones();
  }
}

void detectarColisiones() {
  // Obstáculos fijos
  float[][] obstaculos = {
    {200, 200, 50},
    {width - 200, 200, 50},
    {width/2, height - 250, 60},
    {width/4, height/2, 40},
    {3*width/4, height/2, 40}
  };
  
  float radioRobot = 60;
  
  for (float[] obs : obstaculos) {
    float distancia = dist(posX, posY, obs[0], obs[1]);
    if (distancia < (radioRobot + obs[2])) {
      // Golpe detectado
      if (frameCount % 30 == 0) { // Cada medio segundo aprox
        vidas = max(0, vidas - 1);
      }
      
      // Empujar fuera del obstáculo
      float ang = atan2(posY - obs[1], posX - obs[0]);
      posX = obs[0] + cos(ang) * (radioRobot + obs[2] + 5);
      posY = obs[1] + sin(ang) * (radioRobot + obs[2] + 5);
    }
  }
}

void dibujarAreaSimulacion() {
  // Fondo con cuadrícula
  stroke(40, 40, 50);
  strokeWeight(1);
  for (int x = 50; x < width; x += 50) {
    line(x, 50, x, height - 250);
  }
  for (int y = 50; y < height - 200; y += 50) {
    line(50, y, width - 50, y);
  }
  
  // Borde del área
  noFill();
  stroke(80);
  strokeWeight(3);
  rect(45, 45, width - 90, height - 290, 10);
  
  // Obstáculos
  fill(180, 80, 80, 180);
  noStroke();
  ellipse(200, 200, 100, 100);
  ellipse(width - 200, 200, 100, 100);
  ellipse(width/2, height - 250, 120, 120);
  ellipse(width/4, height/2, 80, 80);
  ellipse(3*width/4, height/2, 80, 80);
  
  // Zona de meta
  fill(80, 180, 80, 100);
  stroke(100, 220, 100);
  strokeWeight(2);
  rect(width - 150, height - 300, 100, 100, 10);
  fill(100, 220, 100);
  textAlign(CENTER);
  text("META", width - 100, height - 250);
}

void dibujarRobot4W() {
  pushMatrix();
  translate(posX, posY);
  
  // Cuerpo del robot (cuadrado para 4 ruedas)
  fill(robotActivo ? color(30, 120, 220) : color(80, 80, 80));
  stroke(robotActivo ? color(50, 170, 255) : color(120, 120, 120));
  strokeWeight(3);
  rectMode(CENTER);
  rect(0, 0, 120, 120, 15);
  
  // Indicador de dirección frontal
  fill(255, 220, 50);
  noStroke();
  triangle(0, -45, 15, -25, -15, -25);
  
  // Ejes de referencia
  if (modoDebug) {
    stroke(255, 0, 0, 150);
    strokeWeight(2);
    line(-80, 0, 80, 0); // Eje X
    stroke(0, 255, 0, 150);
    line(0, -80, 0, 80); // Eje Y
  }
  
  // Dibujar las 4 ruedas
  for (int i = 0; i < 4; i++) {
    float x = posicionesRuedas[i].x * 50;
    float y = posicionesRuedas[i].y * 50;
    
    // Rueda
    fill(40);
    stroke(100);
    strokeWeight(2);
    ellipse(x, y, 35, 35);
    
    // Indicador de dirección/velocidad
    float vel = velocidadesMotor[i];
    if (abs(vel) > 1) {
      // Flecha indicadora
      float anguloFlecha = 0;
      if (vel > 0) {
        // Motor adelante
        if (i == 0 || i == 3) anguloFlecha = radians(90); // Izquierda
        if (i == 1 || i == 2) anguloFlecha = radians(-90); // Derecha
      } else {
        // Motor atrás
        if (i == 0 || i == 3) anguloFlecha = radians(-90);
        if (i == 1 || i == 2) anguloFlecha = radians(90);
      }
      
      pushMatrix();
      translate(x, y);
      rotate(anguloFlecha);
      
      float largo = map(abs(vel), 0, 200, 0, 20);
      fill(vel > 0 ? color(0, 255, 100) : color(255, 50, 50));
      noStroke();
      triangle(0, -largo, -5, -largo + 10, 5, -largo + 10);
      popMatrix();
    }
    
    // Etiqueta del motor
    fill(255);
    textAlign(CENTER);
    text(char('A' + i), x, y - 25);
  }
  
  // Sistema láser/servo
  pushMatrix();
  rotate(radians(anguloRobot + anguloServo - 90));
  
  // Brazo del servo
  fill(120, 120, 140);
  stroke(180);
  strokeWeight(2);
  rect(0, -10, 15, 60, 5);
  
  // Láser
  if (laserEncendido) {
    // Haz láser principal
    stroke(255, 0, 0, 200);
    strokeWeight(4);
    line(0, 20, 0, 250);
    
    // Efecto de partículas
    for (int i = 0; i < 8; i++) {
      float offset = random(-8, 8);
      float alpha = random(100, 200);
      stroke(255, 100 + random(100), 100, alpha);
      strokeWeight(random(1, 4));
      line(offset, 20 + random(30), offset, 250);
    }
    
    // Punto de impacto
    fill(255, 200, 0, 150);
    noStroke();
    ellipse(0, 250, 20, 20);
  }
  
  popMatrix();
  popMatrix();
}

void dibujarPanelControl() {
  // Panel inferior de control
  fill(35, 35, 45, 220);
  stroke(80);
  strokeWeight(2);
  rect(20, height - 180, width - 40, 160, 15);
  
  // Título
  fill(255);
  textAlign(CENTER);
  textSize(18);
  text("CONTROL ROBOT 4W OMNIDIRECCIONAL", width/2, height - 155);
  
  // Instrucciones
  textSize(14);
  textAlign(LEFT);
  fill(200);
  text("CONTROLES DE TECLADO:", 40, height - 125);
  fill(180);
  
  // Columna 1
  text("W/S: Adelante/Atrás", 60, height - 100);
  text("A/D: Izquierda/Derecha", 60, height - 80);
  text("Q/E: Rotar Izq/Der", 60, height - 60);
  
  // Columna 2
  text("ESPACIO: Activar", 250, height - 100);
  text("X: Desactivar", 250, height - 80);
  text("L: Láser ON/OFF", 250, height - 60);
  
  // Columna 3
  text("Z/C: Servo -/+", 440, height - 100);
  text("R: Reset", 440, height - 80);
  text("F1: Debug", 440, height - 60);
  
  // Joystick virtual
  dibujarJoystickVirtual();
}

void dibujarJoystickVirtual() {
  pushMatrix();
  translate(width - 200, height - 130);
  
  // Título
  fill(200);
  textAlign(CENTER);
  text("JOYSTICK VIRTUAL", 0, -40);
  
  // Área del joystick XY
  fill(45);
  stroke(100);
  strokeWeight(2);
  ellipse(0, 0, 120, 120);
  
  // Marcas de ejes
  stroke(80, 150);
  strokeWeight(1);
  line(-50, 0, 50, 0);
  line(0, -50, 0, 50);
  
  // Punto del joystick
  float joyX = map(joystickX, -100, 100, -40, 40);
  float joyY = map(joystickY, -100, 100, -40, 40);
  
  fill(50, 200, 100);
  noStroke();
  ellipse(joyX, joyY, 25, 25);
  
  // Indicador de rotación
  translate(140, 0);
  fill(45);
  stroke(100);
  strokeWeight(2);
  rect(-30, -30, 60, 60, 8);
  
  float rotY = map(joystickW, -10, 10, -20, 20);
  fill(200, 100, 50);
  ellipse(0, rotY, 20, 20);
  
  // Etiquetas
  fill(150);
  text("X/Y", -140, 60);
  text("ROT", 0, 60);
  
  popMatrix();
}

void dibujarPanelEstado() {
  // Panel lateral de estado
  fill(40, 40, 50, 220);
  stroke(80);
  strokeWeight(2);
  rect(width - 220, 20, 200, 200, 10);
  
  fill(255);
  textAlign(CENTER);
  textSize(16);
  text("ESTADO DEL ROBOT", width - 120, 45);
  
  textAlign(LEFT);
  textSize(14);
  
  // Información de estado
  float y = 75;
  float spacing = 25;
  
  // Estado de activación
  fill(robotActivo ? color(0, 255, 100) : color(255, 100, 0));
  text("Motores: " + (robotActivo ? "ACTIVOS" : "INACTIVOS"), width - 210, y);
  y += spacing;
  
  // Vidas
  fill(vidas > 5 ? color(0, 255, 100) : (vidas > 2 ? color(255, 200, 0) : color(255, 50, 50)));
  text("Vidas: " + vidas + " / 10", width - 210, y);
  y += spacing;
  
  // Láser
  fill(laserEncendido ? color(255, 50, 50) : color(150));
  text("Láser: " + (laserEncendido ? "ON" : "OFF"), width - 210, y);
  y += spacing;
  
  // Servo
  fill(200);
  text("Servo: " + nf(anguloServo, 0, 0) + "°", width - 210, y);
  y += spacing;
  
  // Posición
  fill(180, 180, 255);
  text("Posición:", width - 210, y);
  y += 20;
  text("X: " + nf(posX - width/2, 0, 0), width - 200, y);
  y += spacing - 5;
  text("Y: " + nf(height/2 - posY, 0, 0), width - 200, y);
}

void dibujarPanelMotores() {
  // Panel de motores
  fill(40, 40, 50, 220);
  stroke(80);
  strokeWeight(2);
  rect(20, 20, 400, 200, 10);
  
  fill(255);
  textAlign(CENTER);
  textSize(16);
  text("VELOCIDADES DE MOTORES", 220, 45);
  
  // Barras de velocidad para cada motor
  float startX = 60;
  float startY = 80;
  float barWidth = 60;
  float barSpacing = 80;
  
  for (int i = 0; i < 4; i++) {
    float x = startX + i * barSpacing;
    
    // Etiqueta del motor
    fill(255);
    textAlign(CENTER);
    text("Motor " + char('A' + i), x, startY - 10);
    
    // Fondo de la barra
    fill(60);
    noStroke();
    rect(x - barWidth/2, startY, barWidth, 100);
    
    // Barra de velocidad
    float velocidadRel = velocidadesMotor[i] / 200.0;
    float barHeight = abs(velocidadRel) * 100;
    
    if (velocidadesMotor[i] > 0) {
      // Motor adelante (verde)
      fill(0, 200, 100);
      rect(x - barWidth/2, startY + 100 - barHeight, barWidth, barHeight);
    } else if (velocidadesMotor[i] < 0) {
      // Motor atrás (rojo)
      fill(220, 50, 50);
      rect(x - barWidth/2, startY, barWidth, barHeight);
    }
    
    // Línea central
    stroke(200);
    strokeWeight(1);
    line(x - barWidth/2, startY + 50, x + barWidth/2, startY + 50);
    
    // Valor numérico
    fill(255);
    textAlign(CENTER);
    text(nf(velocidadesMotor[i], 0, 0), x, startY + 120);
  }
  
  // Fórmula utilizada
  if (modoDebug) {
    fill(200, 200, 100);
    textSize(12);
    textAlign(LEFT);
    text("Fórmula 4W: A=Y+RW, B=-X+RW, C=-Y+RW, D=X+RW", 30, 190);
  }
}

void dibujarVistaSuperior() {
  // Vista esquemática superior
  pushMatrix();
  translate(500, height - 130);
  
  fill(45, 45, 55, 220);
  stroke(90);
  strokeWeight(2);
  rect(0, 0, 300, 120, 10);
  
  fill(220);
  textAlign(CENTER);
  text("VISTA SUPERIOR - CONFIGURACIÓN 4W", 150, 15);
  
  // Dibujar esquema del robot
  stroke(150);
  strokeWeight(2);
  noFill();
  rect(50, 40, 200, 60);
  
  // Ejes de coordenadas
  stroke(100, 150);
  strokeWeight(1);
  line(150, 40, 150, 100); // Eje Y
  line(50, 70, 250, 70);   // Eje X
  
  // Ruedas
  fill(80);
  noStroke();
  ellipse(50, 40, 20, 20);  // A
  ellipse(250, 40, 20, 20); // B
  ellipse(250, 100, 20, 20);// C
  ellipse(50, 100, 20, 20); // D
  
  // Etiquetas
  fill(255);
  textSize(12);
  text("A", 50, 35);
  text("B", 250, 35);
  text("C", 250, 115);
  text("D", 50, 115);
  text("Y", 140, 45);
  text("X", 240, 65);
  
  // Direcciones típicas
  stroke(100, 200, 100, 150);
  strokeWeight(2);
  // Adelante
  line(150, 70, 150, 30);
  triangle(150, 30, 145, 40, 155, 40);
  
  popMatrix();
}

void enviarComandosSerial() {
  // Enviar datos en formato compatible con el código Arduino
  // Formato: X,Y,Servo,Rotacion,Activo,Laser\n
  
  String datos = 
    int(constrain(joystickX, -100, 100)) + "," +
    int(constrain(joystickY, -100, 100)) + "," +
    int(anguloServo) + "," +
    int(constrain(joystickW, -10, 10)) + "," +
    (robotActivo ? "1" : "0") + "," +
    (laserEncendido ? "1" : "0") + "\n";
  
  puerto.write(datos);
  
  // Leer respuesta del Arduino
  while (puerto.available() > 0) {
    try {
      String respuesta = puerto.readStringUntil('\n');
      if (respuesta != null && respuesta.trim().length() > 0) {
        if (modoDebug) {
          println("Arduino: " + respuesta.trim());
        }
      }
    } catch (Exception e) {
      // Ignorar errores de lectura
    }
  }
}

void resetSimulacion() {
  vidas = 10;
  posX = width/2;
  posY = height/2 - 100;
  anguloRobot = 0;
  anguloServo = 90;
  robotActivo = false;
  laserEncendido = false;
  
  for (int i = 0; i < 4; i++) {
    velocidadesMotor[i] = 0;
  }
  
  println("Simulación reiniciada");
}

void serialEvent(Serial p) {
  try {
    String data = p.readStringUntil('\n');
    if (data != null) {
      data = data.trim();
      
      // Mostrar datos recibidos en modo debug
      if (modoDebug && data.length() > 0) {
        println("Serial RX: " + data);
      }
    }
  } catch (Exception e) {
    // Ignorar errores de lectura
  }
}

void keyPressed() {
  if (keyCode < teclas.length) {
    teclas[keyCode] = true;
  }
  
  // Teclas de función para debug
  if (key == '1') {
    // Test motor A
    velocidadesMotor[0] = 100;
  }
  if (key == '2') {
    // Test motor B
    velocidadesMotor[1] = 100;
  }
  if (key == '3') {
    // Test motor C
    velocidadesMotor[2] = 100;
  }
  if (key == '4') {
    // Test motor D
    velocidadesMotor[3] = 100;
  }
}

void keyReleased() {
  if (keyCode < teclas.length) {
    teclas[keyCode] = false;
  }
  
  // Resetear test de motores
  if (key == '1' || key == '2' || key == '3' || key == '4') {
    for (int i = 0; i < 4; i++) {
      if (!robotActivo) {
        velocidadesMotor[i] = 0;
      }
    }
  }
}

void mouseDragged() {
  // Control de servo con ratón en panel de estado
  if (mouseX > width - 210 && mouseX < width - 30 &&
      mouseY > 95 && mouseY < 115) {
    anguloServo = int(map(mouseX, width - 210, width - 30, 0, 180));
    anguloServo = constrain(anguloServo, 0, 180);
  }
}
