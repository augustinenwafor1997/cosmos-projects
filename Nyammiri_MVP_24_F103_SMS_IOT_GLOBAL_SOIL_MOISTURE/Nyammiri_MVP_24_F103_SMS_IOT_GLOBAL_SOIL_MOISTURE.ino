#include <U8g2lib.h>
#include <U8x8lib.h>

#include <Wire.h>
#include <stm32f1_rtc.h>
#define FLAGS_REGISTER  1
/* Bit 0 used for internal RTC init flag */
#define HIJRI_MODE_BIT  1
#define HOUR_12_BIT     2
#define ALARM_ENTRY_BIT 3

#define HIJRI_MODE_FLAG  (1 << HIJRI_MODE_BIT)
#define HOUR_12_FLAG     (1 << HOUR_12_BIT)
#define ALARM_ENTRY_FLAG (1 << ALARM_ENTRY_BIT)

#define IS_HIJRI_MODE(x)      ((x.getBackupRegister(FLAGS_REGISTER) & HIJRI_MODE_FLAG)  == HIJRI_MODE_FLAG)
#define IS_HOUR_12(x)         ((x.getBackupRegister(FLAGS_REGISTER) & HOUR_12_FLAG)     == HOUR_12_FLAG)
#define IS_ALARM_ENTRY(x)     ((x.getBackupRegister(FLAGS_REGISTER) & ALARM_ENTRY_FLAG) == ALARM_ENTRY_FLAG)

#include <Chrono.h>
#include <EEPROM.h>
/* Get the rtc object */
STM32F1_RTC rtc;
/* chrono instances */
Chrono irrigationChrono(Chrono::SECONDS);
Chrono lcdChrono(Chrono::SECONDS);
Chrono battChrono(Chrono::SECONDS);
Chrono SMSChrono(Chrono::SECONDS);


#define mySerial Serial1


/* pin declarations */
#define pumpPin PB11
#define valvePin PB3
#define floatSwitch1 PA15
#define floatSwitch2 PA12
#define lcdBacklight PA2
#define soilSensorPin PA0
#define battPin PB1
#define AC_SENSE PB10
const int maxInput = 70;

#define SERIAL_TX_BUFFER_SIZE 256
#define SERIAL_RX_BUFFER_SIZE 256

//U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE /* clock=* 16/ , /* data=* 17*/ );   // ESP32 Thing, HW I2C with pin remapping
U8G2_ST7567_OS12864_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ PA5, /* data=*/ PA7, /* cs=*/ PA4, /* dc=*/ PA3, /* reset=*/ PA1);

#define icons8_empty_battery_32_2__width 15
#define icons8_empty_battery_32_2__height 9
static const unsigned char icons8_empty_battery_32_2__bits[] U8X8_PROGMEM = {
  0xff, 0x1f, 0x01, 0x10, 0x01, 0x10, 0x01, 0x70, 0x01, 0x60, 0x01, 0x70,
  0x01, 0x10, 0x01, 0x10, 0xff, 0x1f
};

#define icons8_moisture_24_20_1__width 12
#define icons8_moisture_24_20_1__height 13
static const unsigned char icons8_moisture_24_20_1__bits[] U8X8_PROGMEM = {
  0x80, 0x00, 0x80, 0x01, 0xde, 0x05, 0xc0, 0x01, 0x00, 0x00, 0x00, 0x00,
  0x99, 0x09, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0xb9, 0x05, 0x38, 0x00,
  0x18, 0x00
};

#define icons8_shower_24_20_1__width 14
#define icons8_shower_24_20_1__height 16
static const unsigned char icons8_shower_24_20_1__bits[] U8X8_PROGMEM = {
  0x80, 0x00, 0x80, 0x00, 0xe0, 0x00, 0x58, 0x0f, 0x0f, 0x1c, 0xff, 0x3f,
  0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x01, 0x00, 0x00,
  0x88, 0x04, 0x00, 0x00, 0x00, 0x00, 0x24, 0x09
};

#define icons8_pump_16_width 14
#define icons8_pump_16_height 12
static const unsigned char icons8_pump_16_bits[] U8X8_PROGMEM = {
  0x1c, 0x00, 0x14, 0x00, 0x94, 0x1f, 0x94, 0x21, 0x74, 0x2f, 0x57, 0x21,
  0x45, 0x2f, 0x47, 0x21, 0x7c, 0x2f, 0x80, 0x21, 0x80, 0x1f, 0x00, 0x00
};

// This is the area inside the battery bitmap, in pixels
#define CHARGE_AREA_START_X     114
#define CHARGE_AREA_START_Y     1
#define CHARGE_AREA_WIDTH       11
#define CHARGE_AREA_HEIGHT      7

#define energy_saver_width 15
#define energy_saver_height 14
static const unsigned char energy_saver_bits[] U8X8_PROGMEM = {
  0xe0, 0x03, 0xf1, 0x47, 0xfa, 0x2f, 0xf8, 0x0f, 0xf8, 0x0f, 0xfb, 0x6f,
  0xf8, 0x0f, 0xf0, 0x07, 0xf4, 0x17, 0xe2, 0x23, 0xc0, 0x01, 0xc0, 0x01,
  0x00, 0x00, 0xc0, 0x01
};



// Resistors for voltage divider
#define R1 4700.0
#define R2 2200.0
#define AVG_NUM 10               // number of iterations of the adc routine to average the adc readings
#define batFactor 10.35
#define AVG 60


float vBat, batsample;
// uint16_t duration, futureYr, yr = 2017;
unsigned long epochTime, futureEpochTime, timeCount = millis();
bool acState, setupState, sendState, messState, recvState, batState = 0, irrigateSwitchState, previous = LOW, previousState = LOW, state = LOW, onState, isHijriMode, isHour12, isAlarmEntry, isAlarmActive = false, resumeState, errorState = 0;
int futureMonth, futureDay, lastButton = 0, startDate, startHour, interval, modeSelected, prevSoilMoisture, debounce = 50, mth = 1, dy = 1;
int  hr = 0, mins = 0, sec = 0, duration = 0, futureYr, yr = 2017, cursor = 0, cursorA = 0, cursorB = 0, count, percent, digiValue, setPoint = 50, globalSoilMoist;
String incomingMessage, phoneNum = "5675332235664336", senderNum = "";
//#include <Bounce2.h>

byte button_pins[] = {PB12, PB13, PB14, PA8, PB15, PB5}; // button pins, 1 = menu 2/3 = up/down, 4/5 = left/right
#define NUMBUTTONS sizeof(button_pins)
//Bounce * buttons = new Bounce[NUMBUTTONS];


#define MENU_SIZE 4
char *menu[MENU_SIZE] = { "AUTOMATIC MODE", "MANUAL MODE", "SET DATE/TIME", "EXIT"};

#define NEW_MENU_SIZE 7
char *new_menu[NEW_MENU_SIZE] = { "START < ", "TIME <    > O'CLOCK", "INTERVAL <  > DAYS", "DURATION  <      > MINS", "HUMIDITY <      > %", "SET", "BACK"};

