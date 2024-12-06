#include <SoftwareSerial.h>
SoftwareSerial bluetoothSerial(2, 3);
char orden;
int VELOCIDAD = 200;

// Motor A
int ENA = 10;
int IN1 = 9;
int IN2 = 8;

// Motor B
int ENB = 5;
int IN3 = 7;
int IN4 = 6;

void setup() {
  bluetoothSerial.begin(9600);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  Serial.begin(9600);
  Serial.println("Inicia");
}

void loop() {
  char crt;  //Variable caracter que recibe los datos via Bluetooth
  if (bluetoothSerial.available()) {
    Serial.println(crt);
    crt = bluetoothSerial.read();
    Serial.println(crt);
    Serial.println("hay algo");
    orden = Serial.read();

    switch (crt) {
      case 'F':
        Adelante();
        Serial.println("Adelante");
        delay(200);
        break;
      case 'B':
        Atras();
        Serial.println("Atras");
        delay(200);
        break;
      case 'L':
        Derecha();
        Serial.println("Derecha");
        delay(200);
        break;
      case 'R':
        Izquierda();
        Serial.println("Izquierds");
        delay(200);
        break;
      case 'U':
        Atras();
        Serial.println("Atras");
        delay(200);
        break;
      case 'G':
        AtrasDerecha();
        Serial.println("AtrasDerecha");
        delay(200);
        break;
      case 'N':
        AtrasIzquierda();
        Serial.println("AtrasIzquierda");
        delay(200);
        break;
      case 'M':
        AdelanteDerecha();
        Serial.println("AdelanteDerecha");
        delay(200);
        break;
      case 'V':
        AdelanteIzquierda();
        Serial.println("AdelanteIzquierda");
        delay(200);
        break;
      case 'W':
        Parar();
        Serial.println("Detener");
        delay(200);
        break;
      default:
        Parar();
        Serial.println("No hay movimiento");
        break;
    }
  }
  delay(10);
}

// Funciones

void Adelante() {
  //Direccion motor A
  analogWrite(ENA, VELOCIDAD);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  //Direccion motor B
  analogWrite(ENB, VELOCIDAD);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void Atras() {
  //Direccion motor A
  analogWrite(ENA, VELOCIDAD);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  //Direccion motor B
  analogWrite(ENB, VELOCIDAD);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void AtrasIzquierda() {
  //Direccion motor A
  analogWrite(ENA, VELOCIDAD);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  //Direccion motor B
  analogWrite(ENB, VELOCIDAD / 2);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void AtrasDerecha() {
  //Direccion motor A
  analogWrite(ENA, VELOCIDAD / 2);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  //Direccion motor B
  analogWrite(ENB, VELOCIDAD);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void Derecha() {
  //Direccion motor A
  analogWrite(ENA, 0);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  //Direccion motor B
  analogWrite(ENB, VELOCIDAD);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void AdelanteDerecha() {
  //Direccion motor A
  analogWrite(ENA, VELOCIDAD / 2);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  //Direccion motor B
  analogWrite(ENB, VELOCIDAD);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void Izquierda() {
  //Direccion motor A
  analogWrite(ENA, VELOCIDAD);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  //Direccion motor B
  analogWrite(ENB, 0);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void AdelanteIzquierda() {
  //Direccion motor A
  analogWrite(ENA, VELOCIDAD);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  //Direccion motor B
  analogWrite(ENB, VELOCIDAD / 2);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void Parar() {
  //Direccion motor A
  analogWrite(ENA, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  //Direccion motor B
  analogWrite(ENB, 0);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
