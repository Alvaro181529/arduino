#include <Servo.h>

Servo servoMotor;
Servo servoMotor1;
Servo servoMotor2;
const int Trigger = 2;  //Pin digital 2 para el Trigger del sensor
const int Echo = 3;     //Pin digital 3 para el echo del sensor
long t;                 //timepo que demora en llegar el eco
long d;                 //distancia en centimetros
int notas[] = { 262, 294, 330, 349 };
int duracion[] = { 100, 100, 100, 100 };
const int foco = 11;
bool estadoBoton = false;
void setup() {
  pinMode(foco, OUTPUT);
  pinMode(Trigger, OUTPUT);    //pin como salida
  pinMode(Echo, INPUT);        //pin como entrada
  digitalWrite(Trigger, LOW);  //Inicializamos el pin con 0
  digitalWrite(foco, LOW);
  Serial.begin(9600);
  servoMotor.attach(9);
  servoMotor1.attach(8);
  servoMotor2.attach(10);
}
int count = 0;
void loop() {

  sensorDisYServo();
}

void sensorDisYServo() {
  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);  //Enviamos un pulso de 10us
  digitalWrite(Trigger, LOW);
  t = pulseIn(Echo, HIGH);  //obtenemos el ancho del pulso
  d = t / 59;               //escalamos el tiempo a una distancia en cm

  Serial.print("Distancia: ");
  Serial.print(d);  //Enviamos serialmente el valor de la distancia
  Serial.print("cm");
  Serial.println();


  delay(100);  //Hacemos una pausa de 100ms

  if (d > 30) {
    // Si la distancia es mayor a 30 cm, activar el rele y mover el servo a 180 grados
    servoMotor2.write(0);
    rele(LOW);
    for (int i = 0; i <= 170; i++) {
      servoMotor1.write(i);  // Angulo 180 grados
    }
     for (int i = 170; i >= 0; i--) {
      servoMotor1.write(i);  // Angulo 180 
    }
    servoMotor.write(140);  // Angulo 180 grados
  } else if (d < 10) {
    // Si la distancia es menor a 10 cm, desactivar el rele y mover el servo a 0 grados
    rele(HIGH);
    buzer();
    servoMotor2.write(90);

  } else {
    // Si la distancia está entre 10 y 30 cm, mover el servo proporcionalmente
    int angulo = map(d, 10, 30, 50, 180);
    servoMotor.write(angulo);
    servoMotor1.write(angulo);
  }
}

void buzer() {
  for (int i = 0; i < 4; i++) {
    tone(5, notas[i]);
    delay(duracion[i]);
    noTone(5);
  }
}

void rele(int estadoFoco) {
  digitalWrite(foco, estadoFoco);
}
