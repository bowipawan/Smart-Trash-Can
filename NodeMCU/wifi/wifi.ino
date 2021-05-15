#include <AuthClient.h>
#include <debug.h>
#include <MicroGear.h>
#include <MQTTClient.h>
#include <PubSubClient.h>
#include <SHA1.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define LED1 16

const char* ssid = "poom";
const char* password = "0896827979";
const char* mqtt_server = "broker.netpie.io";
const int mqtt_port = 1883;
const char* mqtt_Client = "a2357bed-a02a-4c04-985d-065365fd1562";
const char* mqtt_username = "EbL1GkYGoxDqWXJw2Gyj7d3bUCdgsUfS";
const char* mqtt_password = "m-sF0We~JjVulKvxMGITv-DrjoMa$#Lb";

WiFiClient espClient;
PubSubClient client(espClient);

char msg[100];
String data;
int state = 0;
char incomingChar;

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection…");
    if (client.connect(mqtt_Client, mqtt_username, mqtt_password)) {
      Serial.println("connected");
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqtt_server, mqtt_port);

}

void loop() {
  //   int c=0;
  //   data="";
  if (state == 0   ) {
    if (Serial.available() > 0) {
      state = 1;
      incomingChar = Serial.read();
      data += incomingChar;
    }

  } else if (state == 1 ) {

    if (Serial.available() > 0) {
      incomingChar = Serial.read();
      data += incomingChar;
    } else {
      state = 2;
    }

  } else if (state == 2) {

    if (client.connected()) {
      Serial.println(data);
      data = "";
      state = 0;
      data.toCharArray(msg, (data.length() + 1));
      client.publish("@msg/temp", msg);
    }
    
  }


  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  delay(10);
}
