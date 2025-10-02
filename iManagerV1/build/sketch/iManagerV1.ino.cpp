#line 1 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\iManagerV1.ino"
#include <Arduino.h>
#include "declarations.h"
#include "menucontrols.h"
#include "icons.h"
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

#line 19 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\iManagerV1.ino"
void setup(void);
#line 66 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\iManagerV1.ino"
void loop(void);
#line 3 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\1_xml.ino"
void selectBox(uint8_t menu, uint8_t controlType);
#line 22 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\1_xml.ino"
void heading(uint8_t xpos, uint8_t ypos, const char *title);
#line 30 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\1_xml.ino"
void mainmenu(uint8_t marker, uint8_t frame, uint8_t controlType, struct All all_str);
#line 50 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\1_xml.ino"
void mode(uint8_t marker, uint8_t frame, uint8_t controlType, struct All all_str);
#line 65 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\1_xml.ino"
void cycle(uint8_t marker, uint8_t frame, uint8_t controlType, struct All all_str);
#line 82 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\1_xml.ino"
void timer(uint8_t marker, uint8_t frame, uint8_t controlType, struct All all_str);
#line 108 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\1_xml.ino"
void calendar(uint8_t marker, uint8_t frame, uint8_t controlType, struct All all_str);
#line 122 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\1_xml.ino"
void setdate(uint8_t marker, uint8_t frame, uint8_t controlType, struct All all_str);
#line 156 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\1_xml.ino"
void settime(uint8_t marker, uint8_t frame, uint8_t controlType, struct All all_str);
#line 190 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\1_xml.ino"
void outlet(uint8_t marker, uint8_t frame, uint8_t controlType, struct All all_str);
#line 203 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\1_xml.ino"
void sensor(uint8_t marker, uint8_t frame, uint8_t controlType, struct All all_str);
#line 216 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\1_xml.ino"
void network(uint8_t marker, uint8_t frame, uint8_t controlType, struct All all_str);
#line 230 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\1_xml.ino"
void humidity(uint8_t marker, uint8_t frame, uint8_t controlType, struct All all_str);
#line 1 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\Battery.ino"
uint8_t batterypercentage(const uint8_t pin);
#line 1 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\Home.ino"
void homepage();
#line 2 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\Irrigate.ino"
void futureTime(int addValue);
#line 11 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\Irrigate.ino"
void emptystruct();
#line 31 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\Irrigate.ino"
void timer_irrigate();
#line 1 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\Menu.ino"
void menu_controls();
#line 1 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\Pumpwater.ino"
void pumpwater();
#line 2 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\Timer_analysis.ino"
uint8_t fDay(struct Timer *timers);
#line 6 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\Timer_analysis.ino"
uint8_t fMth(struct Timer *timers);
#line 10 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\Timer_analysis.ino"
uint8_t fYr(struct Timer *timers);
#line 14 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\Timer_analysis.ino"
int starthour(struct Timer *timers);
#line 18 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\Timer_analysis.ino"
int startmin(struct Timer *timers);
#line 22 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\Timer_analysis.ino"
int runtime(struct Timer *timers);
#line 26 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\Timer_analysis.ino"
int frequency(struct Timer *timers);
#line 30 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\Timer_analysis.ino"
int valves(struct Timer *timers);
#line 34 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\Timer_analysis.ino"
uint8_t next_irrigation();
#line 1 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\cycle.ino"
void cycle_irrigate();
#line 1 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\eeprom.ino"
void readEEPROM();
#line 34 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\eeprom.ino"
void EEPROMwrite(int addrOffset, const String &strToWrite);
#line 43 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\eeprom.ino"
String EEPROMread(int addrOffset);
#line 1 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\sms.ino"
void sms();
#line 335 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\sms.ino"
void recvSMS(void);
#line 452 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\sms.ino"
void serialEvent1();
#line 462 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\sms.ino"
void collectSmsData(char inChar);
#line 485 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\sms.ino"
String smsVal(String haystack, const char *needle, char seperator);
#line 502 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\sms.ino"
String getValue(String data, char separator, int index);
#line 1 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\soil_moisture.ino"
uint8_t readSoilHumidity();
#line 19 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\iManagerV1.ino"
void setup(void)
{
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
  //pinMode(PB11, OUTPUT);
  //digitalWrite(PB11, HIGH);
  //digitalWrite(PB3, HIGH);
  //digitalWrite(lcdBacklight, LOW);
  //pinMode(PC13, OUTPUT);
  //digitalWrite(PC13, HIGH);
  u8g2.begin();
 u8g2.enableUTF8Print();
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_fub17_tr);// u8g2_font_fub17_tr
    u8g2.drawStr(10, 40, "COSMOS");
    u8g2.setFont(u8g2_font_helvB08_te);// u8g2_font_fub17_tr
    u8g2.drawStr(54, 50, "automation");
  } while ( u8g2.nextPage() );
  rtc.setClockSource(STM32RTC::LSE_CLOCK);
  rtc.begin();
  mySerial.begin(57600);
 //// ySerial.begin(57600);
  

  
 inputString.reserve(300);
  date_time(); //test function
  EEPROMwrite(3 * sizeof(phone), phone);
  EEPROM.put(2 * sizeof(password), password);
  HardwareTimer *MyTim = new HardwareTimer(TIM1);
  MyTim->pause();
  MyTim->setOverflow(10000, HERTZ_FORMAT);
  MyTim->attachInterrupt(serialEvent);
  MyTim->resume();
}

void loop(void)
{
  //readEEPROM();
  homepage();
  // menu_controls();
  // cycle_irrigate();
  // pumpwater();
  // timer_irrigate();
  // sms();
}

#line 1 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\1_xml.ino"
/////////////////////////// selectBox  ////////////////////////////////////////////////////////////////

void selectBox(uint8_t menu, uint8_t controlType)
{
  if (controlType == 0)
  {
    u8g2.setDrawColor(2);
    u8g2.drawBox(0, (1 + (12 * menu)), u8g2.getDisplayWidth(), 12);
  }
  else if (controlType == 1)
  {
    u8g2.setDrawColor(2);
    u8g2.drawBox((0 + (45 * (menu - 1))), 11, 43, 10);
  }
  else
  {
    /* code */
  }
}

//////////////////////////// xml components //////////////////////////////////////////////////////////
void heading(uint8_t xpos, uint8_t ypos, const char *title)
{ // to reduce repetition
  u8g2.setFont(u8g2_font_timB10_tr);
  u8g2.setFontMode(1);
  u8g2.drawStr(xpos, ypos, title); // u8g2_font_u8glib_4_tf
  u8g2.drawLine(0, 11, 127, 11);
  u8g2.setFont(u8g2_font_crox1hb_tr);
}
void mainmenu(uint8_t marker, uint8_t frame, uint8_t controlType, struct All all_str)
{

  u8g2.firstPage();
  do
  {
    heading(44, 10, "MENU");
    u8g2.drawStr(6, height(frame, 12, 1), "IRRIGATION MODE");
    u8g2.drawStr(6, height(frame, 12, 2), "SET DATE/TIME");
    u8g2.drawStr(6, height(frame, 12, 3), "WATER OUTLET");
    u8g2.drawStr(6, height(frame, 12, 4), "RAIN SENSE");
    u8g2.drawStr(6, height(frame, 12, 5), "NETWORK");
    u8g2.drawStr(6, height(frame, 12, 6), "RESET");
    u8g2.drawStr(6, height(frame, 12, 7), "EXIT");
    //    u8g2.drawStr(6, height(frame, 12, 8), "GAMES");
    //    u8g2.drawStr(6, height(frame, 12, 9), "EXIT");
    selectBox(marker, controlType);
  } while (u8g2.nextPage());
}

void mode(uint8_t marker, uint8_t frame, uint8_t controlType, struct All all_str)
{

  u8g2.firstPage();
  do
  {
    heading(44, 10, "MODE");
    u8g2.drawStr(6, height(frame, 12, 1), "CYCLE");
    u8g2.drawStr(6, height(frame, 12, 2), "TIMER");
    u8g2.drawStr(6, height(frame, 12, 3), "MANUAL");
    u8g2.drawStr(6, height(frame, 12, 4), "BACK");
    selectBox(marker, controlType);
  } while (u8g2.nextPage());
}

void cycle(uint8_t marker, uint8_t frame, uint8_t controlType, struct All all_str)
{

  u8g2.firstPage();
  do
  {
    heading(40, 10, "CYCLE");
    u8g2.drawStr(6, height(frame, 12, 1), "RUN<      >MINS");
    u8g2.drawStr(6, height(frame, 12, 2), "REST<      >MINS");
    u8g2.drawStr(6, height(frame, 12, 3), "SET");
    u8g2.drawStr(6, height(frame, 12, 4), "BACK");
    u8g2.drawStr(41, height(frame, 12, 1), u8x8_u16toa(all_str.cyc.Run, 3));
    u8g2.drawStr(47, height(frame, 12, 2), u8x8_u16toa(all_str.cyc.Rest, 3));
    selectBox(marker, controlType);
  } while (u8g2.nextPage());
}

void timer(uint8_t marker, uint8_t frame, uint8_t controlType, struct All all_str)
{
  static const char *valve_value[5] = {"1 >", "2 >", "3 >", "4 >", "ALL >"};
  u8g2.firstPage();
  do
  {
    heading(40, 10, "TIMER");
    u8g2.drawStr(1, height(frame, 12, 1), "PROG<    >");
    u8g2.drawStr(1, height(frame, 12, 2), "START HOUR<    >");
    u8g2.drawStr(1, height(frame, 12, 3), "START MIN<    >");
    u8g2.drawStr(1, height(frame, 12, 4), "DURATION<      MINS");
    u8g2.drawStr(1, height(frame, 12, 5), "INTERVAL<   >DAYS");
    u8g2.drawStr(1, height(frame, 12, 6), "VALVE<");
    u8g2.drawStr(1, height(frame, 12, 7), "SET");
    u8g2.drawStr(1, height(frame, 12, 8), "EXIT");
    u8g2.drawStr(1, height(frame, 12, 9), "BACK");
    u8g2.drawStr(43, height(frame, 12, 1), u8x8_u16toa(all_str.tim.Prog, 2));
    u8g2.drawStr(89, height(frame, 12, 2), u8x8_u16toa(all_str.tim.StartHr, 2));
    u8g2.drawStr(76, height(frame, 12, 3), u8x8_u16toa(all_str.tim.StartMin, 2));
    u8g2.drawStr(73, height(frame, 12, 4), u8x8_u16toa(all_str.tim.Runtime, 3));
    u8g2.drawStr(70, height(frame, 12, 5), u8x8_u16toa(all_str.tim.Interval, 1));
    u8g2.drawStr(50, height(frame, 12, 6), valve_value[all_str.tim.Valve]);
    selectBox(marker, controlType);
  } while (u8g2.nextPage());
}

void calendar(uint8_t marker, uint8_t frame, uint8_t controlType, struct All all_str)
{

  u8g2.firstPage();
  do
  {
    heading(11, 10, "SET DATE/TIME");
    u8g2.drawStr(6, height(frame, 12, 1), "DATE");
    u8g2.drawStr(6, height(frame, 12, 2), "TIME");
    u8g2.drawStr(6, height(frame, 12, 3), "EXIT");
    selectBox(marker, controlType);
  } while (u8g2.nextPage());
}

