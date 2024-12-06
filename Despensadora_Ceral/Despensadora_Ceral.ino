#include <Servo.h>
#define fsrpin A3
// botones
int btn_cereal1 = 13;
int btn_cereal2 = 12;
int btn_cerealM = 11;
int btn_emergencia = 2;

// actuadores
int ser_cereal1 = 10;
int ser_cereal2 = 9;
int ser_despacho = 8;
int bob_leche = 7;
int mot_mezcla = 6;

// servomotores
Servo servoDespacho;
Servo servoCereal1;
Servo servoCereal2;

// sensores tri
int dis_leche = A0;
int dis_cereal1 = A1;
int dis_cereal2 = A2;
// sensores ech
int dis_lecheE = A4;
int dis_cereal1E = A5;
int dis_cereal2E = A6;

// indicadores
int led_ok = 5;
int led_error = 3;
int led_proceso = 4;

// valores iniciales
int inches = 0;
int cm = 0;
bool btnCereal1State = false;
bool btnCereal2State = false;
bool btnCerealMState = false;
bool btnCerealEState = false;

bool lecheState = false;
bool cereal1State = false;
bool cereal2State = false;

int fsrreading;

// lectura ultrasonico
long readUltrasonicDistance(int triggerPin, int echoPin) {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  return pulseIn(echoPin, HIGH);
}

// medir distancia
long measureDistance(int triggerPin, int echoPin) {
  long duration = readUltrasonicDistance(triggerPin, echoPin);
  if (duration == 0) {
    return -1; 
  }
  return duration * 0.01723;
}

void setup() {
  // terminal
  Serial.begin(9600);

  // variables
  pinMode(led_ok, OUTPUT);
  pinMode(led_error, OUTPUT);
  pinMode(led_proceso, OUTPUT);
  pinMode(bob_leche, OUTPUT);
  pinMode(mot_mezcla, OUTPUT);

  pinMode(btn_cereal1, INPUT);
  pinMode(btn_cereal2, INPUT);
  pinMode(btn_cerealM, INPUT);
  pinMode(btn_emergencia, INPUT);

  pinMode(dis_leche, OUTPUT);
  pinMode(dis_cereal1, OUTPUT);
  pinMode(dis_cereal2, OUTPUT);
  pinMode(dis_lecheE, INPUT);
  pinMode(dis_cereal1E, INPUT);
  pinMode(dis_cereal2E, INPUT);

  servoDespacho.attach(ser_despacho);
  servoCereal1.attach(ser_cereal1);
  servoCereal2.attach(ser_cereal2);

  servoCereal1.write(0);
  servoCereal2.write(0);
  servoDespacho.write(0);

  digitalWrite(mot_mezcla, HIGH);
}

void loop() {
  fsrreading = analogRead(fsrpin);
  Serial.println(fsrreading);
  long leche = measureDistance(dis_leche, dis_lecheE);
  long cereal1 = measureDistance(dis_cereal1, dis_cereal1E);
  long cereal2 = measureDistance(dis_cereal2, dis_cereal2E);

  btnCereal1State = digitalRead(btn_cereal1);
  btnCereal2State = digitalRead(btn_cereal2);
  btnCerealMState = digitalRead(btn_cerealM);
  btnCerealEState = digitalRead(btn_emergencia);
  if (fsrreading > 50) {
    if (leche > 10) {
      Serial.println("Hay baja cantidad de leche");
      lecheState = true;
    } else {
      lecheState = false;
    }
    if (cereal1 > 12) {
      Serial.println("Hay baja cantidad de ceral 1");
      cereal1State = true;
    } else {
      cereal1State = false;
    }
    if (cereal2 > 12) {
      Serial.println("Hay baja cantidad de ceral 2");
      cereal2State = true;
    } else {
      cereal2State = false;
    }

    // cereal 1
    if (btnCereal1State == HIGH && !lecheState && !cereal1State) {
      Serial.println("Botón Cereal 1 presionado");
      servoCereal1.write(90);
      digitalWrite(led_ok, HIGH);
      delay(2000);
      digitalWrite(led_ok, LOW);
      servoCereal1.write(0);
      digitalWrite(bob_leche, HIGH);
      delay(2000);
      digitalWrite(bob_leche, LOW);
      servoDespacho.write(90);
      delay(2000);
      servoDespacho.write(0);
    }
    // cereal 2
    if (btnCereal2State == HIGH && !lecheState && !cereal2State) {
      Serial.println("Botón Cereal 2 presionado");
      servoCereal2.write(90);
      digitalWrite(led_proceso, HIGH);
      delay(2000);
      digitalWrite(led_proceso, LOW);
      servoCereal2.write(0);
      digitalWrite(bob_leche, HIGH);
      delay(2000);
      digitalWrite(bob_leche, LOW);
      servoDespacho.write(90);
      delay(2000);
      servoDespacho.write(0);
    }
    // mescla de cereal
    if (btnCerealMState == HIGH && !lecheState && !cereal2State && !cereal1State) {
      Serial.println("Botón Cereal 3 presionado");
      servoCereal1.write(90);
      digitalWrite(led_proceso, HIGH);
      digitalWrite(led_ok, HIGH);
      delay(1000);
      digitalWrite(led_proceso, LOW);
      digitalWrite(led_ok, LOW);
      servoCereal1.write(0);
      servoCereal2.write(90);
      delay(1000);
      servoCereal2.write(0);
      digitalWrite(mot_mezcla, LOW);
      delay(2000);
      digitalWrite(mot_mezcla, HIGH);
      digitalWrite(bob_leche, HIGH);
      delay(2000);
      digitalWrite(bob_leche, LOW);
      servoDespacho.write(90);
      delay(2000);
      servoDespacho.write(0);
    }
    // parado de emergencia
    if (btnCerealEState == HIGH) {
      Serial.println("Botón Emergencia presionado");
      servoCereal1.write(0);
      servoCereal2.write(0);
      servoDespacho.write(0);
      digitalWrite(led_error, HIGH);
      delay(1000);
      digitalWrite(led_error, LOW);
      digitalWrite(bob_leche, LOW);
    }

    Serial.print("Leche: ");
    Serial.println(leche);
    Serial.print("Cereal1: ");
    Serial.println(cereal1);
    Serial.print("Cereal2: ");
    Serial.println(cereal2);
  } else {
    Serial.println("Necesita poner un envase: ");
  }
}