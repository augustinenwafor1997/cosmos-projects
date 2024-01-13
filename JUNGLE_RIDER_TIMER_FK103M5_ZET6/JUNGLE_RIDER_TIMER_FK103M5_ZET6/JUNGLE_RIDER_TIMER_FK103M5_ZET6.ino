#include <SPI.h>      //include the SPI bus library
#include <MFRC522.h>  //include the RFID reader library
#include <FlashStorage_STM32.h>
#include <Chrono.h>
#include <STM32RTC.h>

// Get the RTC object
STM32RTC& rtc = STM32RTC::getInstance();

// Define the pin connections for each segment of the display
#define A_PIN PD0
#define B_PIN PD4
#define C_PIN PB3
#define D_PIN PG9
#define E_PIN PD6
#define F_PIN PD2
#define G_PIN PB5
#define DP_PIN PG11
#define DIGIT1 PC7
#define DIGIT2 PC9
#define DIGIT3 PA9
#define DIGIT4 PA8
#define SS_PIN PA4
#define RST_PIN PA3
#define UPBTN PB9
#define DWNBTN PB7
#define ENTBTN PB8
#define RELAY PD1
#define ACTIVELED PE3
#define INACTIVELED PE5
#define batteryPin PA2  // Analog pin for battery voltage measurement
#define minVoltage 8    // Replace with your battery's minimum voltage (8V for example)
#define maxVoltage 12   // Replace with your battery's maximum voltage (12V)


MFRC522 mfrc522(SS_PIN, RST_PIN);  // instatiate a MFRC522 reader object.
MFRC522::MIFARE_Key key;           //create a MIFARE_Key struct named 'key', which will hold the card information


// Timer variables
unsigned long timer = 0;
bool timerStarted = false;
bool inMenu = false;
bool activated = false;
bool decimalPointOn = false;  // Global variable to control the state of the decimal point
int i = 0;
int usageData;
int State = 0;
uint8_t menu = 1;
uint8_t submenu = 1;
int timeVal;
int volVal;
int countVal;
// start reading from the first byte (address 0) of the EEPROM
int address = sizeof(int);
int volvalAddress = 2 * sizeof(int);
int timevalAddress = 3 * sizeof(int);
int ledPins[] = { PF6, PF4, PF2, PF0 };  // Pins for the four LEDs
Chrono blinkTime;
Chrono blinkTimeM;
Chrono countdownTime;
Chrono menuTime;
Chrono loopTime;


// functions
void writeIntIntoEEPROM(int address, int number) {
  EEPROM.update(address, number >> 8);
  EEPROM.update(address + 1, number & 0xFF);
  EEPROM.commit();
}

int readIntFromEEPROM(int address) {
  return (EEPROM.read(address) << 8) + EEPROM.read(address + 1);
}

void menuTimer() {
  //timer menu
  if (menuTime.hasPassed(5000)) {
    // Return to home display after 5 seconds of inactivity
    inMenu = false;
    EEPROM.update(timevalAddress, timeVal);
    EEPROM.update(volvalAddress, volVal);
    EEPROM.commit();
    menuTime.restart();
  }
}

// Function to display a digit (0-9) on the seven-segment display
void displayDigit(uint8_t digit, bool dp) {
  // Define the bit patterns for each digit (0-9) in a common cathode display
  uint8_t digitSend;
  const uint8_t digitPatterns[13] = {
    B00111111,  // 0
    B00000110,  // 1
    B01011011,  // 2
    B01001111,  // 3
    B01100110,  // 4
    B01101101,  // 5
    B01111101,  // 6
    B00000111,  // 7
    B01111111,  // 8
    B01101111,  // 9
    B00000000,  // blank
    B01000000,  // -
    B10000000,  // .
  };
  // Check if the input is within the valid range (0-9)
  if (digit >= 0 && digit <= 12) {
    if (dp == true) {
      digitSend = digitPatterns[digit] | digitPatterns[12];
    } else {
      digitSend = digitPatterns[digit];
    }
    // Output the corresponding pattern to the seven-segment display
    digitalWrite(A_PIN, bitRead(digitSend, 0));
    digitalWrite(B_PIN, bitRead(digitSend, 1));
    digitalWrite(C_PIN, bitRead(digitSend, 2));
    digitalWrite(D_PIN, bitRead(digitSend, 3));
    digitalWrite(E_PIN, bitRead(digitSend, 4));
    digitalWrite(F_PIN, bitRead(digitSend, 5));
    digitalWrite(G_PIN, bitRead(digitSend, 6));
    digitalWrite(DP_PIN, bitRead(digitSend, 7));
  }
}