void setdate(uint8_t marker, uint8_t frame, uint8_t controlType, struct All all_str)
{
  u8g2.firstPage();
  do
  {
    u8g2.setFont(u8g2_font_helvB08_tr); // u8g2_font_timB10_tr
    u8g2.setFontMode(1);
    u8g2.drawStr(38, 8, "SET DATE");
    u8g2.drawLine(0, 9, 127, 9);
    u8g2.setFont(u8g2_font_helvB08_tr); // u8g2_font_luBS08_tr// show menu items:
    u8g2.drawStr(5, 20, "YEAR");
    u8g2.drawStr(44, 20, "MONTH");
    u8g2.drawStr(100, 20, "DAY");
    u8g2.drawStr(9, 30, u8x8_u16toa((all_str.cal.YR + 2000), 4));
    u8g2.drawStr(59, 30, u8x8_u16toa(all_str.cal.MTH, 2));
    u8g2.drawStr(106, 30, u8x8_u16toa(all_str.cal.DY, 2));
    if (marker == 1)
    {
      u8g2.setDrawColor(2);
      u8g2.drawBox(0, 11, 40, 10);
    }
    if (marker == 2)
    {
      u8g2.setDrawColor(2);
      u8g2.drawBox(39, 11, 49, 10);
    }
    if (marker == 3)
    {
      u8g2.setDrawColor(2);
      u8g2.drawBox(95, 11, 33, 10);
    }
    //  selectBox(marker, controlType);
  } while (u8g2.nextPage());
}
void settime(uint8_t marker, uint8_t frame, uint8_t controlType, struct All all_str)
{
  u8g2.firstPage();
  do
  {
    u8g2.setFont(u8g2_font_helvB08_tr); // u8g2_font_timB10_tr
    u8g2.setFontMode(1);
    u8g2.drawStr(38, 8, "SET TIME");
    u8g2.drawLine(0, 9, 127, 9);
    u8g2.setFont(u8g2_font_helvB08_tr); // u8g2_font_luBS08_tr// show menu items:
    u8g2.drawStr(2, 20, "HOUR");
    u8g2.drawStr(36, 20, "MINUTE");
    u8g2.drawStr(83, 20, "SECOND");
    u8g2.drawStr(10, 30, u8x8_u16toa(all_str.cal.HR, 2));
    u8g2.drawStr(50, 30, u8x8_u16toa(all_str.cal.MIN, 2));
    u8g2.drawStr(98, 30, u8x8_u16toa(all_str.cal.SEC, 2));
    if (marker == 1)
    {
      u8g2.setDrawColor(2);
      u8g2.drawBox(0, 11, 32, 10);
    }
    if (marker == 2)
    {
      u8g2.setDrawColor(2);
      u8g2.drawBox(34, 11, 44, 10);
    }
    if (marker == 3)
    {
      u8g2.setDrawColor(2);
      u8g2.drawBox(81, 11, 47, 10);
    }
    //    selectBox(marker, controlType);
  } while (u8g2.nextPage());
}
void outlet(uint8_t marker, uint8_t frame, uint8_t controlType, struct All all_str)
{

  u8g2.firstPage();
  do
  {
    heading(36, 10, "OUTLET");
    u8g2.drawStr(6, height(frame, 12, 1), "VALVE");
    u8g2.drawStr(6, height(frame, 12, 2), "PUMP");
    u8g2.drawStr(6, height(frame, 12, 3), "BACK");
    selectBox(marker, controlType);
  } while (u8g2.nextPage());
}
void sensor(uint8_t marker, uint8_t frame, uint8_t controlType, struct All all_str)
{

  u8g2.firstPage();
  do
  {
    heading(25, 10, "RAIN SENSE");
    u8g2.drawStr(6, height(frame, 12, 1), "ON");
    u8g2.drawStr(6, height(frame, 12, 2), "OFF");
    u8g2.drawStr(6, height(frame, 12, 3), "BACK");
    selectBox(marker, controlType);
  } while (u8g2.nextPage());
}
void network(uint8_t marker, uint8_t frame, uint8_t controlType, struct All all_str)
{

  u8g2.firstPage();
  do
  {
    heading(29, 10, "NETWORK");
    u8g2.drawStr(6, height(frame, 12, 1), "SMS");
    u8g2.drawStr(6, height(frame, 12, 2), "GPRS");
    u8g2.drawStr(6, height(frame, 12, 3), "OFF");
    u8g2.drawStr(6, height(frame, 12, 4), "BACK");
    selectBox(marker, controlType);
  } while (u8g2.nextPage());
} //
void humidity(uint8_t marker, uint8_t frame, uint8_t controlType, struct All all_str)
{

  u8g2.firstPage();
  do
  {
    heading(29, 10, "SETPOINT");
    u8g2.drawStr(6, height(frame, 12, 1), "HUMIDITY<      >%");
    u8g2.drawStr(6, height(frame, 12, 2), "SET");
    u8g2.drawStr(6, height(frame, 12, 3), "BACK");
    u8g2.drawStr(75, height(frame, 12, 1), u8x8_u16toa(all_str.hum, 3));
    selectBox(marker, controlType);
  } while (u8g2.nextPage());
}

#line 1 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\Battery.ino"
uint8_t batterypercentage(const uint8_t pin)
{
    analogReadResolution(12);
    float batVolt;
    int reading = analogRead(pin);
    batVolt = ((reading * 3.3) / 4096) * 5.55;
    return ((58.37 * batVolt) - 627.16);
}
#line 1 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\Home.ino"
void homepage()
{
  if (page == HOME)
  {
#define TIME_HEIGHT 20 //35
#define DATE_HEIGHT 32 //47
    uint8_t width = 0, percent = 100;

    u8g2.firstPage();
    do
    {

      /***DISPLAY BATTERY PERCENT AND ICON***/
      // u8g2.setFont(u8g2_font_mozart_nbp_tr);
      u8g2.setFont(u8g2_font_trixel_square_tr); //  u8g2_font_tinytim_tr u8g2_font_trixel_square_tr u8g2_font_blipfest_07_tr
      if (percent == 100)
      {
        u8g2.drawStr(102, 6, u8x8_u16toa(percent, 3));
      }
      else if (percent < 100 && percent >= 10)
      {
        u8g2.drawStr(105, 6, u8x8_u16toa(percent, 2));
      }
      else if (percent < 10)
      {
        u8g2.drawStr(109, 6, u8x8_u16toa(percent, 1));
      }

      /***DISPLAY NETWORK BARS***/
      if (simStatus == READY)
      {
        if (sigStrength < 10)
        {
          u8g2.drawXBMP(91, 1, icons8_network_bar__width, icons8_network_bar__height, icons8_network_1_bar);
        }
        else if (sigStrength >= 10 && sigStrength < 15)
        {
          u8g2.drawXBMP(91, 1, icons8_network_bar__width, icons8_network_bar__height, icons8_network_2_bar);
        }
        else if (sigStrength >= 15 && sigStrength < 20)
        {
          u8g2.drawXBMP(91, 1, icons8_network_bar__width, icons8_network_bar__height, icons8_network_3_bar);
        }
        else if (sigStrength >= 20 && sigStrength < 25)
        {
          u8g2.drawXBMP(91, 1, icons8_network_bar__width, icons8_network_bar__height, icons8_network_4_bar);
        }
        else if (sigStrength >= 25)
        {
          u8g2.drawXBMP(91, 1, icons8_network_bar__width, icons8_network_bar__height, icons8_network_5_bar);
        }
      }
      /***DISPLAY BATTERY ICON***/
      u8g2.drawXBMP(113 /*118*/, 0 /*1*/, icons8_empty_battery_32_2__width, icons8_empty_battery_32_2__height, icons8_empty_battery_32_2__bits);
      width = (percent * CHARGE_AREA_WIDTH) / 100;
      u8g2.drawBox(CHARGE_AREA_START_X, CHARGE_AREA_START_Y, width, CHARGE_AREA_HEIGHT);

      /***DISPLAY CALENDAR***/
      u8g2.setFont(u8g2_font_6x13B_tr); // u8g2_font_8x13B_tr
      u8g2.drawStr(0, TIME_HEIGHT, u8x8_u16toa(rtc.getHours(), 2));
      u8g2.drawStr(13, (TIME_HEIGHT - 1), ":");
      u8g2.drawStr(20, TIME_HEIGHT, u8x8_u16toa(rtc.getMinutes(), 2));
      u8g2.drawStr(33, (TIME_HEIGHT - 1), ":");
      u8g2.drawStr(40, TIME_HEIGHT, u8x8_u16toa(rtc.getSeconds(), 2));
      u8g2.drawStr(0, DATE_HEIGHT, u8x8_u16toa(rtc.getDay(), 2));
      u8g2.drawStr(14, DATE_HEIGHT, "/");
      u8g2.drawStr(20, DATE_HEIGHT, u8x8_u16toa(rtc.getMonth(), 2));
      u8g2.drawStr(34, DATE_HEIGHT, "/");
      u8g2.drawStr(40, DATE_HEIGHT, u8x8_u16toa(rtc.getYear() + 2000, 4));

      /***DISPLAY PUMP ICON***/
      // if (wOutlet == VALVE && digitalRead(pumpPin))
      // {
      u8g2.drawXBMP(24, (DATE_HEIGHT + 5), icons8_pump_16_width, icons8_pump_16_height, icons8_pump_16_bits);
      // }

      /***DISPLAY IRRIGATION ICON***/
      // else if (irr_valve == RUNNING || (wOutlet == PUMP && digitalRead(pumpPin)))
      // {
      u8g2.drawXBMP(0, (DATE_HEIGHT + 3), icons8_shower_24_20_1__width, icons8_shower_24_20_1__height, icons8_shower_24_20_1__bits);
      // }

      /***DISPLAY RUN CYCLE***/
      u8g2.drawXBMP(40, (DATE_HEIGHT + 5), icons8_bell_alarm_width, icons8_bell_alarm_height, icons8_bell_alarm);

      /***DISPLAY REST CYCLE***/
      u8g2.drawXBMP(55, (DATE_HEIGHT + 5), icons8_bell_snooze_width, icons8_bell_snooze_height, icons8_bell_snooze);

      /***DISPLAY RUN CYCLE***/
      u8g2.drawXBMP(75, (DATE_HEIGHT + 5), icons8_clock_snooze_width, icons8_clock_snooze_height, icons8_clock_snooze);

      /***DISPLAY REST CYCLE***/
      u8g2.drawXBMP(90, (DATE_HEIGHT + 5), icons8_clock_run_width, icons8_clock_run_height, icons8_clock_run);

      /**DISPLAY SIZE OF PROGRAMS STRUCT***/
      u8g2.setFont(u8g2_font_profont11_tr);
      u8g2.drawStr(0, 62, homeScr.c_str());
      //u8g2.drawStr(100, 62, u8x8_u16toa(sigStrength, 2));

      /****CYCLE MODE***/
      if (irr_mode == LOOP)
      {

        /***DISPLAY LOOP***/
        u8g2.drawXBMP(0, 0, icons8_loop_width, icons8_loop_height, icons8_loop);

        /***DISPLAY SOIL MOISTURE***/
        if (rSense == ON)
        {
          uint8_t soilmoisture = readSoilHumidity();
          // display soil moisture
          u8g2.drawXBMP(86, 31, icons8_moisture_24_20_1__width, icons8_moisture_24_20_1__height, icons8_moisture_24_20_1__bits);
          u8g2.setFont(u8g2_font_profont11_tr); // u8g2_font_ordinarybasis_tr
          // u8g2.setFontMode(1);
          u8g2.setCursor(100, 41);
          u8g2.print(soilmoisture);
          u8g2.print("%");

          //do later
          // if (percent == 100)
          // {
          //   u8g2.drawStr(100, 41, u8x8_u16toa(percent, 3));
          // }
          // else if (percent < 100 && percent >= 10)
          // {
          //   u8g2.drawStr(105, 6, u8x8_u16toa(percent, 2));
          // }
          // else if (percent < 10)
          // {
          //   u8g2.drawStr(109, 6, u8x8_u16toa(percent, 1));
          // }
        }
        // u8g2.setFont(u8g2_font_fub17_tr);
        // u8g2.drawStr(10, 40, "COSMOS");
        // u8g2.setFont(u8g2_font_helvB08_te);
        // u8g2.drawStr(54, 50, "automation");
      } //test code below

      /***TIMER MODE***/
      else if (irr_mode == PROGS)
      {
        /***DISPLAY TIMER***/
        u8g2.drawXBMP(0, 0, icons8_clock_width, icons8_clock_height, icons8_clock);

        /***DISPLAY NEXT IRRIGATION TIME***/
        u8g2.setFont(u8g2_font_mozart_nbp_tr);
        if (next_prog < 13)
        {
          u8g2.drawStr(80, TIME_HEIGHT, u8x8_u16toa(Programs[next_prog - 1].StartHr, 2));
          u8g2.drawStr(94, (TIME_HEIGHT - 1), ":");
          u8g2.drawStr(100, TIME_HEIGHT, u8x8_u16toa(Programs[next_prog - 1].StartMin, 2));
          u8g2.drawStr(90, (DATE_HEIGHT - 4), dayShortStr(Programs[next_prog - 1].futureWkDay));
        }
        else if (next_prog == 13)
        {
          u8g2.drawStr(80, TIME_HEIGHT, u8x8_u16toa(Programs[clashProgs[0]].StartHr, 2));
          u8g2.drawStr(94, (TIME_HEIGHT - 1), ":");
          u8g2.drawStr(100, TIME_HEIGHT, u8x8_u16toa(Programs[clashProgs[0]].StartMin, 2));
          u8g2.drawStr(90, (DATE_HEIGHT - 4), dayShortStr(Programs[clashProgs[0]].futureWkDay));
        }

        /***DISPLAY BELL ICON***/
        u8g2.drawXBMP(115, (TIME_HEIGHT - 3), icons8_bell_width, icons8_bell_height, icons8_bell);

        /***DISPLAY SOIL MOISTURE***/
        if (rSense == ON)
        {
          uint8_t soilmoisture = readSoilHumidity();
          // display soil moisture
          u8g2.drawXBMP(86, 31, icons8_moisture_24_20_1__width, icons8_moisture_24_20_1__height, icons8_moisture_24_20_1__bits);
          u8g2.setFont(u8g2_font_profont11_tr); // u8g2_font_ordinarybasis_tr
          // u8g2.setFontMode(1);
          u8g2.setCursor(100, 41);
          u8g2.print(soilmoisture);
          u8g2.print("%");
        }
        // u8g2.drawStr(2, 60, u8x8_u16toa(All_Str.prog_count, 2));
        // for (int i = 0; i < All_Str.prog_count; i++)
        // {
        //   int irr_time = starttime(&Programs[i]);
        //   int irr_min = startmin(&Programs[i]);
        //   const char *prog = ("PROG " + String(i + 1)).c_str();
        //   u8g2.drawStr(0, (i + 1) * 10, prog);
        //   u8g2.drawStr(45, (i + 1) * 10, u8x8_u16toa(irr_time, 2));
        //   u8g2.drawStr(57, (i + 1) * 10, ":");
        //   u8g2.drawStr(60, (i + 1) * 10, u8x8_u16toa(irr_min, 2));
        // }
        /*print future time*/
        // u8g2.drawStr(20, 60, "NEXT"); //
        // if (next_prog < 13)
        // {
        //   u8g2.drawStr(50, 60, u8x8_u16toa(Programs[next_prog - 1].StartHr, 2));
        //   u8g2.drawStr(65, 60, ":");
        //   //        u8g2.drawStr(70, 60, u8x8_u16toa(Programs[next_prog - 1].StartMin, 2));
        //   u8g2.drawStr(70, 60, u8x8_u16toa(startmin(&Programs[next_prog - 1]), 2));
        //   u8g2.drawStr(100, TIME_HEIGHT, u8x8_u16toa(Programs[next_prog - 1].futureDay, 2));
        //   u8g2.drawStr(115, DATE_HEIGHT, u8x8_u16toa(Programs[next_prog - 1].futureMonth, 2));
        //   u8g2.drawStr(105, TIME_HEIGHT - 12, u8x8_u16toa(Programs[next_prog - 1].futureYr, 4));
        // }
        // else if (next_prog == 13)
        // {
        //   u8g2.drawStr(50, 60, u8x8_u16toa(Programs[clashProgs[0]].StartHr, 2));
        //   u8g2.drawStr(65, 60, ":");
        //   //        u8g2.drawStr(70, 60, u8x8_u16toa(Programs[next_prog - 1].StartMin, 2));
        //   u8g2.drawStr(70, 60, u8x8_u16toa(startmin(&Programs[clashProgs[0]]), 2));
        //   u8g2.drawStr(100, TIME_HEIGHT, u8x8_u16toa(Programs[clashProgs[0]].futureDay, 2));
        //   u8g2.drawStr(115, DATE_HEIGHT, u8x8_u16toa(Programs[clashProgs[0]].futureMonth, 2));
        //   u8g2.drawStr(105, TIME_HEIGHT - 12, u8x8_u16toa(Programs[clashProgs[0]].futureYr, 4));
        // }
        // if (irrState == 1)
        //   u8g2.drawStr(100, 60, "SEEN");
        // //        u8g2.drawStr(100, 60, u8x8_u16toa(sizeof(Programs), 4));
        // /*print future day*/
        // u8g2.drawStr(67, TIME_HEIGHT, "DAY:");
        // u8g2.drawStr(67, DATE_HEIGHT, "MONTH:");
        // u8g2.drawStr(90, TIME_HEIGHT - 12, "Y:");

        // /*print next prog*/
        // u8g2.drawStr(75, 10, "PROG:");
        // u8g2.drawStr(110, 10, u8x8_u16toa(next_prog, 2));

        // u8g2.setFont(u8g2_font_6x13B_tr); // u8g2_font_8x13B_tr
        // u8g2.drawStr(0, TIME_HEIGHT, u8x8_u16toa(rtc.getHours(), 2));
        // u8g2.drawStr(13, (TIME_HEIGHT - 1), ":");
        // u8g2.drawStr(20, TIME_HEIGHT, u8x8_u16toa(rtc.getMinutes(), 2));
        // u8g2.drawStr(33, (TIME_HEIGHT - 1), ":");
        // u8g2.drawStr(40, TIME_HEIGHT, u8x8_u16toa(rtc.getSeconds(), 2));
        // u8g2.drawStr(0, DATE_HEIGHT, u8x8_u16toa(rtc.getDay(), 2));
        // u8g2.drawStr(14, DATE_HEIGHT, "/");
        // u8g2.drawStr(20, DATE_HEIGHT, u8x8_u16toa(rtc.getMonth(), 2));
        // u8g2.drawStr(34, DATE_HEIGHT, "/");
        // u8g2.drawStr(40, DATE_HEIGHT, u8x8_u16toa(rtc.getYear() + 2000, 4));
        //
        //        u8g2.drawStr(100, 10, u8x8_u16toa(count, 2));
        //        for (int i = 0; i < All_Str.prog_count; i++) {
        //          // int irr_time = starttime(&Programs[i]);
        //          // const char* prog = ("PROG " + String(i + 1)).c_str();
        //          // u8g2.drawStr(20, (i + 1) * 10, prog);
        //          u8g2.drawStr(i * 10, 50, u8x8_u16toa(starthr_store[i], 2));
        //        }
      }

      /***MANUAL MODE***/
      else if (irr_mode == MANUAL)
      {
        /***DISPLAY MANUAL***/
        u8g2.drawXBMP(0, 0, icons8_human_width, icons8_human_height, icons8_human);

        /***DISPLAY SOIL MOISTURE***/
        if (rSense == ON)
        {
          uint8_t soilmoisture = readSoilHumidity();
          // display soil moisture
          u8g2.drawXBMP(80, 20, icons8_moisture_24_20_1__width, icons8_moisture_24_20_1__height, icons8_moisture_24_20_1__bits);
          u8g2.setFont(u8g2_font_profont11_tr); // u8g2_font_ordinarybasis_tr
          // u8g2.setFontMode(1);
          u8g2.setCursor(95, 29);
          u8g2.print(soilmoisture);
          u8g2.print("%");
        }
      }

    } while (u8g2.nextPage());
  }
}

