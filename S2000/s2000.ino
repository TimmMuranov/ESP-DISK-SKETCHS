#include "ArduinoJson.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SPI.h>
#include <SD.h>

#include "fsReader.h"
#include "amogus.h"

const char *ssid = "TimsServer"; // Имя вашей Wi-Fi сети
const char *password = "12345678"; // Пароль от вашей Wi-Fi сети

ESP8266WebServer server(80);

String myDir = "/";
String command = "";

//================================================

String takePost(){
  String data = server.arg("plain");
  if (data == "") {
    Serial.println("No data received.");
    return "no data";
  }

  // Parse the JSON data
  StaticJsonDocument<100> doc;
  DeserializationError error = deserializeJson(doc, data);
  if (error) {
    Serial.println("Failed to parse JSON data");
    return "failed to parse JSON";
  }
  String dataFile = doc["data"];
  Serial.println(dataFile);
  command = dataFile;

  if(dataFile == "dir"){
    return dataFile + "\n" + takeDir() + "\n";
  }
  else if(dataFile == "help"){
    return dataFile + "\n" + FsReader("help.txt") + "\n";  
    }
  else if(dataFile == "amogus"){
    return dataFile + "\n" + "ТУ-ТУ-ТУ-ТУ-ТУ-ТУ-ТУ---------ТУ-ТУ-ТУ" + "\n";  
    }
  else if (dataFile == "pwd"){
    return dataFile + "\n" + myDir + "\n";
  }
  else if (dataFile.substring(0, 2) == "cd"){
    return "команда cd пока не работает. Пока...\n";
  }

//здесь можно добавить команды по следующей формуле:
/*else if(dataFile == "command"){
    return dataFile + "\n" + String function() или "текст" + "\n";  
    }*/
//не забудь включить новую команду в help.txt (!)
    
  else{
    return dataFile + "\n" + "command not found..." + "\n";
  }
}

//========================================================
String takeDir(){
  Serial.println("you pressed dir");
  String dirList = "";
    String fileList = "";
    File file = SD.open(myDir);
    if (file) {
        while (true) {
            File whoIs = file.openNextFile();
            if (!whoIs) break;
            if (whoIs.isDirectory()) {
                dirList += whoIs.name() + ' ';
            } else if (!whoIs.isDirectory()) {
                fileList += whoIs.name() + ' ';
            }
            whoIs.close();
        }
    }
    file.close();
    return "directory:\n" + dirList + "\n" + "files:\n" + fileList + "\n";
}
//======================================================
void winOpen(){
  server.send(200, "text/html", FsReader("win.html"));
  Serial.println ("connected to server!");
}
//======================================================
void handleData() {
  server.send(200, "text/plain", takePost());
  Serial.println("handleData are worked");
}
//=====================================================

void setup() {
  Serial.begin(115200);
  
    while(!Serial){}//ждем инициализации serial

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
}

void loop() {
    server.handleClient();
    
    if(command == "amogus"){
      amogus();
      command = "";
    }
}