void displayDigit(uint8_t number, uint8_t digit, bool dp) {
  // Shift out the binary value for the digit to the shift register
  displayDigit(number, dp);
  // Pulse the register clock to latch the data
  digitalWrite(digit, HIGH);
  delay(2);
  digitalWrite(digit, LOW);
}

void displayNumber(int number) {
  // Extract individual digits from the number
  int digit1 = number / 1000;
  int digit2 = (number / 100) % 10;
  int digit3 = (number / 10) % 10;
  int digit4 = number % 10;

  displayDigit(digit1, DIGIT1, false);
  displayDigit(digit2, DIGIT2, false);
  displayDigit(digit3, DIGIT3, false);
  displayDigit(digit4, DIGIT4, false);
}

void displayMenu(int state, int menuNumber, int val, int blinkpos) {
  if (blinkpos == 1) {
    if (State == 0) {
      displayDigit(menuNumber, DIGIT1, false);
      displayDigit(11, DIGIT2, false);
      displayDigit(val / 10, DIGIT3, false);
      displayDigit(val % 10, DIGIT4, false);
    } else {
      displayDigit(10, DIGIT1, false);
      displayDigit(11, DIGIT2, false);
      displayDigit(val / 10, DIGIT3, false);
      displayDigit(val % 10, DIGIT4, false);
    }
  } else {
    if (State == 0) {
      displayDigit(menuNumber, DIGIT1, false);
      displayDigit(11, DIGIT2, false);
      displayDigit(val / 10, DIGIT3, false);
      displayDigit(val % 10, DIGIT4, false);
    } else {
      displayDigit(menuNumber, DIGIT1, false);
      displayDigit(11, DIGIT2, false);
      displayDigit(10, DIGIT3, false);
      displayDigit(10, DIGIT4, false);
    }
  }
}

// button debounce
bool debounce(int btnIndex, int btn) {
  static uint16_t states[3] = { 0 };  // Array to store states for each button
  states[btnIndex] = (states[btnIndex] << 1) | digitalRead(btn) | 0xfe00;
  return (states[btnIndex] == 0xff00) && (states[btnIndex] != 0xffff);
}

// display countdown timer
void displayCountdown(int seconds) {
  int minutes = seconds / 60;
  int remainingSeconds = seconds % 60;
  if (blinkTime.hasPassed(500)) {
    decimalPointOn = !decimalPointOn;
    blinkTime.restart();
  }
  if (decimalPointOn) {
    // Display the decimal point by setting the corresponding bit in the digit pattern
    displayDigit(minutes / 10, DIGIT1, false);  // Set the decimal point for the first digit
    displayDigit(minutes % 10, DIGIT2, true);
    displayDigit(remainingSeconds / 10, DIGIT3, false);
    displayDigit(remainingSeconds % 10, DIGIT4, false);
  } else {
    // Display without the decimal point
    displayDigit(minutes / 10, DIGIT1, false);
    displayDigit(minutes % 10, DIGIT2, false);
    displayDigit(remainingSeconds / 10, DIGIT3, false);
    displayDigit(remainingSeconds % 10, DIGIT4, false);
  }
}

