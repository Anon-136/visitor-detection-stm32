/*  NETPIE ESP8266 basic sample                            */
/*  More information visit : https://netpie.io             */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid     = "FAH";
const char* password = "025366467";

const char* mqtt_server = "mqtt.netpie.io";
const int mqtt_port = 1883;
const char* mqtt_Client = "8db7eb25-5791-42fa-bb2c-0bdf722afe8e";
const char* mqtt_username = "nkkCp51Ls8f5e8csMjn6DdU4D6v1kkZB";
const char* mqtt_password = "JgMMjKv1!2P59X#o8S$cJJI(-v!3(a9G";

WiFiClient espClient;
PubSubClient client(espClient); 

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection…");
    if (client.connect(mqtt_Client, mqtt_username, mqtt_password)) {
      Serial.println("connected");
      client.subscribe("@msg/alarm");
    }
    else {
      Serial.print("failed");
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

  if(String(topic) == "@msg/alarm") {
    if (message == "on"){
      //Send "alarm on" to stm32
      Serial.println("ALARM ON");
    }
  }
}

void setup() {

    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);

    Serial.begin(115200);
    Serial.println("Starting...");

    /* Initial WIFI, this is just a basic method to configure WIFI on ESP8266.                       */
    if (WiFi.begin(ssid, password)) {
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
    }

    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void loop() {
    if (client.connected()) {
        client.loop();
        /*
        //if received image from stm32
        String img;
        String data = "{\"data\": {\"img\":" + img + "}}";
        Serial.println(data);
        data.toCharArray(msg, (data.length() + 1));
        client.publish("@msg/img", msg);
        */
    }
    else {
        reconnect();
    }
    delay(100);
}
