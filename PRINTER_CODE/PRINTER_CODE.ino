#include "Adafruit_Thermal.h"
#include "qrcode.h"
#include "button.h"

Button printButton(3);
//#define printButton 3;
#include "SoftwareSerial.h"
#define TX_PIN 6 // Arduino transmit  YELLOW WIRE  labeled RX on printer
#define RX_PIN 5 // Arduino receive   GREEN WIRE   labeled TX on printer
SoftwareSerial mySerial(RX_PIN, TX_PIN); // Declare SoftwareSerial obj first
Adafruit_Thermal printer(&mySerial);     // Pass addr to printer constructor
void setup() {
  // put your setup code here, to run once:
  mySerial.begin(19200);  // Initialize SoftwareSerial
  //Serial1.begin(19200); // Use this instead if using hardware serial
  printer.begin();        // Init printer (same regardless of serial type)

  Serial.begin(115200);

  
}

void loop() {
  // put your main code here, to run repeatedly:
  //  Serial.print("PARITY: ");
  //  Serial.println(getParity());
  //  Serial.print("TOKEN: ");
  //  Serial.println(getToken());
  //  Serial.print("PAIR: ");
  //  Serial.println(getPairAndErr());
  //  Serial.print("ERROR: ");
  //  Serial.println(getPairAndErr());
  //encode();
  printQRcode(encode());
  //printTicket();
  delay(2000);
}