char *day_to_start[8] = {"TODAY" , "TOMORROW", "NXT 2 DAYS", "NXT 3 DAYS", "NXT 4 DAYS", "NXT 5 DAYS", "NXT 6 DAYS", "NXT 7 DAYS"};

#define DATE_MENU_SIZE 3
char *date_menu[DATE_MENU_SIZE] = { "DATE", "TIME", "EXIT"};

#define DATE_SETTING_SIZE 3
char *date_setting[DATE_SETTING_SIZE] = { "YEAR", "MONTH", "DAY"};

#define TIME_SETTING_SIZE 3
char *time_setting[TIME_SETTING_SIZE] = { "HOUR", "MINUTE", "SECOND"};

const char * weekdayName[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

#define DURATION_ROW 67
#define DURATION_COL 45

#define DAY_ROW 45
#define DAY_COL 18



DateVar date;
TimeVar time;

void readEEPROM() {
  EEPROM.get(1 * sizeof(onState), onState);
  // EEPROM.get(2 * sizeof(resumeState), resumeState);
  EEPROM.get(3 * sizeof(modeSelected), modeSelected);
  EEPROM.get(4 * sizeof(startDate), startDate);
  EEPROM.get(5 * sizeof(startHour), startHour);
  EEPROM.get(6 * sizeof(interval), interval);
  EEPROM.get(7 * sizeof(futureDay), futureDay);
  EEPROM.get(8 * sizeof(futureMonth), futureMonth);
  EEPROM.get(9 * sizeof(duration), duration);
  //  EEPROM.get(10 * sizeof(errorState), errorState);
  EEPROM.get(11 * sizeof(setPoint), setPoint);
  EEPROM.get(12 * sizeof(futureEpochTime), futureEpochTime);
  // EEPROM.get(49, phoneNum);

}
void reads(void);
void recvSMS(void);
void setup() {
  // put your setup code here, to run once:
  u8g2.begin();
  for (int i = 0; i < NUMBUTTONS; i++) {
    pinMode( button_pins[i], INPUT_PULLUP);
    delay(25);
  }
  rtc.begin();
  isHijriMode = IS_HIJRI_MODE(rtc);
  isHour12 = IS_HOUR_12(rtc);
  isAlarmEntry = IS_ALARM_ENTRY(rtc);
  epochTime = rtc.getTime();
  rtc.epochToDate(epochTime, date);
  rtc.epochToTime(epochTime, time);
  // pinMode(manualIrrigateButton, INPUT_PULLUP);
  
  
  pinMode(pumpPin, OUTPUT);
  //digitalWrite(pumpPin, LOW);
  pinMode(valvePin, OUTPUT);
  pinMode(lcdBacklight, OUTPUT);
  pinMode(battPin, INPUT);
  pinMode(AC_SENSE, INPUT);

  irrigateSwitchState = LOW;
  digitalWrite(lcdBacklight, LOW); // turn ON backlight
  // phoneNum = "";
  readEEPROM();
  byte validNum = EEPROM.read(49);
  if (validNum == 14)
    phoneNum = EEPROMread(49);
  else phoneNum = "+2348140699847";
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_fub17_tr);// u8g2_font_fub17_tr
    u8g2.drawStr(10, 40, "COSMOS");
    u8g2.setFont(u8g2_font_helvB08_te);// u8g2_font_fub17_tr
    u8g2.drawStr(54, 50, "automation");
  } while ( u8g2.nextPage() );
  delay(5000);
  mySerial.begin(9600);
  if (date.year < 2017) {// set date and time for the first time
    executeChoice(2);
  }
  digitalWrite(lcdBacklight, HIGH); // turn off backlight
  setupState = 1;
  SMSChrono.restart();
  // incomingMessage.reserve(200);
  HardwareTimer *MyTim = new HardwareTimer(TIM1);
  MyTim->pause();
  MyTim->setOverflow(10000, HERTZ_FORMAT);
  MyTim->attachInterrupt(recvSMS);
  MyTim->resume();
}



void loop() {
  // put your main code here, to run repeatedly:
  homeDisplay();
  sendFirstSMS();
  sendReply();
  SMSconfig();
  waterLevelControl();
  mainMenu();
  performAction();
  turnOffBacklight();
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
  byte newStrLen = EEPROM.read(addrOffset);
  char data[newStrLen + 1];
  for (int i = 0; i < newStrLen; i++)
  {
    data[i] = EEPROM.read(addrOffset + 1 + i);
  }
  data[newStrLen] = '\0'; // !!! NOTE !!! Remove the space between the slash "/" and "0" (I've added a space because otherwise there is a display bug)
  return String(data);
}

void recvSMS(void) {
  checkSerial();
  if (messState == 1) {
    String checkValid = getValue(getValue(incomingMessage, '+', 1), ':', 0);
    if (checkValid == "CMT") {
      senderNum = getValue(getValue(incomingMessage, '"', 1), '"', 0);
      recvState = 1;
    }
    messState = 0;
    incomingMessage = "";
  }
}
void sendReply() {
  if (recvState == 1 && messState == 0) {
    // senderNum.trim();
    if (senderNum.startsWith("+234") && senderNum.length() == 14) {
      // save the new number
      EEPROMwrite(49, senderNum);
      mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
      delay(100);
      mySerial.println("AT+CMGS=\"" + senderNum + "\""); //change ZZ with country code and xxxxxxxxxxx with phone number to sms
      delay(100);
      phoneNum = EEPROMread(49);
      mySerial.print("saved " + phoneNum + " successfully"); //text content
      delay(100);
      mySerial.write(26);

    }
    senderNum = "";
    recvState = 0;
  }
}
void checkSerial() {
  if (mySerial.available()) {
    while (mySerial.available()) {
      processIncomingByte(mySerial.read());
    }
  }
  // recvState = 0;
}

void processIncomingByte (const byte inByte) {
  static char input_line [maxInput];
  static unsigned int input_pos = 0;
  if (inByte == '\n' && input_pos > 50) {
    input_line [input_pos] = 0;  // terminating null byte
    incomingMessage = String(input_line);
    // terminator reached! process input_line here ...
    messState = 1;
    // reset buffer for next time
    input_pos = 0;
  }
  else {
    if (input_pos < (maxInput - 1)) {
      input_line [input_pos] = inByte;
      input_pos++;
    }
  }
} // end of processIncomingByte

String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}