#line 1 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\Irrigate.ino"
/* calculate a future date */
void futureTime(int addValue)
{ //
  unsigned long futureEpochTime = (rtc.getEpoch() + (addValue * 86400));
  All_Str.tim.futureDay = day(futureEpochTime);
  All_Str.tim.futureMonth = month(futureEpochTime);
  All_Str.tim.futureYr = year(futureEpochTime) - 2000;
  All_Str.tim.futureWkDay = weekday(futureEpochTime);
}
/**Program to empty struct array**/
void emptystruct()
{
  struct Timer timers;
  timers = {
      .Prog = 0,
      .StartHr = 0,
      .StartMin = 0,
      .Runtime = 0,
      .Interval = 0,
      .Valve = 0,
      .futureYr = 0,
      .futureMonth = 0,
      .futureDay = 0,
      .futureWkDay = 0};
  for (int i = 0; i < PROG_SIZE; i++)
  {
    Programs[i] = timers;
  }
}
/***Irrigating function for timers***/
void timer_irrigate()
{
  if (irr_mode == PROGS)
  { //if we are in timer mode
    static uint8_t curr_prog = 0, curr_clash_count = 0;
    static int curr_clash_progs[PROG_SIZE];
    static bool mtimerState[PROG_SIZE];
    /***Below block gets next program on startup, menu setting or current program running***/
    if (next_irr_flag == true)
    {
      //EEPROM.get(5 * sizeof(All_Str.prog_count), Programs);
      next_prog = next_irrigation();
      next_irr_flag = false;
    }
    /***Below block handles single timer irrigation***/
    if (next_prog > 0 && next_prog < 13)
    {
      //irrState = 1;//test
      if ((fYr(&Programs[next_prog - 1]) == rtc.getYear()) && (fMth(&Programs[next_prog - 1]) == rtc.getMonth()) && (fDay(&Programs[next_prog - 1]) == rtc.getDay()) && (starthour(&Programs[next_prog - 1]) == rtc.getHours()) && (startmin(&Programs[next_prog - 1]) == rtc.getMinutes()))
      {
        //irrState = 1;                                 // test
        futureTime(Programs[next_prog - 1].Interval); //calculate future time
        /***store the next irrigation date***/
        Programs[next_prog - 1].futureDay = All_Str.tim.futureDay;
        Programs[next_prog - 1].futureMonth = All_Str.tim.futureMonth;
        Programs[next_prog - 1].futureYr = All_Str.tim.futureYr;
        EEPROM.put(7 * sizeof(APN), Programs);
        // emptystruct();                                        //test
        // EEPROM.get(5 * sizeof(All_Str.prog_count), Programs); //test
        next_irr_flag = true;                                 // raise flag
        curr_prog = next_prog;                                // flag current program
        if (rSense == ON)
        { // if soil moisture sensor is connected
          int sensor_val = readSoilHumidity();
          if (sensor_val < All_Str.hum)
          { // if it has not rained
            if (wOutlet == PUMP)
            { //pump irrigation
              digitalWrite(pumpPin, HIGH);
              mtimerState[curr_prog - 1] = true;
              mtimerChrono[curr_prog - 1].restart();
            }
            else if (wOutlet == VALVE)
            { //valve irrigation
            irr_valve = RUNNING;
              char valve_A[4] = {valve_1A, valve_2A, valve_3A, valve_4A};
              char valve_B[4] = {valve_1B, valve_2B, valve_3B, valve_4B};
              uint8_t valve = Programs[curr_prog - 1].Valve; // get valve setting
              if (valve == 0)
              { //valve 1
                digitalWrite(valve_1A, HIGH);
                digitalWrite(valve_1B, LOW);
                delay(100);
                digitalWrite(valve_1A, LOW);
                digitalWrite(valve_1B, LOW);
                mtimerState[curr_prog - 1] = true;
                mtimerChrono[curr_prog - 1].restart();
              }
              else if (valve == 1)
              { //valve 2
                digitalWrite(valve_2A, HIGH);
                digitalWrite(valve_2B, LOW);
                delay(100);
                digitalWrite(valve_2A, LOW);
                digitalWrite(valve_2B, LOW);
                mtimerState[curr_prog - 1] = true;
                mtimerChrono[curr_prog - 1].restart();
              }
              else if (valve == 2)
              { //valve 3
                digitalWrite(valve_3A, HIGH);
                digitalWrite(valve_3B, LOW);
                delay(100);
                digitalWrite(valve_3A, LOW);
                digitalWrite(valve_3B, LOW);
                mtimerState[curr_prog - 1] = true;
                mtimerChrono[curr_prog - 1].restart();
              }
              else if (valve == 3)
              { //valve 4
                digitalWrite(valve_4A, HIGH);
                digitalWrite(valve_4B, LOW);
                delay(100);
                digitalWrite(valve_4A, LOW);
                digitalWrite(valve_4B, LOW);
                mtimerState[curr_prog - 1] = true;
                mtimerChrono[curr_prog - 1].restart();
              }
              else if (valve == 4)
              { //all valves
                for (int i = 0; i < sizeof(valve_A); i++)
                {
                  digitalWrite(valve_A[i], HIGH);
                  digitalWrite(valve_B[i], LOW);
                }
                delay(100); // clarify actual delay time
                for (int i = 0; i < sizeof(valve_A); i++)
                {
                  digitalWrite(valve_A[i], LOW);
                  digitalWrite(valve_B[i], LOW);
                }
                mtimerState[curr_prog - 1] = true;
                mtimerChrono[curr_prog - 1].restart();
              }
            }
          }
        }
        else
        { // soil moisture sensor is not connected
          if (wOutlet == PUMP)
          {
            digitalWrite(pumpPin, HIGH);
            digitalWrite(PC13, LOW); //test
            mtimerState[curr_prog - 1] = true;
            mtimerChrono[curr_prog - 1].restart();
          }
          else if (wOutlet == VALVE)
          {
            irr_valve = RUNNING;
            char valve_A[4] = {valve_1A, valve_2A, valve_3A, valve_4A};
            char valve_B[4] = {valve_1B, valve_2B, valve_3B, valve_4B};
            uint8_t valve = Programs[curr_prog - 1].Valve;
            if (valve == 0)
            {
              // digitalWrite(valve_1A, HIGH);
              // digitalWrite(valve_1B, LOW);
              // delay(100);
              // digitalWrite(valve_1A, LOW);
              // digitalWrite(valve_1B, LOW);
              digitalWrite(valve_1B, LOW);
              mtimerState[curr_prog - 1] = true;
              mtimerChrono[curr_prog - 1].restart();
            }
            else if (valve == 1)
            {
              // digitalWrite(valve_2A, HIGH);
              // digitalWrite(valve_2B, LOW);
              // delay(100);
              // digitalWrite(valve_2A, LOW);
              // digitalWrite(valve_2B, LOW);
              //digitalWrite(PB11, LOW);
              mtimerState[curr_prog - 1] = true;
              mtimerChrono[curr_prog - 1].restart();
            }
            else if (valve == 2)
            {
              digitalWrite(valve_3A, HIGH);
              digitalWrite(valve_3B, LOW);
              delay(100);
              digitalWrite(valve_3A, LOW);
              digitalWrite(valve_3B, LOW);
              mtimerState[curr_prog - 1] = true;
              mtimerChrono[curr_prog - 1].restart();
            }
            else if (valve == 3)
            {
              digitalWrite(valve_4A, HIGH);
              digitalWrite(valve_4B, LOW);
              delay(100);
              digitalWrite(valve_4A, LOW);
              digitalWrite(valve_4B, LOW);
              mtimerState[curr_prog - 1] = true;
              mtimerChrono[curr_prog - 1].restart();
            }
            else if (valve == 4)
            {
              for (int i = 0; i < sizeof(valve_A); i++)
              {
                digitalWrite(valve_A[i], HIGH);
                digitalWrite(valve_B[i], LOW);
              }
              delay(100); // clarify actual delay time
              for (int i = 0; i < sizeof(valve_A); i++)
              {
                digitalWrite(valve_A[i], LOW);
                digitalWrite(valve_B[i], LOW);
              }
              mtimerState[curr_prog - 1] = true;
              mtimerChrono[curr_prog - 1].restart();
            }
          }
        }
      }
    }
    /***Below block stops irrigation when completed***/
    /*if (timerState == ENGAGED && timerChrono.hasPassed(Programs[curr_prog - 1].Runtime))
    {
      if (wOutlet == PUMP)
      {
        digitalWrite(pumpPin, LOW);
        digitalWrite(PC13, HIGH); //test
        //irrState = 0;             // test
        timerState = DISENGAGED;
      }
      else if (wOutlet == VALVE)
      {
        uint8_t valve = Programs[curr_prog - 1].Valve;
        char valve_A[4] = {valve_1A, valve_2A, valve_3A, valve_4A};
        char valve_B[4] = {valve_1B, valve_2B, valve_3B, valve_4B};
        if (valve == 0)
        {
          digitalWrite(valve_1A, LOW);
          digitalWrite(valve_1B, HIGH);
          delay(100);
          digitalWrite(valve_1A, LOW);
          digitalWrite(valve_1B, LOW);
          timerState = DISENGAGED;
        }
        else if (valve == 1)
        {
          digitalWrite(valve_2A, LOW);
          digitalWrite(valve_2B, HIGH);
          delay(100);
          digitalWrite(valve_2A, LOW);
          digitalWrite(valve_2B, LOW);
          timerState = DISENGAGED;
        }
        else if (valve == 2)
        {
          digitalWrite(valve_3A, LOW);
          digitalWrite(valve_3B, HIGH);
          delay(100);
          digitalWrite(valve_3A, LOW);
          digitalWrite(valve_3B, LOW);
          timerState = DISENGAGED;
        }
        else if (valve == 3)
        {
          digitalWrite(valve_4A, LOW);
          digitalWrite(valve_4B, HIGH);
          delay(100);
          digitalWrite(valve_4A, LOW);
          digitalWrite(valve_4B, LOW);
          timerState = DISENGAGED;
        }
        else if (valve == 4)
        {
          for (int i = 0; i < sizeof(valve_A); i++)
          {
            digitalWrite(valve_A[i], LOW);
            digitalWrite(valve_B[i], HIGH);
          }
          delay(100); // clarify actual delay time
          for (int i = 0; i < sizeof(valve_A); i++)
          {
            digitalWrite(valve_A[i], LOW);
            digitalWrite(valve_B[i], LOW);
          }
          timerState = DISENGAGED;
        }
      }
    }*/
    /**Irrigation for clashing times**/
    if (next_prog == 13)
    {
      if ((fYr(&Programs[clashProgs[0]]) == rtc.getYear()) && (fMth(&Programs[clashProgs[0]]) == rtc.getMonth()) && (fDay(&Programs[clashProgs[0]]) == rtc.getDay()) && (starthour(&Programs[clashProgs[0]]) == rtc.getHours()) && (startmin(&Programs[clashProgs[0]]) == rtc.getMinutes()))
      {
        for (int i = 0; i < clashCount; i++)
        {
          futureTime(Programs[clashProgs[i]].Interval); //calculate future time
          /***store the next irrigation date***/
          Programs[clashProgs[i]].futureDay = All_Str.tim.futureDay;
          Programs[clashProgs[i]].futureMonth = All_Str.tim.futureMonth;
          Programs[clashProgs[i]].futureYr = All_Str.tim.futureYr;
        }
        EEPROM.put(7 * sizeof(APN), Programs);
        next_irr_flag = true;
        curr_clash_count = clashCount;
        for (int i = 0; i < curr_clash_count; i++)
        {
          curr_clash_progs[i] = clashProgs[i];
        }
        if (rSense == ON)
        {
          int sensor_val = readSoilHumidity();
          if (sensor_val < All_Str.hum)
          { // if it has not rained
            if (wOutlet == VALVE)
            { //valve irrigation
              irr_valve = RUNNING;
              for (int i = 0; i < curr_clash_count; i++)
              {
                uint8_t valve = Programs[curr_clash_progs[i]].Valve; // get valve setting
                if (valve == 0)
                { //valve 1
                  // digitalWrite(valve_1A, HIGH);
                  // digitalWrite(valve_1B, LOW);
                  // delay(100);
                  // digitalWrite(valve_1A, LOW);
                  // digitalWrite(valve_1B, LOW);
                  digitalWrite(valve_1B, LOW);
                  mtimerState[curr_clash_progs[i]] = true;
                  mtimerChrono[curr_clash_progs[i]].restart();
                }
                else if (valve == 1)
                { //valve 2
                  // digitalWrite(valve_2A, HIGH);
                  // digitalWrite(valve_2B, LOW);
                  // delay(100);
                  // digitalWrite(valve_2A, LOW);
                  // digitalWrite(valve_2B, LOW);
                  //digitalWrite(PB11, LOW);
                  mtimerState[curr_clash_progs[i]] = true;
                  mtimerChrono[curr_clash_progs[i]].restart();
                }
                else if (valve == 2)
                { //valve 3
                  digitalWrite(valve_3A, HIGH);
                  digitalWrite(valve_3B, LOW);
                  delay(100);
                  digitalWrite(valve_3A, LOW);
                  digitalWrite(valve_3B, LOW);
                  mtimerState[curr_clash_progs[i]] = true;
                  mtimerChrono[curr_clash_progs[i]].restart();
                }
                else if (valve == 3)
                { //valve 4
                  digitalWrite(valve_4A, HIGH);
                  digitalWrite(valve_4B, LOW);
                  delay(100);
                  digitalWrite(valve_4A, LOW);
                  digitalWrite(valve_4B, LOW);
                  mtimerState[curr_clash_progs[i]] = true;
                  mtimerChrono[curr_clash_progs[i]].restart();
                }
              }
            }
          }
        }
        else
        {
          if (wOutlet == VALVE)
          { //valve irrigation
          irr_valve = RUNNING;
            for (int i = 0; i < curr_clash_count; i++)
            {
              uint8_t valve = Programs[curr_clash_progs[i]].Valve; // get valve setting
              if (valve == 0)
              { //valve 1
                // digitalWrite(valve_1A, HIGH);
                // digitalWrite(valve_1B, LOW);
                // delay(100);
                // digitalWrite(valve_1A, LOW);
                // digitalWrite(valve_1B, LOW);
                digitalWrite(valve_1B, LOW);
                mtimerState[curr_clash_progs[i]] = true;
                mtimerChrono[curr_clash_progs[i]].restart();
              }
              else if (valve == 1)
              { //valve 2
                // digitalWrite(valve_2A, HIGH);
                // digitalWrite(valve_2B, LOW);
                // delay(100);
                // digitalWrite(valve_2A, LOW);
                // digitalWrite(valve_2B, LOW);
                //digitalWrite(PB11, LOW);
                mtimerState[curr_clash_progs[i]] = true;
                mtimerChrono[curr_clash_progs[i]].restart();
              }
              else if (valve == 2)
              { //valve 3
                digitalWrite(valve_3A, HIGH);
                digitalWrite(valve_3B, LOW);
                delay(100);
                digitalWrite(valve_3A, LOW);
                digitalWrite(valve_3B, LOW);
                mtimerState[curr_clash_progs[i]] = true;
                mtimerChrono[curr_clash_progs[i]].restart();
              }
              else if (valve == 3)
              { //valve 4
                digitalWrite(valve_4A, HIGH);
                digitalWrite(valve_4B, LOW);
                delay(100);
                digitalWrite(valve_4A, LOW);
                digitalWrite(valve_4B, LOW);
                mtimerState[curr_clash_progs[i]] = true;
                mtimerChrono[curr_clash_progs[i]].restart();
              }
            }
          }
        }
      }
    }
    /*Stop irrigation*/
    for (int i = 0; i < PROG_SIZE; i++)
    {
      if (mtimerState[i] == true && mtimerChrono[i].hasPassed(Programs[i].Runtime * 60000))
      {
        if (wOutlet == PUMP)
        {
          digitalWrite(pumpPin, LOW);
          digitalWrite(PC13, HIGH); //test
          //irrState = 0;             // test
          mtimerState[i] = false;
        }
        else if (wOutlet == VALVE)
        {
          irr_valve = SHUTOFF;
          uint8_t valve = Programs[i].Valve;
          char valve_A[4] = {valve_1A, valve_2A, valve_3A, valve_4A};
          char valve_B[4] = {valve_1B, valve_2B, valve_3B, valve_4B};
          if (valve == 0)
          {
            // digitalWrite(valve_1A, LOW);
            // digitalWrite(valve_1B, HIGH);
            // delay(100);
            // digitalWrite(valve_1A, LOW);
            // digitalWrite(valve_1B, LOW);
            digitalWrite(valve_1B, HIGH);
            mtimerState[i] = false;
          }
          else if (valve == 1)
          {
            // digitalWrite(valve_2A, LOW);
            // digitalWrite(valve_2B, HIGH);
            // delay(100);
            // digitalWrite(valve_2A, LOW);
            // digitalWrite(valve_2B, LOW);
            //digitalWrite(PB11, HIGH);
            mtimerState[i] = false;
          }
          else if (valve == 2)
          {
            digitalWrite(valve_3A, LOW);
            digitalWrite(valve_3B, HIGH);
            delay(100);
            digitalWrite(valve_3A, LOW);
            digitalWrite(valve_3B, LOW);
            mtimerState[i] = false;
          }
          else if (valve == 3)
          {
            digitalWrite(valve_4A, LOW);
            digitalWrite(valve_4B, HIGH);
            delay(100);
            digitalWrite(valve_4A, LOW);
            digitalWrite(valve_4B, LOW);
            mtimerState[i] = false;
          }
          else if (valve == 4)
          {
            for (int i = 0; i < sizeof(valve_A); i++)
            {
              digitalWrite(valve_A[i], LOW);
              digitalWrite(valve_B[i], HIGH);
            }
            delay(100); // clarify actual delay time
            for (int i = 0; i < sizeof(valve_A); i++)
            {
              digitalWrite(valve_A[i], LOW);
              digitalWrite(valve_B[i], LOW);
            }
            mtimerState[i] = false;
          }
        }
      }
    }
    // for (int i = 0; i < curr_clash_count; i++)
    // {
    //   if (mtimerState[i] == true && mtimerChrono[i].hasPassed(Programs[curr_clash_progs[i]].Runtime))
    //   {
    //     uint8_t valve = Programs[curr_clash_progs[i]].Valve;
    //     if (valve == 0)
    //     {
    //       digitalWrite(valve_1A, LOW);
    //       digitalWrite(valve_1B, HIGH);
    //       delay(100);
    //       digitalWrite(valve_1A, LOW);
    //       digitalWrite(valve_1B, LOW);
    //       mtimerState[i] = false;
    //     }
    //     else if (valve == 1)
    //     {
    //       digitalWrite(valve_2A, LOW);
    //       digitalWrite(valve_2B, HIGH);
    //       delay(100);
    //       digitalWrite(valve_2A, LOW);
    //       digitalWrite(valve_2B, LOW);
    //       mtimerState[i] = false;
    //     }
    //     else if (valve == 2)
    //     {
    //       digitalWrite(valve_3A, LOW);
    //       digitalWrite(valve_3B, HIGH);
    //       delay(100);
    //       digitalWrite(valve_3A, LOW);
    //       digitalWrite(valve_3B, LOW);
    //       mtimerState[i] = false;
    //     }
    //     else if (valve == 3)
    //     {
    //       digitalWrite(valve_4A, LOW);
    //       digitalWrite(valve_4B, HIGH);
    //       delay(100);
    //       digitalWrite(valve_4A, LOW);
    //       digitalWrite(valve_4B, LOW);
    //       mtimerState[i] = false;
    //     }
    //   }
    // }
  }
}

