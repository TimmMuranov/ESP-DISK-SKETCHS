#include "ArduinoJson.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SPI.h>
#include <SD.h>
#include "LittleFS.h"

#define MAX_BUFFER 256 // максимальный размер буфера копирования текста

const char *ssid = "TimsServer"; // Имя вашей Wi-Fi сети
const char *password = "12345678"; // Пароль от вашей Wi-Fi сети

ESP8266WebServer server(80);

String serverDir = "/"; // начальная директория сервера


//============= Функция чтения файлов из файловой системы МК ===============

String FsReader(String in){
  if (!LittleFS.begin()) {
    return "An Error has occurred while mounting LittleFS";
  }
  File file = LittleFS.open(in, "r");
  if (!file) {
    return "Failed to open file for reading";
  }
  String str = "";
  uint8_t buffer[MAX_BUFFER];
  size_t bytesRead;
  do {
    bytesRead = file.readBytes((char *)buffer, sizeof(buffer));
    
    if (bytesRead > 0) {
      str.concat((char*)buffer, bytesRead);
    }
  } while (bytesRead == sizeof(buffer));
  file.close();
  return str;
}
//============= Функция построения списка файлов ===============
// при первом прогоне (flag=d) функция записывает только директории, при втором (flag=f) - файлы.
void list(){
  char flag = 'd';
  String answer = FsReader("head.html");// Заголовок html страницы
  File files = SD.open(serverDir);
  if (!files) {
    answer += "<h1>Что-то пошло не так...</h1><h3>Перезапустите сервер</h3></body></html>";
    }
  answer += "<h1 style='margin-bottom: 10px;'>Файлы и папки:</h1>";
  while (true) {
    File entry = files.openNextFile();
    
    if (!entry && flag == 'd'){
      flag = 'f';
      files.rewindDirectory();
      }
    else if (!entry && flag == 'f') break;
    
    if (entry.isDirectory() == true && flag == 'd') {
      answer += "<button type='button' style='background-color:Blue;' onclick='dirText(event)'>";
      answer += entry.name();
      answer += "</button><p>";
      }
    if (!entry.isDirectory() && flag == 'f'){
      answer += "<button type='button' style='background-color:Gray;' onclick='dirText(event)'>";
      answer += entry.name();
      answer += "</button><p>";
      }
    entry.close();
  }
  
  answer += "<hr><p><button onclick = \"location.href='/'\">На главную страницу</button>";
  answer += "<script>";
  answer += FsReader("fList.js");//Функции, связанные с отправкой на сервер имен файлов
  answer += "</script></body></html>";
  server.send(200, "text/html", answer);
}

//=============== Домашняя страница =====================
void homePage(){
  server.send(200, "text/html", FsReader("homePage.html"));
}

//============= Страница с оприсанием сервера ==========
void readme(){
  server.send(200, "text/html", FsReader("readme.html"));
}

//====================================================
/////////////// Настройки сервера////////////////////
//=====================================================
void setup() {
  Serial.begin(115200);
  
  while(!Serial){} // Ждём инициализации работы с шиной UART
  Serial.print("Initializing SD card...");
  if(!SD.begin(4)){Serial.println("initialization failed!"); return;}
  Serial.println("initialization done.");
  Serial.println("done!");

  WiFi.mode(WIFI_AP); // Установка режима точки доступа
  WiFi.softAP(ssid, password); // Создание точки доступа с указанным именем и паролем
  IPAddress ip(192, 168, 1, 1); // IP-адрес точки доступа
  IPAddress gateway(192, 168, 1, 1); // Шлюз по умолчанию
  IPAddress subnet(255, 255, 255, 0); // Маска подсети
  WiFi.softAPConfig(ip, gateway, subnet); // Настройка параметров точки доступа

  server.on("/", homePage);
  server.on("/list", list);
  server.on("/readme", readme);

  server.begin();
  Serial.println("Access Point started");

  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
}

void loop() {
  server.handleClient();
}
