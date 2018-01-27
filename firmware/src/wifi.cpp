#ifndef APP
#define APP
#include "app.h"
#endif

AsyncWebServer server(80);
DNSServer dns;

void configModeCallback (AsyncWiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  // print the ssid that we should connect to to configure the ESP8266
  Serial.print("Created config portal AP ");
  Serial.println(myWiFiManager->getConfigPortalSSID());
}


void setupWifi() {
  //ESP.getFreeHeap()
  // respond to GET requests on URL /heap
  server.on("/log", HTTP_GET, [](AsyncWebServerRequest *request){
      //Download index.htm
      request->_tempFile = SPIFFS.open("datalog.txt", "r");
      request->send(request->_tempFile, request->_tempFile.name(), String(), true);
      //AsyncResponseStream *response = request->beginResponseStream("text/html");
      //response->print(request->_tempFile);
      //request->send(response);
      //AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "log", String(), true);
 
      // AsyncResponseStream *response = request->beginResponseStream("text/plain", 100);
      // for(uint16_t i = 0; i < 5000; i++){
      //   response->printf("%10u",i);
      // }
      // request->send(response);

      // //Kill a kilobyte
      // //    malloc(1024);
      // Serial.println(ESP.getFreeHeap());
      // for(int16_t i = 0; i<1000; i++)
      //   char* a = new char;
      // Serial.println(ESP.getFreeHeap());
    });

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  //Download index.htm
  // respond to GET requests on URL /heap
  AsyncWiFiManager wifiManager(&server,&dns);
  //reset settings - for testing
  //wifiManager.resetSettings();

  //set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
  wifiManager.setAPCallback(configModeCallback);
  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  if(!wifiManager.autoConnect()) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(1000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");

}