// void manual_irrigate(){
//   if(irr_mode == MANUAL)
//   {
//     if(w)
//   }
// }
#line 1 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\Menu.ino"
void menu_controls()
{
  /*
     menu code below
  */
  if (Debounce(&buttons[2]) == HIGH && page == HOME)
    page = MENU;
  if (page == MENU)
  {
    // struct Timer * all;
    uint8_t select = menufunc(7, 4, mainmenu, 0, cusor, All_Str);
    switch (select)
    {
    case 1:
      page = MODE;
      cusor = 1;
      break;
    case 2:
      page = CALENDAR;
      cusor = 1;
      break;
    case 3:
      page = OUTLET;
      cusor = 1;
      break;
    case 4:
      page = SENSOR;
      cusor = 1;
      break;
    case 5:
      page = NETWORK;
      cusor = 1;
      break;
    case 6:
      // RESET ALL TIMERS***
      irr_mode = MANUAL;
      EEPROM.put(0 * sizeof(irr_mode), irr_mode);
      All_Str.tim.Prog = 1;
      All_Str.tim.StartHr = 0;
      All_Str.tim.StartMin = 0;
      All_Str.tim.Runtime = 0;
      All_Str.tim.Interval = 1;
      page = HOME;
      cusor = 1;
      break;
    case 7:
      // EXIT MENU
      page = HOME;
      cusor = 1;
      break;
    default:
      cusor = select;
      break;
    }
  }
  else if (page == MODE)
  {
    uint8_t select = menufunc(4, 4, mode, 0, cusor, All_Str);
    switch (select)
    {
    case 1:
      page = CYCLE;
      cusor = 1;
      break;
    case 2:
      page = TIMER;
      cusor = 1;
      break;
    case 3:
      // SAVE MANUAL MODE HERE & EXIT
      if (irr_mode != MANUAL)
      {
        irr_mode = MANUAL;
        EEPROM.put(0 * sizeof(irr_mode), irr_mode);
      }
      page = HOME;
      cusor = 1;
      break;
    case 4:
      // RETURN TO PREVIOUS MENU
      page = MENU;
      cusor = 1;
      break;
    default:
      cusor = select;
      break;
    }
  }
  else if (page == CYCLE)
  {
    uint8_t select = menufunc(4, 4, cycle, 0, cusor, All_Str);
    switch (select)
    {
    case 3:
      // set: save & exit cycle**
      if (irr_mode != LOOP)
      {
        irr_mode = LOOP;
        EEPROM.put(0 * sizeof(irr_mode), irr_mode);
      }
      // const uint8_t offset = 2;
      EEPROM.put(2 * sizeof(All_Str.cyc), All_Str.cyc);
      firstRun = true;
      loopState = REST;
      page = HOME;
      cusor = 1;
      break;
    case 4:
      // back to previous menu
      page = MODE;
      cusor = 1;
      break;
    case 10:
      // set All_Str.tim.Runtime increment
      cusor = select / 10;
      if (All_Str.cyc.Run >= 10)
        All_Str.cyc.Run += 10;
      else
        All_Str.cyc.Run++;
      if (All_Str.cyc.Run > 100)
        All_Str.cyc.Run = 1;
      break;
    case 100:
      // set All_Str.tim.Runtime decrement
      cusor = select / 100;
      if (All_Str.cyc.Run <= 10)
        All_Str.cyc.Run--;
      else
        All_Str.cyc.Run -= 10;
      if (All_Str.cyc.Run < 0)
        All_Str.cyc.Run = 100;
      break;
    case 20:
      // set resttime increment
      cusor = select / 10;
      if (All_Str.cyc.Rest >= 10)
        All_Str.cyc.Rest += 10;
      else
        All_Str.cyc.Rest++;
      if (All_Str.cyc.Rest > 100)
        All_Str.cyc.Rest = 1;
      break;
    case 200:
      // set resttime decrement
      cusor = select / 100;
      if (All_Str.cyc.Rest <= 10)
        All_Str.cyc.Rest--;
      else
        All_Str.cyc.Rest -= 10;
      if (All_Str.cyc.Rest < 0)
        All_Str.cyc.Rest = 100;
      break;
    default:
      cusor = select;
      break;
    }
  }
  else if (page == TIMER)
  {
    uint8_t startdate = 0;
    static bool EEPROM_FLAG = false;
    //static struct Timer Programs[PROG_SIZE];//test
    //EEPROM.get(5 * sizeof(All_Str.prog_count), Programs);
    // static uint8_t prog_count;
    if (irr_mode != PROGS)
      All_Str.prog_count = 0;
    uint16_t select = menufunc(9, 4, timer, 0, cusor, All_Str);
    switch (select)
    {
    case 7:
      // SET: SAVE CURRENT TIMER
      // DO ANALYSIS OF STRUCT HERE
      if (irr_mode != PROGS)
      { // save irrigation mode only once
        irr_mode = PROGS;
        EEPROM.put(0 * sizeof(irr_mode), irr_mode);
      }

      if (All_Str.tim.Prog > All_Str.prog_count)
      { // keep record of number of Programs set
        All_Str.prog_count = All_Str.tim.Prog;
        EEPROM.put(4 * sizeof(All_Str.prog_count), All_Str.prog_count);
      }
      /*
               below block of if statement is to handle the exception of setting irrigation time that has passed
               if the set time has elapsed, it automatically postpones the irrigation to the next day
        */
      if ((All_Str.tim.StartHr < rtc.getHours()) || (All_Str.tim.StartHr == rtc.getHours() && All_Str.tim.StartMin < rtc.getMinutes()))
        startdate = 1;

      futureTime(startdate);                          //get the day to start irrigation. Normally starts today calc future day, month and year
      Programs[(All_Str.tim.Prog - 1)] = All_Str.tim; //add new program to struct array
      EEPROM_FLAG = true;
      cusor = 1;
      break;
    case 8:
      // COMMIT & EXIT TO HOME
      if (EEPROM_FLAG == true)
      {
        EEPROM_FLAG = false;
        EEPROM.put(7 * sizeof(APN), Programs);
        // emptystruct();
        // EEPROM.get(5 * sizeof(All_Str.prog_count), Programs);
        next_irr_flag = true; //test line
      }
      page = HOME;
      cusor = 1;
      break;
    case 9:
      // back to previous menu
      page = MODE;
      cusor = 1;
      break;
    case 10:
      // PROG increment
      cusor = select / 10;
      All_Str.tim.Prog++;
      if (All_Str.tim.Prog > 12)
        All_Str.tim.Prog = 1;
      break;
    case 100:
      // PROG decrement
      cusor = select / 100;
      All_Str.tim.Prog--;
      if (All_Str.tim.Prog < 1)
        All_Str.tim.Prog = 12;
      break;
    case 20:
      // START HOUR increment
      cusor = select / 10;
      All_Str.tim.StartHr++;
      if (All_Str.tim.StartHr > 23)
        All_Str.tim.StartHr = 0;
      break;
    case 200:
      // START HOUR decrement
      cusor = select / 100;
      All_Str.tim.StartHr--;
      if (All_Str.tim.StartHr < 0)
        All_Str.tim.StartHr = 23;
      break;
    case 30:
      // START MINUTE increment
      cusor = select / 10;
      All_Str.tim.StartMin++;
      if (All_Str.tim.StartMin > 59)
        All_Str.tim.StartMin = 0;
      break;
    case 300:
      // START MINUTE decrement
      cusor = select / 100;
      All_Str.tim.StartMin--;
      if (All_Str.tim.StartMin < 0)
        All_Str.tim.StartMin = 59;
      break;
    case 40:
      // DURATION increment
      cusor = select / 10;
      if (All_Str.tim.Runtime >= 10)
        All_Str.tim.Runtime += 5;
      else
        All_Str.tim.Runtime++;
      if (All_Str.tim.Runtime > 720)
        All_Str.tim.Runtime = 0;
      break;
    case 400:
      // DURATION decrement
      cusor = select / 100;
      if (All_Str.tim.Runtime <= 10)
        All_Str.tim.Runtime--;
      else
        All_Str.tim.Runtime -= 5;
      if (All_Str.tim.Runtime < 0)
        All_Str.tim.Runtime = 720;
      break;
    case 50:
      // INTERVAL increment
      cusor = select / 10;
      All_Str.tim.Interval++;
      if (All_Str.tim.Interval > 7)
        All_Str.tim.Interval = 1;
      break;
    case 500:
      // INTERVAL decrement
      cusor = select / 100;
      All_Str.tim.Interval--;
      if (All_Str.tim.Interval < 1)
        All_Str.tim.Interval = 7;
      break;
    case 60:
      // VALVE increment
      cusor = select / 10;
      All_Str.tim.Valve++;
      if (All_Str.tim.Valve > 4)
        All_Str.tim.Valve = 0;
      break;
    case 600:
      // VALVE decrement
      cusor = select / 100;
      All_Str.tim.Valve--;
      if (All_Str.tim.Valve < 0)
        All_Str.tim.Valve = 4;
      break;
    default:
      cusor = select;
      break;
    }
  }
  else if (page == CALENDAR)
  {
    uint8_t select = menufunc(3, 4, calendar, 0, cusor, All_Str);
    switch (select)
    {
    case 1:
      page = DATE;
      cusor = 1;
      break;
    case 2:
      page = TIME;
      cusor = 1;
      break;
    case 3:
      // EXIT
      page = HOME;
      cusor = 1;
      break;
    default:
      cusor = select;
      break;
    }
  }
  else if (page == DATE)
  {
    int select = menufunc(3, 3, setdate, 1, cusor, All_Str);
    switch (select)
    {
    case 1:
      // SET: SAVE CURRENT DATE
      rtc.setDate(All_Str.cal.DY, All_Str.cal.MTH, All_Str.cal.YR);
      page = CALENDAR;
      cusor = 1;
      break;
    case 2:
      // SET: SAVE CURRENT DATE
      rtc.setDate(All_Str.cal.DY, All_Str.cal.MTH, All_Str.cal.YR);
      page = CALENDAR;
      cusor = 1;
      break;
    case 3:
      // SET: SAVE CURRENT DATE
      rtc.setDate(All_Str.cal.DY, All_Str.cal.MTH, All_Str.cal.YR);
      page = CALENDAR;
      cusor = 1;
      break;
    case 100:
      // page = 2;
      cusor = select / 100;
      All_Str.cal.YR++;
      break;
    case 10:
      // page = 2;
      cusor = select / 10;
      All_Str.cal.YR--;
      if (All_Str.cal.YR < 22)
        All_Str.cal.YR = 22;
      break;
    case 200:
      // page = 2;
      cusor = select / 100;
      All_Str.cal.MTH++;
      if (All_Str.cal.MTH > 12)
        All_Str.cal.MTH = 1;
      break;
    case 20:
      // page = 2;
      cusor = select / 10;
      All_Str.cal.MTH--;
      if (All_Str.cal.MTH < 1)
        All_Str.cal.MTH = 12;
      break;
    case 300:
      // page = 2;
      cusor = select / 100;
      All_Str.cal.DY++;
      if (All_Str.cal.DY > 31)
        All_Str.cal.DY = 1;
      break;
    case 30:
      // page = 2;
      cusor = select / 10;
      All_Str.cal.DY--;
      if (All_Str.cal.DY < 1)
        All_Str.cal.DY = 31;
      break;
    default:
      cusor = select;
      break;
    }
  }
  else if (page == TIME)
  {
    //    static uint8_t All_Str.cal.HR = 20, All_Str.cal.MIN = 1, All_Str.cal.SEC = 1;
    uint16_t select = menufunc(3, 3, settime, 1, cusor, All_Str);
    switch (select)
    {
    case 1:
      // SET: SAVE CURRENT TIME
      rtc.setTime(All_Str.cal.HR, All_Str.cal.MIN, All_Str.cal.SEC);
      page = CALENDAR;
      cusor = 1;
      break;
    case 2:
      // SET: SAVE CURRENT TIME
      rtc.setTime(All_Str.cal.HR, All_Str.cal.MIN, All_Str.cal.SEC);
      page = CALENDAR;
      cusor = 1;
      break;
    case 3:
      // SET: SAVE CURRENT TIME
      rtc.setTime(All_Str.cal.HR, All_Str.cal.MIN, All_Str.cal.SEC);
      page = CALENDAR;
      cusor = 1;
      break;
    case 100:
      cusor = select / 100;
      All_Str.cal.HR++;
      if (All_Str.cal.HR > 23)
        All_Str.cal.HR = 0;
      break;
    case 10:
      cusor = select / 10;
      All_Str.cal.HR--;
      if (All_Str.cal.HR < 0)
        All_Str.cal.HR = 23;
      break;
    case 200:
      // page = 2;
      cusor = select / 100;
      All_Str.cal.MIN++;
      if (All_Str.cal.MIN > 59)
        All_Str.cal.MIN = 0;
      break;
    case 20:
      // page = 2;
      cusor = select / 10;
      All_Str.cal.MIN--;
      if (All_Str.cal.MIN < 0)
        All_Str.cal.MIN = 59;
      break;
    case 300:
      // page = 2;
      cusor = select / 100;
      All_Str.cal.SEC++;
      if (All_Str.cal.SEC > 59)
        All_Str.cal.SEC = 0;
      break;
    case 30:
      // page = 2;
      cusor = select / 10;
      All_Str.cal.SEC--;
      if (All_Str.cal.SEC < 0)
        All_Str.cal.SEC = 59;
      break;
    default:
      cusor = select;
      break;
    }
  }
  else if (page == OUTLET)
  {
    uint8_t select = menufunc(3, 3, outlet, 0, cusor, All_Str);
    switch (select)
    {
    case 1:
      // SET: SAVE VALVE***
      wOutlet = VALVE;
      EEPROM.put(1 * sizeof(wOutlet), wOutlet);
      page = HOME;
      cusor = 1;
      break;
    case 2:
      // SET: SAVE PUMP***
      wOutlet = PUMP;
      EEPROM.put(1 * sizeof(wOutlet), wOutlet);
      page = HOME;
      cusor = 1;
      break;
    case 3:
      // back to previous menu
      page = MENU;
      cusor = 1;
      break;
    default:
      cusor = select;
      break;
    }
  }
  else if (page == SENSOR)
  {
    uint8_t select = menufunc(3, 3, sensor, 0, cusor, All_Str);
    switch (select)
    {
    case 1:
      // GO TO HUMIDITY page
      page = HUMIDITY;
      cusor = 1;
      break;
    case 2:
      // SET: SAVE SENSOR OFF***
      rSense = OFF;
      EEPROM.put(2 * sizeof(rSense), rSense);
      page = HOME;
      cusor = 1;
      break;
    case 3:
      // back to previous menu
      page = MENU;
      cusor = 1;
      break;
    default:
      cusor = select;
      break;
    }
  }
  else if (page == HUMIDITY)
  {
    uint8_t select = menufunc(3, 3, humidity, 0, cusor, All_Str);
    switch (select)
    {
    case 2:
      // SET: SAVE AND EXIT**
      rSense = ON;
      EEPROM.put(2 * sizeof(rSense), rSense);
      EEPROM.put(2 * sizeof(All_Str.hum), All_Str.hum);
      page = HOME;
      cusor = 1;
      break;
    case 3:
      // back to previous menu
      page = SENSOR;
      cusor = 1;
      break;
    case 10:
      // setpoint increment
      cusor = select / 10;
      All_Str.hum += 5;
      if (All_Str.hum > 100)
        All_Str.hum = 0;
      break;
    case 100:
      // setpoint decrement
      cusor = select / 100;
      All_Str.hum -= 5;
      if (All_Str.hum < 0)
        All_Str.hum = 100;
      break;
    default:
      cusor = select;
      break;
    }
  }
  else if (page == NETWORK)
  {
    uint8_t select = menufunc(4, 4, network, 0, cusor, All_Str);
    switch (select)
    {
    case 1:
      // SET: SAVE NETWORK SMS***
      gsmMode = SMS;
      EEPROM.put(3 * sizeof(gsmMode), gsmMode);
      page = HOME;
      cusor = 1;
      break;
    case 2:
      // SET: SAVE NETWORK GPRS***
      gsmMode = GPRS;
      EEPROM.put(3 * sizeof(gsmMode), gsmMode);
      page = HOME;
      cusor = 1;
      break;
    case 3:
      // SET: SAVE NETWORK OFF***
      gsmMode = NaN;
      EEPROM.put(3 * sizeof(gsmMode), gsmMode);
      page = HOME;
      cusor = 1;
      break;
      case 4:
      // back to previous menu
      page = MENU;
      cusor = 1;
      break;
    default:
      cusor = select;
      break;
    }
  }
  // return page;
}

