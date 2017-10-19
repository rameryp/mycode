//----------------------------------------------------------------------------------
// Set NODEMCU in STA mode and assign unique hostname based on MAC address
//----------------------------------------------------------------------------------

#include <ESP8266WiFi.h>

const char* prefixHostname = "NodeMCU";
const char* ssid = "WiFi-2.4-CAF3";  
const char* pass = "15A235D1E4";      

//#define DEEPDEBUG
#define DEBUG


void setup()
{
    Serial.begin(115200);

  //----------------------------------------------------------------------------------
  #ifdef DEEPDEBUG
  Serial.println();
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
  String hostname = prefixHostname;
  hostname += "-";
  hostname += WiFi.macAddress().c_str();
  WiFi.hostname(hostname);
 
  
  //----------------------------------------------------------------------------------
   #ifdef DEBUG
  Serial.println();
  Serial.println();
  Serial.printf("Hostname: %s\n", WiFi.hostname().c_str());
  Serial.printf("RSSI: %d dBm\n", WiFi.RSSI());
  Serial.print("DHCP: ");
  Serial.println(WiFi.localIP());
  #endif
  //----------------------------------------------------------------------------------

  
}

void loop() {}