// mian project display function 
void menuAccess() {
  menuTimer();

  // Countdown timer
  if (activated) {

    ////// POWER OUTPUT  AND INDICATORS //////////
    digitalWrite(RELAY, HIGH);
    digitalWrite(ACTIVELED, HIGH);
    digitalWrite(INACTIVELED, LOW);

    // Start countdowns if not already running
    if (!timerStarted) {
      rtc.setEpoch(1451606400);  //  reset epoch to Jan 1, 2016
      timer = rtc.getEpoch();
      timerStarted = true;
    }

    // Calculate and display remaining time in MM:SS format
    unsigned long remainingTime = countVal - (rtc.getEpoch() - timer);
    displayCountdown(remainingTime);

    // Check for countdown completion
    if (timerStarted && (rtc.getEpoch() - timer >= countVal)) {
      displayCountdown(0);
      timerStarted = false;
      activated = false;
      timer = 0;// clear counter after each countdown
      countVal = 0;// clear count value
    }
    // displayCountdown(countVal);
    // if (countdownTime.hasPassed(1000)) {
    //   if (countVal > 0) {
    //     countVal--;
    //   } else {
    //     activated = false;
    //   }
    //   countdownTime.restart();
    // }
  } else {

    ////// POWER OUTPUT  AND INDICATORS //////////
    digitalWrite(RELAY, LOW);
    digitalWrite(ACTIVELED, LOW);
    digitalWrite(INACTIVELED, HIGH);

    /// Displays
    if (!inMenu) {
      // Display home screen
      displayNumber(usageData);
      menu = 1;
      submenu = 0;
    } else {
      if (blinkTimeM.hasPassed(500)) {
        blinkTimeM.restart();
        // Toggle the state of the decimal point for the next iteration
        if (State == 0) {
          State = 1;
        } else {
          State = 0;
        }
      }
      // Display menu screen
      if ((menu == 1) && (submenu == 0)) {
        displayMenu(State, menu, timeVal, 1);
      } else if ((menu == 1) && (submenu == 1)) {
        displayMenu(State, menu, timeVal, 2);
      } else if ((menu == 2) && (submenu == 0)) {
        displayMenu(State, menu, volVal, 1);
      } else if ((menu == 2) && (submenu == 2)) {
        displayMenu(State, menu, volVal, 2);
      }
    }
  }

  /// Menu Controls
  if (debounce(1, ENTBTN)) {
    menuTime.restart();
    if (inMenu == false) {
      inMenu = true;
    } else if (inMenu && (menu == 1) && (submenu == 0)) {
      submenu = 1;
    } else if (inMenu && (menu == 2) && (submenu == 0)) {
      submenu = 2;
    } else if (inMenu && ((menu == 1) || (menu == 2)) && ((submenu == 1) || (submenu == 2))) {
      EEPROM.update(timevalAddress, timeVal);
      EEPROM.update(volvalAddress, volVal);
      EEPROM.commit();
      submenu = 0;
    }
  }

  if (debounce(2, UPBTN)) {
    if (inMenu && (menu == 1) && (submenu == 1)) {
      menuTime.restart();
      if (timeVal == 30) {
        timeVal = 30;
      } else {
        timeVal++;
      }
    } else if (inMenu && (menu == 1) && (submenu == 0)) {
      menuTime.restart();
      menu++;
      if (menu == 2) menu = 2;
    } else if (inMenu && (menu == 2) && (submenu == 2)) {
      menuTime.restart();
      if (volVal == 20) {
        volVal = 20;
      } else {
        volVal++;
      }
    }
  }

  if (debounce(3, DWNBTN)) {
    if (inMenu && (menu == 1) && (submenu == 1)) {
      menuTime.restart();

      if (timeVal == 0) {
        timeVal = 0;
      } else {
        timeVal--;
      }
    } else if (inMenu && (menu == 2) && (submenu == 0)) {
      menuTime.restart();
      menu--;
      if (menu == 1) menu = 1;
    } else if (inMenu && (menu == 2) && (submenu == 2)) {
      menuTime.restart();

      if (volVal == 0) {
        volVal = 0;
      } else {
        volVal--;
      }
    }
  }
}

/// RFID FUNCTIONS
void init_Rfid() {
  SPI.begin();         // Init SPI bus
  mfrc522.PCD_Init();  // Init MFRC522 card (in case you wonder what PCD means: proximity coupling device)
  // Prepare the security key for the read and write functions.
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;  //keyByte is defined in the "MIFARE_Key" 'struct' definition in the .h file of the library
  }
}


