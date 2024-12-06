#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <RBDdimmer.h>  //

//#define USE_SERIAL  SerialUSB //Serial for boards whith USB serial port
#define zerocross 2      // for boards with CHANGEBLE input pins
#define ADC_VREF_mV 5.0  // in millivolt
#define ADC_RESOLUTION 1024.0
#define PIN_LM35 A1  // LM35


const int ventilador = 5;
const int foco = 6;

dimmerLamp dimmer(foco);  //initialase port for dimmer for MEGA, Leonardo, UNO, Arduino M0, Arduino Zero

const int botonMasPin = 9;     // pin desde el boton mas
const int botonMenosPin = 10;  // pin desde el boton menos
int temperaturaControl = 0;

int adcVal;
float milliVolt;
float tempC;
float tempF;
int outVal;
int outValVent;
// variables para trabajar con los botones
boolean lastBotonMas = LOW;
boolean currentBotonMas = LOW;
boolean lastBotonMenos = LOW;
boolean currentBotonMenos = LOW;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void InfoSensorLM35(void);
void desplegarInformacion(void);
void butons(void);
void setup() {
  Serial.begin(9600);
  lcd.init();  // inicializa el LCD
  lcd.backlight();
  lcd.clear();

  dimmer.begin(NORMAL_MODE, ON);       //dimmer initialisation: name.begin(MODE, STATE)

  pinMode(ventilador, OUTPUT);

  pinMode(botonMasPin, INPUT);
  pinMode(botonMenosPin, INPUT);

  lcd.setCursor(0, 0);
  lcd.print("Inicializando...");
  for (int i = 0; i <= 15; i++) {
    lcd.setCursor(i, 1);
    lcd.write((byte)4);
    delay(250);
  }

dimmer.setPower(10);
    delay(250);
  lcd.clear();
  delay(150);
  temperaturaControl = 0;
}
void LM35() {
  tempC = analogRead(PIN_LM35);
  // Calculamos la temperatura con la fórmula
  tempC = (ADC_VREF_mV * tempC * 100) / ADC_RESOLUTION;
  tempC = int(tempC);
  delay(150);
  // Envia el dato al puerto serial
  Serial.println(tempC);
}

