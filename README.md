# Proyecto
Universidad de Costa Rica

Escuela de Ingeniería Eléctrica

IE-0624 Laboratorio de Microcontroladores

III-2022

Estudiante 1: Kevin Chen Wu

Carné 1: B92215

Estudiante 2: Jhon Antonio Gaitan Aguilar

Carné 2: B73059

Este es el repositorio del Proyecto para el curso

---
## Software, drivers y bibliotecas
Para hacer uso del código de este repositorio, se necesita instalar lo siguiente para cada microcontrolador. En caso de usar versiones diferentes de software, drivers y/o bibliotecas que las indicadas a continuación, no se asegura el correcto y esperado funcionamiento del código.

### Arduino Nano 33 BLE Sense Lite:
<ul>
  <li> <b>GUI</b>: Arduino IDE versión 2.0.3, se descarga desde la <a href="https://www.arduino.cc/en/software"> página oficial de Arduino</a>.</li>
  <li> <b>Boards Manager</b>: "Arduino Mbed OS Nano Boards" versión 3.5.4 de Arduino , se instala dentro del Arduino IDE.</li>
  <li> <b>Sensor DHT11</b>: "DHT sensor library" versión 1.4.4 de Adafruit y "Adafruit Unified Sensor" versión 1.1.7 de Adafruit, se instalan dentro del Arduino IDE.</li>
  <li> <b>Sensor APDS9960</b>: "Arduino_APDS9960" versión 1.0.4 de Arduino, se instala dentro del Arduino IDE.</li>
  <li> <b>Sensor LTR390</b>: "Adafruit LTR390 Library" versión 1.0.1 de Adafruit y "Adafruit BusIO" versión 1.14.1 de Adafruit, se instalan dentro del Arduino IDE.</li>
</ul>

### LILYGO TTGO LoRa32
<ul>
  <li> <b>GUI</b>: Arduino IDE versión 2.0.3, se descarga desde la <a href="https://www.arduino.cc/en/software"> página oficial de Arduino</a>.</li>
  <li>
    <b>Boards Manager</b>: se debe agregar el siguiente URL al "Additional boards manager URLs". Los pasos son los siguientes:
    <ul>
      <li> Abrir Arduino IDE. </li>
      <li> En la barra superior de herramientas, irse a File -> Preferences. </li>
      <li> Buscar la casilla "Additional boards manager URLs" y darle en el botón respectivo. </li>
      <li> Agregar el URL "https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json". </li>
      <li> Instalar "esp32" versión 2.0.7 de Espressif Systems. </li>
      <li> En caso de dudas, visitar la <a href="https://github.com/Xinyuan-LilyGO/LilyGo-LoRa-Series"> página oficial de LilyGo.</a> </li>
    </ul>
  </li>
  <li> <b>Display SSD1306</b>: "Adafruit SSD1306" versión 2.5.7, "Adafruit BusIO" versión 1.14.1 de Adafruit y "Adafruit GFX Library" versión 1.11.5 de Adafruit, se instala dentro del Arduino IDE.</li>
  <li> <b>Módulo WiFi</b>: no hay necesidad de instalar alguna biblioteca, viene nativo del Board Manager "esp32" .</li>
  <li> <b>Comunicación MQTT</b>: "PubSubClient" versión 2.8.0 de Nick O'Leary y "ArduinoJson" versión 6.20.1 de Benoit Blanchon, se instala dentro del Arduino IDE.</li>
</ul>
