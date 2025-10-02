#ifndef _DECLARATIONS_H_
#define _DECLARATIONS_H_

#define mySerial Serial1
// #define ySerial Serial3
#include <U8g2lib.h>
//#include <EEPROM.h>
#include <STM32RTC.h>
#include <TimeLib.h>
#include <Chrono.h>

/* chrono instances */
Chrono runChrono(Chrono::SECONDS);
Chrono restChrono(Chrono::SECONDS);
Chrono lcdChrono(Chrono::SECONDS);
Chrono loadChrono(Chrono::SECONDS);
Chrono checkChrono(Chrono::SECONDS);
Chrono mtimerChrono[12];
/* Get the rtc object */
STM32RTC& rtc = STM32RTC::getInstance();

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

const uint8_t FS1 = PA12;
const uint8_t FS2 = PA15;
const uint8_t pumpPin = PB9;
const uint8_t valve_1A = PB4;
const uint8_t valve_1B = PB3;
const uint8_t valve_2A = PB6;
const uint8_t valve_2B = PB5;
const uint8_t valve_3A = PB7;
const uint8_t valve_3B = PB8;
const uint8_t valve_4A = PC14;
const uint8_t valve_4B = PC13;
const uint8_t lcdBacklight = PA2;
//const uint8_t lcdBacklight = PA3;
const uint8_t rSensor = PA6;
//const uint8_t rSensor = PA7;
const uint8_t battPin = PB0;
const uint8_t chargePin = PB1;

// U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE, /* clock=*/16, /* data=*/17); // ESP32 Thing, HW I2C with pin remapping
U8G2_ST7567_OS12864_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/PA5, /* data=*/PA7, /* cs=*/PA4, /* dc=*/PA3, /* reset=*/PA1);
// End of constructor list
//U8G2_ST7567_OS12864_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/PA6, /* data=*/PB0, /* cs=*/PA5, /* dc=*/PA4, /* reset=*/PA2);


//const char * weekdayName[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

/*
   initialization of structures takes place here
*/
static enum pageSetup {HOME, MENU, MODE, CALENDAR, DATE, TIME, OUTLET, SENSOR, NETWORK, CYCLE, TIMER, HUMIDITY} page;
static enum irrigation_mode {MANUAL, LOOP, PROGS} irr_mode;
static enum rain_sense_mode {OFF, ON} rSense;
static enum water_outlet {VALVE, PUMP} wOutlet;
static enum network_mode {NaN, SMS, GPRS} gsmMode;
static enum p_state {PUMP_OFF, PUMP_ON} pumpState;
static enum l_state {REST, RUN} loopState;
static enum irr_valve_state {SHUTOFF, RUNNING} irr_valve;
enum checkSim {NUL, EROR, READY, WAIT} simStatus;
//static enum t_state {DISENGAGED, ENGAGED} timerState;
static uint8_t cusor = 1, clashCount = 0, next_prog = 0, sigStrength = 0;
static bool next_irr_flag, pump_init_flag, firstRun, stringComplete, messagestatus, loadState = false, checkState;
static const int PROG_SIZE = 12;
static int clashProgs[PROG_SIZE];
static String USER = "", PASS = "", inputString = "", homeScr = "", messageBody = "", password = "1xx4", phone = "+2348XXX699847", APN = "web.gprs.mtnnigeria.net";
unsigned long counter;
Button power(PA0); // define button instance by selecting using the function Button <your instance name>(digital pin,pullup or pulldown)
Button manual(PA0);
volatile bool holdCheck = false;// hold check to check button hold on a button

struct Calendar {
  uint16_t YR;
  uint8_t MTH;
  uint8_t DY;
  int HR;
  int MIN;
  int SEC;
} ;
struct Timer {
  uint8_t Prog;
  int StartHr;
  int StartMin;
  int Runtime;
  uint8_t Interval;
  uint8_t Valve;
  uint8_t futureYr;
  uint8_t futureMonth;
  uint8_t futureDay;
  uint8_t futureWkDay;
} ;
struct Cycle {
  int Run;
  int Rest;
} ;
struct All {// struct containing other structs for easy reference
  struct Calendar cal;
  struct Timer tim;
  struct Cycle cyc;
  int hum;
  uint8_t prog_count;
} ;
static struct Timer Programs[PROG_SIZE];
struct Calendar cal;// initialised struct
struct Timer Tim;
static struct All All_Str = {
  cal.YR = 22,
  cal.MTH = 1,
  cal.DY = 1,
  cal.HR = 0,
  cal.MIN = 0,
  cal.SEC = 0,
  Tim.Prog = 1,
  Tim.StartHr = 0,
  Tim.StartMin = 0,
  Tim.Runtime = 0,
  Tim.Interval = 1
};
void date_time() {// test function
  if (rtc.getYear() < 22) {
    rtc.setTime(0, 0, 0);
    rtc.setDate(1, 1, 22);
  }
}

#endif
