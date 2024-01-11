#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <EEPROM.h>

RF24 radio(10, 9);  // nRF24L01 (CE,CSN)

const byte address[][6] = { "00001A", "00001B", "00001C", "00001D", "00001E" };
const byte config_address[6] = "000011";
bool setupComplete = 0;
static enum operating_mode { SETUP,
                             NORMAL } mode;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);
   radio.setDataRate(RF24_2MBPS);

  // GET THE EEPROM CHANNEL
  uint8_t channel;
  EEPROM.get(0 * sizeof(channel), channel);
  Serial.print("channel saved in eeprom: ");
  Serial.println(channel);
  //SET OPERATING MODE
  if (channel != 0 && channel < 126) {
    radio.setChannel(channel);
    mode = NORMAL;
    Serial.println("normal mode");
    for (int i = 0; i < 5; i++) {
    radio.openReadingPipe(i + 1, address[i]);
    delay(5);
  }
  } else {
    mode = SETUP;
    Serial.println("setup mode");
    radio.openReadingPipe(1, config_address);
  }
  


  radio.startListening();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (mode == SETUP && !setupComplete) {
    //Serial.println("check for radio signal");
    if (radio.available()) {
      Serial.println("radio found!");
      uint8_t chann;
      radio.read(&chann, sizeof(chann));
      Serial.println(chann);
      //make sure channel is good and store to EEPROM
      if (chann > 0 && chann < 126) {
        EEPROM.put(0 * sizeof(chann), chann);
        setupComplete = 1;
        Serial.println("SETUP COMPLETE");
      }
    }
  }
}

// #include <SPI.h>
// #include <nRF24L01.h>
// #include <RF24.h>
// #include <ArduinoJson.h>

// RF24 radio(10, 9); // CE, CSN pins
// const byte address1[6] = "00001A";
// const byte address2[6] = "00001B";
// const int LED_PIN = 2;

// void setup() {
//   Serial.begin(9600);
//   pinMode(LED_PIN, OUTPUT);
//   radio.begin();
//   radio.openReadingPipe(1, address1); // Listen on both addresses
//   radio.openReadingPipe(2, address2);
//   radio.startListening();
// }

// void loop() {
//   if (radio.available()) {
//     //int pipe = radio.available(); // Identify the pipe
//     char text[32] = "";
//     radio.read(&text, sizeof(text));
// Serial.println(text);
//     // StaticJsonDocument doc;
//     // DeserializationError error = deserializeJson(doc, text);

//     // if (!error) {
//     //   bool anyOne = doc["ph-f"] | doc["ph-d"] | doc["ph-hv"];
//     //   digitalWrite(LED_PIN, anyOne ? HIGH : LOW);
//     // } else {
//     //   Serial.println("JSON parsing error");
//     // }
//   }
// }