#line 1 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\Pumpwater.ino"
void pumpwater()
{
    if (wOutlet == VALVE)
    {
        if ((digitalRead(FS1) == LOW || digitalRead(FS2) == LOW) && pumpState == PUMP_OFF)
        {
            pumpState = PUMP_ON;
            //EEPROM.put(13 * sizeof(pumpState), pumpState); // revise EEPROM ADDRESS
            digitalWrite(pumpPin, HIGH);
            digitalWrite(PC13, LOW); //test
        }
        if (digitalRead(FS1) == HIGH && digitalRead(FS2) == HIGH && pumpState == PUMP_ON)
        {
            pumpState = PUMP_OFF;
            //EEPROM.put(13 * sizeof(pumpState), pumpState);
            digitalWrite(pumpPin, LOW);
            digitalWrite(PC13, HIGH);//TEST
        }
        // if (pumpState == PUMP_ON && pump_init_flag == true)
        // {
        //     pump_init_flag = false;
        //     digitalWrite(pumpPin, HIGH);
        //     digitalWrite(PC13, LOW); //test
        // }
    }
}

#line 1 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\Timer_analysis.ino"
//next_irr_flag = true
uint8_t fDay(struct Timer *timers)
{
  return (*timers).futureDay;
}
uint8_t fMth(struct Timer *timers)
{
  return (*timers).futureMonth;
}
uint8_t fYr(struct Timer *timers)
{
  return (*timers).futureYr;
}
int starthour(struct Timer *timers)
{
  return (*timers).StartHr;
}
int startmin(struct Timer *timers)
{
  return (*timers).StartMin;
}
int runtime(struct Timer *timers)
{
  return (*timers).Runtime;
}
int frequency(struct Timer *timers)
{
  return (*timers).Interval;
}
int valves(struct Timer *timers)
{
  return (*timers).Valve;
}
uint8_t next_irrigation()
{
  const uint8_t CLASH = 13;
  int fYr_store[All_Str.prog_count];
  int fMth_store[All_Str.prog_count];
  int fDay_store[All_Str.prog_count];
  int all_prog_pos[All_Str.prog_count];
  int corr_yr_progs[All_Str.prog_count];
  int corr_mth_progs[All_Str.prog_count];
  int corr_day_progs[All_Str.prog_count];
  int corr_starthr_progs[All_Str.prog_count];
  int corr_startmin_progs[All_Str.prog_count];
  // int corr_starthr_addr[All_Str.prog_count];
  // int corr_startmin_addr[All_Str.prog_count];
  int starthr_store[All_Str.prog_count]; //
  int startmin_store[All_Str.prog_count];
  //unsigned long nearest_epoch = 0UL;
  int nearest_hour = 0, nearest_min = 0;
  uint8_t nearest_yr = 0, nearest_mth = 0, nearest_day = 0, yrcount = 0, hrcount = 0, mthcount = 0, daycount = 0, progpos = 0, mincount = 0, occured_at = 0;

  // struct Timer Programs[PROG_SIZE];
  // EEPROM.get(5 * sizeof(All_Str.prog_count), Programs);
  if (All_Str.prog_count > 1)
  {
    /****get all future years****/
    for (int i = 0; i < PROG_SIZE; i++)
    { //go through all possible Programs
      if (fYr(&Programs[i]) >= 22)
      {                                         // pick Programs set
        fYr_store[progpos] = fYr(&Programs[i]); // store the program years
        all_prog_pos[progpos] = i;              //store the program positions in array
        progpos++;
      }
    }
    /*****get nearest year*****/
    for (int i = 0; i < All_Str.prog_count; i++)
    {
      if (i == 0)
      {
        nearest_yr = fYr_store[i];
        // corr_prog = i + 1;
      }
      else
      {
        if (fYr_store[i] < nearest_yr)
        {
          nearest_yr = fYr_store[i];
          // corr_prog = i + 1;
        }
      }
    }
    // if(nearest_yr == 0)//test
    //   irrState = 1;
    /*****get number of occurences of nearest year*****/
    for (int i = 0; i < All_Str.prog_count; i++)
    {
      if (nearest_yr == fYr_store[i])
      {
        occured_at = all_prog_pos[i] + 1;
        corr_yr_progs[yrcount] = all_prog_pos[i]; // from all program pos get all corresponding Programs where nearest year occured
        yrcount++;                                //print for test
      }
    }
    // return yrcount; //test print
    /*****return corresponding program (not array position) if it occured only once*****/
    if (yrcount == 1)
    {
      // corr_prog = occured_at;
      return occured_at;
    }
    else
    {
      /*****get all months within the nearest year*****/
      for (int i = 0; i < yrcount; i++)
      {
        int pos = corr_yr_progs[i];
        fMth_store[i] = fMth(&Programs[pos]);
      }
      /*****get nearest month*****/
      for (int i = 0; i < yrcount; i++)
      {
        if (i == 0)
        {
          nearest_mth = fMth_store[i];
        }
        else
        {
          if (fMth_store[i] < nearest_mth)
          {
            nearest_mth = fMth_store[i];
          }
        }
      }
      /*****get number of occurences of nearest month*****/
      for (int i = 0; i < yrcount; i++)
      {
        if (nearest_mth == fMth_store[i])
        {
          occured_at = corr_yr_progs[i] + 1;
          corr_mth_progs[mthcount] = corr_yr_progs[i]; // from all program pos get all corresponding Programs where nearest year occured
          mthcount++;                                  //print for test
        }
      }
      // return mthcount; //test print
      /*****return corresponding program (not array position) if it occured only once*****/
      if (mthcount == 1)
      {
        // corr_prog = occured_at;
        return occured_at;
      }
      else
      {
        /*****get all days within the nearest month*****/
        for (int i = 0; i < mthcount; i++)
        {
          int pos = corr_mth_progs[i];
          fDay_store[i] = fDay(&Programs[pos]);
        }
        /*****get nearest day*****/
        for (int i = 0; i < mthcount; i++)
        {
          if (i == 0)
          {
            nearest_day = fDay_store[i];
          }
          else
          {
            if (fDay_store[i] < nearest_day)
            {
              nearest_day = fDay_store[i];
            }
          }
        }
        /*****get number of occurences of nearest day*****/
        for (int i = 0; i < mthcount; i++)
        {
          if (nearest_day == fDay_store[i])
          {
            occured_at = corr_mth_progs[i] + 1;
            corr_day_progs[daycount] = corr_mth_progs[i]; // from all program pos get all corresponding Programs where nearest day occured
            daycount++;                                   //print for test
          }
        }
        // return daycount; //test print
        /*****return corresponding program (not array position) if it occured only once*****/
        if (daycount == 1)
        {
          // corr_prog = occured_at;
          return occured_at;
        }
        else
        {
          /*****get all start hrs within the nearest day*****/
          for (int i = 0; i < daycount; i++)
          {
            int pos = corr_day_progs[i];
            starthr_store[i] = starthour(&Programs[pos]);
          }
          /*****get nearest hour*****/
          for (int i = 0; i < daycount; i++)
          {
            if (i == 0)
            {
              nearest_hour = starthr_store[i];
            }
            else
            {
              if (starthr_store[i] < nearest_hour)
              {
                nearest_hour = starthr_store[i];
              }
            }
          }
          /*****get number of occurences of nearest hour*****/
          for (int i = 0; i < daycount; i++)
          {
            if (nearest_hour == starthr_store[i])
            {
              occured_at = corr_day_progs[i] + 1;
              corr_starthr_progs[hrcount] = corr_day_progs[i]; // from all program pos get all corresponding Programs where nearest hour occured
              hrcount++;                                       //print for test
            }
          }
          // return hrcount; //test print
          /*****return corresponding program (not array position) if it occured only once*****/
          if (hrcount == 1)
          {
            // corr_prog = occured_at;
            return occured_at;
          }
          else
          {
            /*****get all start mins within the nearest hr*****/
            for (int i = 0; i < hrcount; i++)
            {
              int pos = corr_starthr_progs[i];
              startmin_store[i] = startmin(&Programs[pos]);
            }
            /*****get nearest min*****/
            for (int i = 0; i < hrcount; i++)
            {
              if (i == 0)
              {
                nearest_min = startmin_store[i];
              }
              else
              {
                if (startmin_store[i] < nearest_min)
                {
                  nearest_min = startmin_store[i];
                }
              }
            }
            /*****get number of occurences of nearest min*****/
            for (int i = 0; i < hrcount; i++)
            {
              if (nearest_min == startmin_store[i])
              {
                occured_at = corr_starthr_progs[i] + 1;
                corr_startmin_progs[mincount] = corr_starthr_progs[i]; // from all program pos get all corresponding Programs where nearest hour occured
                mincount++;                                            //print for test
              }
            }
            // return mincount; //test print
            /*****return corresponding program (not array position) if it occured only once*****/
            if (mincount == 1)
            {
              // corr_prog = occured_at;
              return occured_at;
            }
            else
            {
              /*****return a constant to let us know there is a clash*****/
              //irrState = 1;
              clashCount = mincount; // collect number of clashes
              for (int i = 0; i < mincount; i++)
              {
                clashProgs[i] = corr_startmin_progs[i]; //collect corresponding clashing programs
              }
              return CLASH;
            }
          }
        }
      }
    }
  }
  else
  {
    for (int i = 0; i < PROG_SIZE; i++)
    { //go through all possible Programs
      if (fYr(&Programs[i]) >= 22)
      {                                         // pick Programs that are set
        fYr_store[progpos] = fYr(&Programs[i]); // store the program years
        all_prog_pos[progpos] = i;              //store the program positions in array
        progpos++;
      }
    }
    /*****get nearest year*****/
    for (int i = 0; i < All_Str.prog_count; i++)
    {
      if (i == 0)
      {
        nearest_yr = fYr_store[i];
        occured_at = all_prog_pos[i] + 1;
      }
      else
      {
        if (fYr_store[i] < nearest_yr)
        {
          nearest_yr = fYr_store[i];
          occured_at = all_prog_pos[i] + 1;
        }
      }
    }
    return occured_at;
  }
}
// /*****get all epochs*****/
// for (int i = 0; i < All_Str.prog_count; i++) {
//   epoch_store[i] = epochs(&Programs[i]);
// }
// /*****get nearest epoch*****/
// for (int i = 0; i < All_Str.prog_count; i++) {
//   if (i == 0) {
//     nearest_epoch = epoch_store[i];
//     // corr_prog = i + 1;
//   }
//   else {
//     if (epoch_store[i] < nearest_epoch) {
//       nearest_epoch = epoch_store[i];
//       // corr_prog = i + 1;
//     }
//   }
// }
// /*****get number of occurences of nearest epoch*****/
// for (int i = 0; i < All_Str.prog_count; i++) {
//   if (nearest_epoch == epoch_store[i]) {
//     occured_at = i + 1;
//     corr_epoc_progs[count] = i;// get all corresponding Programs where it occured
//     count++;//print for test
//   }
// }
// //return count;//test value
// // return All_Str.prog_count;//test value
// /*****return corresponding program if it occured only once*****/
// if (count == 1) {
//   // corr_prog = occured_at;
//   return occured_at;
// }
// /*****if shortest epoch occured more than once*****/
// else {
//   /*****get all start hrs within the shortest epoch*****/
//   for (int i = 0; i < count; i++) {
//     int pos = corr_epoc_progs[i];
//     starthr_store[i] = starthour(&Programs[pos]);//print for test
//   }
//   /*****get nearest start hours*****/
//   for (int i = 0; i < count; i++) {
//     if (i == 0) {
//       nearest_hour = starthr_store[i];
//       occured_at = corr_epoc_progs[i] + 1;// corresponding program where the nearest hour occured
//     }
//     else {
//       if (starthr_store[i] < nearest_hour) {
//         nearest_hour = starthr_store[i];
//         occured_at = corr_epoc_progs[i] + 1;// corresponding program where the nearest hour occured
//       }
//     }
//   }
//   /*****get number of occurences of nearest start hour*****/
//   for (int i = 0; i < count; i++) {
//     if (nearest_hour == starthr_store[i]) {
//       // occured_at = i + 1;
//       corr_starthr_addr[hrcount] = i;// get all corresponding position where nearest hour was found in the store
//       hrcount++;
//     }
//   }
//   /*****return corresponding program if it occured only once*****/
//   if (hrcount == 1) {
//     return occured_at;
//   }
//   /*****if shortest start hour occured more than once*****/
//   else {
//     /*****loop through corresponding epoch Programs and get the Programs corresponding to start hour addresses*****/
//     uint8_t hrprogcount = 0;
//     for (int i = 0; i < hrcount; i++) {
//       for (int j = 0; j < count; j++) {
//         if (corr_starthr_addr[i] == j) {
//           corr_starthr_progs[hrprogcount] = corr_epoc_progs[j];
//           hrprogcount++;
//         }
//       }

