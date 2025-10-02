#include <Chrono.h>
#include <EEPROM.h>
#include <Servo.h>

Chrono networkChrono(Chrono::SECONDS);
Chrono deployChrono(Chrono::SECONDS);
Chrono SMSChrono(Chrono::SECONDS);
Chrono retractChrono(Chrono::SECONDS);

Servo myservo;

#define trigger PB12
#define echo PB13
#define motion_sensor PB14
#define dir1A PB15
#define dir2A PA8
#define full_bin_indicator PA12 // BLUE
#define scissor_indicator PA15 // RED*
#define busy_indicator PB8 // GREEN
#define alarm PA6
#define operate_switch PB7
#define upSwitch PB10
#define downSwitch PA2
#define MAX_HEIGHT 4
#define SERIAL_NUM "SN1"
bool countState, retractState, firstAlert, scissorState, motionState, bin_full_state, deployState, scissorDeploy, alertState, connectState, setupState, sendState, notifyState;
int count, pos, disconnectCount;
unsigned long connectTime = 60000L, lastReconnectAttempt;
String phoneNum;

#define TINY_GSM_MODEM_SIM800
#define sim Serial1
// set GSM PIN, if any
#define GSM_PIN ""

// Your GPRS credentials, if any
const char apn[] = "web.gprs.mtnnigeria.net";
const char gprsUser[] = "";
const char gprsPass[] = "";


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
Adafruit_MQTT_Subscribe PHONE_NUM = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/address");
//Adafruit_MQTT_Publish SETTINGS = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/settings");
Adafruit_MQTT_Publish STATUS = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/status");
//Adafruit_MQTT_Publish FREQUENT = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/frequent");
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
#include <U8g2lib.h>
#include <U8x8lib.h>

U8G2_ST7567_OS12864_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ PA5, /* data=*/ PA7, /* cs=*/ PA4, /* dc=*/ PA3, /* reset=*/ PA1);

void setup() {
  // put your setup code here, to run once:
  u8g2.begin();

  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(motion_sensor, INPUT);
  pinMode(dir1A, OUTPUT);
  pinMode(dir2A, OUTPUT);
  pinMode(full_bin_indicator, OUTPUT);
  pinMode(busy_indicator, OUTPUT);
  pinMode(scissor_indicator, OUTPUT);
  pinMode(alarm, OUTPUT);
  pinMode(operate_switch, INPUT_PULLUP);
  pinMode(upSwitch, INPUT_PULLUP);
  pinMode(downSwitch, INPUT_PULLUP);
  HardwareTimer *binTim = new HardwareTimer(TIM4);
  myservo.attach(PB9);  // attaches the servo on pin 9 to the servo object
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    //delay(15);                       // waits 15ms for the servo to reach the position
  }
  sim.begin(9600);
  //delay(1000);
  modem.init();
  byte validNum = EEPROM.read(49);
  if (validNum == 14)
    phoneNum = EEPROMread(49);
  else phoneNum = "+2348144855290";
  connectState = 1;
  setupState = 1;
  mqtt.subscribe(&PHONE_NUM);
  SMSChrono.restart();
  binTim->pause();
  binTim->setOverflow(60000, HERTZ_FORMAT);
  binTim->attachInterrupt(bin_control);
  binTim->resume();
}

void loop() {
  // these first five functions are the network functions that cause //delay
  SMSconfig();
  connectGPRS();
  mqttConnect();
  receive_readings();
  CHECK_NETWORK();
  //.....
  // this one goes to the timer interrupt
}
void bin_control() {
  if (!digitalRead(operate_switch)) {
    digitalWrite(busy_indicator, LOW);
    notifyState = 0;
    if (scissorState == 0 && bin_full_state == 0)
      bin_open_close();
    bin_full_alarm();
    if (bin_full_state == 0)
      check_waste_level();
    bin_full_alert();
  }
  else {
    digitalWrite(busy_indicator, HIGH);
    digitalWrite(full_bin_indicator, LOW);
    digitalWrite(alarm, LOW);
    if (pos < 179) {
      for (pos = 80; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        //delay(20);                       // waits 15ms for the servo to reach the position
      }
    }
    bin_full_state = 0;
    if (connectState == 0 && notifyState == 0) {
      String message = "SN1 is ";
      if (bin_full_state == 1)
        message += "full";
      else
        message += "empty";
      const char *mess = message.c_str();
      STATUS.publish(mess);
      sendSMS();
      notifyState = 1;
    }
  }
}
void bin_open_close() {
  bool motionVal = digitalRead(motion_sensor);
  if (motionVal == 1 && motionState == 0 && bin_full_state == 0) {
    digitalWrite(full_bin_indicator, HIGH);
    for (pos = 180; pos >= 80; pos -= 1) { // goes from 180 degrees to 0 degrees
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      //delay(20);                       // waits 15ms for the servo to reach the position
    }
    motionState = 1;
  }
  if (motionVal == 0 && motionState == 1) {
    digitalWrite(full_bin_indicator, LOW);
    for (pos = 80; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      //delay(20);                       // waits 15ms for the servo to reach the position
    }
    motionState = 0;
  }
}

void bin_full_alarm() {
  bool motionVal = digitalRead(motion_sensor);
  if (motionVal == 1 && bin_full_state == 1) {
    digitalWrite(alarm, HIGH);
  }
  else digitalWrite(alarm, LOW);
}

int waste_level() {
  digitalWrite (trigger, HIGH);
  delayMicroseconds (10);
  digitalWrite (trigger, LOW);
  float time = pulseIn (echo, HIGH);
  int distance = (time * 0.034) / 2;
  return distance;
}

