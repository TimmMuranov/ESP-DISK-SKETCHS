#include "ArduinoJson.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SPI.h>
#include <SD.h>
#include <FS.h>

#define MAX_BUFFER 1024

const char *ssid = "TimsServer"; // Имя вашей Wi-Fi сети
const char *password = "12345678"; // Пароль от вашей Wi-Fi сети

File root;

ESP8266WebServer server(80);

String nameList = "";
String butText = "";

void buildNameList() {
    butText ="";
    while (true) {
        File entry = root.openNextFile();
        if (!entry) break; 
        nameList += entry.name();
        if (nameList.length() > 0 && nameList[nameList.length() - 1] != '/') {
            nameList += "&";
        }
        entry.close(); // Закрываем текущий файл перед началом нового цикла
    }
    // Выводим список файлов в консоль
    Serial.println(nameList);

  String textNameMenue = 
  "<!DOCTYPE html><html><head><title>Title</title><meta charset=\"UTF-8\">" 
  "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"></head>" 
  "<body>"
  "<style>.butStyle {background-color: #888; width:40%;margin-bottom: 10px;} .butStyleX {width: 100%;}"
  " * {padding: 0;margin: 0;box-sizing: border-box;}"
  ".wrapper{width: 100%;height: 100vh;display: flex;align-items: center;justify-content: center;background: #fff; margin-bottom: 15px;}"
  ".btn {position: relative;text-decoration: none;color: #fff;text-transform: uppercase;font-size: 13px;"
  "width: 40%;min-height: 28px;padding: 5px 0px;display: flex;align-items: center;justify-content: center;" //параметры всей кнопки
  "font-family: \"Micra Normal\";text-align: center;background-color: rgba(24, 27, 38, 0.8);overflow: hidden;"
  "border-radius: 2px;border: 1px solid #0f3d5b;clip-path: polygon(0% 36%,13% 0%,100% 0%,100% 0%,100% 65%,87.5% 100%,0% 100%,0% 100% );}"//параметры обрезаний
  ".btn:before, .btn:after, .btn > span:nth-of-type(2):before, .btn > span:nth-of-type(2):after {"
  "content: "";position: absolute;background: #0f3d5b;} .btn:before, .btn:after {width: calc(100% - 12px);height: 1px;display: none;}"
  ".btn:before {top: 0;left: 12px;position: absolute;} .btn:after {bottom: 0;left: 12px;}"
  ".btn > span {position: absolute;top: 0;left: 0;width: 100%;height: 100%;}"
  ".btn > span:nth-of-type(2):before {height: 26px;width: 1px; -webkit-transform: rotate(54deg); transform: rotate(49deg);}"
  ".btn > span:nth-of-type(2):after {height: 26px;width: 1px; -webkit-transform: rotate(45deg);transform: rotate(49deg);}"
  ".btn > span:nth-of-type(2):before {top: -6.3px;left: 1.8px;}"
  ".btn > span:nth-of-type(2):after {bottom: -6.3px;right: 1.8px;}"
  "</style>";//создаем заголовок html файла
  
  String fileName = "";
  int butNumber = 0;
  
  if(nameList){
    fileName = "";
    butNumber = 0;
    for (int x=0; x<nameList.length(); ++x){
      if(nameList[x] != '&'){
        fileName += nameList[x];
      }
      else {
        textNameMenue += "<button id='";
        textNameMenue += butNumber;
        textNameMenue += "' class=\"butStyle\">";
        textNameMenue += fileName;
        textNameMenue += "</button><p>";
        butNumber ++; fileName = "";
      }
   }
    Serial.println("buttons are created");
  }
  else {
    //если нет файлов
    textNameMenue += "No files!";
    textNameMenue += "<p><button id='creatButton'>Creat file</button>";
    Serial.println("no files");
  }

  textNameMenue += "<div class=\"btn\" ><a href=\"/textWin\" class=\"btn\" id='creatButton'>Creat</a></div>";

  textNameMenue += "<script>";

  textNameMenue +=
  "function sendButtonText(button) {"
  "var xhr = new XMLHttpRequest();xhr.onreadystatechange = function() {"
  "if (xhr.readyState === 4 && xhr.status === 200) {"
  "console.log(\"Ответ получен: \", xhr.responseText);}};"
  "xhr.open(\"POST\", \"/load\", true);"
  "xhr.setRequestHeader(\"Content-Type\", \"application/x-www-form-urlencoded\");"
  "xhr.send(\"buttonText=\" + button.textContent);"
  "window.location.href = '/textWin';}"
  "document.querySelectorAll('.butStyle').forEach(button => {"
  "button.addEventListener('click', function() {sendButtonText(this);});});";
    
  textNameMenue += "</script>";

  textNameMenue += "</body></html>"; //закрываем форму

  Serial.println("the final document is" + textNameMenue);
  
   server.send(200, "text/html", textNameMenue);
}

void catchFile() {
    
    butText = server.arg("buttonText");
    Serial.println("opened: " + butText);
}

void openTextWindow(){
    
    File file = SD.open(butText);
    if (!file) {
        Serial.println("Failed to open file");
    }
    String fileData = "";
    int avail = file.available();
    uint8_t buffer[MAX_BUFFER];
    while (avail > 0) {
        if (avail > MAX_BUFFER) avail = MAX_BUFFER;
        file.read(buffer, avail);
        for (int i = 0; i < avail; ++i) {
            fileData += (char)buffer[i];
        }
        avail = file.available();
    }
    file.close();
  
  String textWindow =  
  "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'>" 
  "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
  "<title>Title</title></head><body>"
  "<style>.butStyle {width: 100%;} .textarea{width: 100vw; height: 50vh; resize: none;}</style>";

  textWindow += "Start edit: " + butText;

  textWindow += "<textarea id='story' class = 'textarea' placeholder='To write...'>" + fileData + "</textarea>";

    textWindow += "<button class='butStyle' onclick=\"location.href='/'\">Back to home</button><p>";
    textWindow += "<script></script>";
    textWindow += "</body></html>";
    
    server.send(200, "text/html", textWindow);
}

void setup() {
  Serial.begin(9600);

  while(!Serial){} // Ждём инициализации работы с шиной UART
  Serial.print("Initializing SD card...");
  if(!SD.begin(4)){Serial.println("initialization failed!"); return;}
  Serial.println("initialization done.");
  root = SD.open("/");
  Serial.println("done!");

    WiFi.mode(WIFI_AP); // Установка режима точки доступа
    WiFi.softAP(ssid, password); // Создание точки доступа с указанным именем и паролем
    IPAddress ip(192, 168, 1, 1); // IP-адрес точки доступа
    IPAddress gateway(192, 168, 1, 1); // Шлюз по умолчанию
    IPAddress subnet(255, 255, 255, 0); // Маска подсети
    WiFi.softAPConfig(ip, gateway, subnet); // Настройка параметров точки доступа
    // Регистрация обработчика корневого пути
    server.on("/", buildNameList);
    server.on("/load", catchFile);
    server.on("/textWin", openTextWindow);

    server.begin();
    Serial.println("Access Point started");

    pinMode(2, OUTPUT);
    digitalWrite(2, HIGH);
}

void loop() {
    server.handleClient();
}
