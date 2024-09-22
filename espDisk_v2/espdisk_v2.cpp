#include <arduino.h>
#include "ArduinoJson.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SPI.h>
#include <SD.h>
#include <FS.h>

int main (){
//======= настройки сервера ========
  Serial.begin(115200);

    while(!Serial){}//ждем serial

    Serial.print("Initializing SD card...");
    if(!SD.begin(4)) Serial.println("initialization failed!");
    Serial.println("initialization SD card done.");

    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);

    server.on("/", winOpen);
    server.on("/q", HTTP_POST, handleData);
    server.begin();
    Serial.println("Access Point started");

    pinMode(2, OUTPUT);
    digitalWrite(2, HIGH);

//===== основной цикл программы =====

  while(1){
    server.handleClient();
    
  }
//============= функции =============

void handleData() {
  server.send(200, "text/plain", takePost());
  Serial.println("handleData are worked");
}

String takePost(){
  
}

return 0;
}