/**
   check if bin is at setpoint and deploy scissor to compress
*/
void check_waste_level() {
  if (retractState == 0) {
    int level = waste_level();
    // delay(60);
    if (level <= MAX_HEIGHT) {
      scissorState = 1;
      deploy_scissor();
    }
    stop_scissor_deploy();
    retract_scissor();
  }
  stop_scissor_retract();
}

void deploy_scissor() {
  if (deployState == 0 && scissorDeploy == 0) {
    if (pos < 179) {
      for (pos = 80; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        //delay(20);                       // waits 15ms for the servo to reach the position
      }
    }
    digitalWrite(scissor_indicator, HIGH);
    digitalWrite(dir1A, HIGH);
    digitalWrite(dir2A, LOW);
    // deployChrono.restart();
    deployState = 1;
  }
}

void retract_scissor() {
  if (deployState == 0 && scissorDeploy == 1) {
    digitalWrite(dir1A, LOW);
    digitalWrite(dir2A, HIGH);
    // retractChrono.restart();
    deployState = 2;
    retractState = 1;
    //    //delay(5000);
    //    digitalWrite(dir1A, LOW);
    //    digitalWrite(dir2A, LOW);
  }
}

void stop_scissor_deploy() {
  if (deployState == 1 && !digitalRead(downSwitch)) {
    digitalWrite(dir1A, LOW);
    digitalWrite(dir2A, LOW);
    deployState = 0;
    scissorDeploy = 1;
  }
}

void stop_scissor_retract() {
  if (retractState == 1 && !digitalRead(upSwitch)) {
    digitalWrite(scissor_indicator, LOW);
    digitalWrite(dir1A, LOW);
    digitalWrite(dir2A, LOW);
    deployState = 0;
    scissorDeploy = 0;
    retractState = 0;
    scissorState = 0;
    int level = waste_level();
    if (level < MAX_HEIGHT + 3) {
      bin_full_state = 1;
      alertState = 0;
      digitalWrite(full_bin_indicator, HIGH);
    }
  }
}

void bin_full_alert() {
  if (bin_full_state == 1 && alertState == 0) {
    send_alert();
  }
}

void send_alert() {
  if (count == 3) {
    count = 0;
    sendSMS();
    alertState = 1;
  }
  if (connectState == 0) {
    if (mqtt.connected()) {
      //      if (firstAlert == 0) {
      String message = "SN1 is ";
      if (bin_full_state == 1)
        message += "full";
      else
        message += "empty";
      const char *mess = message.c_str();
      //      String num = String(bin_full_state);
      //      const char *mess = num.c_str();
      // const char *mess = (char)(bin_full_state);
      STATUS.publish(mess);
      sendSMS();
      alertState = 1;
      //        firstAlert = 1;
      //      }
    }
    else count++;
  }
}

void SMSconfig() {
  if (SMSChrono.hasPassed(10) && setupState == 1) {
    sim.println("AT"); //Once the handshake test is successful, it will back to OK
    delay(100);
    sim.println("AT+CMGF=1"); // Configuring TEXT mode
    delay(100);
    sim.println("AT+CNMI=1,2,0,0,0"); // Decides how newly arrived SMS messages should be handled
    delay(100);
    sim.println("AT+CMGDA=\"DEL ALL\"");
    delay(100);
    sendState = 1;
    setupState = 0;
  }
}

void sendSMS() {
  if (sendState == 1) {
    //  EEPROM.get(49, phoneNum);
    // if (phoneNum.startsWith("+234")) {
    String message;
    message = "SN1 is ";
    if (bin_full_state == 1)
      message += "full";
    else
      message += "empty";
    message += "\n\n\n\n";
    message += "Cheers";
    sim.println("AT+CMGDA=\"DEL ALL\"");
    delay(50);
    sim.println("AT+CMGF=1"); // Configuring TEXT mode
    delay(100);
    sim.println("AT+CMGS=\"" + phoneNum + "\""); // phone number to sms
    delay(100);
    sim.print(message); //text content
    delay(100);
    sim.write(26);
  }
}

void mqttConnect() {
  if (connectState == 0) {
    if (!mqtt.connected()) {
      long now = millis();
      if (now - lastReconnectAttempt > 5000) {
        lastReconnectAttempt = now;
        // Attempt to reconnect
        if (MQTT_connect()) {
          lastReconnectAttempt = 0;
        }
      }
    }
  }
}

void connectGPRS() {
  if (connectState == 1 && millis() >= connectTime) {
    if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
      //delay(10);
      return;
    }
    connectState = 0;
    Serial.println("GPRS CONNECTED");
    networkChrono.restart();
  }
}

void receive_readings() {
  if (connectState == 0) {
    Adafruit_MQTT_Subscribe *subscription_name;
    while ((subscription_name = mqtt.readSubscription(200))) {
      if (subscription_name == &PHONE_NUM) {
        char *value = (char *)PHONE_NUM.lastread;
        phoneNum = String(value);
        phoneNum.trim();
        EEPROMwrite(49, phoneNum);
      }
    }
  }
}

void CHECK_NETWORK() {
  if (networkChrono.hasPassed(120) && connectState == 0) {// returns true if it passed 120 s since it was started
    check_network();
    networkChrono.restart();
  }
}
void check_network() {
  if (!modem.isGprsConnected()) {
    if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
      disconnectCount++;
      if (disconnectCount == 3) {
        disconnectCount = 0;
        countState = 1;// do something here
      }
    }
  }
  else {
    countState = 0;
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
  data[newStrLen] = '\0'; // !!! NOTE !!! Remove the space between the slash "/" and "0" (I've added a space because otherwise there is a display bug)
  return String(data);
}
