#include <Servo.h>
// servo motor
Servo servoLavado;
Servo servoEnjuage;
Servo servoInicio;

const int cinta = 11;

// pines
const int finalCarreraEmpiezo = 2;
const int finalCarreraLavado = 3;
const int finalCarreraEnjuage =4;
const int finalCarreraSalida = 5;
//LAVADO
const int lavadoLlenado = 6;
const int lavadoMangueraB = 7;
const int lavadoMangueraA = 8;
const int lavadoValvula = 9;


const int reguladorLavado = A0;
const int ledLavado = 9;
const int TriggerL = 10;
const int EchoL = 11;
//EJUAGUE
const int enjuageLlenado = 14;
const int enjuageMangueraB = 15;
const int enjuageMangueraA = 16;
const int enjuageValvula = 17;

const int ledEnjuage = 18;
const int reguladorEnjuage = A1;
const int TriggerE = 19;
const int EchoE = 20;

// variables
int potL = 0;
int estadoTL = 1;
int estadoLav = 1;

int potE = 0;
int estadoTE = 1;
int estadoEnj = 1;

const int max = 20;    // maximo
const int medio = 30;  // lleno
const int min = 200;   // vacio

const int maxT = 950;    // maximo
const int medioT = 400;  // lleno
const int minT = 20;     // vacio

void setup() {
  Serial.begin(9600);
  // Configura los pines como salidas
  pinMode(lavadoLlenado, OUTPUT);
  pinMode(lavadoValvula, OUTPUT);
  pinMode(lavadoMangueraA, OUTPUT);
  pinMode(lavadoMangueraB, OUTPUT);
  pinMode(EnjuageLlenado, OUTPUT);
  pinMode(EnjuageValvula, OUTPUT);
  pinMode(EnjuageMangueraA, OUTPUT);
  pinMode(EnjuageMangueraB, OUTPUT);
  pinMode(cinta, OUTPUT);
  pinMode(TriggerE, OUTPUT);  //pin comox salida
  pinMode(ledEnjuage, OUTPUT);

  // servoInicio.attach(9);
  // servoEnjuage.attach(9);
  servoLavado.attach(7);

  // Configura los pines como entradas
  pinMode(finalCarreraLavado, INPUT_PULLUP);
  pinMode(finalCarreraEmpiezo, INPUT_PULLUP);
  pinMode(finalCarreraEnjuage, INPUT_PULLUP);
  pinMode(finalCarreraSalida, INPUT_PULLUP);
  pinMode(EchoL, INPUT);  //pin como entrada
  pinMode(EchoE, INPUT);  //pin como entrada
  pinMode(reguladorLavado, INPUT);

  // inicializacion
  digitalWrite(lavadoLlenado, LOW);   //Inicializamos
  digitalWrite(lavadoValvula, LOW);   //Inicializamos
  digitalWrite(TriggerL, LOW);        //Inicializamos
  digitalWrite(ledLavado, LOW);       //Inicializamos
  digitalWrite(EnjuageLlenado, LOW);  //Inicializamos
  digitalWrite(EnjuageValvula, LOW);  //Inicializamos
  digitalWrite(TriggerE, LOW);        //Inicializamos
  digitalWrite(ledEnjuage, LOW);      //Inicializamos

  servoLavado.write(90);
}
int ValvulaLavado(int potL, long value) {
  int estadoTL = TanaqueLavado(potL);
  Serial.println(estadoTL);
  if (estadoTL == 1) {
    if (value > min) {
      // Cuando este vacio
      digitalWrite(lavadoValvula, LOW);
      digitalWrite(lavadoLlenado, HIGH);
      estadoLav = 0;
    } else if (value < max) {
      // Cuando este en desbordamiento
      digitalWrite(lavadoValvula, LOW);
      digitalWrite(lavadoLlenado, LOW);
      digitalWrite(ledLavado, HIGH);
      estadoLav = 2;
    } else if (value < medio) {
      // Cuando este lleno
      digitalWrite(lavadoValvula, LOW);
      digitalWrite(lavadoLlenado, LOW);
      estadoLav = 1;
    } else {
      // Cuando este lleno
      estadoLav = 1;
    }
  } else if (estadoTL == 2) {
    if (value < 300) {
      // Cuando este vacio
      digitalWrite(lavadoLlenado, HIGH);
      estadoLav = 0;
      if (value < 40) {
        // Cuando este en desbordamiento
        digitalWrite(lavadoValvula, HIGH);
        digitalWrite(ledLavado, HIGH);
        estadoLav = 2;
      } else {
        digitalWrite(lavadoValvula, LOW);
        estadoLav = 2;
      }
    } else {
      digitalWrite(lavadoValvula, LOW);
      // Cuando este lleno
      estadoLav = 1;
    }
  } else {
    // Gira el motor 2 hacia la derecha
    digitalWrite(lavadoValvula, LOW);
    digitalWrite(lavadoLlenado, LOW);
  }
  return estadoLav;
}
int TanaqueLavado(int potL) {
  Serial.println(potL);
  if (potL < min) {
    // Cuando este vacio
    digitalWrite(ledLavado, HIGH);
    estadoTL = 0;
  } else if (potL > maxT) {
    // Cuando este en desvordamiento
    digitalWrite(ledLavado, HIGH);
    estadoTL = 2;
  } else if (potL > medioT) {
    // Cuando este lleno
    digitalWrite(ledLavado, LOW);
    estadoTL = 1;
  } else {
    // Cuando este normal
    digitalWrite(ledLavado, LOW);
    estadoTL = 1;
  }
  return estadoTL;
}