void writeVal() {
  // that is: sector #1, covering block #4 up to and including block #7
  // Look for new cards
  if (activated == false) {
    if (!mfrc522.PICC_IsNewCardPresent()) {
      return;
    }

    // Select one of the cards
    if (!mfrc522.PICC_ReadCardSerial()) {
      return;
    }

    byte sector = 1;
    byte blockAddr = 4;
    byte dataBlock[] = { "DEACTIVATED" };
    byte trailerBlock = 7;
    MFRC522::StatusCode status;
    byte buffer[18];
    byte size = sizeof(buffer);
    // Authenticate using key A
    status = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
      return;
    }

    // Read data from the block
    status = (MFRC522::StatusCode)mfrc522.MIFARE_Read(blockAddr, buffer, &size);
    if (status != MFRC522::STATUS_OK) {
    }

    bool isactive = dump_byte_array(buffer, 16);

    if (isactive) {
      // Write data to the block
      status = (MFRC522::StatusCode)mfrc522.MIFARE_Write(blockAddr, dataBlock, 16);
      if (status != MFRC522::STATUS_OK) {
      }
      ///SET ACTION
      activated = true;
      countVal = timeVal * 60;
      usageData++;
      EEPROM.put(address, usageData);
    }
    // Halt PICC
    mfrc522.PICC_HaltA();
    // Stop encryption on PCD
    mfrc522.PCD_StopCrypto1();
  }
}

bool dump_byte_array(byte* Databuffer, byte DatabufferSize) {
  // Create a character array to hold the string
  char str[DatabufferSize + 1];  // +1 for the null character
  // Copy the bytes from the buffer to the string
  for (byte i = 0; i < DatabufferSize; i++) {
    str[i] = Databuffer[i];
  }
  // Null-terminate the string
  str[DatabufferSize] = '\0';

  // Compare the string with "JRIDE-ACTIVATED"
  if (strcmp(str, "ACTIVATED") == 0) {
    return true;
  } else {
    return false;
  }
}

///////////////// BATTERY VOLTAGE MEASUREMENT /////////////////////////

void readBatteryVoltage() {
  int rawValue = analogRead(batteryPin);

  // The ADC reference voltage for ESP8266 is 3.3V (default)
  int voltage = (rawValue * 3.3 * 10.8) / 1024;
  int numLEDs = map(voltage, minVoltage, maxVoltage, 0, 4);
  for (int i = 0; i < 4; i++) {
    if (i < numLEDs) {
      digitalWrite(ledPins[i], HIGH);
    } else {
      digitalWrite(ledPins[i], LOW);
    }
  }
}


void setup() {
  // Set digit pins as outputs
  pinMode(A_PIN, OUTPUT);
  pinMode(B_PIN, OUTPUT);
  pinMode(C_PIN, OUTPUT);
  pinMode(D_PIN, OUTPUT);
  pinMode(E_PIN, OUTPUT);
  pinMode(F_PIN, OUTPUT);
  pinMode(G_PIN, OUTPUT);
  pinMode(DP_PIN, OUTPUT);
  pinMode(DIGIT1, OUTPUT);
  pinMode(DIGIT2, OUTPUT);
  pinMode(DIGIT3, OUTPUT);
  pinMode(DIGIT4, OUTPUT);

  pinMode(RELAY, OUTPUT);
  pinMode(ACTIVELED, OUTPUT);
  pinMode(INACTIVELED, OUTPUT);

  // init RTC
  rtc.begin();

  // initialize digital pin LED_BUILTIN as an output.
  for (int i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }

  // initialize the BTNS as an input.
  pinMode(UPBTN, INPUT_PULLUP);
  pinMode(DWNBTN, INPUT_PULLUP);
  pinMode(ENTBTN, INPUT_PULLUP);

  // read a byte from the current address of the EEPROM
  // usageData = 10;
  // EEPROM.put(address, usageData);
  EEPROM.get(address, usageData);
  timeVal = EEPROM.read(timevalAddress);
  volVal = EEPROM.read(volvalAddress);
  init_Rfid();
  readBatteryVoltage();
}

void loop() {
  writeVal();
  menuAccess();
  if (loopTime.hasPassed(10000)) {
    readBatteryVoltage();
    loopTime.restart();
  }
}
