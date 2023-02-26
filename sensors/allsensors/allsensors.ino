#include <DHT.h>
#include <Arduino_APDS9960.h>
#include <Adafruit_LTR390.h>

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht11(DHTPIN, DHTTYPE);
Adafruit_LTR390 ltr390 = Adafruit_LTR390();

void setup() {
  // put your setup code here, to run once:
  // Serial monitor del Arduino IDE
  Serial.begin(115200);
  // Serial port del Arduino Nano 33 BLE Sense (pines TX y RX)
  Serial1.begin(115200);

  // Inicialización de los sensores  
  dht11.begin();
  APDS.begin();
  ltr390.begin();
  ltr390.setMode(LTR390_MODE_UVS);
  ltr390.setGain(LTR390_GAIN_1);
  ltr390.setResolution(LTR390_RESOLUTION_16BIT);
}

void loop() {
  // put your main code here, to run repeatedly:
  float humidity, temperature;
  int r, g, b, a;
  uint32_t uv;
  delay(1000);

  while (!APDS.colorAvailable()) {
    delay(5);
  }
  APDS.readColor(r, g, b, a);
  if (ltr390.newDataAvailable()) {
    uv = ltr390.readUVS();
  }
  humidity = dht11.readHumidity();
  temperature = dht11.readTemperature();

  // Mandar los datos al Serial Monitor
  Serial.println(String(humidity) + "," + String(temperature) + "," + String(a) + "," + String(uv));
  // Mandar los datos al Serial port
  Serial1.println(String(humidity) + "," + String(temperature) + "," + String(a) + "," + String(uv));
}