//версия 2.0
#include "ArduinoJson.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SPI.h>
#include <SD.h>
#include <FS.h>
#include <string.h>

#define MAX_BUFFER 1024

const char *ssid = "TimsServer"; // Имя вашей Wi-Fi сети
const char *password = "12345678"; // Пароль от вашей Wi-Fi сети

File root;

ESP8266WebServer server(80);

String butText = "";
String myDir = "/";

void buildNameList() {
  String nameList = "";
  root = SD.open(myDir);
  butText ="";
    while (true) {
        File entry = root.openNextFile();
        if (!entry) break; 
        if (entry.isDirectory()) {
            nameList += entry.name();
            if (nameList.length() > 0 && nameList[nameList.length() - 1] != '/') {
                nameList += "?";
            }
            Serial.println("dir++");
        } else {
            nameList += entry.name();
            if (nameList.length() > 0 && nameList[nameList.length() - 1] != '/') {
                nameList += "&";
            }
        }
        entry.close();
    }

    String str1=""; String file=""; String dir="";
    for(int x=0; x<nameList.length(); ++x){
      if(nameList[x]=='&'){file += str1 + '&'; str1="";}
      else if(nameList[x]=='?'){dir += str1 + '?'; str1="";}
      else{str1+=nameList[x];}
    }
    nameList.clear();
    nameList = dir + file;
    
    Serial.println(nameList);

  String textNameMenue = 
  "<!DOCTYPE html><html><head><title>Title</title><meta charset=\"UTF-8\">" 
  "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"></head>" 
  "<body>"
  "<style>.butStyle {background-color: #888; width:80%;margin-bottom: 10px;} .butStyleX {width: 100%;}"
  " * {padding: 0;margin: 0;box-sizing: border-box;}"
  ".wrapper{width: 100%;height: 100vh;display: flex;align-items: center;justify-content: center;background: #fff; margin-bottom: 15px;}"
  ".btn {position: relative;text-decoration: none;color: #fff;text-transform: uppercase;font-size: 13px;"
  "width: 100%;min-height: 28px;padding: 5px 0px;display: flex;align-items: center;justify-content: center;" //параметры всей кнопки
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

  textNameMenue += "<div style=' width:100%;text-align:center; border:5px solid teal; margin-bottom: 10px; color:aqua'>DIRECTORY IS: " + myDir + "</div>";
  
  String fileName = "";
  int butNumber = 0;
  
  if(nameList){
    fileName = "";
    butNumber = 0;
    for (int x=0; x<nameList.length(); ++x){
      if(nameList[x] == '&'){
        textNameMenue += "<button id='";
        textNameMenue += butNumber;
        textNameMenue += "' class='butStyle'>";
        textNameMenue += fileName;
        textNameMenue += "</button><p>";
        butNumber ++; fileName = "";
      }
      else if(nameList[x] == '?'){
        textNameMenue += "<button id='";
        textNameMenue += butNumber;
        textNameMenue += "dir' class='butStyle' style='background-color:DeepSkyBlue'>";
        textNameMenue += fileName;
        textNameMenue += "</button><p>";
        butNumber ++; fileName = "";
      }
      else {
        fileName += nameList[x];
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

  textNameMenue += "<div class='btn' style='margin-bottom: 10px;'><a class='btn'><button id='creatBut' style='background:teal; width: 75%;' onclick = 'CreatFile()'>NEW FILE</button></a></div><p>";
  textNameMenue += "<div class='btn' style='margin-bottom: 10px;'><a class='btn'><button id='creatDir' style='background:SteelBlue; width: 75%;' onclick = 'CreatDir()'>NEW DIRECTORY</button></a></div>";
  textNameMenue += "<div class='btn' style='margin-bottom: 10px;'><a class='btn' href='/readme' style='color: teal;'>README</a></div>";
  
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
  "button.addEventListener('click', function() {sendButtonText(this);});});"
  
  "function CreatFile(){location.href = 'textWin';}"
  
  "function CreatDir(){var userInput = prompt('Enter directory name:');"
  "if (!userInput) {alert('Enter directory name!'); return;}"
  "var xhr = new XMLHttpRequest(); xhr.open('POST', '/creatDir', true); xhr.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');"
  "xhr.onreadystatechange = function () {if (xhr.readyState === 4 && xhr.status === 200) {console.log('Запрос успешно отправлен');"
  "} else {console.error('Ошибка при отправке запроса');}}; xhr.send('text=' + userInput);}";
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
  "<style> .textarea{width: 100vw; height: 50vh; resize: none;}"
  " * {padding: 0;margin: 0;box-sizing: border-box;}"
  ".wrapper{width: 100%;height: 100vh;display: flex;align-items: center;justify-content: center;background: #fff; margin-bottom: 15px;}"
  ".btn {position: relative;text-decoration: none;color: #fff;text-transform: uppercase;font-size: 13px;"
  "width: 100%;min-height: 28px;padding: 5px 0px;display: flex;align-items: center;justify-content: center;" //параметры всей кнопки
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
  "</style>";

  textWindow += "<div style='solid 4px; width:100%;text-align: center;border: 5px solid teal; margin-bottom: 5px; color:aqua;'>FILE NAME: <input id='textNameWindow' type='text' value='" +  butText + "' style='color:PaleTurquoise;'></div>";

  textWindow += "<textarea id='story' class = 'textarea' placeholder='To write...' style=' margin-bottom: 15px; color:teal;'>" + fileData + "</textarea>";

    textWindow += "<div class='btn' style='margin-bottom: 10px;'><a class='btn' id='saveFile' style='color:aqua;'><button id='saveFileBtn' style='color:aqua; width: 75%;' onclick = 'Save()'>SAVE FILE</button></a></div><p>";
    textWindow += "<div class='btn' style='margin-bottom: 10px;'><a class='btn' id='deleteFile' style='color:aqua;'><button id='saveFileBtn' style='color:aqua; width: 75%;' onclick = 'Delete()'>DELETE FILE</button></a></div><p>";
    textWindow += "<div class='btn' style='margin-bottom: 10px;'><a href='/' class='btn' id='toHome' style='color:aqua'>Back to home</a></div><p>";    
    textWindow += "<script>"
    "function Save(){var textName = document.getElementById('textNameWindow').value;"
    "if (textName === '') {alert('Введите название текста');}"
    "else {location.href = '/textWin';}}"
    
    "function Delete(){location.href='/textWin'}"
    "</script>";
    textWindow += "</body></html>";
    
    server.send(200, "text/html", textWindow);
}

void creatDir(){
  String dirName = server.arg("text");
  Serial.println(dirName);
  if(SD.exists(dirName)){
    File file = SD.open(dirName);
    if(file.isDirectory()){
      return;
    }
  }
  Serial.println(SD.mkdir(myDir + dirName));
}

void readme(){
  String rText=
  "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'>" 
  "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
  "<title>Title</title></head><body>"
  "<div style='background-color: DarkSlateGrey; width: 100%; color:aqua;'>"
  "Добро пожаловать на мини сервер ESP-DISK!<p>"
  "Здесь можно создавать текстовые файлы и папки (каталоги).<p>"
  "Кнопки 'Creat File', 'Creat directory' создают эти объекты в том каталоге, в котором вы находитесь (Изначальная позиция - корневой каталог /).<p>"
  "Кнопки создают два типа файлов: серые файлы - это текстовые документы. Синие файлы - это каталоги.<p>"
  "Нажатие на каталог переместит вас в него и отобразит все файлы этого каталога. Нажатие на текстовый документ откроет его в окне для редактирования.<p>" 
  "В названии файлов и каталогов могут фигурировать символы русского, английского алфавита в любом регистре и цифры. Также доступны скобки, запятые и точки.<p>"
  "Использование прочих служебных символов запрещено. Возможна некорректная работа сервера<p>"
  "</div>"
  "<button style='background-color:Teal' onclick=location.href='/'>BACK TO HOME</button>"
  "</body></html>";
  server.send(200, "text/html", rText);
}

void setup() {
  Serial.begin(9600);

  while(!Serial){} // Ждём инициализации работы с шиной UART
  Serial.print("Initializing SD card...");
  if(!SD.begin(4)){Serial.println("initialization failed!"); return;}
  Serial.println("initialization done.");
  //root = SD.open("/");
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
    server.on("/readme", readme);
    server.on("/creatDir", creatDir);

    server.begin();
    Serial.println("Access Point started");

    pinMode(2, OUTPUT);
    digitalWrite(2, HIGH);
}

void loop() {
    server.handleClient();
}
