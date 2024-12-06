#include <LiquidCrystal.h>
#define ADC_VREF_mV 5.0  // in millivolt
#define ADC_RESOLUTION 1023.0
#define PIN_LM35 A1  // LM35

#include "DHT.h"

#define DHTPIN A2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
const int ventilador = 10;
const int foco = 11;

const int botonMasPin = 13;    // pin desde el boton mas
const int botonMenosPin = 12;  // pin desde el boton menos
int temperaturaControl = 0;
int next = 0;
int count = 0;

int adcVal;
float milliVolt;
float tempC;
float tempF;
float humidity;
float temperature;
int time;
int vel;
// Definir los pines del L293D para el segundo motor
const int pinENA2 = 9;
const int pinIN3 = A4;
const int pinIN4 = A5;

// variables para trabajar con los botones
boolean lastBotonMas = LOW;
boolean currentBotonMas = LOW;
boolean lastBotonMenos = LOW;
boolean currentBotonMenos = LOW;

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);  // objeto lcd

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);  // inicializa el LCD
  lcd.clear();
  lcd.clear();
  pinMode(ventilador, OUTPUT);
  pinMode(foco, OUTPUT);
  digitalWrite(ventilador, LOW);
  digitalWrite(foco, LOW);
  pinMode(pinENA2, OUTPUT);
  pinMode(pinIN3, OUTPUT);
  pinMode(pinIN4, OUTPUT);

  pinMode(botonMasPin, INPUT);
  pinMode(botonMenosPin, OUTPUT);

  lcd.setCursor(0, 0);
  lcd.print("Inicializando...");
  for (int i = 0; i <= 15; i++) {
    lcd.setCursor(i, 1);
    lcd.write((byte)4);
    delay(250);
  }
  delay(250);
  lcd.clear();
  delay(500);
  temperaturaControl = 0;
  dht.begin();
}
void loop() {
  buttons();
  desplegarInformacion(temperaturaControl);
}
void DHT11S() {
  // Read temperature and humidity from DHT11 sensor
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
}
void LM35() {
  tempC = analogRead(PIN_LM35);
  // Calculamos la temperatura con la fórmula
  tempC = (ADC_VREF_mV * tempC * 90.0) / ADC_RESOLUTION;
  tempC = int(tempC);
}
void buttons() {
  currentBotonMas = digitalRead(botonMasPin);
  currentBotonMenos = digitalRead(botonMenosPin);
  // subir temperatura de control
  if (currentBotonMas == HIGH && lastBotonMas == LOW) {
    delay(15);  // debouncing time
    if (digitalRead(botonMasPin)) {
      temperaturaControl++;
      count = 0;
      lastBotonMas = HIGH;
    }
    if (temperaturaControl > 5) {
      temperaturaControl = 0;
    }
  } else {
    lastBotonMas = digitalRead(botonMasPin);
  }


  if (digitalRead(botonMenosPin)) {
    lastBotonMenos = HIGH;
    MotorD(1500, 200);
    MotorS(4000, 0);
    next++;
  }
}
void MotorS(int time, int vel) {
  // Hacer girar el segundo motor en la dirección opuesta a velocidad media durante 2 segundos
  digitalWrite(pinIN3, LOW);  // Establecer dirección del segundo motor
  digitalWrite(pinIN4, LOW);  // Establecer dirección del segundo motor
  analogWrite(pinENA2, vel);  // Establecer velocidad media del segundo motor (128 = ~50%)
  delay(time);                // Esperar 2 segundos
}

void MotorD(int time, int vel) {
  // Hacer girar el segundo motor en la dirección opuesta a velocidad media durante 2 segundos
  digitalWrite(pinIN3, LOW);   // Establecer dirección del segundo motor
  digitalWrite(pinIN4, HIGH);  // Establecer dirección del segundo motor
  analogWrite(pinENA2, vel);   // Establecer velocidad media del segundo motor (128 = ~50%)
  delay(time);                 // Esperar 2 segundos
}

void MotorA(int time, int vel) {
  // Hacer girar el segundo motor en la dirección opuesta a velocidad media durante 2 segundos
  digitalWrite(pinIN3, HIGH);  // Establecer dirección del segundo motor
  digitalWrite(pinIN4, LOW);   // Establecer dirección del segundo motor
  analogWrite(pinENA2, vel);   // Establecer velocidad media del segundo motor (128 = ~50%)
  delay(time);                 // Esperar 2 segundos
}

