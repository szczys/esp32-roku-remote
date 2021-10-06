#include <WiFiUdp.h>
#include <WiFi.h>
#include <PubSubClient.h>

/*Define these in the wificredentials.h file */
#include "wificredentials.h"
const char* ssid     = WIFISSID;
const char* password = WIFIPWD;
/********this file is excluded by.gitignore***/
const char* topic = "roku/laundryroom/keypress";
const char* mqtt_server = "192.168.1.135";

WiFiClient espClient;
PubSubClient client(espClient);

/*Button Mapping*/
#define HOME  32
#define BACK  33
#define UP    25
#define DOWN  26
#define LEFT  27
#define RIGHT 14
#define SELECT  12

void setup() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
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

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  // Button setup
  pinMode(HOME, INPUT_PULLDOWN);
  pinMode(BACK, INPUT_PULLDOWN);
  pinMode(UP, INPUT_PULLDOWN);
  pinMode(DOWN, INPUT_PULLDOWN);
  pinMode(LEFT, INPUT_PULLDOWN);
  pinMode(RIGHT, INPUT_PULLDOWN);
  pinMode(SELECT, INPUT_PULLDOWN);
  

}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("lighting/porchlight/status");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  //if (digitalRead(23)) client.publish(topic, "Info");
  if (digitalRead(HOME)) client.publish(topic, "Home");
  if (digitalRead(BACK)) client.publish(topic, "Back");
  if (digitalRead(UP)) client.publish(topic, "Up");
  if (digitalRead(DOWN)) client.publish(topic, "Down");
  if (digitalRead(LEFT)) client.publish(topic, "Left");
  if (digitalRead(RIGHT)) client.publish(topic, "Right");
  if (digitalRead(SELECT)) client.publish(topic, "Select");
  delay(300);
}
