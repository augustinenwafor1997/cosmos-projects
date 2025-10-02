#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);
#include <SoftwareSerial.h>
SoftwareSerial SerialAT(3, 2);
#define TINY_GSM_MODEM_SIM800
#define SerialMon Serial

// set GSM PIN, if any
#define GSM_PIN ""

// Your GPRS credentials, if any
const char apn[] = "web.gprs.mtnnigeria.net";
const char gprsUser[] = "web";
const char gprsPass[] = "web";

#include <TinyGsmClient.h>

#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif
TinyGsmClient client(modem);

#include <ArduinoJson.h>
#include <Chrono.h>
#include <EEPROM.h>
#include "ACS712.h"
#define sensorInput1 A0  // sensor connects to analog 0 
#define sensorInput2 A1
ACS712 sensor30(ACS712_20A, sensorInput1);
ACS712 sensor20(ACS712_20A, sensorInput2);
#define buzzer 15
#define cutOffRelay 13
#define breachPin 2

#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME  "obt_ernest"
#define AIO_KEY  "09626c0b0cc4475785feee9486a73258"
int publishDelay = 0;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// Adafruit_MQTT_Subscribe writing = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/writing");
Adafruit_MQTT_Subscribe onlinePay = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/ROOM2");
Adafruit_MQTT_Subscribe reset1 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/ROOM1");
Adafruit_MQTT_Subscribe shutDown = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/TOTAL SHUTDOWN");

boolean MQTT_connect();
boolean MQTT_connect() {
  int8_t ret;
  if (mqtt.connected()) {
    return true;
  }  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) {
    mqtt.disconnect();
    delay(2000);
    retries--;
    if (retries == 0) {
      return false;
    }
  } return true;
}
Adafruit_MQTT_Publish Energy = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/ROOM1 ENERGY");
Adafruit_MQTT_Publish Units = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/ROOM2 ENERGY");
Adafruit_MQTT_Publish PowerState = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/ROOM1 CUTOFFSTATE");
Adafruit_MQTT_Publish Total_energy = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/ROOM2 CUTOFFSTATE");
Adafruit_MQTT_Publish Total_units = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/ROOM1 SWITCHSTATE");
Adafruit_MQTT_Publish pay = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/ROOM2 SWITCHSTATE");
Adafruit_MQTT_Publish Breach = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/ROOM2 SWITCHSTATE");
Adafruit_MQTT_Publish Bypass = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/ROOM2 SWITCHSTATE");
Adafruit_MQTT_Publish UnitsX = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/ROOM2 SWITCHSTATE");

Chrono beepSlowChrono;
Chrono beepFastChrono;
Chrono displayChrono;
Chrono breachChrono;
Chrono uploadChrono(Chrono::SECONDS);

#include <Keypad.h>
const byte ROWS = 4;
const byte COLS = 4;
char token[21];
int i = 0, fraudState, payState = 0, count = 5;
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

//char* token = "12914728696354244999";// 12914728696354244999 12914728696352646999
char* meternumber = "107042448433";
char tokenarray[21];
char arrtoken[20];
char settoken[7];
char paritybit[3];
char numbers[12];
char four[5];

struct breakup
{
  int rechargetime;
  int meternum;
  int token;
  int date;
};
breakup variable;

///////////GET-RECHARGE-TOKEN///////////
int getrecharge(int input) {
  //  return mapFloat(input, 1026.0, 9054.0, 0.0, 1000.0);
  if (input >= 1026 && input <= 9054) {
    return map(input, 1026, 9054, 0, 1000);
  }
  else
    return 0;
}

////GET-METER-NUMBER///////
int Getmeternumber(char* input)
{
  strncpy(numbers, input, sizeof(numbers));
  int j = 0;
  for (int i = 0; i <= 12; i++)
  {
    if (i >= 4 && i <= 7)
    {
      four[j] = numbers[i];
      j++;

    }
  }
  return atoi(four);
}