//     }
//     /*****get all start mins within the shortest start hrs*****/
//     for (int i = 0; i < hrcount; i++) {
//       int pos = corr_starthr_progs[i];
//       startmin_store[i] = startmin(&Programs[pos]);
//     }
//     /*****get nearest start mins*****/
//     for (int i = 0; i < hrcount; i++) {
//       if (i == 0) {
//         nearest_min = startmin_store[i];
//         occured_at = corr_starthr_progs[i] + 1;
//       }
//       else {
//         if (startmin_store[i] < nearest_min) {
//           nearest_min = startmin_store[i];
//           occured_at = corr_starthr_progs[i] + 1;
//         }
//       }
//     }
//     /*****get number of occurences of nearest start mins*****/
//     for (int i = 0; i < hrcount; i++) {
//       if (nearest_min == startmin_store[i]) {
//         // occured_at = i + 1;
//         // return occured_at;
//         corr_startmin_addr[mincount] = i;// get all corresponding Programs where it occured
//         mincount++;
//       }
//     }
//     /*****return corresponding program if it occured only once*****/
//     if (mincount == 1) {
//       return occured_at;
//     }
//     else {
//       for (int i = 0; i < mincount; i++) {
//         for (int j = 0; j < hrcount; j++) {
//           if (corr_startmin_addr[i] == j) {
//             corr_startmin_progs[minprogcount] = corr_starthr_progs[j];//get the program addresses of Programs with the same time settings
//             minprogcount++;
//           }
//         }
//       }
//       return 13;
//     }
//   }
// }
//    count = 0;
//    for (int i = 0; i < All_Str.prog_count; i++) {
//      starthr_store[i] = 0;
//    }

