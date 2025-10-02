#include <U8g2lib.h>
#include <U8x8lib.h>
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE /* clock=* 16/ , /* data=* 17*/ );   //  HW I2C with pin remapping

/* image formating on progmem*/
/* image for AC VOLTAGE symbol */
#define noun_ac_20voltage_width 16
#define noun_ac_20voltage_height 8
static const unsigned char noun_ac_20voltage_bits[] U8X8_PROGMEM = {
  0x38, 0x00, 0x7c, 0x00, 0xc6, 0x00, 0x83, 0x81, 0x81, 0xc1, 0x00, 0x63, 0x00, 0x3e, 0x00, 0x1c
};

/* image for DC VOLTAGE symbol */
#define noun_dc_20current_2761907_width 12
#define noun_dc_20current_2761907_height 8
static const unsigned char noun_dc_20current_2761907_bits[] U8X8_PROGMEM = {
  0xff, 0x0f, 0xff, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x9b, 0x0d, 0x9b, 0x0d
};

#define icons8_full_battery_24_width 7
#define icons8_full_battery_24_height 12
/*
   made changes in the code below
*/
static const unsigned char icons8_full_battery_24_bits[] U8X8_PROGMEM = {
  0x1c, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f
};

/* image for empty battery */
#define battery_1_width 24
#define battery_1_height 35
static const unsigned char battery_1_bits[] U8X8_PROGMEM = {
  0x80, 0xff, 0x01, 0x80, 0xff, 0x01, 0x80, 0x81, 0x01, 0xff, 0x81, 0xff,
  0xff, 0x81, 0xff, 0x03, 0x00, 0xc0, 0x03, 0x00, 0xc0, 0x03, 0x00, 0xc0,
  0x03, 0x00, 0xc0, 0x03, 0x00, 0xc0, 0x03, 0x00, 0xc0, 0x03, 0x00, 0xc0,
  0x03, 0x00, 0xc0, 0x03, 0x00, 0xc0, 0x03, 0x00, 0xc0, 0x03, 0x00, 0xc0,
  0x03, 0x00, 0xc0, 0x03, 0x00, 0xc0, 0x03, 0x00, 0xc0, 0x03, 0x00, 0xc0,
  0x03, 0x00, 0xc0, 0x03, 0x00, 0xc0, 0x03, 0x00, 0xc0, 0x03, 0x00, 0xc0,
  0x03, 0x00, 0xc0, 0x03, 0x00, 0xc0, 0x03, 0x00, 0xc0, 0x03, 0x00, 0xc0,
  0x03, 0x00, 0xc0, 0x03, 0x00, 0xc0, 0x03, 0x00, 0xc0, 0x03, 0x00, 0xc0,
  0x03, 0x00, 0xc0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

#define energy_saver_width 13
#define energy_saver_height 20
static const unsigned char energy_saver_bits[] U8X8_PROGMEM = {
  0x40, 0x00, 0x80, 0x00, 0xe0, 0x01, 0xb8, 0x00, 0x4c, 0x04, 0x04, 0x04,
  0x02, 0x08, 0x02, 0x08, 0x02, 0x08, 0x02, 0x08, 0x00, 0x15, 0x0c, 0x0e,
  0x1c, 0x06, 0x2a, 0x02, 0x10, 0x00, 0xf0, 0x01, 0x00, 0x00, 0xe0, 0x00,
  0x00, 0x00, 0x40, 0x00

};

/* this is used to display the battery percentage in bars */
#define CHARGE_AREA_START_X     57
#define CHARGE_AREA_START_Y     15
#define CHARGE_AREA_WIDTH       20
#define CHARGE_AREA_HEIGHT      28

#define SD0 PA10  // Shut down MOSFET driver ICs           (D8 i.e. bit-0 in PORTB)  (HIGH: shutdown, LOW: run)
#define HS1 PB_13 // High side drive pin of Half Bridge #1 (D9 i.e. bit-1 in PORTB)
#define LS1 PA_8  // Low side drive pin of Half Bridge #1  (D10 i.e. bit-2 in PORTB)
#define LS2 PB_15 // High side drive pin of Half Bridge #2 (D11 i.e. bit-3 in PORTB)
#define HS2 PB_14 // Low side drive pin of Half Bridge #2  (D12 i.e. bit-4 in PORTB)
#define SD1 PB5  //shut down for boost converter
#define ENSAVER_PIN PB3
#define POWER_PIN PB4
#define SOLAR_PWM PA_15
#define FAN_PIN PB12       // Cooling fan
#define AC_SENS_PIN PA1    // Output of the optocoupler
#define SOLAR_SENS_PIN PA7 // To sense solar input
#define BUZZ_PIN PB9
#define VO_SENS_PIN PA3 // output AC feed
#define T_SENS_PIN PA2  // tempperature sensing pin
//#define LED_GRID A2
// PB6 & PB7 are used in I2C communication
#define BAT_SENS_PIN PA4       // Battery voltage monitor
#define I_SENS_PIN PA5         // Current monitor
#define BAT_SENS_FACTOR 6.64   // Battery sensor factor
#define SOLAR_SENS_FACTOR 6.64 // Current sensor factor
#define T_HIGH 0.8
#define T_LOW 0.2
#define T_C 1
#define battFul 16.8
#define PLOAD 0.35
#define ENLOAD 0.15
#define VB_LOW 10.5
#define VB_CUTOFF 10.0
#define GRID_MODE 0
#define INV_MODE 1
#define SOLAR_MODE 2
#define AVG_NUM 10               // number of iterations of the adc routine to average the adc readings

unsigned int LookupTable[600] = {0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , 11 , 13 , 14 , 15 , 16 , 17 , 18 , 19 , 20 , 21 , 22 , 23 , 24 ,
                                 25 , 26 , 27 , 28 , 29 , 30 , 31 , 32 , 33 , 34 , 35 , 36 , 37 , 38 , 39 , 40 , 41 , 42 , 43 , 44 , 44 , 45 , 46 , 47 , 48 , 49 , 50 , 51 , 52 , 53,
                                 54 , 54 , 55 , 56 , 57 , 58 , 59 , 60 , 60 , 61 , 62 , 63 , 64 , 65 , 65 , 66 , 67 , 68 , 68 , 69 , 70 , 71 , 71 , 72 , 73 , 74 , 74 , 75 , 76 , 76,
                                 77 , 78 , 78 , 79 , 80 , 80 , 81 , 82 , 82 , 83 , 83 , 84 , 84 , 85 , 86 , 86 , 87 , 87 , 88 , 88 , 89 , 89 , 90 , 90 , 90 , 91 , 91 , 92 , 92 , 93,
                                 93 , 93 , 94 , 94 , 94 , 95 , 95 , 95 , 96 , 96 , 96 , 97 , 97 , 97 , 97 , 98 , 98 , 98 , 98 , 98 , 99 , 99 , 99 , 99 , 99 , 99 , 99, 100, 100, 100,
                                 100 , 100 , 100 , 100 , 100 , 100 , 100 , 100 , 100 , 100 , 100 , 100 , 100 , 100 , 100 , 100 , 99 , 99 , 99 , 99 , 99 , 99 , 99 , 98 , 98 , 98,
                                 98 , 98 , 97 , 97 , 97 , 97 , 96 , 96 , 96 , 95 , 95 , 95 , 94 , 94 , 94 , 93 , 93 , 93 , 92 , 92 , 91 , 91 , 90 , 90 , 90 , 89 , 89 , 88 , 88 , 87,
                                 87 , 86 , 86 , 85 , 84 , 84 , 83 , 83 , 82 , 82 , 81 , 80 , 80 , 79 , 78 , 78 , 77 , 76 , 76 , 75 , 74 , 74 , 73 , 72 , 71 , 71 , 70 , 69 , 68 , 68,
                                 67 , 66 , 65 , 65 , 64 , 63 , 62 , 61 , 60 , 60 , 59 , 58 , 57 , 56 , 55 , 54 , 54 , 53 , 52 , 51 , 50 , 49 , 48 , 47 , 46 , 45 , 44 , 44 , 43 , 42,
                                 41 , 40 , 39 , 38 , 37 , 36 , 35 , 34 , 33 , 32 , 31 , 30 , 29 , 28 , 27 , 26 , 25 , 24 , 23 , 22 , 21 , 20 , 19 , 18 , 17 , 16 , 15 , 14 , 13 , 11,
                                 10 , 9 , 8 , 7 , 6 , 5 , 4 , 3 , 2 , 1 , 100 , 99 , 98 , 97 , 96 , 95 , 94 , 93 , 92 , 91 , 90 , 89 , 87 , 86 , 85 , 84 , 83 , 82 , 81 , 80 , 79 , 78,
                                 77 , 76 , 75 , 74 , 73 , 72 , 71 , 70 , 69 , 68 , 67 , 66 , 65 , 64 , 63 , 62 , 61 , 60 , 59 , 58 , 57 , 56 , 56 , 55 , 54 , 53 , 52 , 51 , 50 , 49,
                                 48 , 47 , 46 , 46 , 45 , 44 , 43 , 42 , 41 , 40 , 40 , 39 , 38 , 37 , 36 , 35 , 35 , 34 , 33 , 32 , 32 , 31 , 30 , 29 , 29 , 28 , 27 , 26 , 26 , 25,
                                 24 , 24 , 23 , 22 , 22 , 21 , 20 , 20 , 19 , 18 , 18 , 17 , 17 , 16 , 16 , 15 , 14 , 14 , 13 , 13 , 12 , 12 , 11 , 11 , 10 , 10 , 10 , 9 , 9 , 8 , 8,
                                 7 , 7 , 7 , 6 , 6 , 6 , 5 , 5 , 5 , 4 , 4 , 4 , 3 , 3 , 3 , 3 , 2 , 2 , 2 , 2 , 2 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0,
                                 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 2 , 2 , 2 , 2 , 2 , 3 , 3 , 3 , 3 , 4 , 4 , 4 , 5 , 5 , 5 , 6 , 6 , 6 , 7 , 7 , 7 , 8 , 8 , 9 , 9 , 10,
                                 10 , 10 , 11 , 11 , 12 , 12 , 13 , 13 , 14 , 14 , 15 , 16 , 16 , 17 , 17 , 18 , 18 , 19 , 20 , 20 , 21 , 22 , 22 , 23 , 24 , 24 , 25 , 26 , 26 , 27,
                                 28 , 29 , 29 , 30 , 31 , 32 , 32 , 33 , 34 , 35 , 35 , 36 , 37 , 38 , 39 , 40 , 40 , 41 , 42 , 43 , 44 , 45 , 46 , 46 , 47 , 48 , 49 , 50 , 51 , 52,
                                 53 , 54 , 55 , 56 , 56 , 57 , 58 , 59 , 60 , 61 , 62 , 63 , 64 , 65 , 66 , 67 , 68 , 69 , 70 , 71 , 72 , 73 , 74 , 75 , 76 , 77 , 78 , 79 , 80 , 81,
                                 82 , 83 , 84 , 85 , 86 , 87 , 89 , 90 , 91 , 92 , 93 , 94 , 95 , 96 , 97 , 98 , 99
                                };

volatile bool sensVoltage = false;
volatile bool knockKnock = false;
volatile int tick2msCounter = 0;
bool flagChangeDuty = false, lowBat = false, criticalBat = false, overloadflag = false, checkAc = false, criticaltemp = false, flagBeep = false, flagSwitchToGrid = false, power = false, ensaver = false;
bool flagCharging = false, powerup = false, powerdown = false, flagShutDown = false, flagFan = false, checkSolar = false, changetogrid = false;
byte mode = INV_MODE;
float vOut, vBat, temp, load, solar, acSence; // Output voltage from feedback transformer, battery voltage, pd across current sense resistor
float ModIndex = 0.9;                    // Start the inverter with this modulation index
unsigned int chargeduty = 0;                  // charger duty cycle
float batsample, solarsample, loadsample, tempsample, acsample = 0;
enum beepType
{
  shortBeep = 100,
  longBeep = 300
};
unsigned long eventTime = 0;

void changeDuty(void);
void dispRefresh(void);
void switchToInv(void);
void switchToGrid(void);
void isGridOn(void);
void shutDown(void);
void powerOn(void);
void timertask(void);
void beep(byte t = shortBeep, byte n = 1, unsigned int p = 200);

// Instantiate HardwareTimer object. Thanks to 'new' instanciation, HardwareTimer is not destructed when setup() function is finished.
HardwareTimer *SineTable = new HardwareTimer(TIM1);
//HardwareTimer *Display = new HardwareTimer(TIM3);

void setup()
{

  // configure pin in output mode
  pinMode(SD0, OUTPUT);
  pinMode(SD1, OUTPUT);
  digitalWrite(SD1, HIGH);
  pinMode(BUZZ_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
  pinMode(SOLAR_PWM, OUTPUT);
  pinMode(ENSAVER_PIN, INPUT_PULLUP);
  pinMode(POWER_PIN, INPUT_PULLUP);
  u8g2.begin();
  SineTable->pause();
  SineTable->setOverflow(30000, HERTZ_FORMAT);
  SineTable->attachInterrupt(SpwmGenerator);
  SineTable->resume();
  //  Display->pause();
  //  Display->setOverflow(1, HERTZ_FORMAT);
  //  Display->attachInterrupt(dispRefresh);
  //  Display->resume();
}

void loop()
{
  /*#################################################################################################################*/
  /*                                       ~~~~~~~~INVERTER MODE~~~~~~~~                                             */
  /*#################################################################################################################*/
  timertask();
  dispRefresh();



  if (knockKnock)
  {
    checkState();
    knockKnock = false;
  }

  if (mode == INV_MODE)
  {
    if (power)
    {
      if (!powerup) {
        switchToInv();
      }

      if (vBat < VB_LOW)
        lowBat = true;
      if (vBat < VB_CUTOFF)
        criticalBat = true;
      if (temp > T_HIGH)
      {
        if (temp > T_C)
          criticaltemp = true;
        else
          criticaltemp = false;
        if (!flagFan)
        {
          digitalWrite(FAN_PIN, HIGH);
          flagFan = true;
        }
      }
      else if (temp < T_LOW && flagFan)
      {
        digitalWrite(FAN_PIN, LOW);
        flagFan = false;
      }
      if (criticalBat || criticaltemp)
      { // Low battery or short circuit occured or critically high tempperature
        shutDown();
      }
      if (lowBat && !flagBeep)
      {
        lowBat = false;
        beep(longBeep, 1, 2000);
      }
      if (flagChangeDuty)
        changeDuty();
      if (ensaver)
      {
        if (load > ENLOAD)
        {
          unsigned long timeNow = millis();
          unsigned long timeElapsed = eventTime > timeNow ? (4294967295 - eventTime) + timeNow : timeNow - eventTime;
          if (timeElapsed > 5000)
          {
            if (load > ENLOAD)
            {
              if (!flagBeep)
              {
                beep(longBeep, 3, 1000);
              }
              shutDown();
              overloadflag = true;
            }
          }
        }
      }
      else
      {
        if (load > PLOAD)
        {
          unsigned long timeNow = millis();
          unsigned long timeElapsed = eventTime > timeNow ? (4294967295 - eventTime) + timeNow : timeNow - eventTime;
          if (timeElapsed > 5000)
          {
            if (load > PLOAD)
            {
              if (!flagBeep)
              {
                beep(longBeep, 3, 1000);
              }
              shutDown();
              overloadflag = true;
            }
          }
        }
      }
    } else shutDown();
  }
  /*#################################################################################################################*/
  /*                                       ~~~~~~~~GRID MODE~~~~~~~~                                                 */
  /*#################################################################################################################*/
  else if (mode == GRID_MODE)
  {
    // Charging Control...
    unsigned long timeNow = millis();
    unsigned long timeElapsed = eventTime > timeNow ? (4294967295 - eventTime) + timeNow : timeNow - eventTime;
    if (!flagCharging)
    {
      if (timeElapsed > 3000)
      { // Start charging after 3 s
        flagCharging = true;
      }
    }
    else
    {
      eventTime = millis();
    }
    if (vBat >= 16.8)
    {
      flagCharging = false;
    }
    if (temp > T_HIGH)
    {
      if (!flagFan)
      {
        digitalWrite(FAN_PIN, HIGH);
        flagFan = true;
      }
    }
    else if (temp < T_LOW && flagFan)
    {
      digitalWrite(FAN_PIN, LOW);
      flagFan = false;
    }
  }
  //  /*#################################################################################################################*/
  //  /*                                       ~~~~~~~~SOLAR MODE~~~~~~~~                                                 */
  //  /*#################################################################################################################*/
  else if (mode == SOLAR_MODE)
  {
    pwm_start(SOLAR_PWM, 1000, chargeduty, PERCENT_COMPARE_FORMAT);
    if (vBat < battFul)
    {
      if (chargeduty != 100)
        chargeduty++;
    }
    if (vBat > battFul)
    {
      if (chargeduty != 0)
        chargeduty--;
    }
    if (vBat >= 16.8)
    {
      flagCharging = false;
    }
    else
    {
      flagCharging = true;
    }
    if (temp > T_HIGH)
    {
      if (!flagFan)
      {
        digitalWrite(FAN_PIN, HIGH);
        flagFan = true;
      }
    }
    else if (temp < T_LOW && flagFan)
    {
      digitalWrite(FAN_PIN, LOW);
      flagFan = false;
    }
    if (power && vBat > 14)
    {
      switchToInv();
      if (flagChangeDuty)
        changeDuty();
      if (vBat < 15 && load > 0.7)
      {
        unsigned long timeNow = millis();
        unsigned long timeElapsed = eventTime > timeNow ? (4294967295 - eventTime) + timeNow : timeNow - eventTime;
        if (timeElapsed > 5000)
        {
          if (load > 0.7)
          {
            if (!flagBeep)
            {
              beep(longBeep, 3, 1000);
            }
            shutDown();
            overloadflag = true;
          }
        }
      }

      if (vBat >= 16.8 && load > PLOAD)
      {
        unsigned long timeNow = millis();
        unsigned long timeElapsed = eventTime > timeNow ? (4294967295 - eventTime) + timeNow : timeNow - eventTime;
        if (timeElapsed > 5000)
        {
          if (load > PLOAD)
          {
            if (!flagBeep)
            {
              beep(longBeep, 3, 1000);
            }
            shutDown();
            overloadflag = true;
          }
        }
      }
    }
    else
    {
      shutDown();
    }
  }

  if (flagBeep)
    beep();


}


void timertask(void)
{
  static int count;
  analogReadResolution(12);
  float batval = (analogRead(BAT_SENS_PIN) * 3.3 * BAT_SENS_FACTOR) / 4096;
  float solarval = (analogRead(SOLAR_SENS_PIN) * 3.3 * SOLAR_SENS_FACTOR) / 4096;
  float tempval = (analogRead(T_SENS_PIN) * 3.3) / 4096;
  float loadval = (analogRead(I_SENS_PIN) * 3.3) / 4096;
  float acsenceval = (analogRead(AC_SENS_PIN) * 3.3 * SOLAR_SENS_FACTOR) / 4096;
  if (batval > batsample) batsample = batval;
  if (solarval > solarsample) solarsample = solarval;
  if (tempval > tempsample) tempsample = tempval;
  if (loadval > loadsample) loadsample = loadval;
  if (acsenceval > acsample) acsample = acsenceval;
  if (count == AVG_NUM) {
    vBat = (float)((int)(batsample * 10 + .5)) / 10;
    solar = (float)((int)(solarsample * 10 + .5)) / 10;
    load = (float)((int)(loadsample * 10 + .5)) / 10;
    temp = (float)((int)(tempsample * 10 + .5)) / 10;
    acSence = (float)((int)(acsample * 10 + .5)) / 10;
    batsample = 0;
    solarsample = 0;
    loadsample = 0;
    tempsample = 0;
    acsample = 0;
    count = 0;
  }
  if (acSence > 5)
  {
    checkAc = true;
  }
  else
    checkAc = false;
  if (solar > 16)
  {
    checkSolar = true;
  }
  else
    checkSolar = false;
  if (digitalRead(POWER_PIN) == LOW)
  {
    power = true;
  }
  else
    power = false;
  if (digitalRead(ENSAVER_PIN) == LOW)
  {
    ensaver = true;
  }
  else
    ensaver = false;
  flagChangeDuty = true;
  knockKnock = true;
  count++;
}

void SpwmGenerator(void) // ISR for spwm control
{
  static int i = 0; // Static means it will NOT be reinitialized
  int DutyCycle;
  if (i >= 600)
  {
    i = 0;
  }
  DutyCycle = (LookupTable[i] * ModIndex);
  start_comp_pwm(1, 30000, DutyCycle, 165, PERCENT_COMPARE_FORMAT);
  if (i <= 299 || i >= 599) {
    if (i == 599) {
      start_comp_pwm(2, 30000, DutyCycle , 0, PERCENT_COMPARE_FORMAT);
    } else start_comp_pwm(2, 30000, 0 , 165, PERCENT_COMPARE_FORMAT);
  }
  else {
    start_comp_pwm(2, 30000, 100 , 165, PERCENT_COMPARE_FORMAT);
  }
  i++;
}

void changeDuty()
{
  static int anologVal;
  analogReadResolution(12);
  anologVal = analogRead(VO_SENS_PIN);
  if (anologVal < 2048) {
    ModIndex = ModIndex - 0.001;
    if (ModIndex < 0) ModIndex = 0;
  }
  else if (anologVal > 2048) {
    ModIndex = ModIndex + 0.001;
    if (ModIndex > 1) ModIndex = 1;
  }
  flagChangeDuty = false;
}

void beep(byte t, byte n, unsigned int p) // beep(<type of beep>, <number of beeps>, <pause time in ms between beeps>)
{
  static bool beepState = false;
  static unsigned int beepTime;
  static unsigned int beepPause;
  static unsigned long beepToggled = 0;
  static int noOfBeep = 0;
  if (!flagBeep)
  {
    beepTime = t;
    beepPause = p;
    noOfBeep = n;
    digitalWrite(BUZZ_PIN, HIGH);
    beepToggled = millis();
    beepState = true;
    flagBeep = true;
  }
  if (flagBeep)
  {
    unsigned long timeNow = millis();
    unsigned long beepRun = beepToggled > timeNow ? (4294967295 - beepToggled) + timeNow : timeNow - beepToggled;
    if (beepState)
    {
      if (beepRun > beepTime)
      {
        digitalWrite(BUZZ_PIN, LOW);
        beepToggled = millis();
        beepState = false;
        noOfBeep--;
      }
    }
    else
    {
      if (beepRun > beepPause)
      { // Blanking time 500 ms between two beeps
        if (noOfBeep)
        {
          digitalWrite(BUZZ_PIN, HIGH);
          beepToggled = millis();
          beepState = true;
        }
        else
        {
          flagBeep = false;
        }
      }
    }
  }
}

void shutDown(void)
{
  if (!powerdown) {

    //ModIndex = 0;
    digitalWrite(SD0, HIGH); // bridge driver off
    digitalWrite(SD1, HIGH);
    eventTime = millis();
    powerup = false;
    powerdown = true;
  }
}

void powerOn(void)
{
  digitalWrite(SD0, LOW); // bridge driver off
  digitalWrite(SD1, LOW); // Switch boost converter off
  flagShutDown = false;
  eventTime = millis();
  //ModIndex = 1;
}

void checkState(void)
{
  if (checkAc)
  {
    mode = GRID_MODE;
    switchToGrid();
  }
  else if (checkSolar)
  {
    mode = SOLAR_MODE;
    changetogrid = true;
  }
  else
  {
    mode = INV_MODE;
    //switchToInv();
  }
}

void switchToGrid(void)
{
  if (changetogrid) {
    shutDown();
    flagCharging = false;
    criticaltemp = false;
    eventTime = millis();
    changetogrid = false;
  }
}

void switchToInv(void)
{
  powerOn();
  lowBat = false; // Reset all the protection flags
  criticaltemp = false;
  changetogrid = true;
  beep(shortBeep, 1);
  powerup = true;
  powerdown = false;
}

/* DISPLAY FUNCTIONS */

void dispRefresh(void) {
  if (GRID_MODE) {
    drawGrid();
  }
  else if (INV_MODE) {
    drawInv();
  }
  else if (SOLAR_MODE) {
    drawSolar();
  }
}

uint8_t batteryPercent(float voltage) {
  uint8_t percent = map(voltage, 9.0, 16.8, 0, 100);
  percent = constrain(percent, 0, 100);
  return percent;
}

void drawBattery() {
  uint8_t percent = batteryPercent(vBat);
  uint8_t height, sheight;
  u8g2.setCursor(55, 59);// I have not tested this before so please play around with it
  u8g2.print(percent);
  u8g2.print("%");
  u8g2.setFont(u8g2_font_profont15_tr);// u8g2_font_ordinarybasis_tr
  u8g2.setFontMode(1);
  u8g2.setDrawColor(2);

  u8g2.drawXBMP(55, 10, battery_1_width, battery_1_height, battery_1_bits); // draw empty battery

  height = (percent * CHARGE_AREA_HEIGHT) / 100;
  if (percent < 100) height += 1;
  float pert = (1.0 - (float(percent) / 100.0));
  float conv = (pert * CHARGE_AREA_HEIGHT);
  sheight = (conv + CHARGE_AREA_START_Y); // (100 * CHARGE_AREA_START_Y) / percent;
  if (percent == 75 || percent == 50 || percent == 25 || percent == 0) height -= 1;

  u8g2.drawBox(CHARGE_AREA_START_X, sheight, CHARGE_AREA_WIDTH, height);
  if (percent == 100) u8g2.drawBox(CHARGE_AREA_START_X + 7, CHARGE_AREA_START_Y - 3, CHARGE_AREA_WIDTH - 14, 3);

  if (flagCharging) {
    u8g2.setFont(u8g2_font_open_iconic_other_2x_t);
    u8g2.drawGlyph(60, 37, 0x0040);
  }
  if (overloadflag) {
    u8g2.setFont(u8g2_font_ncenR14_tr); // you can get a new font through this link: https://github.com/olikraus/u8g2/wiki/fntlistall
    u8g2.drawStr(10, 52/* play around with these vals. I haven't tested*/, "(!)");
  }
  if (ensaver) {
    u8g2.drawXBMP(110, 35/* not tested before so play around with the numbers*/, energy_saver_width, energy_saver_height, energy_saver_bits); // draw energy saver symbol
  }
}
void drawGrid() {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_freedoomr10_mu);// u8g2_font_ordinarybasis_tr
    u8g2.drawStr(85, 20, "220");
    u8g2.drawXBMP(110, 9, noun_ac_20voltage_width, noun_ac_20voltage_height, noun_ac_20voltage_bits);
    u8g2.drawStr(20, 20, "220");
    u8g2.drawXBMP(0, 9, noun_ac_20voltage_width, noun_ac_20voltage_height, noun_ac_20voltage_bits);
    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(17, 30, "input");
    u8g2.drawStr(85, 30, "output");
    drawBattery();

  } while ( u8g2.nextPage() );
}
void drawInv() {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_freedoomr10_mu);// u8g2_font_ordinarybasis_tr
    if (powerup)
      u8g2.drawStr(85, 20, "220");
    /*
       made changes in the code below
    */
    else u8g2.drawStr(101, 20, "0");
    u8g2.drawXBMP(110, 9, noun_ac_20voltage_width, noun_ac_20voltage_height, noun_ac_20voltage_bits);
    /*
      added a new line below
    */
    int batt = vBat;
    u8g2.setCursor(8, 20);
    u8g2.print(batt);// you understand
    u8g2.drawXBMP(30, 9, noun_dc_20current_2761907_width, noun_dc_20current_2761907_height, noun_dc_20current_2761907_bits);

    u8g2.drawXBMP(0, 7, icons8_full_battery_24_width, icons8_full_battery_24_height, icons8_full_battery_24_bits);
    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(7, 30, "input");
    u8g2.drawStr(85, 30, "output");
    drawBattery();
  } while ( u8g2.nextPage() );
}
void drawSolar() {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_freedoomr10_mu);// u8g2_font_ordinarybasis_tr
    if (powerup)
      u8g2.drawStr(85, 20, "220");
    /*
      made changes in the code below
    */
    else u8g2.drawStr(101, 20, "0");
    u8g2.drawXBMP(109, 9, noun_ac_20voltage_width, noun_ac_20voltage_height, noun_ac_20voltage_bits);
    /*
      added a new line below
    */
    int sol = solar;
    u8g2.setCursor(15, 20);
    u8g2.print(sol);// you understand
    u8g2.drawXBMP(35, 9, noun_dc_20current_2761907_width, noun_dc_20current_2761907_height, noun_dc_20current_2761907_bits);

    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(15, 30, "input");
    u8g2.drawStr(85, 30, "output");
    u8g2.setFont(u8g2_font_open_iconic_weather_2x_t);
    u8g2.drawGlyph(0, 22, 0x0045); // hex value for sushine indicating solar
    drawBattery();
  } while ( u8g2.nextPage() );
}
