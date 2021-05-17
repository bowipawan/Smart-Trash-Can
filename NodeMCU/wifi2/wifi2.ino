#include <AuthClient.h>
#include <debug.h>
#include <MicroGear.h>
#include <MQTTClient.h>
#include <PubSubClient.h>
#include <SHA1.h>

#include <ESP8266WiFi.h>

const char* ssid     = "poom_wifi";  //ชื่อ wifi
const char* password = "0896827979";  //รหัสผ่าน wifi

#define APPID   "SmartTrashCan"                    //ใส่ appid จากเว็บ
#define KEY     "M6maS44ThkDHosb"              //ใส่ key จากเว็บ
#define SECRET  "AGuGl54sVzBoIsHRnyGAVss1j"    //ใส่ secret จากเว็บ
#define ALIAS   "nodeMCU"                 //ชื่อ device

WiFiClient client;
MicroGear microgear(client);

unsigned long previousMillis = 0;
const long interval = 1000;
int x1, x2;

char msg[100];
String data;
int state = 0;
char incomingChar;

/* If a new message arrives, do this */
void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) {
  Serial.print("Incoming message --> ");
  msg[msglen] = '\0';
  Serial.println((char *)msg);
}

void onFoundgear(char *attribute, uint8_t* msg, unsigned int msglen) {
  /*
  Serial.print("Found new member --> ");
  for (int i = 0; i < msglen; i++)
    Serial.print((char)msg[i]);
  Serial.println();
*/
}

void onLostgear(char *attribute, uint8_t* msg, unsigned int msglen) {
  /*
  Serial.print("Lost member --> ");
  for (int i = 0; i < msglen; i++)
    Serial.print((char)msg[i]);
  Serial.println();
*/
}

void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) {
  Serial.println("Connected to NETPIE...");
  microgear.setAlias(ALIAS);
}

void setup() {

  microgear.on(MESSAGE, onMsghandler);
  microgear.on(PRESENT, onFoundgear);
  microgear.on(ABSENT, onLostgear);
  microgear.on(CONNECTED, onConnected);

  Serial.begin(115200);
  Serial.println("Starting...");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }


  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  microgear.init(KEY, SECRET, ALIAS);
  microgear.connect(APPID);

  digitalWrite(2, HIGH);  //LED บนบอร์ด(GPIO 2)
  pinMode(2, OUTPUT);

}

void loop() {

  if (microgear.connected()) {
    digitalWrite(2, LOW); //ไฟบนบอร์ดติดถ้าเชื่อมต่อ NETPIE
    microgear.loop();

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

      Serial.println(data);
      microgear.publish("/x", data); 
      data = "";
      state = 0;
      data.toCharArray(msg, (data.length() + 1));
       

    
  }
//    unsigned long currentMillis = millis();
//    if (currentMillis - previousMillis >= interval) { //ทำงานทุกๆ 1 วินาที(interval)
//      previousMillis = currentMillis;
//      
//      x1++;               //บวกเลขที่ละ 1
//      x2 = random(100);   //สุ่มตัวเลข 0-100
//
//      Serial.println("Publish...");
//
//      sprintf(data, "%d,%d", x1, x2);   //แปรง int รวมกันใน char ชื่อ data
//      Serial.println(data);
//      microgear.publish("/x", data);      //ส่งค่าขึ้น NETPIE
//
////      microgear.publish("/x1", String(x1));     //หรือจะส่งทีละค่าก็ได้
////      microgear.publish("/x2", String(x2));
//    }
  }
  else {
    digitalWrite(2, HIGH); //ไฟบนบอร์ดดับถ้าเชื่อมต่อ NETPIE ไม่ได้
    Serial.println("connection lost, reconnect...");
    microgear.connect(APPID);
  }

  delay(5);
}
