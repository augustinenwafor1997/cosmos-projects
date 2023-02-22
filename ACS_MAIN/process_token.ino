void processToken(const char* data) {
  String input = String(data);
  if (input.length() > 13) {
    memset(data, 0, sizeof(data));
    String uid = input.substring(0, 4);
    String token = input.substring(4, 11);
    Serial.print("UID: ");
    Serial.println(uid);
    Serial.print("TOKEN: ");
    Serial.println(token);
    //    Serial.print("OWNER: ");
    String storedOwner = checkUID(uid);
    if (storedOwner == "admin" || storedOwner == "tenant" || storedOwner == "cleaner") {
      Serial.println("stored owner is admin or tenant");
      //unlock door
      if (!digitalRead(entrySwitch) && digitalRead(entrySolenoid)) {
        digitalWrite(entrySolenoid, LOW);//unlock
        Serial.println("door unlocked");
        digitalWrite(buzzer, HIGH);
        delay(200);
        digitalWrite(buzzer, LOW);
        digitalWrite(amberLed, LOW);
        digitalWrite(greenLed, HIGH);
        Serial.println("GREEN LED ON");
        entryChrono.restart();
        //log usage
        logUsage(uid, storedOwner);
      }

    }
    else if (storedOwner == "user") {
      Serial.println("stored owner is user");
      //check token
      if (checkToken(token)) {
        Serial.println("token already exists. user na thief");
        digitalWrite(redLed, HIGH);
      }
      else {
        Serial.println("token not used, unlock door and log usage");
        //unlock door
        if (!digitalRead(entrySwitch) && digitalRead(entrySolenoid)) {
          digitalWrite(entrySolenoid, LOW);//unlock
          Serial.println("door unlocked");
          digitalWrite(buzzer, HIGH);
          delay(200);
          digitalWrite(buzzer, LOW);
          digitalWrite(amberLed, LOW);
          digitalWrite(greenLed, HIGH);
          Serial.println("GREEN LED ON");

          entryChrono.restart();
          //log usage
          logUsage(uid, storedOwner);
          //store token
          storeToken(token);
        }
        //...

      }


    }

  }
}

String checkUID(String uid) {
  checkFileExists("uids.csv");
  File dataFile = SD.open("uids.csv", FILE_READ);
  if (dataFile) {
    while (dataFile.available()) {
      String line = dataFile.readStringUntil('\n');
      String storedUID = getValue(line, ',', 0);
      if (storedUID == uid) {
        String storedOwner = getValue(line, ',', 1);
        Serial.print("OWNER: ");
        Serial.println(storedOwner);
        dataFile.close();
        return storedOwner;
      }
    }
    Serial.println("UID not found");
    dataFile.close();
  }
}

bool checkToken(String token) {
  checkFileExists("tokens.txt");
  File file = SD.open("tokens.txt", FILE_READ);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  boolean tokenExists = false;
  while (file.available()) {
    String line = file.readStringUntil('\n');
    //Serial.println(line);
    if (line.startsWith(token)) {
      tokenExists = true;
      break;
    }
  }
  file.close();
  return tokenExists;
}

void checkFileExists(String filename) {
  if (SD.exists(filename)) {
    Serial.println(String(filename) + " exists.");
  } else {
    Serial.println(String(filename) + " doesn't exist.");
    File file = SD.open(filename, FILE_WRITE);
    file.close();
    Serial.println("created one now.");
  }

}

void logUsage(String uid, String owner) {
  DateTime now = rtc.now();
  String date = String(now.day()) + "/" + String(now.month()) + "/" + String(now.year() - 2000);
  String time = String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());

  checkFileExists("logs.csv");

  File file = SD.open("logs.csv", FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  file.println(uid + "," + owner + "," + date + "," + time);
  file.close();
  Serial.print('\n');
  Serial.print("LOG:   ");
  Serial.println(uid + "," + owner + "," + date + "," + time);
}

void storeToken(String token) {
  File file = SD.open("tokens.txt", FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  file.println(token);
  file.close();
  tokenCount += 1;
  EEPROM.put(0 * sizeof(tokenCount), tokenCount);
  Serial.println("Token logged.");
}
//1300451140723810 1202735273120100 1043521722810002 1202735273195100 1197762179911000 1209459324297100 get,1/1/23,20/2/23

void clearTokenLog() {
  DateTime now = rtc.now();
  if (tokenCount > 10000 && now.hour() > 21) {
    File file = SD.open("tokens.txt", FILE_READ);
    if (!file) {
      Serial.println("Failed to open file for writing");
      return;
    }
    tokenCount = 0;
    long count = 0;
    String temp = "";
    while (file.available()) {
      String line = file.readStringUntil('\n');
      count ++;
      if (count > 5000) {
        temp += line + "\n";
        tokenCount++;
      }
    }
    file.close();
    EEPROM.put(0 * sizeof(tokenCount), tokenCount);
    SD.remove("tokens.txt");
    file = SD.open("tokens.txt", FILE_WRITE);
    if (file) {
      file.print(temp);
      file.close();
      Serial.println("5000 tokens deleted successfully");
    }
  }
}
