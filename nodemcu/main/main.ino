/*  NETPIE ESP8266 basic sample                            */
/*  More information visit : https://netpie.io             */

#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid     = "Home";
const char* password = "0836122227";

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

const char json[] = "{ \
  \"token\": \"eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ1c2VyIjoic2FuZXlha29ybkBnbWFpbC5jb20iLCJkZXZpY2VOYW1lIjoiZnJvbnQiLCJpYXQiOjE2MjI0NzM1NDR9.k5iA03uj3oDnwl6-uIb9X2huUr4CpxhIl31xZfh0JYE\", \
  \"image\": \"data:image/jpeg;base64,/9j/4AAQSkZJRgABAQAASABIAAD/4QBMRXhpZgAATU0AKgAAAAgAAYdpAAQAAAABAAAAGgAAAAAAA6ABAAMAAAABAAEAAKACAAQAAAABAAAAEKADAAQAAAABAAAAEAAAAAD/7QA4UGhvdG9zaG9wIDMuMAA4QklNBAQAAAAAAAA4QklNBCUAAAAAABDUHYzZjwCyBOmACZjs+EJ+/8AAEQgAEAAQAwEiAAIRAQMRAf/EAB8AAAEFAQEBAQEBAAAAAAAAAAABAgMEBQYHCAkKC//EALUQAAIBAwMCBAMFBQQEAAABfQECAwAEEQUSITFBBhNRYQcicRQygZGhCCNCscEVUtHwJDNicoIJChYXGBkaJSYnKCkqNDU2Nzg5OkNERUZHSElKU1RVVldYWVpjZGVmZ2hpanN0dXZ3eHl6g4SFhoeIiYqSk5SVlpeYmZqio6Slpqeoqaqys7S1tre4ubrCw8TFxsfIycrS09TV1tfY2drh4uPk5ebn6Onq8fLz9PX29/j5+v/EAB8BAAMBAQEBAQEBAQEAAAAAAAABAgMEBQYHCAkKC//EALURAAIBAgQEAwQHBQQEAAECdwABAgMRBAUhMQYSQVEHYXETIjKBCBRCkaGxwQkjM1LwFWJy0QoWJDThJfEXGBkaJicoKSo1Njc4OTpDREVGR0hJSlNUVVZXWFlaY2RlZmdoaWpzdHV2d3h5eoKDhIWGh4iJipKTlJWWl5iZmqKjpKWmp6ipqrKztLW2t7i5usLDxMXGx8jJytLT1NXW19jZ2uLj5OXm5+jp6vLz9PX29/j5+v/bAEMAAgICAgICAwICAwQDAwMEBQQEBAQFBwUFBQUFBwgHBwcHBwcICAgICAgICAoKCgoKCgsLCwsLDQ0NDQ0NDQ0NDf/bAEMBAgICAwMDBgMDBg0JBwkNDQ0NDQ0NDQ0NDQ0NDQ0NDQ0NDQ0NDQ0NDQ0NDQ0NDQ0NDQ0NDQ0NDQ0NDQ0NDQ0NDf/dAAQAAf/aAAwDAQACEQMRAD8A/WXW/H39t61fWxuZI7GzuHtooIpGjEhiOxnkKEMxZgQq52gAcEmt6HxHd+DNc0jTr25ZoNVZI5rSWQyG3MzbY2UsSytkjcucEHpkZrx/xboHxJ+FHi/W9Y8L6K3ifwxr9w2oC2ji8+SyvCd4JRf3gCS/Mu3KkHBwea5v4Y/Dz4lePPHVh4s8a217p2k6ddLfyG/BimvZ0O6NViPzBQ+GYsAMDAz2+LeNxaxKoqEnUcuzslfe+zTR+ePMccsWqCpydVy7PlUb732aa/q5/9k=\" \
}";

void loop() {
    if (client.connected()) {
        client.loop();
        if (Serial.available() > 0) {
          char c = Serial.read();
          unsigned int njson = strlen(json);
          bool success = client.beginPublish("@msg/img", njson, false);
          if (success) {
            for (int i = 0; i < njson; i++) {
              client.write(json[i]);
            }
            success = client.endPublish();
          }
          if (success) {
            Serial.println("Message a sent successfully");
          } else {
            Serial.println("Message a failed successfully");
          }
        }
    } else {
        reconnect();
    }
    delay(100);
}
