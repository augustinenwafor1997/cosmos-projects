#define reedSwitch PA15
#define battPin PB1
#define STROKE 0.425
#define firstHour 12
#define secondHour 18
#define thirdHour 00
#define AVG_NUM 10               // number of iterations of the adc routine to average the adc readings
#define batFactor 10.35
#define AVG 60

const int maxInput = 70;
float vBat, batsample;
unsigned long timeCount = millis();
bool batState = 0, previousState = LOW, gprsState, sendState, countState, messState, recvState, dateRecv, placeRecv, smssetupState, smsState, setupState;
int count, debounce = 200, totalCount, digiValue;
double totalLitres;
String incomingMessage, senderNum, messageBody, date_time_string, locationString, coordinates, phoneNum;
#include <Chrono.h>
#include <EEPROM.h>
#include <Wire.h>
#include <STM32RTC.h>
#include <TimeLib.h>
STM32RTC& rtc = STM32RTC::getInstance();

#define mySerial Serial1
/* chrono instances */
Chrono countChrono(Chrono::SECONDS);
Chrono SMSChrono(Chrono::SECONDS);

enum operateState {startup, runing} state;
enum liters {Full, Empty} totalState;
enum sendTimes {firstPush, secondPush, thirdPush} sendCount;
enum simMode {sms, gprs} mode;

#define TINY_GSM_MODEM_SIM800
#include <TinyGsmClient.h>
TinyGsm modem(Serial1);
TinyGsmClient client(modem);
// adafruit client
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME  "nyammiri"
#define AIO_KEY       "aio_QJZk43FP4JjO0ydg83emntp315Hq"
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Publish data = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/data");
boolean MQTT_connect();
boolean MQTT_connect() {
  int8_t ret;
  if (mqtt.connected()) {
    return true;
  } // uint8_t retries = 3;
  if ((ret = mqtt.connect()) != 0) {
    mqtt.disconnect();
    return false;
  } else return true;
}
void reads(void);
void recvSMS(void);
void setup() {
  // put your setup code here, to run once:
  pinMode(reedSwitch, INPUT_PULLUP);
  pinMode(battPin, INPUT);
  mySerial.begin(9600);
  setupState = 1;
  rtc.setClockSource(STM32RTC::LSE_CLOCK);
  rtc.begin();
  SMSChrono.restart();
  state = startup;
  HardwareTimer *MyTim = new HardwareTimer(TIM1);
  MyTim->pause();
  MyTim->setOverflow(10000, HERTZ_FORMAT);
  MyTim->attachInterrupt(recvSMS);
  MyTim->resume();
}

