#include <LiquidCrystal_I2C.h> // this is the library for our LCD
LiquidCrystal_I2C lcd(0x3F, 20, 4); // this line inputs the address of our i2c lcd as well as the type; 20*4
#include <Chrono.h> // this library is used for timing
#include "SPIFFS.h" // this library is used for saving files to the esp32
#include <ArduinoJson.h> // this library is used for coding and decoding the files we wish to save
#include <Wire.h> // the wire library helps us in serial communication
#include <OneWire.h> // the one wire library is useful in the temperature sensor
#include <DallasTemperature.h> // this library is also used in the ds18b29 temp sensor
#include "DHT.h"// this library is used for reading the humidity
#include <Bounce2.h>
#include <WiFiClient.h>
#include <WiFi.h>
#include "ESP32_MailClient.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME  "energymeter31"
#define AIO_KEY  "aio_iFcH26req3J9jW8HJaSHoaNL6uNO"
#define emailSenderAccount    "incubator2k21@gmail.com"
#define emailSenderPassword   "incubator123"
#define emailRecipient        "ankaleonardo@gmail.com"
#define smtpServer            "smtp.gmail.com"
#define smtpServerPort        465
#define emailSubject          "INCUBATION STATUS"
const char *ssid =  "SPONSOR";
const char *pass =  "project21";

//buttons
//byte button_pins[] = {27, 12}; // button pins, 1 = menu 2/3 = up/down, 4/5 = left/right
//#define NUMBUTTONS sizeof(button_pins)
//Bounce * buttons = new Bounce[NUMBUTTONS];

// menu and state variables
char menuOptions [4][15] = {" FOWL", " DUCK", " TURKEY", " GUINEA FOWL"}, SelectMode[] = " SELECT ANIMAL ";
int daysSoFar, counter, days, modeSelected, Celcius, publishState = 1, emailState = 0, prevCounter = 0, debounce = 100;
byte lastButton = 0, configState = 0, timePeriodState = 0, candlingState = 0, displayState = 0, alarmState, lowerLimit, upperLimit;
unsigned long timeCount = millis();
bool previousState = LOW;
//humidity sensor
#define DHTPIN 26     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE);

//temp sensor
#define ONE_WIRE_BUS 13 // this defines the pin our temp sensor is connected to
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress tempDeviceAddress;

// timers
Chrono daysChrono(Chrono::SECONDS);
Chrono candlingChrono(Chrono::SECONDS);
Chrono lcdChrono(Chrono::SECONDS);
Chrono alarmChrono;
Chrono adafruitChrono;
Chrono emailChrono(Chrono::SECONDS);

//pin declarations
#define motorPin 33 //4
#define alarmPin 32 //17
#define bulbPin 25

