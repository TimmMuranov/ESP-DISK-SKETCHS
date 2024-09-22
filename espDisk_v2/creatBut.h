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
    "<button style='background-color:Blue'>" +
     str +
    "</button><p>";
    }
  isDir.close();
  }
  while(1){
  File f = file.openNextFile();
  if(!f) break;
  if(!f.isDirectory()){
    String str(f.name());
    buttons +=
    "<button style='background-color:Gray'>" +
    str +
    "</button><p>";
    }
  f.close();
  }
  Serial.println("создание кнопок успешно");
  file.close();
  return buttons;
}
