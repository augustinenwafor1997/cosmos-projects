#include <Chrono.h> // this library is used for timing
#include <Wire.h> // the wire library helps us in serial communication
#include <OneWire.h> // the one wire library is useful in the temperature sensor
#include <DallasTemperature.h> // this library is also used in the ds18b29 temp sensor

/*
   pin declarations
*/
#define startButton 4
#define turbidityPin A0
#define phPin A1
#define stirMotor 5
#define FermOutValve 6
#define fermErrorLed 7
#define fermErrorBuzzer 8
#define idleStateLed 3
#define fermFloatSwitch 2
#define distillFloatSwitch 9
#define heater 10
#define coldWaterValve 12
#define operationalLed 13
#define steamSensor A2

#define days 10 // no. of days to allow fermentation before the system determines there is a problem with the fermentation
bool previousState = LOW, idleState, fermState, fermErrorState, stirState, finalPhase, lastDay, distillState; //various states used in the state machine
/*
some other declaratory variables
*/
int debounce = 500, lastButton = 0, counter = 0;
unsigned long timeCount = millis();
float map_low = 0.00, map_high = 2482.326, map_steamlow = 0.00, map_steamhigh = 1024;// values used to convert measured values to percentages

/*
 * timer objects for counting time using chrono library
 */
Chrono daysChrono(Chrono::SECONDS);
Chrono stirChrono;

//temp sensor
#define ONE_WIRE_BUS 11 // this defines the pin our temp sensor is connected to
OneWire oneWire(ONE_WIRE_BUS);// creating a onewire object
DallasTemperature sensors(&oneWire);//creating a sensor object
DeviceAddress tempDeviceAddress;

void setup() {
  // initializations
  pinMode(startButton, INPUT_PULLUP);
  pinMode(fermErrorLed, OUTPUT);
  pinMode(idleStateLed, OUTPUT);
  pinMode(operationalLed, OUTPUT);
  pinMode(fermErrorBuzzer, OUTPUT);
  pinMode(FermOutValve, OUTPUT);
  pinMode(stirMotor, OUTPUT);
  pinMode(fermFloatSwitch, INPUT_PULLUP);
  pinMode(distillFloatSwitch, INPUT_PULLUP);
  pinMode(coldWaterValve, OUTPUT);
  pinMode(heater, OUTPUT);
  sensors.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  startFermentation();
  monitorFermentationDays();
  monitorFermentationpH();
  stirMixture();
  startDistillation();
  endDistillation();
  indicators();
}

//==============================================================================================================================

//function to start fermentation process
void startFermentation() {
  if (idleState == 0 && fermState == 0 && lastButton == 1) {//if button is pressed while we are in idle state
    idleState = 1;// we are no longer in idle mode
    fermState = 1;// fermentation has started
    lastButton = 0;// release the button
    daysChrono.restart();// start counting the days from now
  }
}

//==============================================================================================================================


void monitorFermentationDays() {
  if (idleState == 1 && fermState == 1 && daysChrono.hasPassed(86400)) {// if 24hrs has passed since we started fermentation.
    counter = counter + 1; // ...* counts one day
    daysChrono.restart(); // ...* restarts the counter
  }
  if (counter == days && fermState == 1 && finalPhase == 0) {// if the set date has reached and fermentation is still not complete
    counter = 0;  // reset the count of days
    digitalWrite(fermErrorBuzzer, HIGH);// alert the staff
    fermState = 0;// end the fermentation process
    fermErrorState = 1; //indicate error
  }

  if (counter == 2 && fermState == 1 && finalPhase == 1 && lastDay == 0) {// if two days have passed since pH sensor indicated fermentation complete
    counter = 0;  // reset the count of days
    lastDay = 1;// confirm fermentation is actually complete
  }

  if (fermErrorState == 1 && fermState == 0 && lastButton == 1) {// if the staff resolves an error in the fermentation process and pushes the button to confirm it
    digitalWrite(fermErrorBuzzer, LOW ); //stop the buzzer
    fermErrorState = 0;// clear the error
    lastButton = 0;// release the button
  }
}

//==============================================================================================================================

//function to check when fermentation is complete
void monitorFermentationpH() {
  if (idleState == 1 && fermState == 1) {// if fermentation is going on
    if (finalPhase == 0) {// if we are not in the final 2 days yet
      float pH = phMeasurement();// read the pH
      if (pH <= 4.0) {//if pH is good
        daysChrono.restart();//restart timer
        counter = 0;// restart counter
        finalPhase = 1;// we have now entered into the final 2 days to confirm that fermentation is complete
      }
    }
    if (lastDay == 1) {// if two days have passed
      float pH = phMeasurement();//check ph
      if (pH <= 4.0) {// if pH is still good
        daysChrono.restart();
        counter = 0;//  default other values
        finalPhase = 0;
        lastDay = 0;
        fermState = 0;// fermentation is now complete
      }
    }
  }
}

//==============================================================================================================================

