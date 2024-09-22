#include "LittleFS.h"
#define MAX_BUFFER 256
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
