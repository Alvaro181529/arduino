#include <Servo.h>
// servo motor
Servo servoLavado;
Servo servoEnjuage;
Servo servoInicio;
Servo servoInicio1;
const int cinta = 28;

// BOTONES
const int botonInicio = 53;
const int botonEmergencia = 52;
bool botonInicioEstado = false;
bool botonEmergenciaEstado = false;
int estadoInicio = 0;
int estadoEmergencia = 0;
// FinalCarrera
const int finalCarreraEmpiezo = 2;
const int finalCarreraLavado = 3;
const int finalCarreraEnjuage = 4;
const int finalCarreraSalida = 5;

//LAVADO
const int lavadoLlenado = 6;
const int lavadoMangueraB = 7;
const int lavadoMangueraA = 8;
const int lavadoValvula = 9;

const int reguladorLavado = A0;
const int ledLavado = 10;
const int TriggerL = 11;
const int EchoL = 12;
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

const int max = 13;    // maximo
const int medio = 17;  // lleno
const int min = 19;   // vacio

const int maxT = 50;    // maximo
const int medioT = 20;  // lleno
const int minT = 10;     // vacio

void setup() {
  Serial.begin(9600);
  // Configura los pines como salidas
  pinMode(lavadoLlenado, OUTPUT);
  pinMode(lavadoValvula, OUTPUT);
  pinMode(lavadoMangueraA, OUTPUT);
  pinMode(lavadoMangueraB, OUTPUT);
  pinMode(enjuageLlenado, OUTPUT);
  pinMode(enjuageValvula, OUTPUT);
  pinMode(enjuageMangueraA, OUTPUT);
  pinMode(enjuageMangueraB, OUTPUT);
  pinMode(cinta, OUTPUT);
  pinMode(TriggerE, OUTPUT);  //pin comox salida
  pinMode(TriggerL, OUTPUT);  //pin comox salida
  pinMode(ledEnjuage, OUTPUT);

  servoInicio.attach(22);
   servoInicio1.attach(23);
  servoLavado.attach(24);
  servoEnjuage.attach(25);

  // Configura los pines como entradas
  pinMode(finalCarreraLavado, INPUT_PULLUP);
  pinMode(finalCarreraEmpiezo, INPUT_PULLUP);
  pinMode(finalCarreraEnjuage, INPUT_PULLUP);
  pinMode(finalCarreraSalida, INPUT_PULLUP);
  pinMode(EchoL, INPUT);  //pin como entrada
  pinMode(EchoE, INPUT);  //pin como entrada
  pinMode(reguladorLavado, INPUT);
  pinMode(reguladorEnjuage, INPUT);
  pinMode(botonInicio, INPUT);
  pinMode(botonEmergencia, INPUT);

  // inicializacion
  digitalWrite(cinta, LOW);          //Inicializamos
  digitalWrite(lavadoLlenado, LOW);   //Inicializamos
  digitalWrite(lavadoValvula, LOW);   //Inicializamos
  digitalWrite(TriggerL, LOW);        //Inicializamos
  digitalWrite(ledLavado, LOW);       //Inicializamos
  digitalWrite(enjuageLlenado, LOW);  //Inicializamos
  digitalWrite(enjuageValvula, LOW);  //Inicializamos
  digitalWrite(TriggerE, LOW);        //Inicializamos
  digitalWrite(ledEnjuage, LOW);      //Inicializamos

  servoLavado.write(90);
  servoEnjuage.write(0);
  servoInicio.write(90);
  servoInicio1.write(180);
}
int ValvulaLavado(int potL, int value) {
  int estadoTL = TanaqueLavado(potL);
  Serial.print("Estado PotL: ");
  Serial.println(estadoTL);
  Serial.print("distancia: ");
  Serial.println(value);  // Imprimimos la distancia
  if (estadoTL == 1) {
    if (value < max) {
      Serial.println("estado: desbordamiento");
      // Cuando esté vacío
      digitalWrite(lavadoValvula, LOW);
      digitalWrite(lavadoLlenado, LOW);
      estadoLav = 0;
    } else if (value >= medio || value >= min) {
      Serial.println("estado: vacio");
      // Cuando esté en desbordamiento
      digitalWrite(lavadoValvula, LOW);
      digitalWrite(lavadoLlenado, HIGH);
      if ( value >= min) {
        digitalWrite(ledLavado, HIGH);
      }
      estadoLav = 2;
    } else if (value > medio ) {
      Serial.println("estado: lleno");
      // Cuando esté lleno
      digitalWrite(lavadoValvula, LOW);
      digitalWrite(lavadoLlenado, LOW);
      digitalWrite(ledLavado, LOW);
      estadoLav = 1;
    } else {
      Serial.println("estado: intermedio");
      // Este caso cubre cuando está en un estado intermedio
      estadoLav = 1;
    }
  } else if (estadoTL == 2) {
    if (value < min) {
      // Cuando este vacio
      digitalWrite(lavadoLlenado, HIGH);
      estadoLav = 0;
      if (value <= max) {
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
  Serial.print("Valor potenciometro L:" );
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


int distanciaLavado() {
  int t;
  int d;

  digitalWrite(TriggerL, HIGH);
  delayMicroseconds(5);  // Enviamos un pulso de 5us
  digitalWrite(TriggerL, LOW);

  t = pulseIn(EchoL, HIGH);  // Obtenemos el ancho del pulso
  d = t / 59;                // Escalamos el tiempo a una distancia en cm

  return d;
}

void MangeraLavado() {
  delay(3000);
  digitalWrite(lavadoMangueraA, HIGH);
  digitalWrite(lavadoMangueraB, LOW);
  delay(4000);
  digitalWrite(lavadoMangueraA, LOW);
  digitalWrite(lavadoMangueraB, LOW);
  delay(2000);
  digitalWrite(lavadoValvula, HIGH);
  delay(2000);
  digitalWrite(lavadoValvula, LOW);
  delay(.4000);
  digitalWrite(lavadoMangueraA, LOW);
  digitalWrite(lavadoMangueraB, HIGH);
  delay(5000);
  digitalWrite(lavadoMangueraA, LOW);
  digitalWrite(lavadoMangueraB, LOW);
}
void Lavado() {
  potL = analogRead(reguladorLavado);
  int lavadoDistancia = distanciaLavado();
  int lavado = ValvulaLavado(potL, lavadoDistancia);
  int estadoLav = digitalRead(finalCarreraLavado);
  if (estadoLav == LOW) {
    Serial.println("esta activo Lavado");
    // Acción cuando el final de carrera Lavado está activado
    if (lavado == 1 || lavado == 2) {
        delay(1000);
      digitalWrite(cinta, LOW);
      servoLavado.write(180);
      MangeraLavado();
      servoLavado.write(90);
      delay(2000);
      digitalWrite(cinta, HIGH);
      delay(2000);
    } else {
      digitalWrite(lavadoValvula, LOW);
    }
  } else {
    digitalWrite(lavadoValvula, LOW);
  }
  Serial.println("estado lavado:" + lavado);
}

int ValvulaEnjuage(int potE, int value) {
  int estadoTE = TanaqueEnjuage(potE);
  if (estadoTE == 1) {
    if (value < max) {
      Serial.println("estado: desbordamiento");
      // Cuando esté vacío
      digitalWrite(enjuageValvula, LOW);
      digitalWrite(enjuageLlenado, LOW);
      digitalWrite(ledEnjuage, HIGH);
      estadoEnj = 0;
    } else if (value >= medio || value >= min) {
      Serial.println("estado: vacio");
      digitalWrite(enjuageValvula, LOW);
      digitalWrite(enjuageLlenado, HIGH);
      if ( value >= min) {
        digitalWrite(ledEnjuage, HIGH);
      }
      estadoEnj = 2;
    } else if (value > medio ) {
      Serial.println("estado: lleno");
      // Cuando este lleno
      digitalWrite(enjuageValvula, LOW);
      digitalWrite(enjuageLlenado, LOW);
      digitalWrite(ledEnjuage, LOW);
      estadoEnj = 1;
    } else {
      Serial.println("estado: intermedio");
      digitalWrite(ledEnjuage, LOW);
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
        digitalWrite(ledEnjuage, HIGH);
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
    // apaga valvula y tanque
    digitalWrite(enjuageValvula, LOW);
    digitalWrite(enjuageLlenado, LOW);
  }
  return estadoEnj;
}

int TanaqueEnjuage(int potE) {
  Serial.println(potE);
  if (potE < min) {
    // Cuando este vacio
    digitalWrite(ledEnjuage, HIGH);
    estadoTE = 0;
  } else if (potE > maxT) {
    // Cuando este en desvordamiento
    digitalWrite(ledEnjuage, HIGH);
    estadoTE = 2;
  } else if (potE > medioT) {
    // Cuando este lleno
    digitalWrite(ledEnjuage, LOW);
    estadoTE = 1;
  } else {
    // Cuando este normal
    digitalWrite(ledEnjuage, LOW);
    estadoTE = 1;
  }
  return estadoTE;
}

int distanciaEnjuage() {
  int t;
  int d;

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
  delay(5000);
  digitalWrite(enjuageMangueraA, LOW);
  digitalWrite(enjuageMangueraB, LOW);
  delay(2000);
  digitalWrite(enjuageValvula, HIGH);
  delay(10000);
  digitalWrite(enjuageValvula, LOW);
  delay(2000);
  digitalWrite(enjuageMangueraA, LOW);
  digitalWrite(enjuageMangueraB, HIGH);
  delay(3000);
  digitalWrite(enjuageMangueraA, LOW);
  digitalWrite(enjuageMangueraB, LOW);
}
void Enjuage() {
  potE = analogRead(reguladorEnjuage);
  Serial.print("POT ENJUAGE");
  Serial.println(potE);
  int lavadoEnjuage = distanciaEnjuage();
  int enjuage = ValvulaEnjuage(potE, lavadoEnjuage);
  int estadoEjuage = digitalRead(finalCarreraEnjuage);
  if (estadoEjuage == LOW) {
    Serial.println("esta activo Ejuage");
    // Acción cuando el final de carrera Lavado está activado
    if (enjuage == 1 || enjuage == 2) {
          delay(1000);
      digitalWrite(cinta, LOW);
      servoEnjuage.write(90);
      MangeraEnjuage();
      servoEnjuage.write(0);
      delay(2000);
      digitalWrite(cinta, HIGH);
      delay(4000);
    } else {
      digitalWrite(enjuageValvula, LOW);
    }
  } else {
    digitalWrite(enjuageValvula, LOW);
  }
  Serial.println("estado enjuage:" + enjuage);
}
// Función para apagar todas las salidas del sistema
void ApagarSistema() {
  digitalWrite(cinta, LOW);
  digitalWrite(lavadoLlenado, LOW);
  digitalWrite(lavadoValvula, LOW);
  digitalWrite(TriggerL, LOW);
  digitalWrite(enjuageLlenado, LOW);
  digitalWrite(enjuageValvula, LOW);
  digitalWrite(TriggerE, LOW);
  digitalWrite(lavadoMangueraA, LOW);
  digitalWrite(lavadoMangueraB, LOW);
  digitalWrite(enjuageMangueraA, LOW);
  digitalWrite(enjuageMangueraB, LOW);
}
void loop() {
  estadoInicio = digitalRead(botonInicio);
  estadoEmergencia = digitalRead(botonEmergencia);

  if (estadoInicio == HIGH) {
    digitalWrite(cinta, HIGH);
    servoInicio.write(180);
    servoInicio1.write(90);
    delay(7500);
    servoInicio1.write(180);
    servoInicio.write(90);
    delay(2000);

    botonInicioEstado = true;
    botonEmergenciaEstado = false;
    Serial.println("Botón de inicio presionado");
  } else if (estadoEmergencia == HIGH) {
    servoInicio1.write(180);
    servoInicio.write(90);
    botonInicioEstado = false;
    botonEmergenciaEstado = true;
    Serial.println("Botón de EMERGENCIA presionado");
  }

  if (botonInicioEstado) {
    servoInicio1.write(180);
    servoInicio.write(90);
    Lavado();
    Enjuage();
  } else if (botonEmergenciaEstado) {
    ApagarSistema();
  }
}
  // Eliminamos las llamadas repetitivas a servoInicio1.write(180)
  // Eliminamos