// function mixes the fermented alcohol by stirring
void stirMixture() {
  if (idleState == 1 && fermState == 0 && fermErrorState == 0 && !digitalRead(stirMotor) && stirState == 0) {//if fermentation is complete and we are not stirring yet
    digitalWrite(stirMotor, HIGH);// start stirring
    stirChrono.restart();//start counter
  }
  if (digitalRead(stirMotor) && stirChrono.hasPassed(1200)) {// if we have stirred for 20 mins
    int turbidity = readTurbidity();// check turbidity
    if (turbidity > 60) {// if it is turbid enough
      digitalWrite(stirMotor, LOW);// stop stirring
      digitalWrite(FermOutValve, HIGH);// open the valve so the liquid flows into the distill chamber
      stirState = 1;// we have completed stirring
    }
    else// otherwise
      stirChrono.restart();// stir another 20 mins
  }
  if (digitalRead(FermOutValve) && !digitalRead(fermFloatSwitch)) {//if float switch detects no more liquid
    digitalWrite(FermOutValve, LOW);// close the valve
  }
}

//==============================================================================================================================

//function distill the liquid
void startDistillation() {
  if (stirState == 1 && fermState == 0) {// if stirring is complete
    if (digitalRead(distillFloatSwitch) && distillState == 0) {//if float switch in distill chamber detects liquid entering
      distillState = 1;// start distillation
      digitalWrite(heater, HIGH);// turn ON the heater
    }
    if (distillState == 1) {// if distillation is ongoing
      int temperature = readTemp();//check temperature
      if (temperature >= 79)//if it gets too hot
        digitalWrite(heater, LOW);// turn off the heater for regulation
      else if (temperature <= 77)// if it gets too cold
        digitalWrite(heater, HIGH);// turn ON the heater
      int steamValue = checkSteam();//check for steam
      if (steamValue >= 60)//if steam is detected
        digitalWrite(coldWaterValve, HIGH);// turn ON the cold water valve for condensation
    }
  }
}

//==============================================================================================================================

//function stops distillation when complete
void endDistillation() {
  int steamValue = checkSteam();//check steam level
  if (distillState == 1 && !digitalRead(distillFloatSwitch) && steamValue < 50) {//if we cannot detect steam and any liquid in the chamber anymore
    idleState = 0; //return to idle state because distillation is complete
    stirState = 0;//default other values
    distillState = 0;
    digitalWrite(coldWaterValve, LOW);// turn off the valve
    digitalWrite(heater, LOW);//turn off the heater
  }
}

//==============================================================================================================================

//function turns ON LEDs to indicate different states
void indicators() {
  if (idleState == 0)//if we are in idle state
    digitalWrite(idleStateLed, HIGH);// turn ON idle LED
  else digitalWrite(idleStateLed, LOW);//otherwise turn it OFF

  if (idleState == 1 && fermErrorState == 0)// if we are in operational mode
    digitalWrite(operationalLed, HIGH);// turn ON the operation LED
  else digitalWrite(operationalLed, LOW);//otherwise turn it OFF

  if (fermErrorState == 1)// if we have an error
    digitalWrite(fermErrorLed, HIGH);// turn error LED ON
  else digitalWrite(fermErrorLed, LOW);//otherwise turn it off
}

//==============================================================================================================================

//function to read push button
void readButton() {
  if (!digitalRead(startButton) && previousState == HIGH && millis() - timeCount > debounce) {//if push is detected
    lastButton = 1; // indicate to the system
    timeCount = millis();// start checking again
    previousState = !previousState;
  }
  else {
    lastButton = 0;//otherwise ignore noise
    previousState = !previousState;
  }
}

//==============================================================================================================================

//function to measure turbidity level of liquid
int readTurbidity() {
  int sensorValue = analogRead(turbidityPin);// read the input on analog pin
  int value = map(sensorValue, map_low, map_high, 0, 100);// convert it to percentage
  return value;
}

//==============================================================================================================================

//function to measure the pH value of the liquid
float phMeasurement() {
  int field[10];
  int backup;
  unsigned  long  int averageId = 0 ;
  // read 10 samples after 10 ms into the
  for ( int i = 0 ; i < 10 ; i ++) {
    field[i] = analogRead (phPin);
    delay(10);
  }
  // sort the array of measured results by size
  for (int i = 0 ; i < 9 ; i ++) {
    for (int j = i + 1 ; j < 10 ; j ++) {
      if (field [i] > field[j] ) {
        backup = field [i];
        field[i] = field[j];
        field[j] = backup;
      }
    }
  }
  // save the 2nd to 8th result in
  // a variable from which to compute the average
  // (omitting two field members at the beginning
  // and end for better accuracy)
  for (int i = 2 ; i < 8 ; i ++) {
    averageId += field [i];
  }
  // calculate the average pH value of
  // the measurement and conversion into a pH range of 0-14
  float avgPH = (float)averageId * 5.0 / 4096 / 6 ;
  float resultsPH = -5.18 * avgPH + 32.39;
  return resultsPH;
}

//==============================================================================================================================

// this function reads temperature
int readTemp() {
  sensors.setWaitForConversion(false);
  sensors.requestTemperatures();
  int c = sensors.getTempCByIndex(0);
  return c;
}

//==============================================================================================================================

//this function measures the steam in the distillation chamber
int checkSteam() {
  int sensorValue = analogRead(steamSensor);// read the input on analog pin
  int value = map(sensorValue, map_steamlow, map_steamhigh, 0, 100);// convert to percentage
  return value;
}
