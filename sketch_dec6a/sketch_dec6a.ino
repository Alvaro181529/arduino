#include <WiFi.h>
#include <Wire.h>  // library for I2C communication

float temp;                           // variable to store temperature value
const char* ssid = "RamilaFacil";     // Tu SSID
const char* password = "1234567890";  //Tu Clave

const int LM35_PIN = 34;

WiFiServer server(80);

String header;

void setup(void) {
  Serial.begin(9600);  //define the baudrate of 9600
  Serial.print("Connecting to Wifi Network");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Successfully connected to WiFi.");
  Serial.println("IP address of ESP32 is : ");
  Serial.println(WiFi.localIP());
  server.begin();
  Serial.println("Server started");
}
void leerTemperatura() {
  // Leemos el valor analógico del pin del sensor
  int valorAnalogico = analogRead(LM35_PIN);

  // Convertimos el valor analógico a temperatura
  temp = (valorAnalogico * 5.0) / 1024.0;

  // Redondeamos la temperatura a dos decimales
  temp = round(temp * 100) / 100;
}
void loop(void) {
  leerTemperatura();

  // this section displays value of adc and temperature on serial monitor
  Serial.print("tTempertaure: ");
  Serial.println(temp);
  Serial.println();
  // This line checks if web client is available or not
  WiFiClient client = server.available();
  // if client is available 'if' condition becomes true
  if (client) {
    Serial.println("Web Client connected ");       //print on serial monitor
    String request = client.readStringUntil('r');  // wait untill http get request ends
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println("Connection: close");
    client.println();

    // This part send HTML page to web client
    // HTML page contains temperature values
    client.println("<!DOCTYPE html>n");
    client.println("<html>n");
    client.println("<meta name='viewport' content='width=device-width, initial-scale=1.0'>n");
    client.println("<body>n");
    client.println("<h1>WEB SERVER LM35 SENSOR</h1>n");
    client.println("<p style='color:red'>Temperature ='");
    client.println("*C</p>n");
    client.println(temp);
    client.println("<p style='color:red'>Temperature in Farenhite ='");
    client.println(1.8 * temp + 32);
    client.println("*F</p>n");
    client.println("</body></html>");
    client.println();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}