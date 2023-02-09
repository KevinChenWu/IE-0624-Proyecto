#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht11(DHTPIN, DHTTYPE);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht11.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);

  float humidity = dht11.readHumidity();
  float temperature = dht11.readTemperature();
  Serial.println("Humedad: " + String(humidity));
  Serial.println("Temperatura: " + String(temperature));
}