struct Config {
  // char hostname[64] = "MY NAME";
  int Days;
  int Modeselected;
  int Counter;
  int Configstate;
  int Timeperiodstate;
  int Candlingstate;
  int Displaystate;
  int LowerLimit;
  int UpperLimit;
};
const char *parameters = "/incubator.txt";  // <- SD library uses 8.3 parameters
Config config;                         // <- global configuration object
byte customChar[] = {
  B01110,
  B01010,
  B01110,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

// adafruit feeds
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Subscribe animal = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/animal");
Adafruit_MQTT_Publish state = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/state");
Adafruit_MQTT_Publish hum = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/humidity");
Adafruit_MQTT_Publish temperature = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temperature");
Adafruit_MQTT_Publish day = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/day");
boolean MQTT_connect();


SMTPData smtpData;
// Callback function to get the Email sending status
void sendCallback(SendStatus info);

void setup() {
  // put your setup code here, to run once:
  lcd.init();
  Serial.begin(9600);
  connectWIFI();
  //  for (int i = 0; i < NUMBUTTONS; i++) {
  //    buttons[i].attach( button_pins[i], INPUT_PULLUP); // setup the bounce instance for the current button
  //    buttons[i].interval(20); // interval in ms
  //  }

  pinMode(27, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  sensors.begin();
  dht.begin();
  pinMode(alarmPin, OUTPUT);
  pinMode(bulbPin, OUTPUT);
  pinMode(motorPin, OUTPUT);
  if (!SPIFFS.begin(true)) {
    return;
  }
  lcd.setCursor(0, 0);
  lcd.print(F("  ANKA'S INCUBATOR"));
  loadConfiguration(parameters, config);
  Celcius = readTemp();// read the temperature
  //delay(1000);
  lcd.clear();
  if (configState != 1) {
    lcd.backlight();
    lastButton = 1;
    mainMenu();
    lcd.noBacklight();
  }
  mqtt.subscribe(&animal);
  if (displayState == 0 && timePeriodState == 0)
    if (MQTT_connect()) state.publish("Normal");
  if (displayState == 1 && candlingState == 0)
    if (MQTT_connect()) state.publish("ready to hatch");
  if (timePeriodState == 1)
    if (MQTT_connect()) state.publish("ready for candling");
  if (candlingState == 1)
    if (MQTT_connect()) state.publish("candling complete");
  if (MQTT_connect()) day.publish((char)(counter + 1));
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Sending Email...");
  sendEmail();
  lcd.clear();
  lcd.noBacklight();
}

void connectWIFI() {
  lcd.setCursor(0, 0);
  lcd.print("Connecting to WiFi");
  Serial.print("connecting to wifi");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    lcd.print(".");
  }
  lcd.setCursor(0, 1);
  lcd.print("WiFi connected");
  Serial.println("WIFI connected");
  delay(2000);
  lcd.clear();
}
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

// function for saving settings
void saveConfiguration(const char *filename, const Config &config) {
  File file = SPIFFS.open(filename, FILE_WRITE);
  if (!file) {
    return;
  }

  // Allocate the memory pool on the stack
  // Don't forget to change the capacity to match your JSON document.
  // Use https://arduinojson.org/assistant/ to compute the capacity.
  StaticJsonBuffer<1024> jsonBuffer;

  // Parse the root object
  JsonObject &root = jsonBuffer.createObject();

  // Set the values
  root["Days"] = days;
  root["Modeselected"] = modeSelected;
  root["Count"] = counter;
  root["Configstate"] = configState;
  root["Timeperiodstate"] = timePeriodState;
  root["Candlingstate"] = candlingState;
  root["Displaystate"] = displayState;
  root["LowerLimit"] = lowerLimit;
  root["UpperLimit"] = upperLimit;

  // Serialize JSON to file
  if (root.printTo(file) == 0) {
  }

  // Close the file (File's destructor doesn't close the file)
  file.close();
}


//reads the file and loads the configuration
void loadConfiguration(const char *filename, Config &config) {
  // Open file for reading
  File file = SPIFFS.open(filename);

  // Allocate the memory pool on the stack.
  // Don't forget to change the capacity to match your JSON document.
  // Use arduinojson.org/assistant to compute the capacity.
  StaticJsonBuffer<1024> jsonBuffer;

  // Parse the root object
  JsonObject &root = jsonBuffer.parseObject(file);

  // Copy values from the JsonObject to the Config
  config.Days = root["Days"];
  config.Modeselected = root["Modeselected"];
  config.Counter = root["Count"];
  config.Configstate = root["Configstate"];
  config.Timeperiodstate = root["Timeperiodstate"];
  config.Candlingstate = root["Candlingstate"];
  config.Displaystate = root["Displaystate"];
  config.LowerLimit = root["LowerLimit"];
  config.UpperLimit = root["UpperLimit"];
  // Close the file (File's destructor doesn't close the file)
  file.close();
  days = config.Days;
  modeSelected = config.Modeselected;
  counter = config.Counter;
  configState = config.Configstate;
  timePeriodState = config.Timeperiodstate;
  candlingState = config.Candlingstate;
  displayState = config.Displaystate;
  lowerLimit = config.LowerLimit;
  upperLimit = config.UpperLimit;
}




//this function checks if a button is pushed
//void readButtons() {
//  // lastButton = 0;
//  for (int i = 0; i < NUMBUTTONS; i++) {
//    buttons[i].update(); // Update the Bounce instance
//    if ( buttons[i].fell() ) { // If it fell
//      lcd.backlight();
//      lcdChrono.restart();
//      lastButton = i + 1;
//      // process button press:
//    }
//  }
//}
void readButtons() {
  if (!digitalRead(27) && previousState == HIGH && millis() - timeCount > debounce) {
    lcd.backlight();
    lcdChrono.restart();
    lastButton = 1;
    timeCount = millis();
    previousState = !previousState;
  }
  else if (!digitalRead(12) && previousState == HIGH && millis() - timeCount > debounce) {
    lcd.backlight();
    lcdChrono.restart();
    lastButton = 2;
    timeCount = millis();
    previousState = !previousState;
  }
  else {
    lastButton = 0;
    previousState = !previousState;
  }
  //  }
}



// function that displays the menu
void mainMenu() {
  readButtons();
  switch (lastButton) {
    case 1: {
        lastButton = 0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("     MAIN  MENU     ");
        delay(1000);
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print(SelectMode);
        lcd.setCursor(0, 1);
        lcd.print(menuOptions[0]);
        lcd.setCursor(0, 2);
        lcd.print(menuOptions[1]);
        lcd.setCursor(0, 3);
        lcd.print(menuOptions[2]);
        lcd.setCursor(8, 1);
        lcd.print(menuOptions[3]);

        modeSelected = 0;
        while (lastButton != 1) {
          delay(60);
          lastButton = 0;
          readButtons();
          selectModeInput();
        }
        displayConfiguration();
        lastButton = 0;
        counter = 0;
        daysChrono.restart();
        if (MQTT_connect()) state.publish("Normal");
        break;
      }
  }
}


// this function moves the cursor on the lcd menu
void selectModeInput() {
  if (modeSelected == 0) {
    lcd.setCursor(0, 1);
    lcd.print(">");
    lcd.setCursor(0, 2);
    lcd.print(" ");
    lcd.setCursor(0, 3);
    lcd.print(" ");
    lcd.setCursor(8, 1);
    lcd.print(" ");
  }

  else if (modeSelected == 1) {
    lcd.setCursor(0, 1);
    lcd.print(" ");
    lcd.setCursor(0, 2);
    lcd.print(">");
    lcd.setCursor(0, 3);
    lcd.print(" ");
    lcd.setCursor(8, 1);
    lcd.print(" ");
  }

  else if (modeSelected == 2) {
    lcd.setCursor(0, 1);
    lcd.print(" ");
    lcd.setCursor(0, 2);
    lcd.print(" ");
    lcd.setCursor(0, 3);
    lcd.print(">");
    lcd.setCursor(8, 1);
    lcd.print(" ");
  }

  else  {
    lcd.setCursor(0, 1);
    lcd.print(" ");
    lcd.setCursor(0, 2);
    lcd.print(" ");
    lcd.setCursor(0, 3);
    lcd.print(" ");
    lcd.setCursor(8, 1);
    lcd.print(">");
  }

  if (lastButton == 2) {
    while (!digitalRead(12));
    //    buttons[1].update();
    //    while (buttons[1].fell());
   // lastButton = 0;
    modeSelected++;
    modeSelected = modeSelected % 4;
  }
}


// this function displays and saves the configuration
void displayConfiguration() {
  if (modeSelected == 0) {
    days = 18;
    configState = 1;
    timePeriodState = 0;
    candlingState = 0;
    displayState = 0;
    lowerLimit = 56;
    upperLimit = 62;
    counter = 0;
    // Serial.println(F("Saving configuration..."));
    saveConfiguration(parameters, config);
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("    FOWL SELECTED   ");
    delay(500);
    lcd.clear();
    daysChrono.restart(); // ...* restarts the counter
  }
  else if (modeSelected == 1) {
    days = 25;
    configState = 1;
    timePeriodState = 0;
    candlingState = 0;
    displayState = 0;
    lowerLimit = 53;
    upperLimit = 60;
    counter = 0;
    // Serial.println(F("Saving configuration..."));
    saveConfiguration(parameters, config);
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("    DUCK SELECTED   ");
    delay(500);
    lcd.clear();
    daysChrono.restart(); // ...* restarts the counter
  }
  else if (modeSelected == 2) {
    days = 25;
    configState = 1;
    timePeriodState = 0;
    candlingState = 0;
    displayState = 0;
    lowerLimit = 51;
    upperLimit = 56;
    counter = 0;
    // Serial.println(F("Saving configuration..."));
    saveConfiguration(parameters, config);
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("  TURKEY SELECTED");
    delay(500);
    lcd.clear();
    daysChrono.restart(); // ...* restarts the counter
  }
  else {
    days = 25;
    configState = 1;
    timePeriodState = 0;
    candlingState = 0;
    displayState = 0;
    lowerLimit = 51;
    upperLimit = 56;
    counter = 0;
    //  Serial.println(F("Saving configuration..."));
    saveConfiguration(parameters, config);
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("GUINEA FOWL SELECTED");
    delay(500);
    lcd.clear();
    daysChrono.restart(); // ...* restarts the counter
  }
}


// this function reads temperature
int readTemp() {
  sensors.setWaitForConversion(false);
  sensors.requestTemperatures();
  int c = sensors.getTempCByIndex(0);
  return c;
}

int readDHT() {
  int h = dht.readHumidity();
  if (isnan(h)) {
    return 0;
  }
  return h;
}

// this function displays the animal we have chosen
void displayAnimalToLcd() {
  if (modeSelected == 0) {
    lcd.setCursor(2, 0);
    lcd.print("FOWL INCUBATION");
  }
  else if (modeSelected == 1) {
    lcd.setCursor(2, 0);
    lcd.print("DUCK INCUBATION");
  }
  else if (modeSelected == 2) {
    lcd.setCursor(1, 0);
    lcd.print("TURKEY INCUBATION");
  }
  else {
    lcd.setCursor(2, 0);
    lcd.print("GUINEA INCUBATION");
  }
}


// this function is the home screen display
void lcdDisplay() {
  if (timePeriodState == 0 && displayState == 0) {// if we are cleared to display home screen
    int humidity = readDHT();
    Celcius = readTemp();// read the temperature
    // readButtons();// check if any button is pushed
    displayAnimalToLcd();// display the bird we are incubating
    lcd.setCursor(0, 1);
    lcd.print("TEMP:");
    lcd.print(Celcius);// display temperature
    lcd.createChar(0, customChar);
    lcd.setCursor(7, 1);
    lcd.write(0);
    lcd.print("C  ");
    lcd.print(" HUM:");
    lcd.print(humidity);
    lcd.print("%");
    lcd.print(" ");
    lcd.setCursor(0, 2);
    lcd.print("DAY ");//display the days so far
    lcd.print(counter + 1);
    lcd.print(" ");
    // delay(10);
  }
}



// this function controls the incubation process
void incubate() {
  // readButtons();
  if (timePeriodState == 0 && displayState == 0 && candlingState == 0) { //1. this condition checks if we are within the first incubation period and if we are it:
    digitalWrite(motorPin, HIGH); // ...* puts the motor in rotation
    int humidity = readDHT();
    // int Celcius = readTemp();
    //    if (Celcius > tempCounter) tempCounter = Celcius;
    //    counter += 1;

    //    if (counter == 11) {
    //      counter = 0;
    //      if (tempCounter <= 37) { // ...* checks if the temperature is above the setpoint. if it is above it:...
    //        digitalWrite(bulbPin, HIGH); // ...*turns the bulb off so the incubator cools down.
    //      }
    //      else { // ...*otherwise it leaves the bulb ON
    //        digitalWrite(bulbPin, LOW);
    //      }
    //    }
    if (Celcius <= 36) { // ...* checks if the temperature is above the setpoint. if it is above it:...
      digitalWrite(bulbPin, HIGH); // ...*turns the bulb off so the incubator cools down.
    }
    else if (Celcius >= 38) { // ...*otherwise it leaves the bulb ON
      digitalWrite(bulbPin, LOW);
    }
    if (alarmChrono.hasPassed(3000) && alarmState == 1) {
      digitalWrite(alarmPin, HIGH);
      delay(100);
      digitalWrite(alarmPin, LOW);
      alarmChrono.restart();
    }
    if (humidity < lowerLimit || humidity > upperLimit) { // ...* checks if the humidity is below or above the set limits. if it is below it:...
      lcd.setCursor(0, 3);
      lcd.print("(!)"); // ...*displays a warning icon to the user to top water
      // digitalWrite(alarmPin, HIGH);
      alarmState = 1;
    }
    else { // ...*otherwise it operates normally
      lcd.setCursor(0, 3);
      lcd.print("   ");
      if (alarmState == 1) {
        digitalWrite(alarmPin, LOW);
        alarmState = 0;
      }
    }
  }
  if (timePeriodState == 0 && displayState == 0 && candlingState == 1) { //1. this condition checks if we are within the first incubation period and if we are it:
    digitalWrite(motorPin, HIGH); // ...* puts the motor in rotation
    int humidity = readDHT();
    // int Celcius = readTemp();
    //    if (Celcius > tempCounter) tempCounter = Celcius;
    //    counter += 1;

    //    if (counter == 11) {
    //      counter = 0;
    //      if (tempCounter <= 37) { // ...* checks if the temperature is above the setpoint. if it is above it:...
    //        digitalWrite(bulbPin, HIGH); // ...*turns the bulb off so the incubator cools down.
    //      }
    //      else { // ...*otherwise it leaves the bulb ON
    //        digitalWrite(bulbPin, LOW);
    //      }
    //    }
    if (Celcius <= 36) { // ...* checks if the temperature is above the setpoint. if it is above it:...
      digitalWrite(bulbPin, HIGH); // ...*turns the bulb off so the incubator cools down.
    }
    else if (Celcius >= 38) { // ...*otherwise it leaves the bulb ON
      digitalWrite(bulbPin, LOW);
    }
    if (alarmChrono.hasPassed(3000) && alarmState == 1) {
      digitalWrite(alarmPin, HIGH);
      delay(100);
      digitalWrite(alarmPin, LOW);
      alarmChrono.restart();
    }
    if (humidity < 70 || humidity > 83) { // ...* checks if the humidity is below or above the set limits. if it is below it:...
      lcd.setCursor(0, 3);
      lcd.print("(!)"); // ...*displays a warning icon to the user to top water
      // digitalWrite(alarmPin, HIGH);
      alarmState = 1;
    }
    else { // ...*otherwise it operates normally
      lcd.setCursor(0, 3);
      lcd.print("   ");
      if (alarmState == 1) {
        digitalWrite(alarmPin, LOW);
        alarmState = 0;
      }
    }
  }
  if (timePeriodState == 0 && displayState == 0 && daysChrono.hasPassed(300)) {// 2.this condition checks if 24hrs has passed if so it:...

    counter = counter + 1; // ...* counts it as another day
    // Serial.println(F("Saving configuration...")); //* saves the current day incase of power off
    saveConfiguration(parameters, config);
    daysChrono.restart(); // ...* restarts the counter
    if (MQTT_connect()) day.publish((char)(counter + 1));
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Sending Email...");
    sendEmail();
    lcd.clear();
    lcd.noBacklight();
  }
  if (counter == days && timePeriodState == 0) {// 3.this condition checks if the set date has reached if so:...
    counter = 0;  // reset the count of days
    timePeriodState = 1;
    // Serial.println(F("Saving configuration...")); // ...* save the state incase of power off
    saveConfiguration(parameters, config);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("READY FOR CANDLING!");
    digitalWrite(motorPin, LOW); // turns OFF the motor and bulb
    digitalWrite(bulbPin, LOW);
    if (MQTT_connect()) state.publish("ready for candling");
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Sending Email...");
    sendEmail();
    lcd.clear();
    lcd.noBacklight();
    for (int i = 0; i <= 10; i++) {
      digitalWrite(alarmPin, HIGH); //alerts the user to start the candling
      delay(200);
      digitalWrite(alarmPin, LOW);
      delay(200);
    }
  }
  if (timePeriodState == 1 && lastButton == 2) {// 4. if RESUME button is pressed indicating that candling is over
    candlingState = 1; // set the state to indicate that candling is over
    timePeriodState = 0; // set the state to resume the rotating of the motor and counting days
    // Serial.println(F("Saving configuration..."));// save states incase of power off
    saveConfiguration(parameters, config);
    lastButton = 0;
    if (MQTT_connect()) state.publish("candling complete");
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Sending Email...");
    sendEmail();
    lcd.clear();
    lcd.noBacklight();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" CANDLING COMPLETE");
    for (int i = 0; i <= 2; i++) {
      digitalWrite(alarmPin, HIGH);// alerts that the system is about to resume
      delay(200);
      digitalWrite(alarmPin, LOW);
      delay(200);
    }
    delay(600);
    lcd.clear();
    // previous = !previous;
    counter = 0;
    candlingChrono.restart();
  }
  // previous = !previous;
  if (candlingState == 1 && candlingChrono.hasPassed(300)) {// 5. checks if 24hrs have passed post-candling

    counter = counter + 1; // counts the days
    // Serial.println(F("Saving configuration..."));// saves the configuration incase of power off
    saveConfiguration(parameters, config);
    candlingChrono.restart();
  }
  if (counter == 3 && candlingState == 1) {// 6. checks if three days have passed after candling
    counter = 0;// resets the count
    candlingState = 0; // resets the state to pre-candling so that a new cycle can start
    displayState = 1; // sets the lcd display to not display the home screen
    // Serial.println(F("Saving configuration..."));// saves the configuration
    saveConfiguration(parameters, config);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("EGGS READY TO HATCH!");
    digitalWrite(motorPin, LOW);
    digitalWrite(bulbPin, LOW);
    if (MQTT_connect()) state.publish("Ready to Hatch");
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Sending Email...");
    sendEmail();
    lcd.clear();
    lcd.noBacklight();
    for (int i = 0; i <= 5; i++) {// alerts the user that the incubation period is complete
      digitalWrite(alarmPin, HIGH);
      delay(200);
      digitalWrite(alarmPin, LOW);
      delay(200);
    }
  }
  if (displayState == 1 && lastButton == 2) { //7. if button is pressed to start a new incubation cycle
    // previous = !previous;
    lastButton = 0;
    displayState = 0; // sets the lcd to display the home screen
    counter = 0; //resets the count
    //  Serial.println(F("Saving configuration..."));
    saveConfiguration(parameters, config); // saves the configuration
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("STARTING INCUBATION");
    if (MQTT_connect()) state.publish("Normal");
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Sending Email...");
    sendEmail();
    lcd.clear();
    lcd.noBacklight();
    for (int i = 0; i <= 2; i++) { // signify that a new cycle is starting
      digitalWrite(alarmPin, HIGH);
      delay(200);
      digitalWrite(alarmPin, LOW);
      delay(200);
    }
    delay(600);
    lcd.clear();
    //  previous = !previous;
    daysChrono.restart(); // ...* restarts the counter
  }
  // previous = !previous;
}

