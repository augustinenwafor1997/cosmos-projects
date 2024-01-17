#include <EEPROM.h>
#include <Chrono.h>
Chrono pumpChrono(Chrono::SECONDS);
#include <STM32RTC.h>

// Get the RTC object
STM32RTC& rtc = STM32RTC::getInstance();

#define pump1 PA3
#define pump2 PA4
#define floatSwitch1 PA5
#define light1 PA7
#define light2 PA6
#define lightAuto PB0
#define button1 PB11
#define button2 PB10
#define buttonAuto PB1


static enum pump_mode { AUTOMATIC,
                        PUMP1,
                        PUMP2 } mode;
static enum pump_state { UNLOCK,
                         LOCK } lockState;
bool pump1state, pump2state;

// Timer variables
unsigned long long timer = 0;
bool timerStarted = false;
unsigned long timecount = 3600;
bool pumpState = false;
bool startup = false;
bool startTimer = true;




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
  rtc.begin();  // initialize RTC 24H format
  pump1state = LOW;
  pump2state = LOW;
  //mode = AUTOMATIC;
  //EEPROM.put(0 * sizeof(mode), mode);  //save to EEPROM
  EEPROM.get(0 * sizeof(mode), mode);  //read EEPROM
  lockState = UNLOCK;
  //Serial.begin(9600);
}

void loop() {
  // // put your main code here, to run repeatedly:
  checkButtonPress();


  /* Check if both tanks are below setpoint
   * turn off pumps if tanks are below
   * lock the rest of the loop
  */

  if (digitalRead(floatSwitch1) == LOW && lockState == UNLOCK) {
    lockState = LOCK;
    pump2state = LOW;
    pump1state = LOW;
    digitalWrite(pump1, pump1state);
    digitalWrite(light1, pump1state);
    digitalWrite(pump2, pump2state);
    digitalWrite(light2, pump2state);
    startup = false;
  }


  /* Check if any tank is above setpoint
   * unlock the rest of the loop if so
  */
  if (digitalRead(floatSwitch1) == HIGH && lockState == LOCK) {
    lockState = UNLOCK;
    //Serial.println("unlock loop");
  }

  if (lockState == UNLOCK) {
    if (mode == AUTOMATIC) {

      if (!startup) {
        pump1state = HIGH;
        pump2state = LOW;
        digitalWrite(pump1, pump1state);
        digitalWrite(light1, pump1state);
        digitalWrite(lightAuto, HIGH);
        digitalWrite(pump2, pump2state);
        digitalWrite(light2, pump2state);
        restart_hour_timer();
        startup = true;
        pumpState = true;
      }

      /* Check if pump 1 is on and 1 hour has passed
       * turn off pump 1 and turn on pump 2
      */

      if (hour_timer(timecount)) {
        if (pumpState) {
          pumpState = false;
          pump2state = HIGH;
          pump1state = LOW;
          digitalWrite(pump1, pump1state);
          digitalWrite(light1, pump1state);
          digitalWrite(pump2, pump2state);
          digitalWrite(light2, pump2state);
          restart_hour_timer();
        }

        /* Check if pump 2 is On and 1 hour has passed
       * turn off pump 2 and turn on pump 1
      */
        else {
          pumpState = true;
          pump2state = LOW;
          pump1state = HIGH;
          digitalWrite(pump1, pump1state);
          digitalWrite(light1, pump1state);
          digitalWrite(pump2, pump2state);
          digitalWrite(light2, pump2state);
          restart_hour_timer();
        }
      }
    } else if (mode == PUMP1) {
      /* turn On pump 1 if not On already
      */
      pumpState = true;
      pump2state = LOW;
      digitalWrite(pump2, pump2state);
      digitalWrite(light2, pump2state);
      pump1state = HIGH;
      digitalWrite(pump1, pump1state);
      digitalWrite(light1, pump1state);
    } else if (mode == PUMP2) {
      /* turn On pump 2 if not On already
      */
      pumpState = false;
      pump1state = LOW;
      digitalWrite(pump1, pump1state);
      digitalWrite(light1, pump1state);
      pump2state = HIGH;
      digitalWrite(pump2, pump2state);
      digitalWrite(light2, pump2state);
    }
  }
}

bool debounse(int btnIndex, int btn) {
  static uint16_t states[3] = { 0 };  // Array to store states for each button
  states[btnIndex] = (states[btnIndex] << 1) | digitalRead(btn) | 0xfe00;
  return (states[btnIndex] == 0xff00) && (states[btnIndex] != 0xffff);
}

void checkButtonPress() {
  if (debounse(0, buttonAuto)) {
    mode = AUTOMATIC;
    EEPROM.put(0 * sizeof(mode), mode);  //save to EEPROM
    digitalWrite(lightAuto, HIGH);
    restart_hour_timer();
  } else if (debounse(1, button1)) {
    mode = PUMP1;
    EEPROM.put(0 * sizeof(mode), mode);  //save to EEPROM
    digitalWrite(lightAuto, LOW);
    stop_hour_timer();
  } else if (debounse(2, button2)) {
    mode = PUMP2;
    EEPROM.put(0 * sizeof(mode), mode);  //save to EEPROM
    digitalWrite(lightAuto, LOW);
    stop_hour_timer();
  }
}

bool hour_timer(unsigned long input) {
  if (startTimer) {
    // Start countdowns if not already running
    if (!timerStarted) {
      rtc.setEpoch(1451606400);  // Jan 1, 2016
      timer = rtc.getEpoch();
      timerStarted = true;
    }

    // Check for countdown completion
    if (timerStarted && (rtc.getEpoch() - timer >= input)) {
      timerStarted = false;
      timer = 0;
      startTimer = false;
      return true;
    } else {
      return false;
    }
  }
}

// restart timer
void restart_hour_timer() {
  timerStarted = false;
  startTimer = true;
}

void stop_hour_timer() {
  timer = 0;
  startTimer = false;
}
