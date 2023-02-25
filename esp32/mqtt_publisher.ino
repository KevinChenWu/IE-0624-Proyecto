#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>



//Como Paho de Python, ponemos los datos de Thingsboard
const char* broker = "iot.eie.ucr.ac.cr";
const int port = 1883;
const char* token = "Bahn0VJnwX25hHNNoawH";
const char* topic = "v1/devices/me/telemetry";
//Aqui ponemos la informacion del WiFi al cual nos vamos a conectar
const char* ssid = "El conuco";
const char* password = "zxcvbn111";
/*Aqui se intentaron varias bibliotecas pero la que funciono bien sin dar errores fue 
Adafruit_GFX.h y Adafruit_SSD1306.h*/
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//Arduino trae algunos ejemplos, tambien esta es la fuente:
//https://pubsubclient.knolleary.net/api
WiFiClient wifiClient;
PubSubClient client(wifiClient);

void setup() 
{
  // Conectarse al WiFi, lo del serial es con fines de debug.
  Serial.begin(115200);
  while (!Serial);
 
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.println("Conectandose a la red WiFi");
  }
  Serial.println("Connected to WiFi");
  //Iniciar la pantalla
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) 
  {
    Serial.println("No se pudo iniciar la pantalla");
    while (1);
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Miniestacion meteorologica");
  display.display();
  delay(2000);

  //Conectarse a MQTT iot.eie.ucr.ac.cr
  //
  client.setServer(broker, port);
  while (!client.connected()) 
  {
    Serial.println("Conectandose al Broker MQTT de la Escuela de Ingenieria Electrica, UCR");
    if (client.connect("lora32-client", token, "")) 
    {
      Serial.println("Conectado al Broker MQTT de la Escuela de Ingenieria Electrica, UCR");
      client.subscribe(topic);
    }
    else {
      Serial.print("Conexion al Broker fallida");
      Serial.println(client.state());
      delay(1000);
    }
  }
}
//funcion principal sin retorno
void loop() 
{
  //loop que chequea si esta conectado y busca reconectarse
  if (!client.connected()) 
  {
    reconnect();
  }
  //Mensajes
  client.loop();

  //para debugeo
  if (Serial.available()) 
  {
    String mensaje = Serial.readStringUntil('\n');
    //Para publicar en MQTT se necesitan crear diccionarios
    const size_t capacidad = JSON_OBJECT_SIZE(1) + 20;
    DynamicJsonDocument doc(capacidad);
    doc["mensaje"] = mensaje;
    // Serialize the object to a JSON string
    String json;
    serializeJson(doc, json);
    // Publish the JSON string to the MQTT broker
    client.publish(topic, json.c_str());
    Serial.print("Mensaje enviado: ");
    Serial.println(mensaje);
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(mensaje);
    display.display();
  }
}
void reconnect() 
{
  while (!client.connected()) {
    Serial.println("Intentando conectarse al Broker");
    //Intentar conectarse
    if (client.connect("lora32-client", token, "")) {
      Serial.println("Conectado al Broker MQTT de la Escuela de Ingenieria Electrica, UCR");
     client.subscribe(topic);
    }
    else {
      Serial.print("Conexion al Broker fallida");
      Serial.println(client.state());
      delay(1000);
    }
  }
}