#line 1 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\cycle.ino"
void cycle_irrigate()
{
    if (irr_mode == LOOP)
    {
        char valve_A[4] = {valve_1A, valve_2A, valve_3A, valve_4A};
        char valve_B[4] = {valve_1B, valve_2B, valve_3B, valve_4B};
        if (loopState == REST && (firstRun == true || restChrono.hasPassed(All_Str.cyc.Rest * 60)))
        {
            //start running
            if (wOutlet == VALVE)
            {
                for (int i = 0; i < sizeof(valve_A); i++)
                {
                    digitalWrite(valve_A[i], HIGH);
                    digitalWrite(valve_B[i], LOW);
                }
                delay(100); // clarify actual delay time
                for (int i = 0; i < sizeof(valve_A); i++)
                {
                    digitalWrite(valve_A[i], LOW);
                    digitalWrite(valve_B[i], LOW);
                }
            }
            else if (wOutlet == PUMP)
            {
                digitalWrite(pumpPin, HIGH);
                // digitalWrite (PC13, LOW);//test
            }
            runChrono.restart();
            loopState = RUN;
            firstRun = false;
        }
        if (loopState == RUN && runChrono.hasPassed(All_Str.cyc.Run * 60))
        {
            // stop running
            if (wOutlet == VALVE)
            {
                for (int i = 0; i < sizeof(valve_A); i++)
                {
                    digitalWrite(valve_A[i], LOW);
                    digitalWrite(valve_B[i], HIGH);
                }
                delay(100); // clarify actual delay time
                for (int i = 0; i < sizeof(valve_A); i++)
                {
                    digitalWrite(valve_A[i], LOW);
                    digitalWrite(valve_B[i], LOW);
                }
            }
            else if (wOutlet == PUMP)
            {
                digitalWrite(pumpPin, LOW);
                // digitalWrite (PC13, HIGH);//test
            }
            restChrono.restart();
            loopState = REST;
        }
    }
}
#line 1 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\eeprom.ino"
void readEEPROM()
{
  static bool initialised = false;
  if (!initialised)
  {
    pump_init_flag = true;
    EEPROM.get(0 * sizeof(irr_mode), irr_mode);
    EEPROM.get(1 * sizeof(wOutlet), wOutlet);
    EEPROM.get(2 * sizeof(rSense), rSense);
    EEPROM.get(3 * sizeof(gsmMode), gsmMode);
    EEPROM.get(4 * sizeof(All_Str.prog_count), All_Str.prog_count);
    if (irr_mode == LOOP)
    {
      EEPROM.get(2 * sizeof(All_Str.cyc), All_Str.cyc);
      firstRun = true;
      loopState = REST;
    }
    else if (irr_mode == PROGS)
    {
      EEPROM.get(7 * sizeof(APN), Programs);
      next_irr_flag = true;
    }
    EEPROM.get(2 * sizeof(All_Str.hum), All_Str.hum);
    // EEPROM.get(2 * sizeof(password), password);
    // EEPROM.get(3 * sizeof(phone), phone);
    // EEPROM.get(4 * sizeof(APN), APN);
    password = EEPROMread(2 * sizeof(password));
    phone = EEPROMread(3 * sizeof(phone));
    APN = EEPROMread(5 * sizeof(APN));
    //EEPROM.get(13 * sizeof(pumpState), pumpState);
  }
  initialised = true;
}
void EEPROMwrite(int addrOffset, const String &strToWrite)
{
  byte len = strToWrite.length();
  EEPROM.write(addrOffset, len);
  for (int i = 0; i < len; i++)
  {
    EEPROM.write(addrOffset + 1 + i, strToWrite[i]);
  }
}
String EEPROMread(int addrOffset)
{
  int newStrLen = EEPROM.read(addrOffset);
  char data[newStrLen + 1];
  for (int i = 0; i < newStrLen; i++)
  {
    data[i] = EEPROM.read(addrOffset + 1 + i);
  }
  data[newStrLen] = '\0'; // !!! NOTE !!! Remove the space between the slash "/" and "0" (I've added a space because otherwise there is a display bug)
  return String(data);
}