///// RE-ARRANGE TOKEN ////////////
double rearrangetoken(char* input)
{
  float rechargetoken = 0;
  strncpy(tokenarray, input, sizeof(tokenarray));
  int i = 0;
  int j = 0;
  int line;
  int value = 0;
  paritybit[0] = tokenarray[0];
  paritybit[1] = tokenarray[1];
  int parity = atoi(paritybit);
  if (parity < 18)
  {
    for (line = 0; line < 18; line++)
    {
      if (parity <= 19)
      {
        arrtoken[line] = tokenarray[(parity + 1)];
        parity = parity + 1;
      }
      if (parity > 19)
      {
        arrtoken[line] = tokenarray[(parity - 19) + 1];
        parity = parity + 1;
      }
    }
    for (line = 0; line < 18; line++)
    {
      if (i <= 3)
      {
        settoken[i] = arrtoken[line];
        i++;
      }
      if (i > 3)
      {
        switch (j)
        {
          case 0:
            variable.meternum = atoi(settoken);
            j++;
            i = 0;
            break;
          case 1:
            variable.token = atoi(settoken);
            j++;
            i = 0;
            break;
          case 2:
            variable.date = atoi(settoken);
            j++;
            i = 0;
            break;
          case 3:
            variable.rechargetime = atoi(settoken);
            j++;
            i = 0;
            break;
        }
      }
    }
  }
  if (variable.meternum == Getmeternumber(meternumber)) {
    //  return getrecharge(variable.token);
    return 1;
  }
  else
    return 0;
}


float voltage = 220, live_current, neutral_current, power, energy_kwH, units_kwH, units_bought = 2.0, total_units, tokVal, total_energy;
int energyState = 0, buzzerState = 0, buzzerState2 = 0, dateState = 0, timeState = 1, unitState = 1, breachState = 0, unitXState = 0, saveState = 0, paymentState = 0, physicalPaymentState = 0, totalEnerguState = 0, powerState = 0, uploadPowerOffState = 0, uploadPowerOnState = 0, publishState = 1, bypassState = 0, tamperState = 0;
unsigned long lastTime;
unsigned long currentTime;


void setup() {
  // put your setup code here, to run once:
  SerialMon.begin(9600);
  SerialAT.begin(9600);
  SerialMon.println("Initializing modem...");
  modem.init();
  lcd.init();
  lcd.backlight();
  pinMode(cutOffRelay, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(breachPin, INPUT_PULLUP);
  sensor30.setZeroPoint(510);
  if ( GSM_PIN && modem.getSimStatus() != 3 ) {
    modem.simUnlock(GSM_PIN);
  }

  // GPRS connection parameters are usually set after network registration
  SerialMon.print(F("Connecting to "));
  SerialMon.print(apn);
  if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
    SerialMon.println(" fail");
    delay(100);
    return;
  }
  SerialMon.println(" success");

  if (modem.isGprsConnected()) {
    SerialMon.println("GPRS connected");
  }

  mqtt.subscribe(&reset1);
  mqtt.subscribe(&onlinePay);
  mqtt.subscribe(&shutDown);
}

void loop() {
  // put your main code here, to run repeatedly:


}

void readEEPROM() {
  EEPROM.get(0, energy_kwH );
  EEPROM.get(1, units_kwH);
  EEPROM.get(2, units_bought);
  EEPROM.get(3, bypassState);
  EEPROM.get(4, tamperState);
  EEPROM.get(5, total_units);
  EEPROM.get(6, total_energy );
  //  EEPROM.get(7, futureMonth );
}

