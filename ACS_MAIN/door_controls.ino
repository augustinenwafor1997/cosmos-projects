
void checkEntryDoorState() {
  if (digitalRead(entrySwitch) && !digitalRead(entrySolenoid) && entryDoorState == closed) {
    entryDoorState = open;
    Serial.println("DOOR IS OPEN NOW");
  }
}

void closeEntryDoor() {
  if ((!digitalRead(entrySwitch) && entryDoorState == open) || (!digitalRead(entrySolenoid) && entryChrono.hasPassed(10))) {
    digitalWrite(entrySolenoid, HIGH);//lock
    Serial.println("DOOR IS LOCKED");
    entryDoorState = closed;
    digitalWrite(amberLed, HIGH);
    digitalWrite(greenLed, LOW);
    Serial.println("GREEN LED IS OFF");
  }
}

void checkFailedEntry() {
  if (digitalRead(redLed)) {
    Serial.println("BEEPING ALARM FOR FAILED ENTRY");
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        digitalWrite(buzzer, LOW);
        delay(300);
        digitalWrite(buzzer, HIGH);
        delay(100);
      }
      delay(200);
    }
    digitalWrite(amberLed, HIGH);
    digitalWrite(redLed, LOW);
  }
}

void entryControls() {
  checkEntryDoorState();
  closeEntryDoor();
  checkFailedEntry();
}

bool checkExitSensor() {
  float volts = analogRead(exitSensor) * 0.0048828125; // value from sensor * (5/1024)
  int distance = 13 * pow(volts, -1);
  if (distance < 50) {
    return true;
  }
  return false;
}

void openExitDoor() {
  if (checkExitSensor() && digitalRead(exitSolenoid) && !digitalRead(exitSwitch)) {
    digitalWrite(exitSolenoid, LOW);//unlock
    Serial.println("exit door unlocked");
    //    digitalWrite(exitBuzzer, HIGH);
    //    delay(200);
    //    digitalWrite(exitBuzzer, LOW);
    digitalWrite(exitAmberLed, LOW);
    digitalWrite(exitGreenLed, HIGH);
    Serial.println("EXIT GREEN LED ON");
    exitChrono.restart();
  }
}

void checkExitDoorState() {
  if (digitalRead(exitSwitch) && !digitalRead(exitSolenoid) && exitDoorState == closed) {
    exitDoorState = open;
    Serial.println("EXIT DOOR IS OPEN NOW");
  }
}

void closeExitDoor() {
  if ((!digitalRead(exitSwitch) && exitDoorState == open) || (!digitalRead(exitSolenoid) && exitChrono.hasPassed(10))) {
    digitalWrite(exitSolenoid, HIGH);//lock
    Serial.println("EXIT DOOR IS LOCKED");
    exitDoorState = closed;
    digitalWrite(exitAmberLed, HIGH);
    digitalWrite(exitGreenLed, LOW);
    Serial.println("EXIT GREEN LED IS OFF");
  }
}

void exitControls() {
  openExitDoor();
  checkExitDoorState();
  closeExitDoor();
}
