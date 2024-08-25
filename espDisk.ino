#include "ArduinoJson.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SPI.h>
#include <SD.h>

File root;
const char *ssid = "TimsServer"; // Имя вашей Wi-Fi сети
const char *password = "12345678"; // Пароль от вашей Wi-Fi сети
ESP8266WebServer server(80);

String fileNameToOpen = "";
String fileDataToOpen = "";
byte flag = 0;

String printDirectory(File dir, int numTabs) {
    String result = "";
    while (true) {
        File entry = dir.openNextFile();
        if (!entry) break;
        for (uint8_t i = 0; i < numTabs; i++) {
            result += '\t';
        }
        result += entry.name();
        if (entry.isDirectory()) {
            result += "/";
            result += printDirectory(entry, numTabs + 1);
        }
        result += "<p>";
        entry.close();
    }
    return result;
}
void handleRoot()
{   
    if (server.hasArg("open"))
    {
      server.send(200, "text/html", "<!DOCTYPE html><html><head><title>Title</title><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"></head><body> Loading the text... <script>location.href='/?write=1';</script></body></html>");
    }
    else if (server.hasArg("write"))
    {
        server.send(200, "text/html", "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>Text Editor</title><style>#textEditorWindow {display: none;}.textEditor {width: 100%;height: 400px;padding: 10px;border: 1px solid #ccc;box-sizing: border-box;resize: vertical;}</style></head><body><div id=\"textEditorWindow\"><textarea class=\"textEditor\" id=\"textArea\" placeholder=\"To write...\">" + fileDataToOpen + "</textarea><button type=\"button\" onclick=\"window.close(); location.href='/'\">Close</button><button type=\"button\" onclick=\"saveText();\">Save</button></div>  <div style=\"position: fixed; right: 0;\"><input type=\"text\" id=\"inputText\" placeholder=\"Text name\"></div>    <script>  function saveText() { var textName = document.getElementById('inputText').value; var textData = document.getElementById('textArea').value; if (textName) { fetch('/save', { method: 'POST', headers: {'Content-Type': 'application/json'}, body: JSON.stringify({ name: textName, data: textData }) }) .then(response => response.text()) .then(data => { alert(data);});location.href='/' } else { alert(\"Please enter text name.\"); }}   document.addEventListener('DOMContentLoaded', function() { document.getElementById('textEditorWindow').style.display = 'block'; });      document.addEventListener('DOMContentLoaded', function() {document.getElementById('inputText').value = '" + fileNameToOpen + "';});  </script></body></html>");
    digitalWrite(2, LOW);
    flag = 0;
    }
    else if (server.hasArg("listDocuments"))
    {
        server.send(200, "text/html", "<!DOCTYPE html><html><head><title>Title</title><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"></head><body><button onclick=\"location.href='/'\">Back to home</button><p>" + printDirectory(root, 0) + "</body></html>");
    digitalWrite(2, HIGH);
    Serial.println("запись запущена");
    }
    else if (server.hasArg("readmi"))
    {
      server.send(200, "text/html", "<!DOCTYPE html><html><head><title>Title</title><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"></head><body><h1>Welcome to the web notebock!</h1><p>This devices function is creat, save and delete text files.</p><p>Device based on the tree of the directories.</p><h2>To create a document tap to one-name button.</h2><p>You will be taken to the text entry page.</p><p>The large window is for entering text and the small window at the bottom is for the title of the text. (Entering the title of the text is required!)</p><h2>To see the saved text tap to one-name button (in the home directory).</h2><p>You will be taken to the storage of the text.</p><button onclick=\"location.href='/'\">To home</button></body>");
    digitalWrite(2, HIGH);
    }
    else
    {
        server.send(200, "text/html", "<!DOCTYPE html><html><head><title>Title</title><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"></head><body><button onclick=\"location.href='/?listDocuments=1'\">Document's list</button><p>           <button id=\"openDocButton\">Open document</button>  <div id=\"OpenWindow\" style=\"display: none;\"><input type=\"text\" id=\"textInputOpen\"><button id=\"closeWindowButton\" onclick=\"location.href = '/?open=1'\">Open</button></div>      <p><button id=\"deleteDocButton\" >Delete document</button><div id=\"deleteWindow\" style=\"display: none;\"><input type=\"text\" id=\"textInputDel\"><button id=\"deleteButton\" onclick=\"location.href='/?listDocuments=1'\">Delete</button></div>   <p><button onclick=\"location.href='/?readmi=1'\">Readmi</button>      <script>document.getElementById('openDocButton').addEventListener('click', function() {var openWindow = document.getElementById('OpenWindow');if (openWindow.style.display === 'none') {openWindow.style.display = 'block';} else {var textInput = document.getElementById('textInput');if (!textInput.value) {openWindow.style.display = 'none';} else {window.location.href = '/?write=1/';}}});  document.getElementById('closeWindowButton').addEventListener('click', function() {var openWindow = document.getElementById('OpenWindow');OpenWindow.style.display = 'none';});      function toggleDeleteWindow() {var deleteWindow = document.getElementById('deleteWindow');if (deleteWindow.style.display === 'none') {deleteWindow.style.display = 'block';} else {deleteWindow.style.display = 'none';}} var deleteButton = document.getElementById('deleteDocButton');deleteButton.addEventListener('click', toggleDeleteWindow);     function deleteText() {var fileName = document.getElementById('textInputDel').value;fetch('/delete', { method: 'POST', headers: { 'Content-Type': 'application/json' }, body: JSON.stringify({ fileName: fileName }) }) .then(response => response.json()) .then(data => {console.log(data); }) .catch(error => {console.error('Error:', error);}); }document.addEventListener('DOMContentLoaded', function() {document.getElementById('textInputDel').addEventListener('change', deleteText); });              function sendTextToServer() {var text = document.getElementById('textInputOpen').value;fetch('/open', {method: 'POST', headers: { 'Content-Type': 'application/json'}, body: JSON.stringify({text: text})}) .then(response => response.json()) .then(data => { console.log(data);}) .catch(error => { console.error('Error:', error);});}document.getElementById('closeWindowButton').addEventListener('click', sendTextToServer);   </script>      </body></html>");
    digitalWrite(2, HIGH);
    }
}
void handleSave() {
    if (server.method() == HTTP_POST) {
        String json = server.arg("plain");
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, json);

        String textName = doc["name"];
        String textData = doc["data"];

        if (SD.exists(textName)) {
            SD.remove(textName);
        }

        File myFile = SD.open(textName, FILE_WRITE);
        if (myFile) {
            myFile.println(textData);
            myFile.close();
            Serial.println("File created successfully!");
        } else {
            Serial.println("Failed to create file");
        }
    } else {
        server.send(405, "text/plain", "Method Not Allowed");
    }
}