#line 1 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\sms.ino"
void sms()
{
  static bool errorFlag = 0, flag = 0, cycleFlag = 0;
  /**************** RESTART SIM800 ***************/
  if (simStatus == EROR)
  {
    errorFlag = 1;
    //static bool flag = false;
    static unsigned long erorMills = millis();
    if (!flag && millis() - erorMills >= 1000)
    {
      mySerial.println("AT+CFUN=0"); // turn on flight mode
      erorMills = millis();
      flag = true;
    }
    if (flag && millis() - erorMills >= 300)
    {
      mySerial.println("AT+CFUN=1"); // turn off flight mode
      erorMills = millis();
      flag = false;
    }
  }
  else if (simStatus == READY)
  {
    errorFlag = 0;
  }
  /**********  CHECK FOR SIMCARD  **************/
  if ((gsmMode == SMS || gsmMode == GPRS) && (errorFlag == 0 || flag == 0))
  {
    static unsigned long buff_time = millis();

    if ((millis() - buff_time >= 7000) /* && buff_check == true*/)
    {
      mySerial.println("AT+CPIN?");
      buff_time = millis();
    }
  }

  /**********  GET SIGNAL QUALITY  **************/
  if (simStatus == READY)
  {
    static unsigned long csq = millis();
    if ((millis() - csq >= 20000))
    {
      mySerial.println("AT+CSQ");
      csq = millis();
    }
  }

  /**********  CHECK SIM OWNER  **************/
  if (simStatus == READY && gsmMode == GPRS)
  {
    static bool netCheck = false;
    if (!netCheck)
    {
      mySerial.println("AT+COPS?");
    }
    netCheck = true;
  }

  /**********  GET NETWORK TIME  **************/
  static bool clkSave = false;
  if (simStatus == READY)
  {
    static bool clkCheck = false, synchSet = false;
    static unsigned long saveMills = millis();
    if (!synchSet)
    {
      mySerial.println("AT+CLTS=1");
      synchSet = true;
      saveMills = millis();
    }
    if (synchSet == true && clkSave == false && millis() - saveMills >= 100)
    {
      mySerial.println("AT&W");
      clkSave = true;
      saveMills = millis();
    }
    if (!clkCheck && clkSave == true && millis() - saveMills >= 100)
    {
      mySerial.println("AT+CCLK?");
      clkCheck = true;
      saveMills = 0UL;
    }
  }

  if (gsmMode == SMS && simStatus == READY && clkSave == true)
  {
    static bool config = false, del = false, tmode = false;
    static unsigned long configMills = millis();
    if (!tmode && millis() - configMills >= 15000)
    {
      mySerial.println("AT+CMGF=1");
      tmode = true;
      configMills = millis();
    }
    if (tmode == true && config == false && millis() - configMills >= 100)
    {
      mySerial.println("AT+CNMI=1,2,0,0,0");
      config = true;
      configMills = millis();
    }
    if (config == true && ((del == false && millis() - configMills >= 100) || millis() - configMills >= 86400000))
    {
      mySerial.println("AT+CMGDA=\"DEL ALL\"");
      del = true;
      configMills = millis();
    }
    //mySerial.flush();
  }

  recvSMS(); // PROCCESSES YOUR MESSAGEBODY
  if (messagestatus)
  { //messagestatus says when sms is recieved
    messageBody.toLowerCase();
    messageBody.replace(" ", "");
    messageBody.replace("\r", "");
    messageBody.replace("\n", "");
    homeScr = messageBody;
    //ySerial.println("SMS BODY FORMATTED:");
    //ySerial.println(messageBody); //actual message body

    /*****NORMAL SETTINGS MODE*****/
    if (messageBody.startsWith("pwd"))
    {
      String pass = getValue(getValue(messageBody, ':', 1), ',', 0);
      //homeScr = pass;
      // ySerial.println("PASSWORD:");
      // ySerial.println(pass);
      /*****VALID PASSWORD DETECTION*****/
      if (pass.length() == 4)
      {
        // char *pss = "4444";
        // EEPROM.get(7 * sizeof(pss), pss);
        //homeScr = password;
        // ySerial.println("PASSWORD RECEIVED");
        // ySerial.println(pass);
        // ySerial.println("PASSWORD IN MEMEORY");
        // ySerial.println(password);
        //compare password here
        if (pass == password)
        {
          // ySerial.println("PASSWORD VALID");

          //homeScr = "VALID";
          //  homeScr = getValue(messageBody, ',', 1);
          /*****MODE SELECTION*****/
          if (strstr(messageBody.c_str(), "mode") != NULL)
          {
           // ySerial.println("MODE FOUND");
            //ySerial.println(messageBody);
            // Serial.print("mode found: ");
            //homeScr = "MODE";

            /*****CYCLE IRRIGATION SETTINGS*****/
            if (strstr(messageBody.c_str(), "cycle") != NULL)
            {
             // ySerial.println("CYCLE FOUND");
              irr_mode = LOOP;
              EEPROM.put(0 * sizeof(irr_mode), irr_mode);
              if (strstr(messageBody.c_str(), "run") != NULL)
              {
                //collect run time
                String run = smsVal(messageBody, "run:", ',');
                uint16_t runn = run.toInt();
                if (runn > 0)
                {
                  All_Str.cyc.Run = runn;
                }
              }
              if (strstr(messageBody.c_str(), "rest") != NULL)
              {
                //collect rest time
                String rest = smsVal(messageBody, "rest:", ',');
                uint16_t rstt = rest.toInt();
                if (rstt > 0)
                {
                  All_Str.cyc.Rest = rstt;
                }
              }
              EEPROM.put(2 * sizeof(All_Str.cyc), All_Str.cyc);
              firstRun = true;
              loopState = REST;
              cycleFlag = true;
            }
            //   /*****TIMER IRRIGATION SETTINGS*****/
            //   if (strstr(messageBody.c_str(), "timer") != NULL)
            //   {
            //     Serial.println("timer");
            //     if (strstr(messageBody.c_str(), "prog") != NULL)
            //     {
            //       Serial.print("position: ");
            //       Serial.println(messageBody.indexOf("prog"));
            //       String prog = smsVal(messageBody, "prog:", ',');
            //       if (prog != "")
            //       {
            //         Serial.print("prog: ");
            //         Serial.println(prog);
            //         if (strstr(messageBody.c_str(), "hour") != NULL)
            //         {
            //           String hour = smsVal(messageBody, "hour:", ',');
            //           Serial.print("irrigation hour is: ");
            //           Serial.println(hour);
            //         }
            //         if (strstr(messageBody.c_str(), "min") != NULL)
            //         {
            //           String min = smsVal(messageBody, "min:", ',');
            //           Serial.print("irrigation minute is: ");
            //           Serial.println(min);
            //         }
            //         if (strstr(messageBody.c_str(), "duration") != NULL)
            //         {
            //           String duration = smsVal(messageBody, "duration:", ',');
            //           Serial.print("irrigation duration is: ");
            //           Serial.println(duration);
            //         }
            //         if (strstr(messageBody.c_str(), "interval") != NULL)
            //         {
            //           String interval = smsVal(messageBody, "interval:", ',');
            //           Serial.print("irrigation interval is: ");
            //           Serial.println(interval);
            //         }
            //         if (strstr(messageBody.c_str(), "valve") != NULL)
            //         {
            //           String valve = smsVal(messageBody, "valve:", ',');
            //           Serial.print("irrigation valve is: ");
            //           Serial.println(valve);
            //         }
            //       }
            //     }
            //   }

            //   /*****MANUAL IRRIGATION*****/
            //   if (strstr(messageBody.c_str(), "manual") != NULL)
            //   {
            //     Serial.println("manual");
            //     if (strstr(messageBody.c_str(), "start") != NULL)
            //     {
            //       Serial.println("irrigation started");
            //     }
            //     else if (strstr(messageBody.c_str(), "stop") != NULL)
            //     {
            //       Serial.println("irrigation stopped");
            //     }
            //   }
          }

          // /*****NETWORK SETTING*****/
          // if (strstr(messageBody.c_str(), "network") != NULL)
          // {
          //   Serial.print("network mode: ");
          //   if (strstr(messageBody.c_str(), "sms") != NULL)
          //   {
          //     Serial.println("sms");
          //   }
          //   else if (strstr(messageBody.c_str(), "gprs") != NULL)
          //   {
          //     Serial.println("gprs");
          //   }
          //   else if (strstr(messageBody.c_str(), "off") != NULL)
          //   {
          //     Serial.println("off");
          //   }
          // }

          /*****SYSTEM RESET*****/
          // if (strstr(messageBody.c_str(), "reset") != NULL)
          // {
          //   Serial.println("reset");
          // }
          // /********PHONE NUMBER UPDATE*********/
          // if (strstr(messageBody.c_str(), "phone") != NULL)
          // {
          //   // homeScr = "phone";
          //   String phone = smsVal(messageBody, "phone:", ',');
          //   // homeScr = phone.substring(5);
          //   //homeScr = phone;
          //   Serial.print("phone number: ");
          //   Serial.println(phone);
          // }
        }
      }
    }

    // /*****PASSWORD RESET*****/
    // else if (messageBody.startsWith("rstpwd"))
    // {
    //   Serial.println("reset password");
    //   if (strstr(messageBody.c_str(), "oldpwd") != NULL)
    //   {
    //     String oldpwd = smsVal(messageBody, "oldpwd:", ',');
    //     Serial.print("old password is: ");
    //     Serial.println(oldpwd);
    //     if (strstr(messageBody.c_str(), "newpwd") != NULL)
    //     {
    //       String newpwd = smsVal(messageBody, "newpwd:", ',');
    //       Serial.print("new password is: ");
    //       Serial.println(newpwd);
    //     }
    //   }
    // }
    messagestatus = false;
  }
  static bool sMode = 0, mMode = 0;
  static unsigned long sMills = 0UL;
  if(cycleFlag)
  {
    if(!sMode && !mMode)
    {
      mySerial.println("AT+CMGS=\"" + phone + "\"");
      sMills = millis();
      sMode = true;
    }

    if(sMode && !mMode && millis() - sMills >= 100)
    {
      mySerial.print("Cycle mode set to run for " + String(All_Str.cyc.Run) + " mins and rest for " + String(All_Str.cyc.Rest) + " mins."); //text content
      mMode = true;
      sMills = millis();
    }

    if(sMode && mMode && millis() - sMills >= 100)
    {
      mySerial.write(26);
      sMode = false;
      mMode = false;
      cycleFlag = false;
      sMills = millis() + 100000;
    }
      
  }
}

/***************** GSM DATA PROCESSOR FUNCTION ************************/
void recvSMS(void)
{
  // serialEvent();
  if (stringComplete)
  {
    homeScr = inputString;
   // ySerial.println(inputString);
    if (getValue(getValue(inputString, '+', 1), ':', 0) == "CMT")
    {
      messageBody = getValue(inputString, '\n', 2);
      messagestatus = true;
    }
    if ((getValue(getValue(inputString, '+', 1), '?', 0)) == "CCLK")
    {
      // Serial.println("NETWORK TIME:");
      // Serial.println(getValue(getValue(inputString, '"', 1), '+', 0)); // DATE AND TIME
      uint8_t yr = getValue(getValue(inputString, '"', 1), '/', 0).toInt();
      uint8_t mth = getValue(getValue(inputString, '/', 1), '/', 0).toInt();
      uint8_t dy = getValue(getValue(inputString, '/', 2), ',', 0).toInt();
      uint8_t hr = getValue(getValue(inputString, ',', 1), ':', 0).toInt();
      uint8_t mint = getValue(getValue(inputString, ':', 2), ':', 0).toInt();
      uint8_t secs = getValue(getValue(inputString, ':', 3), '+', 0).toInt();
      if (yr != rtc.getYear() || mth != rtc.getMonth() || dy != rtc.getDay() || hr != rtc.getHours() || mint != rtc.getMinutes())
      {
        rtc.setDate(dy, mth, yr);
        rtc.setTime(hr, mint, secs);
      }
    }
    if ((getValue(getValue(inputString, '+', 1), '?', 0)) == "COPS")
    {
      // Serial.println("SIM OWNER IDENTIFIED:");
      // Serial.println(getValue(getValue(inputString, '\n', 1), '\r', 0));
      // Serial.println("SIM IS :");
      // Serial.println(getValue(getValue(inputString, '"', 1), '"', 0));   //mtn nigeria
      String sim_name = getValue(getValue(inputString, '"', 1), '"', 0); //eg: mtn nigeria
      sim_name.toLowerCase();
      // String APN, USER, PASS;
      if (sim_name.startsWith("mtn"))
      {
        APN = "web.gprs.mtnnigeria.net";
        USER = "";
        PASS = "";
      }
      else if (sim_name.startsWith("glo"))
      {
        APN = "gloflat";
        USER = "flat";
        PASS = "flat";
      }
      else if (sim_name.startsWith("airtel"))
      {
        APN = "internet.ng.airtel.com";
        USER = "";
        PASS = "";
      }
      else if (sim_name.startsWith("9mobile"))
      {
        APN = "9mobile";
        USER = "";
        PASS = "";
      }
    }
    if ((getValue(getValue(inputString, '+', 1), '\r', 0)) == "CSQ")
    {
      //      Serial.println("received signal quality:");
      //      Serial.println(getValue(getValue(inputString, '\n', 1), '\r', 0));
      //      Serial.println("quality is :");
      //      Serial.println(getValue(getValue(inputString, ' ', 1), ',', 0));
      // Serial.print("integar: ");
      // Serial.println(getValue(getValue(inputString, ' ', 1), ',', 0).toInt()); //signal quality integar
      //convert to int here
      sigStrength = getValue(getValue(inputString, ' ', 1), ',', 0).toInt(); //signal quality integar
    }
    if ((getValue(getValue(inputString, '+', 1), '?', 0)) == "CREG")
    {
      //      Serial.println("creg found:");
      //      Serial.println(getValue(getValue(inputString, '\n', 1), '\r', 0));
      //homeScr = getValue(getValue(inputString, '\n', 1), '\r', 0);
      if ((getValue(getValue(inputString, ',', 1), '\r', 0)) == "1")
      {
        //homeScr = "REGISTERED";
        // Serial.println("sim registered");
        // Serial.println(getValue(getValue(inputString, ',', 1), '\n', 0));
        //        Serial.println("destiny");
        simStatus = READY;
        // csq = millis();
      }
    }
    if ((getValue(getValue(inputString, '+', 1), '?', 0)) == "CPIN")
    {
      //      Serial.println("received data correctly");
      //      Serial.println(getValue(getValue(inputString, '\n', 1), '\r', 0));
      homeScr = getValue(getValue(inputString, '\n', 1), '\r', 0);
      if ((getValue(getValue(inputString, '\n', 1), '\r', 0)) == "ERROR")
      {
        simStatus = EROR;
      }
      else if ((getValue(getValue(getValue(inputString, '\n', 1), ' ', 1), '\r', 0)) == "READY")
      {
        // Serial.println("cpin returns:");
        // Serial.println(getValue(getValue(getValue(inputString, '\n', 1), ' ', 1), '\r', 0));
        if (simStatus != READY)
        {
          simStatus = WAIT;
          mySerial.println("AT+CREG?");
        }
        //simStatus = READY;
      }
      //locationString = getValue(inputString, ':', 1);
    }
    inputString = "";
    stringComplete = false;
    //nxl = 0;
  }
}

/**************************************** DATA COLLECTION FUNCTIONS ******************************/
void serialEvent()
{
  while (mySerial.available())
  {
    collectSmsData((char)mySerial.read()); // get the new byte:
    //    Serial.print("received: ");
    //  Serial.println(mySerial.readString());
  }
}

void collectSmsData(char inChar)
{
  static uint8_t nxl = 0;
  //  // add it to the inputString:
  if (inChar == '\r')
  {
    nxl++;
  }
  if (inChar == '\r' && nxl >= 3)
  {
    stringComplete = true;
    nxl = 0;
    // Serial.println("complete sms: ");
    // Serial.println(inputString);
  }
  else
  {
    inputString += inChar;
  }
}

/************************ DATA PROCESSING FUNCTION ***************************/
/*****GET VALUE TO KEY*****/
String smsVal(String haystack, const char *needle, char seperator)
{
  haystack += seperator;
  //  int len = String(needle).length();
  //  int pos = haystack.indexOf(needle);
  int pos = haystack.indexOf(needle) + String(needle).length();
  for (int i = 1; i <= haystack.length(); i++)
  {
    if (haystack.charAt(pos + i) == seperator)
    {
      return haystack.substring(pos, pos + i);
    }
  }
  return "";
}

/****************** DATA FORMATING FUNCTION ***********************/
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++)
  {
    if (data.charAt(i) == separator || i == maxIndex)
    {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

#line 1 "c:\\Users\\HP\\OneDrive\\arduino\\iManagerV1\\soil_moisture.ino"
uint8_t readSoilHumidity()
{
  int readingsBucket[20];
  int acc = 0;
  analogReadResolution(12);
  for (int i = 0; i < 20; i++)
  {
    readingsBucket[i] = analogRead(rSensor);
  }
  for (int i = 0; i < 20; i++)
  {
    acc += readingsBucket[i];
  }
  int value = acc / 20;
  uint8_t mappedValue = map(value, 3200, 1730, 0, 100);
  mappedValue = constrain(mappedValue, 0, 100);

  return mappedValue;
}