void SMSconfig() {
  if (SMSChrono.hasPassed(10) && setupState == 1) {
    mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
    delay(100);
    mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
    delay(100);
    mySerial.println("AT+CNMI=1,2,0,0,0"); // Decides how newly arrived SMS messages should be handled
    delay(100);
    mySerial.println("AT+CMGDA=\"DEL ALL\"");
    delay(100);
    sendState = 1;
    setupState = 0;
  }
}
void sendFirstSMS() {
  if (sendState == 1) {
    sendSMS();
    sendState = 0;
  }
}
void sendSMS() {
  String message; //soilMoistureString = String(soilMoisture);
//  int soilMoisture = 0;
//  for (int i = 0; i <= 20; i++) {
//    int curr = readSoilHumidity();
//    if (curr > soilMoisture)
//      soilMoisture = curr;
//  }
  message = "Hi, ";
  message += "\n";
  message += "Farm condition at this time: ";
  message += "\n";
  message += "Irrigation valves: ";
  if (digitalRead(valvePin)) message += "ON";
  else message += "OFF";
  message += "\n";
  message += "Water pump: ";
  if (digitalRead(pumpPin)) message += "ON";
  else message += "OFF";
  message += "\n";
  message += "Soil Moisture: ";
  message += globalSoilMoist;
  message += "%.";
  message += "\n";
  message += "Mode: ";
  if (modeSelected == 1) message += "AUTOMATIC";
  else message += "MANUAL";
  message += "\n";
  /* stop sending the grid power state for now

  */
  //  message += "Grid power is: ";
  //  if (acState) message += "ON";
  //  else message += "OFF";
  message += "\n\n\n";
  message += "Farm smart, feed the nation...";
  mySerial.println("AT+CMGDA=\"DEL ALL\"");
  delay(50);
  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  delay(200);
  mySerial.println("AT+CMGS=\"" + phoneNum + "\""); // phone number to sms
  delay(200);
  mySerial.print(message); //text content
  delay(200);
  mySerial.write(26);
  delay(300);
  mySerial.flush();
  //}
}
void noSMS() {
  //  EEPROM.get(49, phoneNum);
  //if (phoneNum.startsWith("+234") && phoneNum.length() == 14) {
  String message;
//  int soilMoisture = 0;
//  for (int i = 0; i <= 20; i++) {
//    int curr = readSoilHumidity();
//    if (curr > soilMoisture)
//      soilMoisture = curr;
//  }
  message = "No Irrigation today, soil moisture is ";
  message += globalSoilMoist;
  message += "%";
  message += "\n\n\n\n";
  message += "Cheers";

  mySerial.println("AT+CMGDA=\"DEL ALL\"");
  delay(50);
  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  delay(200);
  mySerial.println("AT+CMGS=\"" + phoneNum + "\""); // phone number to sms
  delay(200);
  mySerial.print(message); //text content
  delay(200);
  mySerial.write(26);
  delay(200);
  mySerial.flush();
  // }
}
void waterLevelControl() {
  if (digitalRead(floatSwitch1) == LOW && digitalRead(floatSwitch2) == LOW && onState == 0) {
    onState = 1;
    EEPROM.put(1 * sizeof(onState), onState);
    digitalWrite(pumpPin, HIGH);
    sendSMS();
  }
  if (digitalRead(floatSwitch1) == HIGH && digitalRead(floatSwitch2) == HIGH && onState == 1) {
    onState = 0;
    EEPROM.put(1 * sizeof(onState), onState);
    digitalWrite(pumpPin, LOW);
    sendSMS();
  }
  if (onState == 1 && resumeState == 0) {
    resumeState = 1;
    digitalWrite(pumpPin, HIGH);
    sendSMS();
  }
}

void performAction() {
  int soilMoisture = readSoilHumidity();
  if (modeSelected == 1) {
    if ( (date.month == futureMonth) && (date.day == futureDay) && (time.hours == startHour)) {
      futureTime(interval);
      EEPROM.put(7 * sizeof(futureDay), futureDay);
      EEPROM.put(8 * sizeof(futureMonth), futureMonth);
      prevSoilMoisture = soilMoisture;
      if (soilMoisture < setPoint) {

        digitalWrite(valvePin, HIGH);
        sendSMS();
        state = HIGH;
        irrigationChrono.restart();
      }
      else
        noSMS();
    }

    int timing = duration * 60;
    if (irrigationChrono.hasPassed(timing) && state == HIGH) {
      digitalWrite(valvePin, LOW);
      sendSMS();
      state = LOW;
    }
    /*
       removing the error code
    */
    //    if (irrigationChrono.hasPassed(300) && state == HIGH && soilMoisture <= (prevSoilMoisture + 5) ) {
    //      digitalWrite(valvePin, LOW);
    //      sendSMS();
    //      state = LOW;
    //      errorState = 1;
    //      EEPROM.put(10 * sizeof(errorState), errorState);
    //    }
  }
  else {
    if (lastButton == 6) {

      irrigateSwitchState = !irrigateSwitchState;
      digitalWrite(valvePin, irrigateSwitchState);
      // sendSMS();
    }
  }
}

void currentTime() {
  if (rtc.isCounterUpdated()) {
    rtc.clearSecondFlag();
    epochTime = rtc.getTime();
    rtc.epochToTime(epochTime, time);
    rtc.epochToDate(epochTime, date);
  }
}
int readSoilHumidity() {
  int readingsBucket[40];
  int acc;
  analogReadResolution(12);
  for (int i = 0; i < 40; i++) {
    readingsBucket[i] = analogRead(soilSensorPin);
  }
  for (int i = 0; i < 40; i++) {
    acc += readingsBucket[i];
  }
  int value = acc / 40;
  int mappedValue = map(value, 3200, 1730, 0, 100);
  mappedValue = constrain(mappedValue, 0, 100);

  return mappedValue;
}

int batteryPercent(float voltage) {
  float rawPercent = ((voltage - 3.2) * 116.28); //93.4579
  int rpercent = rawPercent;
  rpercent = constrain(rpercent, 0, 100);
  return rpercent;
}

void reads(void) {
  //  static int count;
  analogReadResolution(12);
  int readBucket[AVG];
  int readBucket2;
  for (int i = 0; i < AVG; i++) {
    readBucket[i] = analogRead(battPin);
    delay(2);
  }
  for (int i = 0; i < AVG; i++) {
    readBucket2 += readBucket[i];
    delay(2);
  }
  digiValue = readBucket2 / AVG;
  float bucket[AVG];
  float bucket2;
  for (int i = 0; i < AVG; i++) {
    bucket[i] = ((digiValue * batFactor) / 4096.0);
    delay(5);
  }
  for (int i = 0; i < AVG; i++) {
    bucket2 += bucket[i];
    delay(5);
  }
  vBat = bucket2 / 60.0;
}
void checkBattery() {
  if (batState == 0) {
    reads();
    int bucket[AVG];
    int bucket2;
    for (int i = 0; i < AVG; i++) {
      bucket[i] = batteryPercent(vBat);
      delay(2);
    }
    for (int i = 0; i < AVG; i++) {
      bucket2 += bucket[i];
      delay(2);
    }
    percent = bucket2 / AVG;
    batState = 1;
    battChrono.restart();
  }
  if (batState == 1 && battChrono.hasPassed(60)) {
    batState = 0;
  }
}

