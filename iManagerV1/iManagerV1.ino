#include <Arduino.h>
#include "button.h"
#include "declarations.h"
#include "menucontrols.h"
#include "icons.h"
#include <FlashStorage_STM32.h>
//HardwareSerial Serial3(PB11, PB10);
//#define ySerial Serial3
// #include <U8g2lib.h>

// #ifdef U8X8_HAVE_HW_SPI
// #include <SPI.h>
// #endif
// #ifdef U8X8_HAVE_HW_I2C
// #include <Wire.h>
// #endif


// U8G2_ST7567_OS12864_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/PA5, /* data=*/PA7, /* cs=*/PA4, /* dc=*/PA3, /* reset=*/PA1);
void EEPROMwrite(int addrOffset,  String strToWrite);


void setup(void)
{
  pinMode(valve_1A, OUTPUT);
  digitalWrite(valve_1A, LOW);
  Checkoffsate();
  char valve_A[4] = {valve_1A, valve_2A, valve_3A, valve_4A};
  char valve_B[4] = {valve_1B, valve_2B, valve_3B, valve_4B};
  for (int i = 0; i < sizeof(valve_A); i++)
  {
    pinMode(valve_A[i], OUTPUT);
    pinMode(valve_B[i], OUTPUT);
  }
  menubutton_Init(PB13, PB14, PB12, PA8, PB15);
  pinMode(lcdBacklight, OUTPUT);
  pinMode(FS1, INPUT_PULLUP);
  pinMode(FS2, INPUT_PULLUP);
  pinMode(pumpPin, OUTPUT);
  u8g2.begin();
  u8g2.enableUTF8Print();
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_fub17_tr);// u8g2_font_fub17_tr
    u8g2.drawStr(10, 40, "COSMOS");
    u8g2.setFont(u8g2_font_helvB08_te);// u8g2_font_fub17_tr
    u8g2.drawStr(54, 50, "automation");
  } while ( u8g2.nextPage() );
  delay(6000);
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_fub17_tr);// u8g2_font_fub17_tr
    u8g2.drawStr(2, 40, "iMANAGER");
  } while ( u8g2.nextPage() );
  delay(6000);
  rtc.setClockSource(STM32RTC::LSI_CLOCK);
  rtc.begin();
  mySerial.begin(57600);
  //// ySerial.begin(57600);


  //const uint8_t MUX = 6;
  inputString.reserve(300);
  date_time();
  lcdChrono.restart();
  checkState = true;
  //irr_mode = MANUAL;
  //EEPROM.put(0 * sizeof(irr_mode), irr_mode);
  HardwareTimer *MyTim = new HardwareTimer(TIM1);
  MyTim->pause();
  MyTim->setOverflow(10000, HERTZ_FORMAT);
  MyTim->attachInterrupt(serialEvent);
  MyTim->resume();
}

void loop(void)
{
  readEEPROM();
  homepage();
  menu_controls();
  cycle_irrigate();
  pumpwater();
  timer_irrigate();
  manual_irrigate();
  sms();
  shutDown();
  turnOffBacklight();
  turnOnBacklight();
}
