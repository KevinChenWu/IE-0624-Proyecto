#include <Adafruit_LTR390.h>

Adafruit_LTR390 ltr390 = Adafruit_LTR390();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  if (!ltr390.begin()) {
    Serial.println("Couldn't find LTR sensor!");
  }
  ltr390.setMode(LTR390_MODE_UVS);
  ltr390.setGain(LTR390_GAIN_1);
  ltr390.setResolution(LTR390_RESOLUTION_16BIT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (ltr390.newDataAvailable()) {
      Serial.println("UV data: " + String(ltr390.readUVS()));
  }
  
  delay(1000);
}