void getPowerAndEnergy() {
  //  getCurrentAndVoltage();
  live_current = sensor30.getCurrentAC(50);
  neutral_current = sensor20.getCurrentAC(50);
  power = live_current * voltage;

  lastTime = currentTime;
  currentTime = millis();
  if (energyState == 0) {// if the system is just powering on after a poweroff
    energyState = 1;
  }
  else {
    energy_kwH = energy_kwH + (power / 1000) * ((currentTime - lastTime) / 3600000.0);
    units_kwH = units_bought - energy_kwH;
    Serial.println(F("Saving configuration..."));
    EEPROM.put(1, units_kwH);
  }
  //  energy_kwH = 4.89;

}
void low_units_warning() {
  if (buzzerState == 1 && beepSlowChrono.hasPassed(5000)) {
    buzzerState = 0;
    beepSlowChrono.restart();
  }
  if (buzzerState2 == 1 && beepFastChrono.hasPassed(1000)) {
    buzzerState2 = 0;
    beepFastChrono.restart();
  }
  if (units_kwH < 12 && units_kwH > 5 && buzzerState == 0) {
    digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);
    buzzerState = 1;
  }
  else if (units_kwH <= 5 && buzzerState2 == 0) {
    digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);
    buzzerState2 = 1;
  }
}

void cutOff() {
  if (units_kwH == 0) {
    digitalWrite(cutOffRelay, HIGH);
    modem.gprsConnect(apn, gprsUser, gprsPass);
    // upload to server that units have been exhausted
    if (MQTT_connect()) {
      if (unitXState == 0) { // upload that units have finished
        unitXState = 1;
        UnitsX.publish("EMPTY");
      }
    }
  }
  else {
    //    digitalWrite(cutOffRelay, LOW);
    unitXState = 0;
  }
}

void bypass_monitoring() {
  if ((live_current < ((neutral_current + 0.5) || (neutral_current - 0.5))) || (live_current > ((neutral_current + 0.5) || (neutral_current - 0.5)))) {
    digitalWrite(cutOffRelay, HIGH);
    modem.gprsConnect(apn, gprsUser, gprsPass);
    if (MQTT_connect()) {
      if (bypassState == 0) { // 
        bypassState = 1;
        Bypass.publish(float(1));
        //save;
      }
    }
    else {
      Serial.println("problem connecting");
    }
    // bypass  notification upload to server (WRITE THIS LATER)
  }
  else {
    bypassState = 0;
  }
}

void tamper_detection() {
  if (!digitalRead(breachPin)) {
    digitalWrite(cutOffRelay, HIGH);
    Serial.println(1);
    // saveState = 0;
    digitalWrite(buzzer, HIGH);
    modem.gprsConnect(apn, gprsUser, gprsPass);
    breachState = 1;
    if (MQTT_connect()) {
      if (tamperState == 0) { //
        tamperState = 1;
        Breach.publish(float(1));
        //save;
      }
    }
    //    upload readings (WRITE THIS LATER)
  }
  else {
    tamperState = 0;
    // save if (saveState == 0){
    //         eeprom.put(5,tamperState);
    //         saveState = 1;
    //    }
  }
  if (breachState == 1 && breachChrono.hasPassed(20000)) {
    breachState = 0;
    breachChrono.restart();
    digitalWrite(buzzer, LOW);
  }
}

void home_display() {
  if (powerState == 0) {
    if (dateState == 0 && timeState == 1 && unitState == 1 && displayChrono.hasPassed(3000)) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(random(100));
      //    Serial.println("1");
      dateState = 1;
      timeState = 0;
      displayChrono.restart();
    }
    if (timeState == 0 && dateState == 1 && unitState == 1 && displayChrono.hasPassed(3000)) {

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(random(100));
      //    Serial.println("2");
      unitState = 0;
      timeState = 1;
      displayChrono.restart();
    }
    if (unitState == 0 && dateState == 1 && timeState == 1 && displayChrono.hasPassed(3000)) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(units_kwH);
      //    Serial.println("3");
      unitState = 1;
      dateState = 0;
      displayChrono.restart();
    }
  }
  else {
    lcd.clear();
    lcd.noBacklight();
  }
}