void homeDisplay() {
#define TIME_HEIGHT 20
#define DATE_HEIGHT 32
  uint8_t width;
  currentTime();
  checkBattery();
  int soilmoisture = readSoilHumidity();
  if (soilmoisture > globalSoilMoist)
    globalSoilMoist = soilmoisture;
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_profont15_tr);// u8g2_font_ordinarybasis_tr
    u8g2.setCursor(80, 9);
    u8g2.print(percent);
    u8g2.setFont(u8g2_font_crox1c_tr);// u8g2_font_6x13_tr
    u8g2.print("%");
    u8g2.drawXBMP(113, 0, icons8_empty_battery_32_2__width, icons8_empty_battery_32_2__height, icons8_empty_battery_32_2__bits);

    width = (percent * CHARGE_AREA_WIDTH) / 100;

    u8g2.drawBox(CHARGE_AREA_START_X, CHARGE_AREA_START_Y, width, CHARGE_AREA_HEIGHT);
    // display time and date
    u8g2.setFont(u8g2_font_6x13B_tr);// u8g2_font_8x13B_tr
    u8g2.drawStr(0, TIME_HEIGHT, u8x8_u16toa(time.hours, 2));
    u8g2.drawStr(13, (TIME_HEIGHT - 1), ":");
    u8g2.drawStr(20, TIME_HEIGHT, u8x8_u16toa(time.minutes, 2));
    u8g2.drawStr(33, (TIME_HEIGHT - 1), ":");
    u8g2.drawStr(40, TIME_HEIGHT, u8x8_u16toa(time.seconds, 2));
    // u8g2.setFont( u8g2_font_6x13B_tr);//  u8g2_font_mozart_nbp_tr
    u8g2.drawStr(0, DATE_HEIGHT, u8x8_u16toa(date.day, 2));
    u8g2.drawStr(14, DATE_HEIGHT, "/");
    u8g2.drawStr(20, DATE_HEIGHT, u8x8_u16toa(date.month, 2));
    u8g2.drawStr(34, DATE_HEIGHT, "/");
    u8g2.drawStr(40, DATE_HEIGHT, u8x8_u16toa(date.year, 4));

    // display mode
    u8g2.setFont( u8g2_font_6x10_tr);
    u8g2.drawStr(25, 63, "AUTO");
    u8g2.drawStr(72, 63, "MANUAL");

    // display pumping and irrigating ON states
    if (digitalRead(pumpPin))
      u8g2.drawXBMP(24, (DATE_HEIGHT + 5), icons8_pump_16_width, icons8_pump_16_height, icons8_pump_16_bits);


    if (digitalRead(valvePin))
      u8g2.drawXBMP(0, (DATE_HEIGHT + 3), icons8_shower_24_20_1__width, icons8_shower_24_20_1__height, icons8_shower_24_20_1__bits);

    /*
       comment for now
       debounce before reconnecting
    */
    //    if (!digitalRead(AC_SENSE)) { // check if there is nepa light
    //      acState = 1;
    //      u8g2.drawXBMP(40, (DATE_HEIGHT + 3), energy_saver_width, energy_saver_height, energy_saver_bits);
    //    }
    //    else acState = 0;



    // manual mode
    if (modeSelected == 0) {
      // display manual mode pointer
      u8g2.setFont( u8g2_font_m2icon_7_tf);
      u8g2.drawGlyph(62, 62, 0x0052);
      // display soil moisture
      u8g2.drawXBMP(80, 20, icons8_moisture_24_20_1__width, icons8_moisture_24_20_1__height, icons8_moisture_24_20_1__bits);
      u8g2.setFont(u8g2_font_profont11_tr);// u8g2_font_ordinarybasis_tr
      // u8g2.setFontMode(1);
      u8g2.setCursor(95, 29);
      u8g2.print(soilmoisture);
      u8g2.print("%");
    }
    else { // automatic mode
      // get future weekday for irrigation if automatic mode futureEpochTime
      DateVar newDate;
      // TimeVar newTime;
      rtc.epochToDate(futureEpochTime, newDate);
      // rtc.epochToDateTime(futureEpochTime, newDate, newTime);
      // futureDay = newDate.day;

      // display irrigation time if automatic mode
      u8g2.setFont( u8g2_font_mozart_nbp_tr);
      u8g2.drawStr(80, TIME_HEIGHT, u8x8_u16toa(startHour, 2));
      u8g2.drawStr(94, (TIME_HEIGHT - 1), ":");
      u8g2.drawStr(100, TIME_HEIGHT, u8x8_u16toa(0, 2));
      u8g2.drawStr(90, (DATE_HEIGHT - 4), weekdayName[newDate.weekday]);

      //display bell icon
      u8g2.setFont( u8g2_font_open_iconic_embedded_1x_t);
      u8g2.drawGlyph(115, (TIME_HEIGHT + 5), 0x0041);

      // display automatic mode pointer
      u8g2.setFont( u8g2_font_m2icon_7_tf);
      u8g2.drawGlyph(15, 62, 0x0052);

      // display soil moisture
      u8g2.drawXBMP(86, 31, icons8_moisture_24_20_1__width, icons8_moisture_24_20_1__height, icons8_moisture_24_20_1__bits);
      u8g2.setFont(u8g2_font_profont11_tr);// u8g2_font_ordinarybasis_tr
      // u8g2.setFontMode(1);
      u8g2.setCursor(100, 41);
      u8g2.print(soilmoisture);
      u8g2.print("%");
    }
    //display error icon
    //    if (errorState) {
    //      u8g2.setFont( u8g2_font_open_iconic_embedded_1x_t);
    //      u8g2.drawGlyph(0, 8, 0x0047);
    //    }


  } while ( u8g2.nextPage() );
}

void mainMenu() {
  readButtons();
  switch (lastButton) {
    case 1: {
        firstMenuPage();
        break;
      }
      //    case 5:
      //      errorState = 0;
      //      EEPROM.put(10 * sizeof(errorState), errorState);
      //      break;
  }
}

void showMenu() {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_timB10_tr);
    u8g2.setFontMode(1);
    u8g2.drawStr(44, 10, "MENU"); //u8g2_font_u8glib_4_tf
    u8g2.drawLine(0, 11, 127, 11);
    u8g2.setFont(u8g2_font_crox1hb_tr);
    cursorB = 0;
    // show menu items:
    int offset = 22;
    for (int i = 0; i < MENU_SIZE; i++) {
      u8g2.drawStr(6, offset, menu[i]);
      offset += 12;
    }
    u8g2.setDrawColor(2);
    u8g2.drawBox(0, 12, u8g2.getDisplayWidth(), 10);
  } while ( u8g2.nextPage() );

}

/**
   Clear display and show the new menu when automatic mode is selected.
*/

void showNewMenu() {
  u8g2.firstPage();
  do {
    cursorA = 0;
    showNewMenuCursor();
    u8g2.drawBox(0, 10, u8g2.getDisplayWidth(), 8);
  } while ( u8g2.nextPage() );

}


