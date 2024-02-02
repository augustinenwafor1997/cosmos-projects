#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <EEPROM.h>

#include <ShiftRegister74HC595.h>

// create a global shift register object
// parameters: <number of shift registers> (data pin, clock pin, latch pin)
ShiftRegister74HC595<3> sr(PA1, PA3, PA2);

//RF24 radio(10, 9);  // nRF24L01 (CE,CSN)
RF24 radio(PB0, PB1);  // nRF24L01 (CE,CSN)

//const byte address[][6] = { "00001A", "00001B", "00001C", "00001D", "00001E" };
uint8_t address[][6] = { "1Node", "2Node", "3Node", "4Node", "5Node" };


// const int switchPin1 = 5;
// const int switchPin2 = 6;
// const int switchPin3 = 7;
// const int switchPin4 = 8;

const int switchPin1 = PB6;
const int switchPin2 = PB7;
const int switchPin3 = PB8;
const int switchPin4 = PB9;

const int alarmLedPin = 21;
const int buzzerPin = 20;

unsigned long lastBuzzTime = 0, buzzEndTime = 0;
unsigned long lastReceivedTimes[5] = { 0 };  // Track last received times for each ID
bool storedIDs[5] = { false };               // Track stored IDs in EEPROM
bool flags[5] = { false };                   // Track error flags
bool alarmFlag = false;

unsigned long alarmCheckStartTime = 0;
int alarmCheckCount = 0;

// ShiftRegister74HC595<3> sr(2, 3, 4);
//ShiftRegister74HC595<3> sr(PA1, PA3, PA2);
uint8_t channel = 0;
JsonDocument doc;
void setup() {
  //sr.set(0, HIGH);
  pinMode(switchPin1, INPUT_PULLDOWN);
  pinMode(switchPin2, INPUT_PULLDOWN);
  pinMode(switchPin3, INPUT_PULLDOWN);
  pinMode(switchPin4, INPUT_PULLDOWN);
  //   Serial.begin(9600);
  channel += digitalRead(switchPin4) * 8;  // 2^3
  channel += digitalRead(switchPin3) * 4;  // 2^2
  channel += digitalRead(switchPin2) * 2;  // 2^1
  channel += digitalRead(switchPin1) * 1;  // 2^0

  //my debugger
  // if (channel == 1) {
  //   sr.set(0, HIGH);
  // }
  while (channel < 1) {
    // code does not run until channel is set
  }
  radio.begin();
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(channel);
  //   Serial.println("normal mode");
  for (int i = 0; i < 5; i++) {
    radio.openReadingPipe(i + 1, address[i]);
    delay(5);
  }

  radio.startListening();

  //   // Load stored IDs from EEPROM
  for (int i = 0; i < 5; i++) {
    storedIDs[i] = EEPROM.read(i) == 1;
  }
}

void loop() {

   receiveData();
  controlAlarm();
  //sr.setAllLow();
  // sr.set(buzzerPin, HIGH); 
  // sr.set(0, HIGH); 
  // delay(1000);
  // sr.set(buzzerPin, LOW);  
  // sr.set(0, LOW);
  // delay(1000);
}
void receiveData() {
  if (radio.available()) {
   // sr.set(0, HIGH);

    char text[32] = "";
    radio.read(&text, sizeof(text));

    // JsonDocument doc;
    DeserializationError error = deserializeJson(doc, text);
   // Serial.println(text);
    if (!error) {
      uint8_t comm = doc["comm"];
      uint8_t ledIndex = (comm - 1) * 3;  // Calculate LED index based on pipe
      sr.set(comm + 14, HIGH);            //set LED high to show comm is ongoing
      sr.setNoUpdate(ledIndex, doc["f"]);
      sr.setNoUpdate(ledIndex + 1, doc["d"]);
      sr.setNoUpdate(ledIndex + 2, doc["hv"]);
      sr.updateRegisters();

      // Store ID if not already stored
      if (!storedIDs[comm - 1]) {
        EEPROM.write(comm - 1, 1);
        storedIDs[comm - 1] = true;
      }

      lastReceivedTimes[comm - 1] = millis();


    } else {
      //Serial.println("JSON parsing error");
      //sr.set(0, HIGH);
      return;
    }
  }
}
void controlAlarm() {

  // Check for communication loss
  bool anyLoss = false;
  for (int i = 0; i < 5; i++) {
    if (storedIDs[i] && millis() - lastReceivedTimes[i] >= 30000) {
      anyLoss = true;
      uint8_t ledIndex = i * 3;  // Calculate LED index based on array position
      sr.setNoUpdate(ledIndex, LOW);
      sr.setNoUpdate(ledIndex + 1, LOW);
      sr.setNoUpdate(ledIndex + 2, LOW);
      sr.setNoUpdate(i + 15, LOW);  //set LED low to show comm is finished
      sr.updateRegisters();
      //break;
    }
  }

  //Check if any error led is On
  for (int i = 0; i < 5; i++) {
    int startPin = i * 3;
    bool value1 = sr.get(startPin);
    bool value2 = sr.get(startPin + 1);
    bool value3 = sr.get(startPin + 2);
    flags[i] = value1 || value2 || value3;
  }

  bool anyAlarm = false;
  for (int i = 0; i < 5; i++) {
    if (flags[i]) {
      anyAlarm = true;
      break;
    }
  }

  //Control Alarm
  if (anyAlarm || anyLoss) {
    alarmFlag = true;
    sr.set(alarmLedPin, HIGH);
  }

  if (alarmCheckCount == 0) {
    alarmCheckStartTime = millis();
    alarmCheckCount = 1;
  }

  // Check for alarm clearing
  if (alarmCheckCount > 0 && alarmFlag) {  // Only check if alarm is active
    if (millis() - alarmCheckStartTime >= 1000) {
      alarmCheckCount++;
      alarmCheckStartTime = millis();

      bool allValuesZero = true;
      for (int i = 0; i < 5; i++) {
        if (flags[i]) {
          allValuesZero = false;
          break;
        }
      }

      if (allValuesZero && alarmCheckCount >= 6 && anyLoss == false) {  //check that all comms are good too
        alarmFlag = false;
        sr.set(alarmLedPin, LOW);
        alarmCheckCount = 0;
      }
    }
  }

  // if (alarmFlag == false && anyLoss == false) {
  //   sr.set(alarmLedPin, LOW);
  // }

  //control buzzer if any led is On
  if (alarmFlag && millis() - lastBuzzTime >= 5000) {
    buzzEndTime = millis() + 500;  // Set buzzer end time (adjust duration as needed)
    sr.set(buzzerPin, HIGH);       // Turn on buzzer
    lastBuzzTime = millis();
  }
  if (sr.get(buzzerPin) && millis() >= buzzEndTime) {
    sr.set(buzzerPin, LOW);  // Turn off buzzer
  }
}