#include <SD.h>
String creatBut(String myDirectory){
  String buttons="";
  File file = SD.open(myDirectory);
  while(1){
  File isDir = file.openNextFile();
  if(!isDir) break;
  if(isDir.isDirectory()){
    String str(isDir.name());
    buttons +=
    "<button style='background-color:Blue' name = 'dir' id='" + str + "'>" +
     str +
    "</button><p>";
    }
  }
  file.rewindDirectory();
  while(1){
  File f = file.openNextFile();
  if(!f) break;
  if(!f.isDirectory()){
    String str(f.name());
    buttons +=
    "<button style='background-color:Gray' name = 'file' id='" + str + "'>" +
    str +
    "</button><p>";
    }
  f.close();
  }
  buttons += 
  "<hr><button id='Home' style='background-color:red'>Вернуться</button>";
  Serial.println("создание кнопок выполнено");
  file.close();
  return buttons;
}
