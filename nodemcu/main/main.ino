/*  NETPIE ESP8266 basic sample                            */
/*  More information visit : https://netpie.io             */

#include <ESP8266WiFi.h>
#include <MicroGear.h>

const char* ssid     = <WIFI_SSID>;
const char* password = <WIFI_KEY>;

#define APPID       "8db7eb25-5791-42fa-bb2c-0bdf722afe8e"
#define KEY         "nkkCp51Ls8f5e8csMjn6DdU4D6v1kkZB"
#define SECRET      "JgMMjKv1!2P59X#o8S$cJJI(-v!3(a9G"
#define ALIAS       "esp8266"
#define THRESHOLD   100
#define WEBUSER     "mail@test.com"

WiFiClient client;
MicroGear nodemcu(client);

/* If a new message arrives, do this */
void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) {
    Serial.print("Incoming message --> ");
    msg[msglen] = '\0';
    Serial.println((char *)msg);
}

void onFoundgear(char *attribute, uint8_t* msg, unsigned int msglen) {
    Serial.print("Found new member --> ");
    for (int i=0; i<msglen; i++)
        Serial.print((char)msg[i]);
    Serial.println();  
}

void onLostgear(char *attribute, uint8_t* msg, unsigned int msglen) {
    Serial.print("Lost member --> ");
    for (int i=0; i<msglen; i++)
        Serial.print((char)msg[i]);
    Serial.println();
}

/* When a microgear is connected, do this */
void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) {
    Serial.println("Connected to NETPIE...");
    /* Set the alias of this microgear ALIAS */
    nodemcu.setAlias(ALIAS);
}


void setup() {
    /* Add Event listeners */
    /* Call onMsghandler() when new message arraives */
    nodemcu.on(MESSAGE,onMsghandler);

    /* Call onFoundgear() when new gear appear */
    nodemcu.on(PRESENT,onFoundgear);

    /* Call onLostgear() when some gear goes offline */
    nodemcu.on(ABSENT,onLostgear);

    /* Call onConnected() when NETPIE connection is established */
    nodemcu.on(CONNECTED,onConnected);

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

    /* Initial with KEY, SECRET and also set the ALIAS here */
    nodemcu.init(KEY,SECRET,ALIAS);

    /* connect to NETPIE to a specific APPID */
    nodemcu.connect(APPID);
}

void loop() {
    int infrared = analogRead(A0);
    Serial.println("Infrared val: ");
    Serial.println(infrared);
    /* To check if the microgear is still connected */
    if (nodemcu.connected()) {
        Serial.println("connected");

        /* Call this method regularly otherwise the connection may be lost */
        nodemcu.loop();

        if (infrared < THRESHOLD) {
            Serial.println("Chat to web");
            nodemcu.chat("web","json of img and email");
        } 
    }
    else {
        Serial.println("connection lost, reconnect...");
        nodemcu.connect(APPID);
    }
    delay(100);
}