void turnOffBacklight() {
  if (lcdChrono.hasPassed(10)) {
    lcd.noBacklight();
    lcdChrono.restart();
    lastButton = 0;
    //  lcdState = 0;
  }
}

void receiveReadings() {
  char animalChar [4][15] = {"fowl", "duck", "turkey", "guinea fowl"};
  if (MQTT_connect()) {
    Adafruit_MQTT_Subscribe *subscription_name;
    while ((subscription_name = mqtt.readSubscription(3000))) {
      //  Serial.println("In the while loop");
      if (subscription_name == &animal) {
        char *val = (char *)animal.lastread;
        //        Serial.println("Reset Button Pressed...");
        Serial.print("animal received: ");
        Serial.println(val);
        String message = String(val);
        for (int i = 0; i <= 3; i++) {
          if (String(animalChar[i]) == message) {
            Serial.println(animalChar[i]);
            modeSelected = i;
            Serial.println(modeSelected);
          }
        }
        displayConfiguration();
        // String dai = String(counter + 1);
        if (MQTT_connect()) day.publish((char)(counter + 1));
        state.publish("Normal");
        lcd.clear();
        lcd.backlight();
        lcd.setCursor(0, 0);
        lcd.print("Sending Email...");
        sendEmail();
        lcd.clear();
        lcd.noBacklight();

      }
    }
  }
}
void publishReadings() {
  int humidity = readDHT();
  if (MQTT_connect()) {
    if (publishState == 1) { // returns true if it passed 1000 ms since it was started
      publishState = 0;
      temperature.publish(float(Celcius));
      hum.publish(float(humidity));
    }
  }
  //  else {
  //    Serial.println("problem connecting");
  //  }
  if (publishState == 0 && adafruitChrono.hasPassed(15000) ) { // returns true if it passed 1000 ms since it was started
    adafruitChrono.restart();  // restart the crono so that it triggers again later
    publishState = 1;
  }
}