void turnOffBacklight() {
  if (lcdChrono.hasPassed(10)) {
    digitalWrite(lcdBacklight, HIGH);
    //  lcdState = 0;
  }
}
/**
   Read push buttons
 * **/
void readButtons() {
  if (!digitalRead(PB12) && previousState == HIGH && millis() - timeCount > debounce) {
    digitalWrite(lcdBacklight, LOW);
    lcdChrono.restart();
    lastButton = 1;
    timeCount = millis();
    previousState = !previousState;
  }
  else if (!digitalRead(PB13) && previousState == HIGH && millis() - timeCount > debounce) {
    digitalWrite(lcdBacklight, LOW);
    lcdChrono.restart();
    lastButton = 2;
    timeCount = millis();
    previousState = !previousState;
  }
  else if (!digitalRead(PB14) && previousState == HIGH && millis() - timeCount > debounce) {
    digitalWrite(lcdBacklight, LOW);
    lcdChrono.restart();
    lastButton = 3;
    timeCount = millis();
    previousState = !previousState;
  }
  else if (!digitalRead(PA8) && previousState == HIGH && millis() - timeCount > debounce) {
    digitalWrite(lcdBacklight, LOW);
    lcdChrono.restart();
    lastButton = 4;
    timeCount = millis();
    previousState = !previousState;
  }
  else if (!digitalRead(PB15) && previousState == HIGH && millis() - timeCount > debounce) {
    digitalWrite(lcdBacklight, LOW);
    lcdChrono.restart();
    lastButton = 5;
    timeCount = millis();
    previousState = !previousState;
  }
  else if (!digitalRead(PB5) && previousState == HIGH && millis() - timeCount > 250) {
    digitalWrite(lcdBacklight, LOW);
    lcdChrono.restart();
    lastButton = 6;
    timeCount = millis();
    previousState = !previousState;
  }
  else {
    lastButton = 0;
    previousState = !previousState;
  }
  //  }
}

void firstMenuPage() {
  lastButton = 0;
  showMenu();
  while (lastButton != 1) {
    // delay(60);
    readButtons();
    if (lastButton == 1) { // select
      executeChoice(cursorB);
    }
    else {
      // scroll through menu:
      if (lastButton == 3) { // down
        lastButton = 0;
        cursorB++;
        if (cursorB > (MENU_SIZE - 1)) cursorB = 0;
      }
      else if (lastButton == 2) { // up
        lastButton = 0;
        cursorB--;
        if (cursorB < 0) cursorB = (MENU_SIZE - 1);
      }
      // show cursor at new line:
      displayMenuCursor();

    } // end else scroll menu...
  } // end while loop for menu control
  delay(60);
}

void displayMenuCursor() {
  switch (cursorB) {
    case 0:
      u8g2.firstPage();
      do {
        u8g2.setFont(u8g2_font_timB10_tr);
        u8g2.setFontMode(1);
        u8g2.drawStr(44, 10, "MENU"); //u8g2_font_u8glib_4_tf
        u8g2.drawLine(0, 11, 127, 11);

        u8g2.setFont(u8g2_font_crox1hb_tr);
        int offset = 22;
        for (int i = 0; i < MENU_SIZE; i++) {
          u8g2.drawStr(6, offset, menu[i]);
          offset += 12;
        }
        u8g2.setDrawColor(2);
        u8g2.drawBox(0, 12, u8g2.getDisplayWidth(), 10);
      } while ( u8g2.nextPage() );
      break;
    case 1:
      u8g2.firstPage();
      do {
        u8g2.setFont(u8g2_font_timB10_tr);
        u8g2.setFontMode(1);
        u8g2.drawStr(44, 10, "MENU"); //u8g2_font_u8glib_4_tf
        u8g2.drawLine(0, 11, 127, 11);

        u8g2.setFont(u8g2_font_crox1hb_tr);
        int offset = 22;
        for (int i = 0; i < MENU_SIZE; i++) {
          u8g2.drawStr(6, offset, menu[i]);
          offset += 12;
        }
        u8g2.setDrawColor(2);
        u8g2.drawBox(0, 24, u8g2.getDisplayWidth(), 10);
      } while ( u8g2.nextPage() );
      break;
    case 2:
      u8g2.firstPage();
      do {
        u8g2.setFont(u8g2_font_timB10_tr);
        u8g2.setFontMode(1);
        u8g2.drawStr(44, 10, "MENU"); //u8g2_font_u8glib_4_tf
        u8g2.drawLine(0, 11, 127, 11);

        u8g2.setFont(u8g2_font_crox1hb_tr);
        int offset = 22;
        for (int i = 0; i < MENU_SIZE; i++) {
          u8g2.drawStr(6, offset, menu[i]);
          offset += 12;
        }
        u8g2.setDrawColor(2);
        u8g2.drawBox(0, 36, u8g2.getDisplayWidth(), 10);
      } while ( u8g2.nextPage() );
      break;
    case 3:
      u8g2.firstPage();
      do {
        u8g2.setFont(u8g2_font_timB10_tr);
        u8g2.setFontMode(1);
        u8g2.drawStr(44, 10, "MENU"); //u8g2_font_u8glib_4_tf
        u8g2.drawLine(0, 11, 127, 11);

        u8g2.setFont(u8g2_font_crox1hb_tr);
        int offset = 22;
        for (int i = 0; i < MENU_SIZE; i++) {
          u8g2.drawStr(6, offset, menu[i]);
          offset += 12;
        }
        u8g2.setDrawColor(2);
        u8g2.drawBox(0, 48, u8g2.getDisplayWidth(), 10);
      } while ( u8g2.nextPage() );
      break;

  }
}


