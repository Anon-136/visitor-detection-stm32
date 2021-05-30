/*  NETPIE ESP8266 basic sample                            */
/*  More information visit : https://netpie.io             */

#include <ESP8266WiFi.h>
#include "PubSubClient.h"

const char* ssid     = "FAH";
const char* password = "025366467";

/*#define APPID       "8db7eb25-5791-42fa-bb2c-0bdf722afe8e"
#define KEY         "nkkCp51Ls8f5e8csMjn6DdU4D6v1kkZB"
#define SECRET      "JgMMjKv1!2P59X#o8S$cJJI(-v!3(a9G"
#define ALIAS       "esp8266"
#define THRESHOLD   100
#define WEBUSER     "mail@test.com"*/

// ^^^^^^^^^^^^^^ NETPIE 2020 configuration ^^^^^^^^^^^^^^^^^^
const char* mqtt_server = "mqtt.netpie.io";
const int mqtt_port = 1883;
const char* mqtt_Client = "8db7eb25-5791-42fa-bb2c-0bdf722afe8e";
const char* mqtt_username = "nkkCp51Ls8f5e8csMjn6DdU4D6v1kkZB";
const char* mqtt_password = "JgMMjKv1!2P59X#o8S$cJJI(-v!3(a9G";
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

//WiFiClient client;

WiFiClient espClient;
PubSubClient client(espClient); 

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection…");
    if (client.connect(mqtt_Client, mqtt_username, mqtt_password)) {
      Serial.println("connected");
      client.subscribe("@msg/img");
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String message;
  for (int i = 0; i < length; i++) {
    message = message + (char)payload[i];
  }
  Serial.println(message);

  /*if(String(topic) == "@msg/led") {
    if (message == "on"){
      //digitalWrite(LED1,0);
      client.publish("@shadow/data/update", "{\"data\" : {\"led\" : \"on\"}}");
      Serial.println("LED ON");
    }
    else if (message == "off") {
      //digitalWrite(LED1,1);
      client.publish("@shadow/data/update", "{\"data\" : {\"led\" : \"off\"}}");
      Serial.println("LED OFF");
    }
  }*/
}

void setup() {

    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);

    Serial.begin(115200);
    Serial.println("Starting...");

    /* Initial WIFI, this is just a basic method to configure WIFI on ESP8266.                       */
    /* You may want to use other method that is more complicated, but provide better user experience */
    if (WiFi.begin(ssid, password)) {
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
    }

    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    reconnect();
}

void loop() {
    /* To check if the microgear is still connected */
    if (client.connected()) {
        client.loop();
    }
    else {
        reconnect();
    }
    delay(100);
}
