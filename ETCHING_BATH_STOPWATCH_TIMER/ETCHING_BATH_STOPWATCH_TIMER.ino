#include <Chrono.h>
Chrono timer(Chrono::SECONDS);
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#include <math.h>

#define up 5
#define down 3
#define enter 4
#define buzzer 6
#define pump 2

int mins, debounce = 250;
bool previousState = LOW, startCounter;
unsigned long timeCount = millis(), counter, selTime;

void setup() {
  lcd.init();
  //Serial.begin(115200);
  //  pinMode(9, OUTPUT);
  //  analogWrite(9, 90);

  lcd.backlight();
  pinMode(buzzer, OUTPUT);
  pinMode(pump, OUTPUT);
  pinMode(up, INPUT_PULLUP);
  pinMode(down, INPUT_PULLUP);
  pinMode(enter, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  setTimer();
  stopTimer();
  displayCountDown();
  homeDisplay();
}
void homeDisplay() {
  if (startCounter == 0) {// system is idle
    lcd.setCursor(0, 0);
    lcd.print("  **SET TIMER**");// it tells you to enter your value
    lcd.setCursor(6, 1);
    lcd.print(mins);
    lcd.print(":00");// your values are entered in minutes
    lcd.print("  ");
  }
  else { // when it is counting, it only displays the time left for the countdown to be over
    lcd.setCursor(0, 0);
    lcd.print("** TIME LEFT **");
  }
}
void displayCountDown() {// displays countdown
  if (startCounter == 1) {// system is in operation

    unsigned long currentMillis = (counter - millis());
    long secs = (currentMillis / 1000) % 60;
    long mint = (currentMillis / 1000) / 60;// conversion from miiliseconds-to-seconds-to-minutes
    lcd.setCursor(6, 1);
    lcd.print(mint);
    lcd.print(":");
    lcd.print(secs);// displays the minutes and seconds remaining
    lcd.print("  ");
    //    Serial.print("time remaining: ");
    //    Serial.print(mint);
    //    Serial.print(":");
    //    Serial.print(secs);
    //    Serial.print("    ");
    //    Serial.print("time remaining: ");
    //    Serial.println(currentMillis);
    //int secs = second(currentMillis);

  }
}
void stopTimer() {// function checks when countdown is over
  if (startCounter == 1 && timer.hasPassed(mins * 60)) {// if the set time has elapsed
    digitalWrite(pump, LOW);// turn off the pump
    for (int i = 0; i < 10; i++) {
      digitalWrite(buzzer, HIGH);
      delay(200);
      digitalWrite(buzzer, LOW);// alert the attendant
      delay(200);
    }
    startCounter = 0;// return to the idle state
    lcd.clear();
  }
}
void setTimer() {// function receives stopwatch input from attendant
  if (!digitalRead(up) && previousState == HIGH && millis() - timeCount > debounce) {// if UP button is pressed
    mins++;// increment the timer value in minutes
    startCounter = 0;// maintain the idle state
    digitalWrite(buzzer, HIGH);// notify the attendant that the input is well recieved
    delay(100);
    digitalWrite(buzzer, LOW);
    lcd.clear();

    // Serial.println(mins);
    timeCount = millis();// used for debounce
    previousState = !previousState;// used for debounce
  }
  if (!digitalRead(down) && previousState == HIGH && millis() - timeCount > debounce) {// if DOWN button is pressed
    mins--;// decrement the timer value in minutes
    startCounter = 0;// maintain the idle state
    digitalWrite(buzzer, HIGH);// notify the attendant that the input is well received
    delay(100);
    digitalWrite(buzzer, LOW);
    lcd.clear();
    timeCount = millis();// used for debounce
    previousState = !previousState;// used for debounce
  }
  if (!digitalRead(enter) && previousState == HIGH && millis() - timeCount > debounce) {// if ENTER button is pressed
    digitalWrite(buzzer, HIGH);// notify the attendant that the input is well received
    delay(100);
    digitalWrite(buzzer, LOW);
    startCounter = 1;// change the system state from idle to operational
    timer.restart();// restart the timer and start counting
    selTime = (mins * 60000);// convert the set value from mins to milliseconds
    counter = millis() + selTime;// add the timer value to the time that has elapsed since the system bootup
    lcd.clear();
    digitalWrite(pump, HIGH);// turn on the pump

    /*
       debug code
    */
    //    Serial.print("time set to run: ");
    //    Serial.println(selTime);
    //    Serial.print("time so far: ");
    //    Serial.println(millis());
    //    Serial.print("total time to run: ");
    //    Serial.println(counter);
    timeCount = millis();// used for debounce
    previousState = !previousState;// used for debounce
  }
  else previousState = !previousState;// used for debounce
}