/* calculate a future date */
void futureTime(int addValue) {
  futureEpochTime = (rtc.getTime() + (addValue * 86400));
  DateVar newDate;
  TimeVar newTime;
  rtc.epochToDateTime(futureEpochTime, newDate, newTime);
  futureDay = newDate.day;
  futureMonth = newDate.month;
  futureYr = newDate.year;
  EEPROM.put(12 * sizeof(futureEpochTime), futureEpochTime);
}
/**
   Execute the task which matches the chosen menu item.
*/
void executeChoice(int choice) {
  if (choice == 0) {
    showNewMenu();
    while (true) {
      delay(60);
      readButtons();
      // scroll through menu:
      if (lastButton == 3) { // down
        lastButton = 0;
        cursorA++;
        if (cursorA > (NEW_MENU_SIZE - 1)) cursorA = 0;
      }
      else if (lastButton == 2) { // up
        lastButton = 0;
        cursorA--;
        if (cursorA < 0) cursorA = (NEW_MENU_SIZE - 1);
      }
      // show cursor at new line:
      displayAutomaticModeCursor();
      if (cursorA == 5 && lastButton == 1) {
        modeSelected = 1;
        // save automatic mode and variables here
        futureTime(startDate);
        EEPROM.put(3 * sizeof(modeSelected), modeSelected);
        EEPROM.put(4 * sizeof(startDate), startDate);
        EEPROM.put(5 * sizeof(startHour), startHour);
        EEPROM.put(6 * sizeof(interval), interval);
        EEPROM.put(7 * sizeof(futureDay), futureDay);
        EEPROM.put(8 * sizeof(futureMonth), futureMonth);
        EEPROM.put(9 * sizeof(duration), duration);
        EEPROM.put(11 * sizeof(setPoint), setPoint);
        break;
      }
      if (cursorA == 6 && lastButton == 1) {
        firstMenuPage();
        break;
      }
    }
  }
  if (choice == 1) {
    modeSelected = 0;
    //save manual mode here
    EEPROM.put(3 * sizeof(modeSelected), modeSelected);
  }
  if (choice == 2) {
    date_time_menu();
    //display.clearDisplay();
    //lastButton = 0;
  }
  if (choice == 3) {
    exit;
  }
  if (choice == 4) {
    lastButton = 0;
    cursor = 0;
    showSetDateMenu();
    while (lastButton != 1) {
      delay(60);
      readButtons();
      if (lastButton == 1) { // select
        epochTime = rtc.dateTimeToEpoch(date, time);
        rtc.setTime(epochTime);
        date_time_menu();
      }
      else {
        if (lastButton == 2) { // up
          lastButton = 0;
          cursor++;
          if (cursor > 2) cursor = 0;
        }
        else if (lastButton == 3) { // down
          lastButton = 0;
          cursor--;
          if (cursor < 0) cursor = 2;
        }
        // show cursor at new line:
        displayDateSettingCursor();
      }
    }
  }
  if (choice == 5) {
    lastButton = 0;
    cursor = 0;
    showSetTimeMenu();
    while (lastButton != 1) {
      delay(60);
      readButtons();
      if (lastButton == 1) { // select
        epochTime = rtc.dateTimeToEpoch(date, time);
        rtc.setTime(epochTime);
        date_time_menu();
      }
      else {
        if (lastButton == 2) { // up
          lastButton = 0;
          cursor++;
          if (cursor > 2) cursor = 0;
        }
        else if (lastButton == 3) { // down
          lastButton = 0;
          cursor--;
          if (cursor < 0) cursor = 2;
        }
        // show cursor at new line:
        displayTimeSettingCursor();
      }
    }
  }
  //}
}
void showDateMenuCursor() {
  u8g2.setFont(u8g2_font_crox1hb_tr); // u8g2_font_timB10_tr old font
  u8g2.setFontMode(1);
  u8g2.drawStr(18, 10, "SET DATE/TIME"); //u8g2_font_u8glib_4_tf
  u8g2.drawLine(0, 11, 127, 11);
  u8g2.setFont(u8g2_font_crox1hb_tr);
  // show menu items:
  int offset = 22;
  for (int i = 0; i < DATE_MENU_SIZE; i++) {
    u8g2.drawStr(6, offset, date_menu[i]);
    offset += 12;
  }
  u8g2.setDrawColor(2);
}
/**
   Clear display and show the new menu.
*/
void showDateMenu() {
  cursor = 0;
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_crox1hb_tr);
    u8g2.setFontMode(1);
    u8g2.drawStr(18, 10, "SET DATE/TIME"); //u8g2_font_u8glib_4_tf
    u8g2.drawLine(0, 11, 127, 11);
    u8g2.setFont(u8g2_font_crox1hb_tr);
    // show menu items:
    int offset = 22;
    for (int i = 0; i < DATE_MENU_SIZE; i++) {
      u8g2.drawStr(6, offset, date_menu[i]);
      offset += 12;
    }
    u8g2.setDrawColor(2);
    u8g2.drawBox(0, 12, u8g2.getDisplayWidth(), 10);
  } while ( u8g2.nextPage() );

}
void displayDateTimeCursor() {
  switch (cursor) {
    case 0:
      u8g2.firstPage();
      do {
        showDateMenuCursor();
        u8g2.drawBox(0, 12, u8g2.getDisplayWidth(), 10);
      } while ( u8g2.nextPage() );
      break;
    case 1:
      u8g2.firstPage();
      do {
        showDateMenuCursor();
        u8g2.drawBox(0, 24, u8g2.getDisplayWidth(), 10);
      } while ( u8g2.nextPage() );
      break;
    case 2:
      u8g2.firstPage();
      do {
        showDateMenuCursor();
        u8g2.drawBox(0, 36, u8g2.getDisplayWidth(), 10);
      } while ( u8g2.nextPage() );
      break;
  }
}
void date_time_menu() {

  showDateMenu();
  while (true) {
    delay(60);
    readButtons();
    // scroll through menu:
    if (lastButton == 3) { // down
      lastButton = 0;
      cursor++;
      if (cursor > (DATE_MENU_SIZE - 1)) cursor = 0;
    }
    else if (lastButton == 2) { // up
      lastButton = 0;
      cursor--;
      if (cursor < 0) cursor = (DATE_MENU_SIZE - 1);
    }

    // show cursor at new line:
    //    display.setCursor(0, cursor);
    //    display.print('>');
    displayDateTimeCursor();
    if (cursor == 0) {
      if (lastButton == 1) { // select
        lastButton = 0;
        executeChoice(4);
      }
    }
    if (cursor == 1) {
      if (lastButton == 1) {
        lastButton = 0;
        executeChoice(5);
      }
    }
    if (cursor == 2) {
      if (lastButton == 1) {
        //lastButton = 0;
        //display.clearDisplay();
        break;
      }
    }
  }
}

/**
   Clear display and show the new menu.
*/
void showSetDateMenu() {
  // cursor = 0;
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_helvB08_tr);// u8g2_font_timB10_tr
    u8g2.setFontMode(1);
    u8g2.drawStr(35, 8, "SET DATE");
    u8g2.drawLine(0, 9, 127, 9);
    u8g2.setFont(u8g2_font_helvB08_tr); //u8g2_font_luBS08_tr

    // show menu items:
    int offset = 0;
    for (int i = 0; i < DATE_SETTING_SIZE; i++) {
      u8g2.drawStr(offset, 18, date_setting[i]);
      if (i == 1) offset += 50;
      else offset += 40;
    }
    u8g2.setDrawColor(2);
    u8g2.drawBox(0, 10, 35, 8);
  } while ( u8g2.nextPage() );
}