void update_total_units() {
  if (total_units == 0 && tokVal != 0) {
    modem.gprsConnect(apn, gprsUser, gprsPass);
    if (MQTT_connect()) {
      total_units = tokVal;
      Total_units.publish(total_units);
      EEPROM.put(5, total_units); // save
    }
  }
  if (paymentState == 1) {
    modem.gprsConnect(apn, gprsUser, gprsPass);
    if (MQTT_connect()) {
      total_units += tokVal;
      Total_units.publish(total_units);
      EEPROM.put(5, total_units);  // saved
      paymentState = 0;
    }
  }
}

void offline_payment() {
  // upload online that payments have been made
  // if payment is made
  char customKey = customKeypad.getKey();

  if (customKey) {
    if (customKey == '#') {
      i = 0;
      lcd.clear();
      return;
    }
    lcd.backlight();
    if (i <= 19) {
      token[i] = customKey;
      if (i == 16) {
        lcd.setCursor(0, 1);
      }
      i++;
      Serial.print(customKey);
      // lcd.clear();
      lcd.print(customKey);

    }
  }
  if (i == 20) {
    lcd.clear();
    lcd.noBacklight();
    Serial.println();
    Serial.println(token);
    if (rearrangetoken(token) == 1) {
      Serial.println("valid token. check reuse!");
      String date = String(variable.date);
      String times = String(variable.rechargetime);
      String dt = date + times;
      Serial.print("Date: ");
      Serial.print(date);
      Serial.print("  ");
      Serial.print("Time: ");
      Serial.print(times);
      Serial.print("  ");
      Serial.print("Merged: ");
      Serial.println(dt);
      long merger = dt.toInt();
      long mem;
      for (int j = 5; j <=  50; j++) {
        //      Serial.print("\t");
        Serial.print("LOOP: ");
        Serial.print(j);
        EEPROM.get(j * sizeof(mem), mem);
        if (mem == merger) {
          Serial.print("\t");
          Serial.print("FOUND: ");
          Serial.print(mem);
          Serial.print("  ");
          Serial.print("AT ADDRESS: ");
          Serial.print(j);
          Serial.print("\t");
          Serial.println("fraud!");
          fraudState = 1;
          break;
        }
        else {
          fraudState = 0;
          Serial.print(" ");
          Serial.println("NO FRAUD");

        }
      }
      if (fraudState == 0) {
        fraudState = 1;
        Serial.println("CHECK FOR CORRECT PAYMENT TOKEN");
        tokVal = getrecharge(variable.token);
        if (tokVal != 0) {
          Serial.print("VALID TOKEN: ");
          Serial.println(tokVal);
          units_bought = tokVal + units_kwH;
          EEPROM.put(2, units_bought);
          // upload tokVal as the units bought

          // TURN ENERGY TO ZERO HERE AND UPDATE ALL MAN
          paymentState = 1; // this guy updates others
          physicalPaymentState = 1;
          totalEnerguState = 1;
          payState = 1;
        }
        else {
          Serial.println("INVALID TOKEN! ABORT");
          payState = 0;
          //          break; // check this line later.....................
        }
      }
      if (payState == 1) {
        payState = 0;
        Serial.println("SAVE TO MEMORY");
        long info2;
        for (int address = 5; address <=  50; address++) {
          Serial.print("LOOP: ");
          Serial.println(address);
          EEPROM.get(address * sizeof(info2), info2);
          if (info2 == 0) {
            EEPROM.put(address * sizeof(merger), merger);
            Serial.print("SAVED: ");
            Serial.print(merger);
            Serial.print("  ");
            Serial.print("TO ADDRESS: ");
            Serial.println(address);
            count++;
            if (count > 998) {
              int clearVal = 0;
              Serial.println("CLEAR MEMEORY!");
              for (int address = 5; address <=  50; address++) {
                EEPROM.put(address * sizeof(clearVal), clearVal);
              }
              count = 5;
            }
            int address = 2 * sizeof(count);
            EEPROM.put(address, count);
            Serial.print("SAVED COUNT TO ADDRESS ");
            Serial.println(address);
            break;
          }
        }
      }
    }
    i = 0;
  }
}

