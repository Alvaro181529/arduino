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