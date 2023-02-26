#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Datos de Thingsboard
const char* broker = "iot.eie.ucr.ac.cr";
const int port = 1883;
const char* token = "WZVMCLd2U6JaGsfOxlBv";
const char* topic = "v1/devices/me/telemetry";

// Informacion del WiFi para conexión de Internet
const char* ssid = "AndroidAPdaf7";
const char* password = "clox8053";

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Arduino trae algunos ejemplos, también esta página: https://pubsubclient.knolleary.net/api
WiFiClient wifiClient;
PubSubClient client(wifiClient);

void setup() {
  // put your setup code here, to run once:
  // Serial monitor del Arduino IDE
  Serial.begin(115200);
  // Serial port del LILYGO TTGO LoRa32
  Serial1.begin(115200, SERIAL_8N1, 23);
 
  // Inicialización y conexión del WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectandose a la red WiFi");
  }
  Serial.println("Conectado a WiFi");

  //Inicialización de la pantalla
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("No se pudo iniciar la pantalla");
    while (1);
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Miniestacion meteorologica");
  display.display();
  delay(2000);

  // Inicialización del MQTT
  client.setServer(broker, port);
  while (!client.connected()) {
    Serial.println("Conectandose al Broker MQTT de la Escuela de Ingenieria Electrica, UCR");
    if (client.connect("lora32-client", token, "")) {
      Serial.println("Conectado al Broker MQTT de la Escuela de Ingenieria Electrica, UCR");
      client.subscribe(topic);
    } else {
      Serial.print("Conexion al Broker fallida");
      Serial.println(client.state());
      delay(1000);
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  // Verificación de la conexión con la plataforma ThingsBoard
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Lectura de datos del Serial port
  if (Serial1.available() > 0) {
    String datos = Serial1.readStringUntil('\n');  
    Serial.println("Datos recibidos: " + String(datos));

    // Split de datos delimitados por comas    
    // Código basado en https://stackoverflow.com/questions/12874128/error-invalid-conversion-from-const-char-to-char
    char* datosMutable = new char[datos.length() + 1];
    strcpy(datosMutable, datos.c_str());
    char *ptr = strtok(datosMutable, ",");
    float humedad = atof(ptr);
    ptr = strtok(NULL, ",");
    float temperatura = atof(ptr);
    ptr = strtok(NULL, ",");
    float luz = atof(ptr);
    ptr = strtok(NULL, ",");
    float rayosUV = atof(ptr);
    // Se elimina la copia mutable de la cadena para liberar memoria
    delete[] datosMutable;

    // Payload falsos con fines de debug. Para utilizarlos se debe comentar la sección anterior de Split
    // float humedad = random(0, 100);
    // float temperatura = random(15, 30);
    // float luz = random(0, 1024);
    // float rayosUV = random(0, 11);

    // Se crea el JSON con los datos y se envia a Thingsboard
    const size_t capacidad = JSON_OBJECT_SIZE(4);
    DynamicJsonDocument doc(capacidad);
    doc["humedad"] = humedad;
    doc["temperatura"] = temperatura;
    doc["luz"] = luz;
    doc["rayosUV"] = rayosUV;
    String json;
    serializeJson(doc, json);

    // Se envia a ThingsBoard
    client.publish(topic, json.c_str());
    Serial.print("Datos enviados a Thingsboard: ");
    Serial.println(json);

    // Se muestra los datos en la pantalla OLED del LILYGO TTGO LoRa32
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("H: ");
    display.print(humedad);
    display.println("%");
    display.print("T: ");
    display.print(temperatura);
    display.println(" C");
    display.print("L: "); 
    display.println(luz);
    display.print("UV: ");
    display.println(rayosUV);
    display.display();
  }
}

// Función de reconectar en caso de que se haya perdido conexión el servidor
void reconnect() {
  while (!client.connected()) {
    Serial.println("Intentando conectarse al Broker");
    // Se intenta conectarse
    if (client.connect("lora32-client", token, "")) {
      Serial.println("Conectado al Broker MQTT de la Escuela de Ingenieria Electrica, UCR");
      client.subscribe(topic);
    } else {
      Serial.print("Conexion al Broker fallida");
      Serial.println(client.state());
      delay(1000);
    }
  }
}