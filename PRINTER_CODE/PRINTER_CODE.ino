#include "Adafruit_Thermal.h"
#include "qrcode.h"
#include "button.h"
#include <U8g2lib.h>
#include <U8x8lib.h>
#define ROWS 29
#define COLS 29
#define OLED_WIDTH 128
#define OLED_HEIGHT 64
int array[ROWS][COLS];

Button printButton(3);
//#define printButton 3;
#include "SoftwareSerial.h"
#define TX_PIN 6                                                                                               // Arduino transmit  YELLOW WIRE  labeled RX on printer
#define RX_PIN 5                                                                                               // Arduino receive   GREEN WIRE   labeled TX on printer
SoftwareSerial mySerial(RX_PIN, TX_PIN);                                                                       // Declare SoftwareSerial obj first
Adafruit_Thermal printer(&mySerial);                                                                           // Pass addr to printer constructor
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE, /* clock= */ 21, /* data= */ 20);  // ESP32 Thing, HW I2C with pin remapping
//U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, SCL, SDA, RESET);

void setup() {
  // put your setup code here, to run once:
  mySerial.begin(19200);  // Initialize SoftwareSerial
  //Serial1.begin(19200); // Use this instead if using hardware serial
  printer.begin();  // Init printer (same regardless of serial type)
  u8g2.begin();

  Serial.begin(115200);
  printQRcode(encode());
  u8g2.setDrawColor(1);
  u8g2.clearBuffer();  // Clear the display buffer
  int offsetX = 2;
  int offsetY = 2;

  // Draw the array pixels based on their values
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      if (array[i][j] == 1) {
        // Map the array's 30x30 grid to the OLED's 128x64 resolution
        //u8g2.drawPixel(j * (OLED_WIDTH / COLS), i * (OLED_HEIGHT / ROWS));
        //u8g2.drawPixel(j + 2, i + 2);
        int doubleX = 2 * j;
        int doubleY = 2 * i;
        u8g2.drawPixel(offsetX + doubleX, offsetY + doubleY);
      }
    }
  }

  u8g2.sendBuffer();  // Send the updated buffer to the display
}

void loop() {
  // put your main code here, to run repeatedly:
  // Serial.print("PARITY: ");
  // Serial.println(getParity());
  // Serial.print("TOKEN: ");
  // Serial.println(getToken());
  // Serial.print("PAIR: ");
  // Serial.println(getPairAndErr());
  // Serial.print("ERROR: ");
  // Serial.println(getPairAndErr());
  //encode();
  // printQRcode(encode());
  //printTicket();
  delay(2000);
}
