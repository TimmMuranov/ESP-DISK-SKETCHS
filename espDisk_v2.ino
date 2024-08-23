#include "ArduinoJson.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SPI.h>
#include <SD.h>
#include <FS.h>

const char *ssid = "TimsServer"; // Имя вашей Wi-Fi сети
const char *password = "12345678"; // Пароль от вашей Wi-Fi сети

File root;

ESP8266WebServer server(80);

String nameList = "";

void buildNameList() {
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
  "<!DOCTYPE html><html><head><title>Title</title>" 
  "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"></head>" 
  "<body>"
  "<style>.butStyle {width: 50%;}</style>";//создаем заголовок html файла
  
  String fileName = "";
  byte butNumber = 0;
  
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

  textNameMenue += "<script>";

  textNameMenue +=
  "function sendButtonText(button) {"
  "var xhr = new XMLHttpRequest();xhr.onreadystatechange = function() {"
  "if (xhr.readyState === 4 && xhr.status === 200) {"
  "console.log(\"Ответ получен: \", xhr.responseText);}};"
  "xhr.open(\"POST\", \"/load\", true);"
  "xhr.setRequestHeader(\"Content-Type\", \"application/x-www-form-urlencoded\");"
  "xhr.send(\"buttonText=\" + button.textContent);}"
  "document.querySelectorAll('.butStyle').forEach(button => {"
  "button.addEventListener('click', function() {sendButtonText(this);});});";
    
  textNameMenue += "</script>";

  textNameMenue += "</body></html>"; //закрываем форму

  Serial.println("the final document is" + textNameMenue);
  
   server.send(200, "text/html", textNameMenue);
}

void openFile() {
    String buttonText = server.arg("buttonText");
    Serial.println("button " + buttonText);

   //чтение документа с одноименным именем и отправка его на сторону клиента
    
    server.send(200, "text/html", "Not work...");
}

void setup() {
  Serial.begin(9600);
//=============================================================
  while(!Serial){} // Ждём инициализации работы с шиной UART
  Serial.print("Initializing SD card...");
  if(!SD.begin(4)){Serial.println("initialization failed!"); return;}
  Serial.println("initialization done.");
  root = SD.open("/");
  Serial.println("done!");
//=============================================================

    WiFi.mode(WIFI_AP); // Установка режима точки доступа
    WiFi.softAP(ssid, password); // Создание точки доступа с указанным именем и паролем
    
    IPAddress ip(192, 168, 1, 1); // IP-адрес точки доступа
    IPAddress gateway(192, 168, 1, 1); // Шлюз по умолчанию
    IPAddress subnet(255, 255, 255, 0); // Маска подсети
    WiFi.softAPConfig(ip, gateway, subnet); // Настройка параметров точки доступа

    // Регистрация обработчика корневого пути
    server.on("/", buildNameList);
    server.on("/load", openFile);

    server.begin();
    Serial.println("Access Point started");

    pinMode(2, OUTPUT);
    digitalWrite(2, HIGH);
}

void loop() {
    server.handleClient();
}