void loop() {
  buttons();
  desplegarInformacion();
}
void buttons() {
  currentBotonMas = digitalRead(botonMasPin);
  currentBotonMenos = digitalRead(botonMenosPin);
  // subir temperatura de control
  if (currentBotonMas == HIGH && lastBotonMas == LOW) {
    delay(15);  // debouncing time
    if (digitalRead(botonMasPin)) {
      temperaturaControl++;
      // Serial.println(temperaturaControl);
      lastBotonMas = HIGH;
    }
    if (temperaturaControl > 6) {
      temperaturaControl--;
    }
  } else {
    lastBotonMas = digitalRead(botonMasPin);
  }

  // bajar temperatura de control
  if (currentBotonMenos == HIGH && lastBotonMenos == LOW) {
    delay(15);  // debouncing time
    if (digitalRead(botonMenosPin)) {
      temperaturaControl--;
      //Serial.println(temperaturaControl);
      lastBotonMenos = HIGH;
    }
    if (temperaturaControl < 0) {
      temperaturaControl++;
    }
  } else {
    lastBotonMenos = digitalRead(botonMenosPin);
  }
  if (temperaturaControl == 6) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Inicializando...");
    temperaturaControl = 0;
    delay(150);
  }
}
void desplegarInformacion() {
  LM35();
  switch (temperaturaControl) {
    case 1:
      lcd.setCursor(0, 0);
      lcd.print("Temp. Des.: 30 ");
      Serial.println("1 -> 30C");
      lcd.setCursor(0, 1);
      lcd.print("Temp.: ");
      lcd.print(tempC);
      lcd.setCursor(10, 1);
      lcd.print("C");
      if (tempC < 29) {
        outVal = map(tempC, 15, 40, 100, 0);  // analogRead(analog_pin), min_analog, max_analog, 100%, 0%);
        dimmer.setPower(outVal);                // name.setPower(0%-100%)
      } else if (tempC > 30) {
        //outVal = map(tempC, 0, 40, 100, 0);  // analogRead(analog_pin), min_analog, max_analog, 100%, 0%);
        outValVent = map(tempC, 0, 38, 0, 250);
        analogWrite(ventilador, outValVent);
        dimmer.setPower(10);  // name.setPower(0%-100%)
      }
      break;
    case 2:
      lcd.setCursor(0, 0);
      lcd.print("Temp. Des.: 34 ");
      Serial.println("2 -> 34C");
      lcd.setCursor(0, 1);
      lcd.print("Temp.: ");
      lcd.print(tempC);
      lcd.setCursor(10, 1);
      lcd.print("C");
      if (tempC < 33) {
       outVal = map(tempC, 15, 50, 100, 0);  // analogRead(analog_pin), min_analog, max_analog, 100%, 0%);
        dimmer.setPower(outVal);                // name.setPower(0%-100%)
      } else if (tempC > 34) {
        //outVal = map(tempC, 0, 35, 100, 0);  // analogRead(analog_pin), min_analog, max_analog, 100%, 0%);
        outValVent = map(tempC, 0, 40, 0, 250);
        analogWrite(ventilador, outValVent);
        dimmer.setPower(10);  // name.setPower(0%-100%)
      }

      break;
    case 3:
      lcd.setCursor(0, 0);
      Serial.println("3 -> 38C");
      lcd.print("Temp. Des.: 38 ");
      lcd.setCursor(0, 1);
      lcd.print("Temp.: ");
      lcd.print(tempC);
      lcd.setCursor(10, 1);
      lcd.print("C");
      if (tempC < 37) {
         outVal = map(tempC, 22, 55, 100, 0);  // analogRead(analog_pin), min_analog, max_analog, 100%, 0%);
        dimmer.setPower(outVal);                // name.setPower(0%-100%)
      } else if (tempC > 38) {
        //outVal = map(tempC, 0, 33, 100, 0);  // analogRead(analog_pin), min_analog, max_analog, 100%, 0%);
        outValVent = map(tempC, 0, 50, 0, 250);
        analogWrite(ventilador, outValVent);
        dimmer.setPower(10);  // name.setPower(0%-100%)
      }

      break;
    case 4:
      lcd.setCursor(0, 0);
      Serial.println("4 -> 42C");
      lcd.print("Temp. Des.: 42 ");
      lcd.setCursor(0, 1);
      lcd.print("Temp.: ");
      lcd.print(tempC);
      lcd.setCursor(10, 1);
      lcd.print("C ");
      if (tempC < 41) {
         outVal = map(tempC, 22, 60, 100, 0);  // analogRead(analog_pin), min_analog, max_analog, 100%, 0%);
        dimmer.setPower(outVal);                // name.setPower(0%-100%)
      } else if (tempC > 42) {
        //outVal = map(tempC, 0, 45, 100, 0);  // analogRead(analog_pin), min_analog, max_analog, 100%, 0%);
        outValVent = map(tempC, 0, 50, 0, 250);
        analogWrite(ventilador, outValVent);
        dimmer.setPower(10);  // name.setPower(0%-100%)
      }

      break;
    case 5:
      lcd.setCursor(0, 0);
      Serial.println("5 -> 46C");
      lcd.print("Temp. Des.: 46");
      lcd.setCursor(0, 1);
      lcd.print("Temp.: ");
      lcd.print(tempC);
      lcd.setCursor(10, 1);
      lcd.print("C");
      if (tempC < 45) {
        outVal = map(tempC, 22, 65, 100, 0);  // analogRead(analog_pin), min_analog, max_analog, 100%, 0%);
        dimmer.setPower(outVal);                // name.setPower(0%-100%)
      } else if (tempC > 46) {
        //outVal = map(tempC, 0, 44, 100, 0);  // analogRead(analog_pin), min_analog, max_analog, 100%, 0%);
        outValVent = map(tempC, 0, 48, 0, 250);
        analogWrite(ventilador, outValVent);
        dimmer.setPower(10);  // name.setPower(0%-100%)
      }
      break;
    default:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Temperatura ");
      lcd.setCursor(0, 1);
      lcd.print(int(tempC));
      lcd.setCursor(3, 1);
      lcd.print("C");
      Serial.println(temperaturaControl);
  }
}
