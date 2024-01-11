#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(10, 9);  // nRF24L01 (CE,CSN)

const byte address[6] =  "000011" ;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_2MBPS);
  radio.openWritingPipe(address);
  radio.stopListening();

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("writing address");
  uint8_t chann = 5;
  radio.write(&chann, sizeof(chann));
  delay(1000);
}

// #include <SPI.h>
// #include <nRF24L01.h>
// #include <RF24.h>
// #include <ArduinoJson.h>

// RF24 radio(10, 9); // CE, CSN pins
// const byte address2[6] = "00001B"; // Unique address for transmitter 2

// void setup() {
//   radio.begin();
//   radio.openWritingPipe(address2);
//   radio.setPALevel(RF24_PA_MIN);
//   radio.stopListening();
// }

// void loop(){
//   String json = "{I am a string}";
//   radio.write(&json, json.length());
//   delay(1000); // Adjust transmission interval
// }