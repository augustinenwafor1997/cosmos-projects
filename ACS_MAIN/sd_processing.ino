void espCommands() {
  if (ESPSerial.available() > 0) {
    String input = ESPSerial.readString();
    int separatorIndex = input.indexOf("-");
    if (separatorIndex == -1) {
      if (input.startsWith("get,")) {
        String startDateStr = getValue(input, ',', 1);
        String endDateStr = getValue(input, ',', 2);
        Serial.print("START DATE: ");
        Serial.println(startDateStr);
        Serial.print("END DATE: ");
        Serial.println(endDateStr);
        int day = getValue(startDateStr, '/', 0).toInt();
        int month = getValue(startDateStr, '/', 1).toInt();
        int year = getValue(startDateStr, '/', 2).toInt();
        DateTime startEpochTime = DateTime(year, month, day, 0, 0, 0);
        day = getValue(endDateStr, '/', 0).toInt();
        month = getValue(endDateStr, '/', 1).toInt();
        year = getValue(endDateStr, '/', 2).toInt();
        DateTime endEpochTime = DateTime(year, month, day, 0, 0, 0);
        DateTime currentTime = rtc.now();
        DateTime currentEpochTime = DateTime(currentTime.year(), currentTime.month(), currentTime.day(), 0, 0, 0);
        Serial.print("START EPOCH TIME: ");
        Serial.println(startEpochTime.unixtime());
        Serial.print("END EPOCH TIME: ");
        Serial.println(endEpochTime.unixtime());
        Serial.print("CURRENT EPOCH TIME: ");
        Serial.println(currentEpochTime.unixtime());
        if (startEpochTime.unixtime() > endEpochTime.unixtime() || startEpochTime.unixtime() > currentEpochTime.unixtime() || endEpochTime.unixtime() > currentEpochTime.unixtime()) {
          Serial.println("INVALID SELECTION");
          ESPSerial.println("INVALID!");
        }
        else {
          File file = SD.open("logs.csv", FILE_READ);
          if (file) {
            String temp = "";
            bool found = false;
            Serial.println("READING FILE...");
            while (file.available()) {
              String line = file.readStringUntil('\n');
//              Serial.print("DATA: ");
//              Serial.println(line);
              String dateStr = getValue(line, ',', 2);
              day = getValue(dateStr, '/', 0).toInt();
              month = getValue(dateStr, '/', 1).toInt();
              year = getValue(dateStr, '/', 2).toInt();
              DateTime EpochTime = DateTime(year, month, day, 0, 0, 0);
              if (EpochTime.unixtime() >= startEpochTime.unixtime() && EpochTime.unixtime() <= endEpochTime.unixtime()) {
                temp += line + "\n";
                found = true;
              }
            }
            file.close();
            if (found) {
              file = SD.open("logdata.csv", FILE_WRITE);
              if (file) {
                file.print(temp);
                file.close();
                Serial.println("data retrieved successfully.");
                /*
                 * 
                 * Send data to ESP here
                 * 
                 */
                 
                 //destroy file afterwords
                 SD.remove("logdata.csv");
              }
            }
            else{
              Serial.println("NO DATA FOUND.");
              ESPSerial.println("NO DATA FOUND!");
            }
          }
          else{
            Serial.println("error opening file.");
          }
        }
      }
    } else {
      String uid =  getValue(input, '-', 0);
      String owner = getValue(input, '-', 1);
      String action = getValue(input, '-', 2);
      if (action == "activate") {
        storeUID(uid, owner, "uids.csv");
      } else if (action == "deactivate") {
        deleteUID(uid, owner, "uids.csv");
      }
    }
  }
}

void storeUID(String uid, String owner, String fileName) {
  checkFileExists(fileName);
  if (!checkIfUIDExists(uid, fileName)) {
    File dataFile = SD.open(fileName, FILE_WRITE);
    if (dataFile) {
      String dataToWrite = uid + "," + owner + "\n";
      dataFile.print(dataToWrite);
      Serial.println("UID stored");
      ESPSerial.println("user activated");
      dataFile.close();
    } else {
      Serial.println("error opening file");
      ESPSerial.println("error!");
    }
  } else {
    ESPSerial.println("user already exists!");
  }
}

void deleteUID(String uid, String owner, String fileName) {
  if (checkIfUIDExists(uid, fileName)) {
    File file = SD.open(fileName, FILE_WRITE);

    if (file) {
      String temp = "";
      while (file.available()) {
        String line = file.readStringUntil('\n');
        if (!line.startsWith(uid)) {
          temp += line + "\n";
        }
      }
      file.close();
      // Open the file for writing
      SD.remove(fileName);
      file = SD.open(fileName, FILE_WRITE);
      if (!file) {
        Serial.println("Failed to open file for writing");
        ESPSerial.println("error!");
        return;
      }
      file.print(temp);
      file.close();

      Serial.println("UID deleted successfully.");
      ESPSerial.println("user deactivated");
    } else {
      Serial.println("Error opening file.");
      ESPSerial.println("error!");
    }
  } else {
    Serial.println("UID not found.");
    ESPSerial.println("user not found!");
  }
}

bool checkIfUIDExists(String uid, String fileName) {
  File dataFile = SD.open(fileName, FILE_READ);
  if (dataFile) {
    while (dataFile.available()) {
      String line = dataFile.readStringUntil('\n');
      String storedUID = getValue(line, ',', 0);
      if (storedUID == uid) {
        Serial.println("UID exists");
        dataFile.close();
        return true;
      }
    }
    dataFile.close();
    return false;
  }
  return false;
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++)
  {
    if (data.charAt(i) == separator || i == maxIndex)
    {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
