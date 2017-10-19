//---------------------------------------------------------------------------------------
// Set NODEMCU in STA mode and assign unique hostname based on MAC address
// Connect to an MQTT server and publish to a unique topic using => prefix/hostname/suffix
//---------------------------------------------------------------------------------------

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//#define DEEPDEBUG
#define DEBUG

//** Node Identification **
const char* codeversion = "1.2";
const char* hostnamePrefix = "NodeMCU";

//** Wifi **
const char* ssid = "WiFi-2.4-CAF3";  
const char* pass = "15A235D1E4"; 

//** MQTT **
const char* mqtt_server = "test.mosquitto.org";  
const char* topicPrefix = "PRMQTT/";
const char* topicSuffix = "/SalonVergeze";  
const int sendingPeriodinSec = 10;

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char msg[50];
int value = 0;

// Declare a String object here not a constant character arrays 
String newHostname =hostnamePrefix;
// Char array require for MQTT - will receive the prefix/hostname/suffix
char mqttTopic[255];


//**********************************************************************************************
//************************************ SET UP **************************************************
//**********************************************************************************************
void setup()
{
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}
//**********************************************************************************************


void setup_wifi() {
  //----------------------------------------------------------------------------------
  #ifdef DEEPDEBUG
      Serial.println();
      Serial.println("--------------------------------");
      Serial.printf("Code Version  %s\n", codeversion);
      Serial.println("--------------------------------");
      Serial.println();
      Serial.println("setDebugOutput => TRUE");
      Serial.println();
      Serial.setDebugOutput(true);
      Serial.println(" ******  stations (STA) Mode *****");
      Serial.printf("Connecting to %s\n", ssid);
  #endif
  //----------------------------------------------------------------------------------

  // Switching the module to Station mode (STA) is done with begin function
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
    }

  // Create unique hostname inc MAC address
  newHostname += "-";
  newHostname += WiFi.macAddress().c_str();
  WiFi.hostname(newHostname);
   
  //----------------------------------------------------------------------------------
  #ifdef DEBUG
      Serial.println();
      Serial.println("-----------------------------------------------");
      Serial.printf("Hostname: %s\n", WiFi.hostname().c_str());
      Serial.printf("RSSI: %d dBm\n", WiFi.RSSI());
      Serial.print("DHCP: ");
      Serial.println(WiFi.localIP());
      Serial.println("-----------------------------------------------");
      Serial.println();
  #endif
  //----------------------------------------------------------------------------------
}




void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("CALLBACK - New Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    // Attempt to connect
        Serial.println("-----------------------------------------------");
        Serial.print("Attempting to connect ");
        Serial.print(WiFi.hostname().c_str());
        Serial.print(" to ");
        Serial.println(mqtt_server);  
        if (client.connect("ESP8266Client")) {
          Serial.print(WiFi.hostname().c_str());
          Serial.println(" connected !");
          Serial.println("-----------------------------------------------");
          
      // Create unique topic using prefix/hostname/suffix
      strcat(mqttTopic, topicPrefix);
      strcat(mqttTopic, WiFi.hostname().c_str());
      strcat(mqttTopic, topicSuffix);


      
      // Once connected, publish an announcement...
      client.subscribe(mqttTopic);
      client.publish(mqttTopic, "INITIAL PUBLISH");
      
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}



//*********************************************************************************************
//************************************ LOOP ***************************************************
//*********************************************************************************************
void loop() {
   if (!client.connected()) {
      reconnect();
    }
   client.loop();

    long now = millis();
    // Send one message per min
    if (now - lastMsg > (1000*sendingPeriodinSec)) {
      lastMsg = now;
      ++value;
      snprintf (msg, 75, "hello world #%ld", value);
      Serial.print("Publish message: ");
      Serial.println(msg);
      client.publish(mqttTopic, msg);
    }
}
//**********************************************************************************************
