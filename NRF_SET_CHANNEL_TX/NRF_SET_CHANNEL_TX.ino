#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <EEPROM.h>

//RF24 radio(PA9, PA8);  // nRF24L01 (CE,CSN)
RF24 radio(10, 9);  // nRF24L01 (CE,CSN)

const byte address[][7] = { "00001A", "00001B", "00001C", "00001D", "00001E" };


const int channPin1 = 5;
const int channPin2 = 6;
const int channPin3 = 7;
const int channPin4 = 8;

const int addPin1 = 2;
const int addPin2 = 3;
const int addPin3 = 4;

// const int channPin1 = PB6;
// const int channPin2 = PB7;
// const int channPin3 = PB8;
// const int channPin4 = PB9;

// const int addPin1 = PA10;
// const int addPin2 = PA11;
// const int addPin3 = PA12;
uint8_t channel = 0, addrDec = 0, sendCount = 0;

const int numTransmitters = 5;                // Number of transmitters
const unsigned long timeSlotDuration = 4000;  // Time slot duration in milliseconds

//int transmitterID = 0;  // Assign a unique ID to each transmitter (1-5)

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
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(channel);
  radio.openWritingPipe(address[addrDec - 1]);

  radio.stopListening();
}

void loop() {

  //TEST CODE
  // const char test[] = "address 2";
  char test[10];  // Adjust array size if needed
  sprintf(test, "address %d", addrDec);
  // while (radio.testCarrier()) {
  //     delay(random(5, 50)); // Random backoff if channel is busy
  //     Serial.println("delay a little");
  //   }
  // radio.write(&test, sizeof(test));
  // // Collision detection
  //   if (!radio.testCarrier()) { // If carrier is still clear after transmission
  //     // Transmission likely successful
  //     Serial.println(test);
  //   } else {
  //     // Collision detected, retry with backoff
  //     Serial.println("collision");
  //     delay(random(50, 200)); // Longer backoff for collisions
  //   }

  unsigned long currentTime = millis();
  int currentSlot = currentTime / timeSlotDuration;  // Calculate current time slot
  uint8_t currentAddr = currentSlot % numTransmitters;
   
  Serial.print("CURRENT SLOT: ");
  Serial.println(currentAddr + 1);
  if (currentSlot % numTransmitters == addrDec - 1) {  // Check if it's this transmitter's slot
    // Transmit data
    if (sendCount < 3) {
      Serial.print("SLOT: ");
      Serial.println(addrDec);
      radio.powerUp();
      radio.write(&test, sizeof(test));
      sendCount++;
    }
  } else {
    // Wait for the next time slot
    //Serial.println("wait");
    radio.powerDown();  // Conserve power during inactive slots
    sendCount = 0;
  }
  //delay(2000);


  //  Serial.println(test);
  // delay(50);  // Delay to prevent overwhelming the serial output
}
