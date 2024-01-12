#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <EEPROM.h>

#include <ShiftRegister74HC595.h>

RF24 radio(10, 9);  // nRF24L01 (CE,CSN)

const byte address[][6] = { "00001A", "00001B", "00001C", "00001D", "00001E" };


const int switchPin1 = 5;
const int switchPin2 = 6;
const int switchPin3 = 7;
const int switchPin4 = 8;

const int alarmLedPin = 20;
const int buzzerPin = 21;

unsigned long lastBuzzTime = 0, buzzEndTime = 0;
unsigned long lastReceivedTimes[5] = { 0 };  // Track last received times for each ID
bool storedIDs[5] = { false };               // Track stored IDs in EEPROM


ShiftRegister74HC595<3> sr(2, 3, 4);
uint8_t channel = 0;
JsonDocument doc;
void setup() {
  pinMode(switchPin1, INPUT_PULLUP);
  pinMode(switchPin2, INPUT_PULLUP);
  pinMode(switchPin3, INPUT_PULLUP);
  pinMode(switchPin4, INPUT_PULLUP);
  Serial.begin(9600);
  channel += digitalRead(switchPin1) * 8;  // 2^3
  channel += digitalRead(switchPin2) * 4;  // 2^2
  channel += digitalRead(switchPin3) * 2;  // 2^1
  channel += digitalRead(switchPin4) * 1;  // 2^0
  Serial.print("Channel: ");
  Serial.println(channel);
  while (channel < 1) {
    // code does not run until channel is set
  }
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_2MBPS);
  radio.setChannel(channel);
  Serial.println("normal mode");
  for (int i = 0; i < 5; i++) {
    radio.openReadingPipe(i + 1, address[i]);
    delay(5);
  }

  radio.startListening();

  // Load stored IDs from EEPROM
  for (int i = 0; i < 5; i++) {
    storedIDs[i] = EEPROM.read(i) == 1;
  }
}

void loop() {

  if (radio.available()) {

    char text[32] = "";
    radio.read(&text, sizeof(text));

    // JsonDocument doc;
    DeserializationError error = deserializeJson(doc, text);
    Serial.println(text);
    if (!error) {
      uint8_t comm = doc["comm"];
      uint8_t ledIndex = (comm - 1) * 3;  // Calculate LED index based on pipe
      sr.set(comm + 14, HIGH);            //set LED high to show comm is ongoing
      sr.setNoUpdate(ledIndex, doc["f"]);
      sr.setNoUpdate(ledIndex + 1, doc["d"]);
      sr.setNoUpdate(ledIndex + 2, doc["hv"]);
      sr.updateRegisters();

      //Check if any error led is On
      bool anyLedOn = false;
      for (int i = 0; i < 15; i++) {
        if (sr.get(i) == HIGH) {
          anyLedOn = true;
          break;
        }
      }

      // set the alarm led to On/Off depeding on state
      sr.set(alarmLedPin, anyLedOn);
      //digitalWrite(alarmLedPin, anyLedOn);

      //control buzzer if any led is On
      if (anyLedOn && millis() - lastBuzzTime >= 30000) {
        buzzEndTime = millis() + 500;  // Set buzzer end time (adjust duration as needed)
        sr.set(buzzerPin, HIGH);       // Turn on buzzer
        lastBuzzTime = millis();
      }
      if (millis() >= buzzEndTime) {
        sr.set(buzzerPin, LOW);  // Turn off buzzer
      }

      // Store ID if not already stored
      if (!storedIDs[comm - 1]) {
        EEPROM.write(comm - 1, 1);
        storedIDs[comm - 1] = true;
      }

      lastReceivedTimes[comm - 1] = millis();

      // Check for communication loss
      bool anyLoss = false;
      for (int i = 0; i < 5; i++) {
        if (storedIDs[i] && millis() - lastReceivedTimes[i] >= 30000) {
          anyLoss = true;
          break;
        }
      }
      //Do something if there is communication loss

    } else {
      Serial.println("JSON parsing error");
    }
  }
}
