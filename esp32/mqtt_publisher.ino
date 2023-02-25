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
  Serial.println("Conectado a WiFi");
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
  //loop que chequea que si no esta conectado este busque reconectarse con la funcion reconnect();
  if (!client.connected()) 
  {
    reconnect();
  }
  //Mensajes
  client.loop();
  
    
  

  //leer datos del puerto serial
  if (Serial.available() > 0) 
  {
    Serial.print("Debug");
    // reads characters from the serial buffer into a String. The function terminates if it times out (see setTimeout()).
    String datos = Serial.readStringUntil('\n');
    Serial.print("Datos recibidos: ");
    Serial.println(datos);
    //https://stackoverflow.com/questions/12874128/error-invalid-conversion-from-const-char-to-char
    char* datosMutable = new char[datos.length() + 1];
    strcpy(datosMutable, datos.c_str());
    //separar los datos en variables individuales para que sea mas facil en thinsgboard leer variables
    char *ptr = strtok(datosMutable, ",");
    float humedad = atof(ptr);
    ptr = strtok(NULL, ",");
    float temperatura = atof(ptr);
    ptr = strtok(NULL, ",");
    float luz = atof(ptr);
    ptr = strtok(NULL, ",");
    float rayosUV = atof(ptr);

    //eliminar la copia mutable de la cadena
    delete[] datosMutable;
    //payload falsos con fines de debug. para utilizarlos se debe comentar la seccion donde 
    //se declaran las variables que se leen del puerto rcd del lilygo
    //crear el JSON con los datos y enviarlo a Thingsboard
    /*float humedad = random(0, 100);
    float temperatura = random(15, 30);
    float luz = random(0, 1024);
    float rayosUV = random(0, 11);*/

    const size_t capacidad = JSON_OBJECT_SIZE(4);
    DynamicJsonDocument doc(capacidad);
    doc["humedad"] = humedad;
    doc["temperatura"] = temperatura;
    doc["luz"] = luz;
    doc["rayosUV"] = rayosUV;
    String json;
    serializeJson(doc, json);
    //Enviar a Thingsboard
    client.publish(topic, json.c_str());
    Serial.print("Datos enviados a Thingsboard: ");
    Serial.println(json);

    //mostrar los datos en la pantalla OLED
    //hacemos un flush de los datos
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
//Funcion de reconectar en caso de que se haya perdido conexion el servidor
void reconnect() 
{
  while (!client.connected()) 
  {
    Serial.println("Intentando conectarse al Broker");
    //Intentar conectarse
    if (client.connect("lora32-client", token, "")) 
    {
      Serial.println("Conectado al Broker MQTT de la Escuela de Ingenieria Electrica, UCR");
      client.subscribe(topic);
    }
    else 
    {
      Serial.print("Conexion al Broker fallida");
      Serial.println(client.state());
      delay(1000);
    }
  }
}
