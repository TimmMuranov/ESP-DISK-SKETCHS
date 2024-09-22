#include "LittleFS.h"
#include "ArduinoJson.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SPI.h>
#include <SD.h>

#include "amogus.h"
#include "fsReader.h"
#include "creatBut.h"

const char *ssid = "TimsServer"; // Имя вашей Wi-Fi сети
const char *password = "12345678"; // Пароль от вашей Wi-Fi сети

ESP8266WebServer server(80);

String myDir = "/";
String command = "";

String buttons = "";
String page =
  "<!DOCTYPE html>"
  "<html lang='ru'>"
  "<head>"
  "<meta charset='UTF-8'>"
  "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
  "<title>Servak</title>"
  "<style>" +
  FsReader("style.css") +
  "</style>" +
  "<div class='form-container'>"
  "<h2>Welcome to the ESP8266 server!</h2>"
  "<textarea rows='10' cols='50' id='inputArea'></textarea>"
  "<button class='btn-submit' id='submitButton'>Сохранить</button>"
  "<button class='btn-creatF' id='creatFile'>Создать файл</button>"
  "<button class='btn-creatD' id='creatDir'>Создать папку</button>"
  "</div><hr>" +
  creatBut(myDir) +
  "<script>" +
  FsReader("script.js") +
  "</script></body></html>";

/////////////////// настройки сервера ///////////////
void setup (){
  Serial.begin(115200);

  while(!Serial){}//ждем serial

  Serial.print("Initializing SD card...");
  if(!SD.begin(4)) Serial.println("initialization failed!");
  Serial.println("initialization SD card done.");

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  server.on("/", winOpen);
  server.on("/q", HTTP_POST, handleData);
  server.on("/bf", HTTP_POST, handleFile);
  server.on("/bd", HTTP_POST, handleDir);
  server.begin();
  Serial.println("Access Point started");

  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
}

//===== основной цикл программы =====

void loop(){
    server.handleClient();
    if (command == "amogus"){
      amogus();
      command = "";
    }
  }
/////////////////// функции ////////////////////

//============ функция начала ===========
//срабатывает при запуске сервера
void winOpen(){
  server.send(200, "text/html", page);
  Serial.println ("connected to server!");
}

//=========== функции запроса =============
//срабатывает при запросе
//и сразу отправляет ответ

void handleData() {
  server.send(200, "text/plain", takePost());
  Serial.println("handleData are worked");
}

//_____ функция обработки запроса _____

String takePost(){
  String data = server.arg("plain");
  if (data == "") {
    Serial.println("No data received.");
  }

  StaticJsonDocument<100> doc;
  DeserializationError error = deserializeJson(doc, data);
  if (error) {
    Serial.println("Failed to parse JSON data");
    return "failed to parse JSON";
  }
  String dataFile = doc["data"];
  Serial.println(dataFile);
  return "запрос получен: " + dataFile;
}
//========== нажатие на создание файла ============

void handleFile(){
  server.send(200, "text/plain", takePostFile());
  Serial.println("вы нажали на создание файла");
}
//___________ обработка нажатия ________________

String takePostFile(){
  String data = server.arg("plain");
  if (data == "") {
    Serial.println("No data received.");
  }

  StaticJsonDocument<100> doc;
  DeserializationError error = deserializeJson(doc, data);
  if (error) {
    Serial.println("Failed to parse JSON data");
    return "failed to parse JSON";
  }
  String dataFile = doc["data"];
  Serial.println(dataFile);
  return "вы нажали на кнопку: " + dataFile;
}

//========= и на создание директории ================

void handleDir(){
  server.send(200, "text/plain", takePostDir());
  Serial.println("вы нажали на создание директории");
}
//___________ обработка нажатия ________________

String takePostDir(){
  String data = server.arg("plain");
  if (data == "") {
    Serial.println("No data received.");
  }

  StaticJsonDocument<100> doc;
  DeserializationError error = deserializeJson(doc, data);
  if (error) {
    Serial.println("Failed to parse JSON data");
    return "failed to parse JSON";
  }
  String dataFile = doc["data"];
  Serial.println(dataFile);
  return "вы нажали на кнопку: " + dataFile;
}
//===========================================
