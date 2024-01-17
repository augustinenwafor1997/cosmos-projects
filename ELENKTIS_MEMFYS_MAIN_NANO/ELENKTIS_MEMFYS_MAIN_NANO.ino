#include <EEPROM.h>
#include <Chrono.h>
Chrono pumpChrono(Chrono::SECONDS);

#define pump1 PA5
#define pump2 PA6
#define floatSwitch1 PA7
#define floatSwitch2 PB1
#define light1 PA9
#define light2 PA10
#define lightAuto PA13
#define button1 PA0
#define button2 PA1
#define buttonAuto PA2

// #define pump1 31
// #define pump2 27
// #define floatSwitch1 36
// #define floatSwitch2 38
// #define light1 29
// #define light2 25
// #define lightAuto 23
// #define button1 42
// #define button2 44
// #define buttonAuto 46


static enum pump_mode { AUTOMATIC,
                        PUMP1,
                        PUMP2 } mode;
static enum pump_state { UNLOCK,
                         LOCK } lockState;
bool pump1state, pump2state, previousState = LOW;
unsigned long timeCount = millis();
uint8_t debounce = 50;

void setup() {
  // put your setup code here, to run once:
  pinMode(pump1, OUTPUT);
  pinMode(pump2, OUTPUT);
  pinMode(light1, OUTPUT);
  pinMode(light2, OUTPUT);
  pinMode(lightAuto, OUTPUT);
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(buttonAuto, INPUT_PULLUP);
  pinMode(floatSwitch1, INPUT_PULLUP);
  pinMode(floatSwitch2, INPUT_PULLUP);
  pump1state = LOW;
  pump2state = LOW;
  EEPROM.get(0 * sizeof(mode), mode);  //read EEPROM
  lockState = UNLOCK;
  //Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  checkButtonPress();


  /* Check if both tanks are below setpoint 
   * turn off pumps if tanks are below
   * lock the rest of the loop
  */

  if (digitalRead(floatSwitch1) == LOW && digitalRead(floatSwitch2) == LOW && lockState == UNLOCK) {
    lockState = LOCK;
    pump2state = LOW;
    pump1state = LOW;
    digitalWrite(pump1, pump1state);
    digitalWrite(light1, pump1state);
    digitalWrite(pump2, pump2state);
    digitalWrite(light2, pump2state);
  }


  /* Check if any tank is above setpoint 
   * unlock the rest of the loop if so
  */
  if ((digitalRead(floatSwitch1) == HIGH || digitalRead(floatSwitch2) == HIGH) && lockState == LOCK) {
    lockState = UNLOCK;
    //Serial.println("unlock loop");
  }

  if (lockState == UNLOCK) {
    if (mode == AUTOMATIC) {

      /* Check if both pumps are off (first run)
       * turn on pump 1 first and start timing
      */
      if (pump1state == LOW && pump2state == LOW) {
        pump1state = HIGH;
        digitalWrite(lightAuto, HIGH);
        digitalWrite(pump1, pump1state);
        digitalWrite(light1, pump1state);
        pumpChrono.restart();
      }

      /* Check if pump 1 is on and 1 hour has passed
       * turn off pump 1 and turn on pump 2
      */
      if (pump1state == HIGH && pump2state == LOW && pumpChrono.hasPassed(3600)) {
        pump2state = HIGH;
        pump1state = LOW;
        digitalWrite(pump1, pump1state);
        digitalWrite(light1, pump1state);
        digitalWrite(pump2, pump2state);
        digitalWrite(light2, pump2state);
        pumpChrono.restart();
      }

      /* Check if pump 2 is On and 1 hour has passed
       * turn off pump 2 and turn on pump 1
      */
      if (pump2state == HIGH && pump1state == LOW && pumpChrono.hasPassed(3600)) {
        pump1state = HIGH;
        pump2state = LOW;
        digitalWrite(pump2, pump2state);
        digitalWrite(light2, pump2state);
        digitalWrite(pump1, pump1state);
        digitalWrite(light1, pump1state);
        pumpChrono.restart();
      }
    } else if (mode == PUMP1) {
      /* turn On pump 1 if not On already
      */
      if (pump1state == LOW) {
        pump2state = LOW;
        digitalWrite(pump2, pump2state);
        digitalWrite(light2, pump2state);
        pump1state = HIGH;
        digitalWrite(pump1, pump1state);
        digitalWrite(light1, pump1state);
      }
    } else if (mode == PUMP2) {
      /* turn On pump 2 if not On already
      */
      if (pump2state == LOW) {
        pump1state = LOW;
        digitalWrite(pump1, pump1state);
        digitalWrite(light1, pump1state);
        pump2state = HIGH;
        digitalWrite(pump2, pump2state);
        digitalWrite(light2, pump2state);
      }
    }
  }
}
void checkButtonPress() {
  if (!digitalRead(buttonAuto) && previousState == HIGH && millis() - timeCount > debounce) {
    mode = AUTOMATIC;
    EEPROM.put(0 * sizeof(mode), mode);  //save to EEPROM
    digitalWrite(lightAuto, HIGH);
    pumpChrono.restart();
    timeCount = millis();
    previousState = !previousState;
  } else if (!digitalRead(button1) && previousState == HIGH && millis() - timeCount > debounce) {
    mode = PUMP1;
    EEPROM.put(0 * sizeof(mode), mode);  //save to EEPROM
    digitalWrite(lightAuto, LOW);
    timeCount = millis();
    previousState = !previousState;
  } else if (!digitalRead(button2) && previousState == HIGH && millis() - timeCount > debounce) {
    mode = PUMP2;
    EEPROM.put(0 * sizeof(mode), mode);  //save to EEPROM
    digitalWrite(lightAuto, LOW);
    timeCount = millis();
    previousState = !previousState;
  } else {
    previousState = !previousState;
  }
}