long distanciaLavado() {
  long t;
  long d;

  digitalWrite(TriggerL, HIGH);
  delayMicroseconds(5);  //Enviamos un pulso de 5us
  digitalWrite(TriggerL, LOW);

  t = pulseIn(EchoL, HIGH);  //obtenemos el ancho del pulso
  d = t / 59;                //escalamos el tiempo a una distancia en cm
  return d;
}
void MangeraLavado() {
  digitalWrite(lavadoMangueraA, HIGH);
  digitalWrite(lavadoMangueraB, LOW);
  delay(1000);
  digitalWrite(lavadoMangueraA, LOW);
  digitalWrite(lavadoMangueraB, LOW);
  digitalWrite(lavadoValvula, HIGH);
  delay(15000);
  digitalWrite(lavadoMangueraA, HIGH);
  digitalWrite(lavadoMangueraB, LOW);
  delay(1000);
  digitalWrite(lavadoMangueraA, LOW);
  digitalWrite(lavadoMangueraB, LOW);
}
void Lavado() {
  potL = analogRead(reguladorLavado);
  long lavadoDistancia = distancia();
  int lavado = ValvulaLavado(potL, lavadoDistancia);
  int estadoLav = digitalRead(finalCarreraLavado);
  if (estadoLav == LOW) {
    Serial.println("esta activo");
    // Acción cuando el final de carrera Lavado está activado
    if (lavado == 1 || lavado == 2) {
      digitalWrite(cinta, LOW);
      servoLavado.write(0);
      MangeraLavado();
      servoLavado.write(90);
      delay(2000);
      digitalWrite(cinta, HIGH);
      delay(1000);
    } else {
      digitalWrite(lavadoValvula, LOW);
    }
  } else {
    digitalWrite(lavadoValvula, LOW);
  }
  Serial.println(lavado);
}


