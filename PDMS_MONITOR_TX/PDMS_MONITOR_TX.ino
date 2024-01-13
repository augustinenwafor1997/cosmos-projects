#include <Arduino_JSON.h>

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <EEPROM.h>

RF24 radio(10, 9);  // nRF24L01 (CE,CSN)

const byte address[][6] = { "00001A", "00001B", "00001C", "00001D", "00001E" };


const int channPin1 = 5;
const int channPin2 = 6;
const int channPin3 = 7;
const int channPin4 = 8;

const int addPin1 = 2;
const int addPin2 = 3;
const int addPin3 = 4;
uint8_t channel = 0, addrDec = 0;

void setup() {
  pinMode(channPin1, INPUT_PULLUP);
  pinMode(channPin2, INPUT_PULLUP);
  pinMode(channPin3, INPUT_PULLUP);
  pinMode(channPin4, INPUT_PULLUP);
  pinMode(addPin1, INPUT_PULLUP);
  pinMode(addPin2, INPUT_PULLUP);
  pinMode(addPin3, INPUT_PULLUP);
  Serial.begin(9600);
  channel += digitalRead(channPin1) * 8;  // 2^3
  channel += digitalRead(channPin2) * 4;  // 2^2
  channel += digitalRead(channPin3) * 2;  // 2^1
  channel += digitalRead(channPin4) * 1;  // 2^0
  Serial.print("Channel: ");
  Serial.println(channel);
  while (channel < 1) {
    // code does not run until channel is set
  }
  addrDec += digitalRead(addPin1) * 4;
  addrDec += digitalRead(addPin2) * 2;
  addrDec += digitalRead(addPin3) * 1;
  Serial.print("Address  in decimal: ");
  Serial.println(addrDec);
  while (addrDec < 1 || addrDec > 5) {
    // code does not if address is not set
  }
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_2MBPS);
  radio.setChannel(channel);
  radio.openWritingPipe(address[addrDec - 1]);

  radio.stopListening();
}

void loop() {

  //TEST CODE
  JSONVar data;

  data["comm"] = addrDec;
  data["f"] = 0;
  data["d"] = 0;
  data["hv"] = 1;

  String dataString = JSON.stringify(data);
  //const char test[] = "leonardo";
  radio.write(&dataString[0], dataString.length());
  int delay_time = addrDec * 100;
  delay(delay_time + 1000);

  /////////////////////////////////////////////////

  // data["comm"] = addrDec;
  // data["f"] = 0;
  // data["d"] = 0;
  data["hv"] = 0;

  dataString = JSON.stringify(data);
  //const char test[] = "leonardo";
  radio.write(&dataString[0], dataString.length());
  //int delay_time = addrDec * 100;
  delay(delay_time + 2000);

  // data["hv"] = 0;

  //   dataString = JSON.stringify(data);
  //const char test[] = "leonardo";
  radio.write(&dataString[0], dataString.length());
  //int delay_time = addrDec * 100;
  delay(delay_time + 2000);

  // data["hv"] = 0;

  // dataString = JSON.stringify(data);
  //const char test[] = "leonardo";
  radio.write(&dataString[0], dataString.length());
  //int delay_time = addrDec * 100;
  delay(delay_time + 2000);

  // Serial.println(channel);
  // delay(50);  // Delay to prevent overwhelming the serial output
}
