#include <Servo.h>
#include <NewPing.h>

Servo servoMot;

int trigPin = 12;
int echoPin = 13;
int MAX_DISTANCE = 15;

NewPing sonar(trigPin, echoPin, MAX_DISTANCE);

int anguloServo = 90;
int deltaAngulo = 60;

int motorEsqA = 3;
int motorEsqB = 5;
int motorDirA = 6;
int motorDirB = 11;

int velocidadeMotor = 150;

int ledverde = 9;
int ledvermelho = 8;

void setup() {
  servoMot.attach(2);

  pinMode(motorEsqA, OUTPUT);
  pinMode(motorEsqB, OUTPUT);
  pinMode(motorDirA, OUTPUT);
  pinMode(motorDirB, OUTPUT);

  pinMode(ledverde, OUTPUT);
  pinMode(ledvermelho, OUTPUT);

  servoMot.write(anguloServo);
}

void loop() {
  float distancia = sonar.ping_cm();

  if (distancia > 0 && distancia < MAX_DISTANCE) {
    pararMotores();
    digitalWrite(ledverde, LOW);
    digitalWrite(ledvermelho, HIGH);

    // Movimento do servo motor para os dois lados e verificação de obstáculos
    servoMot.write(anguloServo - deltaAngulo); // Girar para a esquerda
    delay(1000);
    float distanciaEsquerda = sonar.ping_cm(); // Verificar distância à esquerda

    servoMot.write(anguloServo + deltaAngulo); // Girar para a direita
    delay(1000);
    float distanciaDireita = sonar.ping_cm(); // Verificar distância à direita

    servoMot.write(anguloServo); // Retornar ao centro
    delay(1000);

    // Decisão baseada nas distâncias dos lados
    if (distanciaEsquerda > 0 && distanciaEsquerda < MAX_DISTANCE) {
      virarDireita(); // Obstáculo à esquerda, desviar para a direita
    } else if (distanciaDireita > 0 && distanciaDireita < MAX_DISTANCE) {
      virarEsquerda(); // Obstáculo à direita, desviar para a esquerda
    } else {
      desviarObstaculo(); // Caso contrário, seguir lógica padrão de desvio
    }
  } else {
    moverFrente();
  }

  delay(100);
}

void moverFrente() {
  analogWrite(motorEsqA, velocidadeMotor);
  digitalWrite(motorEsqB, LOW);
  analogWrite(motorDirA, velocidadeMotor);
  digitalWrite(motorDirB, LOW);

  digitalWrite(ledverde, HIGH);
  digitalWrite(ledvermelho, LOW);
}

void virarDireita() {
  pararMotores();
  analogWrite(motorEsqA, velocidadeMotor);
  digitalWrite(motorEsqB, LOW);
  digitalWrite(motorDirA, LOW);
  digitalWrite(motorDirB, LOW);
  delay(1000);
}

void virarEsquerda() {
  pararMotores();
  digitalWrite(motorEsqA, LOW);
  digitalWrite(motorEsqB, LOW);
  analogWrite(motorDirA, velocidadeMotor);
  digitalWrite(motorDirB, LOW);
  delay(1000);
}

void desviarObstaculo() {
  pararMotores();
  digitalWrite(motorEsqB, LOW);
  analogWrite(motorEsqA, velocidadeMotor);
  digitalWrite(motorDirB, LOW);
  digitalWrite(motorDirA, LOW);
  delay(1000);
}

void pararMotores() {
  digitalWrite(motorEsqA, LOW);
  digitalWrite(motorEsqB, LOW);
  digitalWrite(motorDirA, LOW);
  digitalWrite(motorDirB, LOW);
}