void handleOpen() {
    flag = 1;
    fileNameToOpen = "";
    fileDataToOpen = "";
    String json = server.arg("plain");
    DynamicJsonDocument doc(1024); 
    deserializeJson(doc, json);
    String fileName = doc["text"];
    String fileData = "";
    fileNameToOpen = fileName;
    File file = SD.open(fileName);
    if (file) {
        fileData = "";
        while (file.available()) {
            char c = file.read();
            fileData += c;
        }
        fileDataToOpen = fileData;
        file.close();
    } else {
        Serial.println("File not found");
        return;
    }
    Serial.println("text to " + fileNameToOpen + ": " + fileDataToOpen);
}


void handleDelete() {
    String json = server.arg("plain");
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, json);
    String fileName = doc["fileName"];
    SD.remove(fileName);
    Serial.println("file deleted: " + fileName);
}

void setup() {
    Serial.begin(9600);
while(!Serial){}
  Serial.print("Initializing SD card...");
  if(!SD.begin(4)){Serial.println("initialization failed!"); return;}
  Serial.println("initialization done.");
  root = SD.open("/");
  printDirectory(root, 0);
  Serial.println("done!");

    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);
    IPAddress ip(192, 168, 1, 1);
    IPAddress gateway(192, 168, 1, 1); 
    IPAddress subnet(255, 255, 255, 0);
    WiFi.softAPConfig(ip, gateway, subnet);

    server.on("/", handleRoot);
    server.on("/save", HTTP_POST, handleSave);
    server.on("/delete", HTTP_POST, handleDelete);
    server.on("/open", HTTP_POST, handleOpen);
    server.begin();
    Serial.println("Access Point started");

    pinMode(2, OUTPUT);
    digitalWrite(2, HIGH);
}

void loop() {
    server.handleClient();
}