void sendEmail() {
  int humidity = readDHT();
  String message;
  message = " Hi Anka, ";
  message += "\n";
  message += " Temperature is: ";
  message += String(Celcius);
  message += " deg Celcius.";
  message += " Humidity is: ";
  message += String(humidity);
  message += "%.";
  message += "\n";
  if (displayState == 0 && timePeriodState == 0)
    message += " Normal incubation period";
  if (displayState == 1 && candlingState == 0)
    message += " Eggs are ready to hatch!";
  if (timePeriodState == 1)
    message += " Eggs are ready for candling";
  if (candlingState == 1)
    message += " candling completed";
  message += "\n";
  message += " Day ";
  message += String(counter + 1);
  message += " of incubation";
  message += "\n\n\n\n";
  message += "Cheers";
  // Set the SMTP Server Email host, port, account and password
  smtpData.setLogin(smtpServer, smtpServerPort, emailSenderAccount, emailSenderPassword);

  // For library version 1.2.0 and later which STARTTLS protocol was supported,the STARTTLS will be
  // enabled automatically when port 587 was used, or enable it manually using setSTARTTLS function.
  //smtpData.setSTARTTLS(true);

  // Set the sender name and Email
  smtpData.setSender("SMART INCUBATOR", emailSenderAccount);

  // Set Email priority or importance High, Normal, Low or 1 to 5 (1 is highest)
  smtpData.setPriority("High");

  // Set the subject
  smtpData.setSubject(emailSubject);

  // Set the message with HTML format
  // smtpData.setMessage("<div style=\"color:#2f4468;\"><h1>Hello World!</h1><p>- Sent from ESP32 board</p></div>", true);
  // Set the email message in text format (raw)
  smtpData.setMessage(message, false);

  // Add recipients, you can add more than one recipient
  smtpData.addRecipient(emailRecipient);
  //smtpData.addRecipient("YOUR_OTHER_RECIPIENT_EMAIL_ADDRESS@EXAMPLE.com");

  smtpData.setSendCallback(sendCallback);

  //Start sending Email, can be set callback function to track the status
  if (!MailClient.sendMail(smtpData))
    Serial.println("Error sending Email, " + MailClient.smtpErrorReason());

  //Clear all data from Email object to free memory
  smtpData.empty();
  //    lcd.setCursor(0,1);
  //    lcd.print("mail sent");

  //  if (emailState == 1 && emailChrono.hasPassed(86400)) {
  //    emailState = 0;
  //    emailChrono.restart();
  //  }
}
// Callback function to get the Email sending status
void sendCallback(SendStatus msg) {
  // Print the current status
  Serial.println(msg.info());

  // Do something when complete
  if (msg.success()) {
    Serial.println("----------------");
  }
  // lcd.clear();
}

void loop() {
  // put your main code here, to run repeatedly:
  mainMenu();
  lcdDisplay();
  incubate();
  publishReadings();
  receiveReadings();
  turnOffBacklight();
}
