#include <Arduino_APDS9960.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  if (!APDS.begin()) {
    Serial.println("Error initializing APDS-9960 sensor.");
  }  
}

void loop() {
  // put your main code here, to run repeatedly:
  while (!APDS.colorAvailable()) {
    delay(5);
  }
  
  int r, g, b, a;
  APDS.readColor(r, g, b, a);
  Serial.println("r = " + String(r) + " g = " + String(g) + " b = " + String(b));
  Serial.println("Intensidad de luz ambiental: " + String(a));
  delay(1000);
}