void upload_offline_payment() {
  if (physicalPaymentState == 1) {
    modem.gprsConnect(apn, gprsUser, gprsPass);
    if (MQTT_connect()) {
      pay.publish(tokVal);
      physicalPaymentState = 0;
    }
  }
}

void update_total_energy() {
  if (totalEnerguState == 1) {
    modem.gprsConnect(apn, gprsUser, gprsPass);
    if (MQTT_connect()) {
      total_energy += energy_kwH;
      energy_kwH = 0; // clear energy and start counting afresh for the new payment
      Total_energy.publish(total_energy);
      EEPROM.put(6, total_energy );
      totalEnerguState = 0;
    }
    // save to EEPROM(LATER)
  }
}

void check_power_ON__OFF () {
  // upload changes
  if ((live_current == 0 && neutral_current == 0) || cutOffRelay == HIGH) {
    modem.gprsConnect(apn, gprsUser, gprsPass);
    powerState = 1;
    if (MQTT_connect()) {
      if (uploadPowerOffState == 0) { // upload power Off
        uploadPowerOffState = 1;
        PowerState.publish(float(1));

      }
    }
    //    upload readings (WRITE THIS LATER)
  }
  else {
    powerState = 0;
    modem.gprsConnect(apn, gprsUser, gprsPass);
    if (MQTT_connect()) {
      if (uploadPowerOnState == 0) { // upload power ON
        uploadPowerOnState = 1;
        PowerState.publish(float(0));

      }
    }
  }
  if (powerState == 0 && uploadPowerOffState == 1) {
    uploadPowerOffState = 0;
  }
  if (powerState == 1 && uploadPowerOnState == 1) {
    uploadPowerOnState = 0;
  }
}

void getCommand() {
  if (MQTT_connect()) {
    Adafruit_MQTT_Subscribe *subscription_name;
    while ((subscription_name = mqtt.readSubscription(5000))) {
      if (subscription_name == &onlinePay) {
        Serial.println("a new value on room1");
        char *value = (char *)onlinePay.lastread;
        Serial.println(value);
        tokVal = atof(value);
        units_bought = atof(value) + units_kwH;
        EEPROM.put(2, units_bought);
        paymentState = 1;
        totalEnerguState = 1;
      }

      if (subscription_name == &shutDown) {
        Serial.println("a new value on room2");
        char *value = (char *)shutDown.lastread;
        Serial.println(value);
        // Apply message to lamp
        String message = String(value);
        message.trim();
        if (message == "ON") {
          digitalWrite(cutOffRelay, LOW);
        }
        if (message == "OFF") {
          digitalWrite(cutOffRelay, HIGH);
        }
      }

      if (subscription_name == &reset1) {
        Serial.println("a new value on Total Shutdown");
        char *value = (char *)reset1.lastread;
        Serial.println(value);
        // Apply message to lamp
        String message = String(value);
        message.trim();
        if (message == "reset") {
          energyState = 0, buzzerState = 0, buzzerState2 = 0, dateState = 0, timeState = 1, unitState = 1, breachState = 0, unitXState = 0, saveState = 0, paymentState = 0, physicalPaymentState = 0, totalEnerguState = 0, powerState = 0, uploadPowerOffState = 0, uploadPowerOnState = 0;
        }
      }
    }
  }
  // else connect to gprs(write later)
}
void publishReadings() {
  modem.gprsConnect(apn, gprsUser, gprsPass);
  if (MQTT_connect()) {
    if (publishState == 1) { // returns true if it passed 1000 ms since it was started
      publishState = 0;
      Energy.publish(energy_kwH);
      Units.publish(units_kwH);
    }
  }
  else {
    Serial.println("problem connecting");
  }
  if (publishState == 0 && uploadChrono.hasPassed(86400) ) { // returns true if it passed 1000 ms since it was started
    uploadChrono.restart();  // restart the crono so that it triggers again later
    publishState = 1;
  }
}