void loop() {
  // put your main code here, to run repeatedly:
  readSwitch();// detect the strokes from pump operation
  totalLiters();// calculate quantity of water in litres
  sendDATA();// send the data at specified time intervals
  setcoordinates();// save system location data
  setDateTime();// synchronise system date and time with network date and time
  sendReply();// reply user
  SMSconfig();// configure sms mode
  gprsConfig();// configure gprs mode
  sendFirstSMS();// send initial sms on system startup
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
int checkBattery() {
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
  int percent = bucket2 / AVG;
  return percent;
}


void readSwitch() {
  if (!digitalRead(reedSwitch) && previousState == HIGH && millis() - timeCount > debounce) {
    timeCount = millis();
    previousState = !previousState;
    if (state == runing) {
      if (countChrono.hasPassed(3600)) {
        countChrono.restart();
        totalCount = count;
        totalState = Full;
        count = 1;
      }
      else count += 1;
    }
    if (state == startup) {
      count += 1;
      countChrono.restart();
      state = runing;
    }
  }
}

void totalLiters() {
  if (totalState == Full) {
    // assuming a conversion rate of 1liter/2.35 strokes
    double litres = totalCount * STROKE;
    totalLitres += litres;
    //save here
    totalState = Empty;
    totalCount = 0;
  }
}

void sendDATA() {
  if ((sendCount == firstPush && rtc.getHours() == firstHour ) || (sendCount == secondPush && rtc.getHours() == secondHour ) || (sendCount == thirdPush && rtc.getHours() == thirdHour )) {
    if (mode == gprs)
      sendData();
    else if (mode == sms)
      sendSMS();
    if (sendCount == firstPush)
      sendCount = secondPush;
    else if (sendCount == secondPush)
      sendCount = thirdPush;
    else if (sendCount == thirdPush)
      sendCount = firstPush;
  }
}
void EEPROMwrite(int addrOffset, const String & strToWrite)
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
  data[newStrLen] = '\0';
  return String(data);
}
void recvSMS(void) {
  checkSerial();
  if (messState == 1) {
    String checkValid = getValue(getValue(incomingMessage, '+', 1), ':', 0);
    if (checkValid == "CMT") {
      senderNum = getValue(getValue(incomingMessage, '"', 1), '"', 0);
      messageBody = getValue(getValue(incomingMessage, '"', 1), '"', 1);
      recvState = 1;
    }
    if (checkValid == "CCLK") {
      date_time_string = getValue(getValue(incomingMessage, '"', 1), '+', 0);
      dateRecv = 1;
    }
    if (checkValid == "CIPGSMLOC") {
      locationString = getValue(incomingMessage, ':', 1);
      placeRecv = 1;
    }
    messState = 0;
    incomingMessage = "";
  }
}
void setcoordinates() {
  if (placeRecv == 1) {
    String Long = getValue(locationString, ',', 1);
    String Lat = getValue(locationString, ',', 2);
    coordinates = Lat + ',' + Long;
    //save coordinates here
    locationString = "";
    placeRecv = 0;
  }
}
void setDateTime() {
  if (dateRecv == 1) {
    String Date = getValue(date_time_string, ',', 0);
    String Time = getValue(date_time_string, ',', 1);
    String yr = getValue(Date, '/', 0);
    String mth = getValue(Date, '/', 1);
    String dy = getValue(Date, '/', 2);
    String hr = getValue(Time, ':', 0);
    String mins = getValue(Time, ':', 1);
    String sec = getValue(Time, ':', 2);
    rtc.setDate(dy.toInt(), mth.toInt(), yr.toInt());
    rtc.setTime(hr.toInt(), mins.toInt(), sec.toInt());
    dateRecv = 0;
    date_time_string = "";
  }
}
void sendReply() {
  if (recvState == 1 && messState == 0) {
    if (messageBody.startsWith("MODE:") && senderNum.startsWith("+234") && senderNum.length() == 14) {
      EEPROMwrite(49, senderNum);
      senderNum = phoneNum;
      mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
      delay(100);
      mySerial.println("AT+CMGS=\"" + senderNum + "\"");
      delay(100);
      if (getValue(messageBody, ':', 1) == "gprs") {
        mode = gprs;
        gprsState = 1;
        smsState = 0;
        mySerial.print("GPRS mode saved"); //text content
      }
      else if (getValue(messageBody, ':', 1) == "sms") {
        mode = sms;
        gprsState = 0;
        smsState = 1;
        mySerial.print("SMS mode saved"); //text content
      }
      delay(100);
      mySerial.write(26);
    }
    if (messageBody.startsWith("PING") && senderNum.startsWith("+234") && senderNum.length() == 14) {
      EEPROMwrite(49, senderNum);
      sendSMS();
    }
    messageBody = "";
    senderNum = "";
    recvState = 0;
  }
}
void checkSerial() {
  if (mySerial.available()) {
    while (mySerial.available()) {
      processIncomingByte(mySerial.read());
    }
    mySerial.flush();
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
  if ((smssetupState == 1) || (smsState == 1)) {
    mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
    delay(100);
    mySerial.println("AT+CLTS=1");
    delay(100);
    mySerial.println("AT&w");
    delay(100);
    mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
    delay(100);
    mySerial.println("AT+CNMI=1,2,0,0,0"); // Decides how newly arrived SMS messages should be handled
    delay(100);
    mySerial.println("AT+CMGDA=\"DEL ALL\"");
    delay(100);
    mySerial.println("AT+CCLK?");
    delay(100);
    sendState = 1;
    smssetupState = 0;
    smsState = 0;
  }
}
void gprsConfig() {
  if ((SMSChrono.hasPassed(10) && setupState == 1) || (gprsState == 1)) {
    mySerial.println("ATE1");
    delay(100);
    mySerial.println("AT+CGATT=1");
    delay(100);
    mySerial.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\""); // Configuring TEXT mode
    delay(100);
    mySerial.println("AT+SAPBR=3,1,\"APN\",\"RCMNET\" "); // Decides how newly arrived SMS messages should be handled
    delay(100);
    mySerial.println("AT+SAPBR=1,1");
    delay(100);
    mySerial.println("AT+SAPBR=2,1");
    delay(100);
    mySerial.println("AT+CIPGSMLOC=1,1");
    delay(100);
    setupState = 0;
    gprsState = 0;
    if (gprsState != 1)
      smssetupState = 1;
  }
}
void sendFirstSMS() {
  if (sendState == 1) {
    sendSMS();
    sendState = 0;
  }
}
void sendSMS() {
  String message;
  String datetime = String(rtc.getYear());
  datetime += "/";
  datetime += String(rtc.getMonth());
  datetime += "/";
  datetime += String(rtc.getDay());
  datetime += ",";
  datetime += String(rtc.getHours());
  datetime += ":";
  datetime += String(rtc.getMinutes());
  datetime += ":";
  datetime += String(rtc.getSeconds());
  message = "{\"battery percentage\":";
  message += checkBattery();
  message += ",";
  message += "\"consumption\":";
  message += totalLitres;
  message += ",";
  message += "\"location\":";
  message += coordinates;
  message += ",";
  message += "\"date and time\":";
  message += datetime;
  message = "}";
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
void sendData() {
  String message;
  String datetime = String(rtc.getYear());
  datetime += "/";
  datetime += String(rtc.getMonth());
  datetime += "/";
  datetime += String(rtc.getDay());
  datetime += ",";
  datetime += String(rtc.getHours());
  datetime += ":";
  datetime += String(rtc.getMinutes());
  datetime += ":";
  datetime += String(rtc.getSeconds());
  message = "{\"battery percentage\":";
  message += checkBattery();
  message += ",";
  message += "\"consumption\":";
  message += totalLitres;
  message += ",";
  message += "\"location\":";
  message += coordinates;
  message += ",";
  message += "\"date and time\":";
  message += datetime;
  message = "}";
  const char *mess = message.c_str();
  data.publish(mess);
}
int batteryPercent(float voltage) {
  float rawPercent = ((voltage - 3.2) * 116.28); //93.4579
  int rpercent = rawPercent;
  rpercent = constrain(rpercent, 0, 100);
  return rpercent;
}