void showDateSettingCursor() {
  u8g2.setFont(u8g2_font_helvB08_tr);// u8g2_font_timB10_tr
  u8g2.setFontMode(1);
  u8g2.drawStr(35, 8, "SET DATE");
  u8g2.drawLine(0, 9, 127, 9);
  u8g2.setFont(u8g2_font_helvB08_tr); //u8g2_font_luBS08_tr

  // show menu items:
  int offset = 0;
  for (int i = 0; i < DATE_SETTING_SIZE; i++) {
    u8g2.drawStr(offset, 18, date_setting[i]);
    if (i == 1) offset += 50;
    else offset += 40;
  }
  u8g2.setDrawColor(2);
}
void displayDateSettingCursor() {
  int arr[3] = {0, 38, 88};
  switch (cursor) {
    case 0:
      u8g2.firstPage();
      do {
        showDateSettingCursor();
        u8g2.drawBox(arr[cursor], 10, 30, 8);
        if (lastButton == 4) { // up
          lastButton = 0;
          yr++;
          if (yr > 2030) yr = 2017;
        }
        else if (lastButton == 5) { // countdown
          lastButton = 0;
          yr--;
          if (yr < 2017) yr = 2030;
        }
        date.year = yr;
        u8g2.drawStr(2, 27, u8x8_u16toa(yr, 4));
        u8g2.drawStr(55, 27, u8x8_u16toa(mth, 2));
        u8g2.drawStr(96, 27, u8x8_u16toa(dy, 2));
      } while ( u8g2.nextPage() );
      break;
    case 1:
      u8g2.firstPage();
      do {
        showDateSettingCursor();
        u8g2.drawBox(arr[cursor], 10, 43, 8);
        if (lastButton == 4) { // up
          lastButton = 0;
          mth++;
          if (mth > 12) mth = 1;
        }
        else if (lastButton == 5) { // countdown
          lastButton = 0;
          mth--;
          if (mth < 1) mth = 12;
        }
        date.month = mth;
        u8g2.drawStr(2, 27, u8x8_u16toa(yr, 4));
        u8g2.drawStr(55, 27, u8x8_u16toa(mth, 2));
        u8g2.drawStr(96, 27, u8x8_u16toa(dy, 2));
      } while ( u8g2.nextPage() );
      break;
    case 2:
      u8g2.firstPage();
      do {
        showDateSettingCursor();
        u8g2.drawBox(arr[cursor], 10, 26, 8);
        if (lastButton == 4) { // up
          lastButton = 0;
          dy++;
          if (dy > 31) dy = 1;
        }
        else if (lastButton == 5) { // countdown
          lastButton = 0;
          dy--;
          if (dy < 1) dy = 31;
        }
        date.day = dy;
        u8g2.drawStr(2, 27, u8x8_u16toa(yr, 4));
        u8g2.drawStr(55, 27, u8x8_u16toa(mth, 2));
        u8g2.drawStr(96, 27, u8x8_u16toa(dy, 2));
      } while ( u8g2.nextPage() );
      break;
  }
}

void showSetTimeMenu() {
  // cursor = 0;
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_helvB08_tr);// u8g2_font_timB10_tr
    u8g2.setFontMode(1);
    u8g2.drawStr(35, 8, "SET TIME");
    u8g2.drawLine(0, 9, 127, 9);
    u8g2.setFont(u8g2_font_helvB08_tr); //u8g2_font_luBS08_tr

    // show menu items:
    int offset = 1;
    for (int i = 0; i < TIME_SETTING_SIZE; i++) {
      u8g2.drawStr(offset, 18, time_setting[i]);
      if (i == 1) offset += 45;
      else offset += 37;
    }
    u8g2.setDrawColor(2);
    u8g2.drawBox(0, 10, 35, 8);
  } while ( u8g2.nextPage() );
}

void showTimeSettingCursor() {
  u8g2.setFont(u8g2_font_helvB08_tr);// u8g2_font_timB10_tr
  u8g2.setFontMode(1);
  u8g2.drawStr(35, 8, "SET TIME");
  u8g2.drawLine(0, 9, 127, 9);
  u8g2.setFont(u8g2_font_helvB08_tr); //u8g2_font_luBS08_tr

  // show menu items:
  int offset = 1;
  for (int i = 0; i < TIME_SETTING_SIZE; i++) {
    u8g2.drawStr(offset, 18, time_setting[i]);
    if (i == 1) offset += 45;
    else offset += 37;
  }
  u8g2.setDrawColor(2);
}
void timeDisplay() {
  u8g2.drawStr(10, 27, u8x8_u16toa(hr, 2));
  u8g2.drawStr(55, 27, u8x8_u16toa(mins, 2));
  u8g2.drawStr(100, 27, u8x8_u16toa(sec, 2));

}
void displayTimeSettingCursor() {
  int arr[3] = {0, 36, 82};
  switch (cursor) {
    case 0:
      u8g2.firstPage();
      do {
        showTimeSettingCursor();
        u8g2.drawBox(arr[cursor], 10, 30, 8);
        if (lastButton == 4) { // up
          lastButton = 0;
          hr++;
          if (hr > 23) hr = 0;
        }
        else if (lastButton == 5) { // countdown
          lastButton = 0;
          hr--;
          if (hr < 0) hr = 23;
        }
        time.hours = hr;
        timeDisplay();
      } while ( u8g2.nextPage() );
      break;
    case 1:
      u8g2.firstPage();
      do {
        showTimeSettingCursor();
        u8g2.drawBox(arr[cursor], 10, 43, 8);
        if (lastButton == 4) { // up
          lastButton = 0;
          mins++;
          if (mins > 59) mins = 0;
        }
        else if (lastButton == 5) { // countdown
          lastButton = 0;
          mins--;
          if (mins < 0) mins = 59;
        }
        time.minutes = mins;
        timeDisplay();
      } while ( u8g2.nextPage() );
      break;
    case 2:
      u8g2.firstPage();
      do {
        showTimeSettingCursor();
        u8g2.drawBox(arr[cursor], 10, 46, 8);
        if (lastButton == 4) { // up
          lastButton = 0;
          sec++;
          if (sec > 59) sec = 0;
        }
        else if (lastButton == 5) { // countdown
          lastButton = 0;
          sec--;
          if (sec < 0) sec = 59;
        }
        time.seconds = sec;
        timeDisplay();
      } while ( u8g2.nextPage() );
      break;
  }
}

