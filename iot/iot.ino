#define ADC_VREF_mV 5000.0  // in millivolt
#define ADC_RESOLUTION 4096.0
#define PIN_LM35 36  // ESP32 pin GPIO36 (ADC0) connected to LM35

const int foco = 13;

int adcVal;
float milliVolt;
float tempC;
float tempF;

void setup() {
  Serial.begin(9600);
  pinMode(foco, INPUT);
}
void LM35(void);
void LM35() {
  // read the ADC value from the temperature sensor
  adcVal = analogRead(PIN_LM35);
  // convert the ADC value to voltage in millivolt
  milliVolt = adcVal * (ADC_VREF_mV / ADC_RESOLUTION);
  // convert the voltage to the temperature in °C
  tempC = milliVolt / 10;
  // convert the °C to °F
  tempF = tempC * 9 / 5 + 32;

  // print the temperature in the Serial Monitor:
  Serial.print("Temperature: ");
  Serial.print(tempC);  // print the temperature in °C
  Serial.print("°C");
  Serial.print("  ~  ");  // separator between °C and °F
  Serial.print(tempF);    // print the temperature in °F
  Serial.println("°F");

  delay(500);
}
void loop() {
  LM35();
  if (tempC <= 30) {
    digitalWrite(foco, LOW);
    Serial.print("esto acs");
  } else {
    digitalWrite(foco, HIGH);
    Serial.print("esto acsdsdadsdas");
  }
}