int ValvulaEnjuage(int potE, long value) {
  int estadoTE = TanaqueLavado(potE);
  Serial.println(estadoTE);
  if (estadoTE == 1) {
    if (value > min) {
      // Cuando este vacio
      digitalWrite(enjuageValvula, LOW);
      digitalWrite(enjuageLlenado, HIGH);
      estadoEnj = 0;
    } else if (value < max) {
      // Cuando este en desbordamiento
      digitalWrite(enjuageValvula, LOW);
      digitalWrite(enjuageLlenado, LOW);
      digitalWrite(ledLavado, HIGH);
      estadoEnj = 2;
    } else if (value < medio) {
      // Cuando este lleno
      digitalWrite(enjuageValvula, LOW);
      digitalWrite(enjuageLlenado, LOW);
      estadoEnj = 1;
    } else {
      // Cuando este lleno
      estadoEnj = 1;
    }
  } else if (estadoTE == 2) {
    if (value < 300) {
      // Cuando este vacio
      digitalWrite(enjuageLlenado, HIGH);
      estadoEnj = 0;
      if (value < 40) {
        // Cuando este en desbordamiento
        digitalWrite(enjuageValvula, HIGH);
        digitalWrite(ledLavado, HIGH);
        estadoEnj = 2;
      } else {
        digitalWrite(enjuageValvula, LOW);
        estadoEnj = 2;
      }
    } else {
      digitalWrite(enjuageValvula, LOW);
      // Cuando este lleno
      estadoEnj = 1;
    }
  } else {
    // Gira el motor 2 hacia la derecha
    digitalWrite(enjuageValvula, LOW);
    digitalWrite(enjuageLlenado, LOW);
  }
  return estadoEnj;
}

int TanaqueEnjuage(int potE) {
  Serial.println(potE);
  if (potE < min) {
    // Cuando este vacio
    digitalWrite(ledLavado, HIGH);
    estadoTE = 0;
  } else if (potE > maxT) {
    // Cuando este en desvordamiento
    digitalWrite(ledLavado, HIGH);
    estadoTE = 2;
  } else if (potE > medioT) {
    // Cuando este lleno
    digitalWrite(ledLavado, LOW);
    estadoTE = 1;
  } else {
    // Cuando este normal
    digitalWrite(ledLavado, LOW);
    estadoTE = 1;
  }
  return estadoTE;
}

long distanciaEnjuage() {
  long t;
  long d;

  digitalWrite(TriggerE, HIGH);
  delayMicroseconds(5);  //Enviamos un pulso de 5us
  digitalWrite(TriggerE, LOW);

  t = pulseIn(EchoE, HIGH);  //obtenemos el ancho del pulso
  d = t / 59;                //escalamos el tiempo a una distancia en cm
  return d;
}
void MangeraEnjuage() {
  digitalWrite(enjuageMangueraA, HIGH);
  digitalWrite(enjuageMangueraB, LOW);
  delay(1000);
  digitalWrite(enjuageMangueraA, LOW);
  digitalWrite(enjuageMangueraB, LOW);
  digitalWrite(enjuageValvula, HIGH);
  delay(15000);
  digitalWrite(enjuageMangueraA, HIGH);
  digitalWrite(enjuageMangueraB, LOW);
  delay(1000);
  digitalWrite(enjuageMangueraA, LOW);
  digitalWrite(enjuageMangueraB, LOW);
}
void Enjuage() {
  potE = analogRead(reguladorEnjuage);
  long lavadoDistancia = distancia();
  int enjuage = ValvulaLavado(potE, lavadoDistancia);
  int estadoEjuage = digitalRead(finalCarreraEnjuage);
  if (estadoEjuage == LOW) {
    Serial.println("esta activo");
    // Acción cuando el final de carrera Lavado está activado
    if (enjuage == 1 || enjuage == 2) {
      digitalWrite(cinta, LOW);
      servoLavado.write(0);
      MangeraLavado();
      servoLavado.write(90);
      delay(2000);
      digitalWrite(cinta, HIGH);
      delay(1000);
    } else {
      digitalWrite(enjuageValvula, LOW);
    }
  } else {
    digitalWrite(enjuageValvula, LOW);
  }
  Serial.println(enjuage);
}

void loop() {
  Lavado();
  Enjuage();
  delay(10);  // Espera 10ms
}