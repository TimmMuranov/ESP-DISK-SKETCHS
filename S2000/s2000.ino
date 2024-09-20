#include "ArduinoJson.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SPI.h>
#include <SD.h>
#include <FS.h>

#include "fsReader.h"
#include "amogus.h"
#include "exCom.h"

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

  if(dataFile == "") return "\n>";

  if(exCom(dataFile,1) == "dir"){
    return dataFile + "\n" + takeDir() + "\n>" + myDir;
  }
  
  else if(exCom(dataFile,1) == "help"){
    return dataFile + "\n" + FsReader("help.txt") + "\n>" + myDir;  
    }
    
  else if(exCom(dataFile,1) == "amogus"){
    return dataFile + "\n" + "ТУ-ТУ-ТУ-ТУ-ТУ-ТУ-ТУ---------ТУ-ТУ-ТУ\n>"+ myDir;  
    }
  
  else if(exCom(dataFile,1) == "cd"){
    return exCom(dataFile,1) + "\n" + cd(dataFile) + "\n>" + myDir;
  }

  else if(exCom(dataFile,1) == "mkdir"){
    return dataFile + "\n" + mkDir(exCom(dataFile, 2)) + "\n>";
    //дописать функцию mkDir
    //return "\nmkdir пока не работает\n>" + myDir;
  }

  else if(exCom(dataFile,1) == "rmdir"){
    return dataFile + "\n" + rmDir(exCom(dataFile, 2)) + "\n>" + myDir;
    //дописать функцию rmDir
    //return "\nrmdir пока не работает\n>" + myDir;
  }

  else if(exCom(dataFile,1) == "upload"){
    return exCom(dataFile,1) + "\n>" + myDir;
  }
  

//здесь можно добавить команды по следующей формуле:
/*else if(dataFile == "command"){
    return exCom(dataFile,1) + "\n" + String function() или "текст" + "\n>" + myDir;  
    }*/
//не забудь включить новую команду в help.txt (!)
    
  else{
    return exCom(dataFile,1) + "\n" + "неизвестная команда...\n>" + myDir;
  }
}

//========================================================
String takeDir(){
  String dirList = "";
    String fileList = "";
    File file = SD.open(myDir);
    if (file) {
        while (true) {
            File whoIs = file.openNextFile();
            if (!whoIs) break;
            if (whoIs.isDirectory()) {
                String str(whoIs.name());
                dirList += str + ' ';
            } else if (!whoIs.isDirectory()) {
                String str(whoIs.name());
                fileList += str + ' ';
            }
            whoIs.close();
        }
    }
    file.close();
    if (dirList == "") dirList += "------";
    if (fileList == "") fileList += "------";
    return "папки:\n" + dirList + "\n" + "файлы:\n" + fileList;
}
//======================================================
String cd(String in){
  if(exCom(in,2) == "") return "Введено недостаточно аргументов";
  if(exCom(in,3) != "") return "Введено слишком много аргументов";
  String subDir = exCom(in,2);
  
  if(subDir == ".."){
      myDir = "/";
      return "Переход в начало (/) прошел успешно";
    }
    File file = SD.open(myDir);
    while(1){
      File whereIsDir = file.openNextFile();
      if(!whereIsDir) return "В папке '"+myDir+"' нет папки '"+subDir+"'";
      String name(whereIsDir.name());
      if(whereIsDir.isDirectory() == 1 && name == subDir){
        myDir += subDir + "/";
        return "Переход в папку '"+subDir+"' прошел успешно"+ 
        "текущее положение: '"+myDir+"'";
      }
      whereIsDir.close();
    }
    file.close();
}
//=====================================================
String mkDir(String dir){
  File file = SD.open(myDir + "/" + dir);
  if(!file){
    if (SD.mkdir(myDir + "/" + dir)){
      return "Создана папка '" + dir + "' в папке '" + myDir + "'";
    }
    else return "Во время создания папки произошла ошибка...";
  }
  else return "Папка '" + dir + "' уже существует в папке '" + myDir + "'";
}
//======================================================
String rmDir(String dir){
  File file = SD.open(myDir + "/" + dir);
  if(file){
    File isFiles = file.openNextFile();
    if (isFiles) return "Удаляемая папка должна быть пустой.";
    if (SD.rmdir(myDir + "/" + dir)){
      return "Папка " + dir + " удалена.";
    }
    else return "Во время удаления произошла ошибка...";
  }
  else return "Папки '" + dir + "' нет в '" + myDir + "'";
}
//======================================================
String takeFile() {
  //клиент успешно отправляет файлы в формате blob
  //необходимо принять их, вытащить название и содержимое
  //и создать на карте файл с нужным названием и содержимым
  return "загрузка файлов пока не работает...";
}
//======================================================
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
void handleFile(){
  server.send(200, "text/plain", takeFile());
  Serial.println("file are handled");
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
    server.on("/f", HTTP_POST, handleFile);
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
