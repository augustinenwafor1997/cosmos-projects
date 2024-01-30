#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <EEPROM.h>

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

uint8_t channel = 0;

void setup() {
  pinMode(switchPin1, INPUT_PULLUP);
  pinMode(switchPin2, INPUT_PULLUP);
  pinMode(switchPin3, INPUT_PULLUP);
  pinMode(switchPin4, INPUT_PULLUP);
  Serial.begin(9600);
  channel += digitalRead(switchPin4) * 8;  // 2^3
  channel += digitalRead(switchPin3) * 4;  // 2^2
  channel += digitalRead(switchPin2) * 2;  // 2^1
  channel += digitalRead(switchPin1) * 1;  // 2^0
  Serial.print("Channel: ");
  Serial.println(channel);
  while (channel < 1) {
    // code does not run until channel is set
  }
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(channel);
  Serial.println("normal mode");
  for (int i = 0; i < 5; i++) {
    radio.openReadingPipe(i + 1, address[i]);
    delay(5);
  }

  radio.startListening();
}

void loop() {

  //TEST CODE
  //uint8_t pipeNum;
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    // Serial.print("Pipe Num: ");
    // Serial.println(pipeNum);
    Serial.print("text: ");
    Serial.println(text);
  }

  // Serial.println(channel);
  // delay(50);  // Delay to prevent overwhelming the serial output
}