// Función para controlar dispositivos y ejecutar el código común
void controlarDispositivos(int estadoVentilador, int estadoFoco) {
  count++;
  Serial.println("count: " + String(count));

  if (count == 1) {
    count++;
    Serial.println(count);
    MotorA(1600, 220);
    MotorS(9000, 0);
    MotorD(1500, 200);
    MotorS(0, 0);
  }
  digitalWrite(ventilador, estadoVentilador);
  digitalWrite(foco, estadoFoco);
}
void lcdFun(String titulo, int t, int h, int promedioT, int humidity, int temperaturaControl) {
  lcd.clear();
  if (temperaturaControl >= 1) {
    lcd.setCursor(0, 0);
    lcd.print(titulo);
    lcd.setCursor(5, 0);
    lcd.print("T:");
    lcd.setCursor(7, 0);
    lcd.print(t);
    lcd.setCursor(9, 0);
    lcd.print("C");
    lcd.setCursor(11, 0);
    lcd.print("H:");
    lcd.setCursor(13, 0);
    lcd.print(h);
    lcd.setCursor(15, 0);
    lcd.print("%");
  } else if (temperaturaControl == 0) {
    lcd.setCursor(0, 0);
    lcd.print(titulo);
  }

  lcd.setCursor(0, 1);
  lcd.print("Tem:");
  lcd.setCursor(4, 1);
  lcd.print(int(promedioT));
  lcd.setCursor(6, 1);
  lcd.print("C");
  lcd.setCursor(9, 1);
  lcd.print("Hum:");
  lcd.setCursor(13, 1);
  lcd.print(int(humidity));
  lcd.setCursor(15, 1);
  lcd.print("%");

  Serial.println(temperaturaControl);
}
void desplegarInformacion(int &temperaturaControl) {

  delay(500);
  LM35();
  DHT11S();
  float promedioT = (tempC + temperature) / 2;
  Serial.println(promedioT);
  Serial.println(humidity);

  switch (temperaturaControl) {
    Serial.println("count info");
    case 1:

      lcdFun("Des: ", 20, 50, promedioT, humidity, temperaturaControl);
      if (next >= 1) {
        MotorS(6000, 0);
        if ((promedioT < 20 && humidity < 50) || (promedioT < 20 && humidity > 50)) {
          controlarDispositivos(LOW, HIGH);
        }

        else if ((promedioT > 20 && humidity > 50) || (promedioT > 20 && humidity < 50)) {
          controlarDispositivos(HIGH, LOW);
        }

        else if ((promedioT >= 20 && humidity >= 50) || (promedioT <= 20 && humidity <= 50) || (promedioT == 20 && humidity == 50)) {
          controlarDispositivos(LOW, LOW);
          MotorA(3600, 220);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Finalizado");
          delay(1000);
          MotorS(1600, 220);
          buttons();
          temperaturaControl = 0;
          return;
        }
        break;
        case 2:
          lcdFun("Des: ", 25, 40, promedioT, humidity, temperaturaControl);
          if (next >= 1) {
            MotorS(6000, 0);
            if ((promedioT < 25 && humidity < 40) || (promedioT < 25 && humidity > 40)) {
              controlarDispositivos(LOW, HIGH);
            }

            else if ((promedioT > 25 && humidity > 40) || (promedioT > 25 && humidity < 40)) {
              controlarDispositivos(HIGH, LOW);
            }

            else if ((promedioT >= 25 && humidity >= 40) || (promedioT <= 25 && humidity <= 40) || (promedioT == 25 && humidity == 40)) {
              controlarDispositivos(LOW, LOW);
              MotorA(3600, 220);
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("Finalizado");
              delay(1000);
              MotorS(1600, 220);
              buttons();
              temperaturaControl = 0;
              return;
            }
          }
          break;
        case 3:

          lcdFun("Des: ", 30, 55, promedioT, humidity, temperaturaControl);
          if (next >= 1) {
            MotorS(6000, 0);
            if ((promedioT < 30 && humidity < 30) || (promedioT < 30 && humidity > 30)) {
              controlarDispositivos(LOW, HIGH);
            }

            else if ((promedioT > 30 && humidity > 30) || (promedioT > 30 && humidity < 30)) {
              controlarDispositivos(HIGH, LOW);
            }

            else if ((promedioT >= 30 && humidity >= 30) || (promedioT <= 30 && humidity <= 30) || (promedioT == 30 && humidity == 30)) {
              controlarDispositivos(LOW, LOW);
              MotorA(3600, 220);
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("Finalizado");
              delay(1000);
              MotorS(1600, 220);
              buttons();
              temperaturaControl = 0;
              return;
            }
          }
          break;
        case 4:

          lcdFun("Des: ", 35, 20, promedioT, humidity, temperaturaControl);
          if (next >= 1) {
            MotorS(6000, 0);
            if ((promedioT < 35 && humidity < 20) || (promedioT < 35 && humidity > 20)) {
              controlarDispositivos(LOW, HIGH);
            }

            else if ((promedioT > 35 && humidity > 20) || (promedioT > 35 && humidity < 20)) {
              controlarDispositivos(HIGH, LOW);
            }

            else if ((promedioT >= 35 && humidity >= 20) || (promedioT <= 35 && humidity <= 20) || (promedioT == 35 && humidity == 20)) {
              controlarDispositivos(LOW, LOW);
              MotorA(3600, 220);
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("Finalizado");
              delay(1000);
              MotorS(1600, 220);
              buttons();
              temperaturaControl = 0;
              return;
            }
          }
          break;
        default:
          lcdFun("CAJA1", 0, 0, promedioT, humidity, temperaturaControl);
          break;
      }
  }
}