void displayAutomaticModeCursor() {
  switch (cursorA) {
    case 0:
      u8g2.firstPage();
      do {
        showNewMenuCursor();
        u8g2.drawBox(0, 10, u8g2.getDisplayWidth(), 8);
        if (lastButton == 4) { // up
          lastButton = 0;
          startDate++;
          if (startDate > 7) startDate = 0;
        }
        else if (lastButton == 5) { // countdown
          lastButton = 0;
          startDate--;
          if (startDate < 0) startDate = 7;
        }

        u8g2.setCursor(DAY_ROW, DAY_COL);
        u8g2.print(day_to_start[startDate]); //
        u8g2.print("> ");
        u8g2.drawStr(35, 27, u8x8_u16toa(startHour, 2));
        u8g2.drawStr(62, 36, u8x8_u16toa(interval, 1));
        u8g2.drawStr(DURATION_ROW, DURATION_COL, u8x8_u16toa(duration, 3));
        u8g2.drawStr(62, 54, u8x8_u16toa(setPoint, 3));
      } while ( u8g2.nextPage() );
      break;

    case 1:
      u8g2.firstPage();
      do {
        showNewMenuCursor();
        u8g2.drawBox(0, 19, u8g2.getDisplayWidth(), 8);
        if (lastButton == 4) { // up
          lastButton = 0;
          startHour++;
          if (startHour > 23) startHour = 0;
        }
        else if (lastButton == 5) { // countdown
          lastButton = 0;
          startHour--;
          if (startHour < 0) startHour = 23;
        }
        u8g2.drawStr(35, 27, u8x8_u16toa(startHour, 2));
        u8g2.setCursor(DAY_ROW, DAY_COL);
        u8g2.print(day_to_start[startDate]); //
        u8g2.print("> ");
        u8g2.drawStr(62, 36, u8x8_u16toa(interval, 1));
        u8g2.drawStr(DURATION_ROW, DURATION_COL, u8x8_u16toa(duration, 3));
        u8g2.drawStr(62, 54, u8x8_u16toa(setPoint, 3));
      } while ( u8g2.nextPage() );
      break;

    case 2:
      u8g2.firstPage();
      do {
        showNewMenuCursor();
        u8g2.drawBox(0, 28, u8g2.getDisplayWidth(), 8);
        if (lastButton == 4) { // up
          lastButton = 0;
          interval++;
          if (interval > 7) interval = 0;
        }
        else if (lastButton == 5) { // countdown
          lastButton = 0;
          interval--;
          if (interval < 0) interval = 7;
        }
        u8g2.drawStr(62, 36, u8x8_u16toa(interval, 1));
        u8g2.setCursor(DAY_ROW, DAY_COL);
        u8g2.print(day_to_start[startDate]); //
        u8g2.print("> ");
        u8g2.drawStr(35, 27, u8x8_u16toa(startHour, 2));
        u8g2.drawStr(DURATION_ROW, DURATION_COL, u8x8_u16toa(duration, 3));
        u8g2.drawStr(62, 54, u8x8_u16toa(setPoint, 3));
      } while ( u8g2.nextPage() );
      break;

    case 3:
      u8g2.firstPage();
      do {
        showNewMenuCursor();
        u8g2.drawBox(0, 37, u8g2.getDisplayWidth(), 8);
        if (lastButton == 4) { // up
          lastButton = 0;
          if (duration < 10)duration++;
          else duration += 10;
          if (duration > 300) duration = 0;
        }
        else if (lastButton == 5) { // countdown
          lastButton = 0;
          if (duration <= 10)duration--;
          else duration -= 10;
          if (duration < 0) duration = 300;
        }
        u8g2.drawStr(DURATION_ROW, DURATION_COL, u8x8_u16toa(duration, 3));
        u8g2.setCursor(DAY_ROW, DAY_COL);
        u8g2.print(day_to_start[startDate]); //
        u8g2.print("> ");
        u8g2.drawStr(35, 27, u8x8_u16toa(startHour, 2));
        u8g2.drawStr(62, 36, u8x8_u16toa(interval, 1));
        u8g2.drawStr(62, 54, u8x8_u16toa(setPoint, 3));
      } while ( u8g2.nextPage() );
      break;

    case 4:
      u8g2.firstPage();
      do {
        showNewMenuCursor();
        u8g2.drawBox(0, 46, u8g2.getDisplayWidth(), 8);
        if (lastButton == 4) { // up
          lastButton = 0;
          setPoint += 5;
          if (setPoint > 100) setPoint = 0;
        }
        else if (lastButton == 5) { // countdown
          lastButton = 0;
          setPoint -= 5;
          if (setPoint < 0) setPoint = 100;
        }

        u8g2.drawStr(62, 54, u8x8_u16toa(setPoint, 3));
        u8g2.setCursor(DAY_ROW, DAY_COL);
        u8g2.print(day_to_start[startDate]); //
        u8g2.print("> ");
        u8g2.drawStr(35, 27, u8x8_u16toa(startHour, 2));
        u8g2.drawStr(62, 36, u8x8_u16toa(interval, 1));
        u8g2.drawStr(DURATION_ROW, DURATION_COL, u8x8_u16toa(duration, 3));
      } while ( u8g2.nextPage() );
      break;

    case 5:
      u8g2.firstPage();
      do {
        showNewMenuCursor();
        u8g2.drawBox(0, 55, u8g2.getDisplayWidth(), 8);
        u8g2.setCursor(DAY_ROW, DAY_COL);
        u8g2.print(day_to_start[startDate]); //
        u8g2.print("> ");
        u8g2.drawStr(35, 27, u8x8_u16toa(startHour, 2));
        u8g2.drawStr(62, 36, u8x8_u16toa(interval, 1));
        u8g2.drawStr(DURATION_ROW, DURATION_COL, u8x8_u16toa(duration, 3));
        u8g2.drawStr(62, 54, u8x8_u16toa(setPoint, 3));
      } while ( u8g2.nextPage() );
      break;

    case 6:
      u8g2.firstPage();
      do {
        u8g2.setFont(u8g2_font_helvB08_tr);
        u8g2.setFontMode(1);
        u8g2.drawStr(15, 8, "AUTOMATIC MODE");// prints mode
        u8g2.drawLine(0, 9, 127, 9);// underlines the "automatic mode"
        u8g2.setFont(u8g2_font_helvB08_tr); //u8g2_font_luBS08_tr
        // show menu items:
        int offset = 18;
        for (int i = 1; i < NEW_MENU_SIZE; i++) {
          u8g2.drawStr(1, offset, new_menu[i]);
          offset += 9;
        }

        u8g2.setDrawColor(2);
        u8g2.drawBox(0, 55, u8g2.getDisplayWidth(), 8);
        //        u8g2.setCursor(DAY_ROW, DAY_COL);
        //        u8g2.print(day_to_start[startDate]); //
        //        u8g2.print("> ");
        u8g2.drawStr(35, 18, u8x8_u16toa(startHour, 2));
        u8g2.drawStr(62, 27, u8x8_u16toa(interval, 1));
        u8g2.drawStr(DURATION_ROW, 36, u8x8_u16toa(duration, 3));
        u8g2.drawStr(62, 45, u8x8_u16toa(setPoint, 3));
      } while ( u8g2.nextPage() );
      break;

  }
}

// shows when automatic mode is selected
void showNewMenuCursor() {
  u8g2.setFont(u8g2_font_helvB08_tr);
  u8g2.setFontMode(1);
  u8g2.drawStr(15, 8, "AUTOMATIC MODE");// prints mode
  u8g2.drawLine(0, 9, 127, 9);// underlines the "automatic mode"
  u8g2.setFont(u8g2_font_helvB08_tr); //u8g2_font_luBS08_tr
  // show menu items:
  int offset = 18;
  for (int i = 0; i < NEW_MENU_SIZE; i++) {
    u8g2.drawStr(1, offset, new_menu[i]);
    offset += 9;
  }
  u8g2.setDrawColor(2);
}
