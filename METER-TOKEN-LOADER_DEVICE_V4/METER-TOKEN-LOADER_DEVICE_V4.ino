#include <Keypad.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(9, 10); // CE, CSN

#include <EEPROM.h>
#include "MapFloat.h"
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#include <Chrono.h>
#define led A1
const unsigned int MAX_INPUT = 512;
Chrono lcdChrono(Chrono::SECONDS);
Chrono displayChrono;
const byte addresses[][6] = {"00001", "00002"};
//////////// Initialize keypad ///////////////
const byte ROWS = 4;
const byte COLS = 4;
char token[21];
char datebuf[16];
char timesbuf[16];
char unitbuf[16];
char val[30];
char lcddisplay[21];
char lcddisplay1[16];
char lcddisplay2[16];
char lcddisplay3[16];
char lcddisplay4[16];
double tokVal = 0;
double unit;
int lcdState = 0, i = 0, fraudState, payState = 0, count = 0, physicalPaymentState = 0, dateState = 0, timeState = 1, unitState = 1, connectbool = 0;
char hexaKeys[ROWS][COLS] = {
  {'4', '1', '7', '*'},
  {'5', '2', '8', '0'},
  {'6', '3', '9', '#'},
  {'B', 'A', 'C', 'D'}
};
byte rowPins[ROWS] = {4, 5, 6, 7};
byte colPins[COLS] = {0, 1, 2, 3};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
char customKey;
char data[3][50];

// the 8 arrays that form each segment of the custom numbers
byte LA[8] =
{
  B00100, B01100, B10100, B00100, B00100, B00100, B00100, B00100
};

byte LE[8] =
{
  B10000, B10000, B10000, B10000, B10000, B10000, B10000, B11111
};

byte LT[8] =
{
  B11111, B10001, B10001, B10001, B10001, B10001, B10001, B10001
};
byte UB[8] =
{
  B10001, B10001, B10001, B10001, B10001, B10001, B10001, B11111
};
byte RT[8] =
{
  B00001, B00001, B00001, B00001, B00001, B00001, B00001, B00001
};
byte LL[8] =
{
  B11111, B00001, B00001, B00001, B00001, B00001, B00001, B11111
};
byte LB[8] =
{
  B11111, B10000, B10000, B10000, B10000, B10000, B10000, B11111
};
byte LR[8] =
{
  B11111, B00001, B00001, B00001, B00001, B00001, B00001, B00001
};
byte MB[8] =
{
  B11111, B10001, B10001, B10001, B10001, B10001, B10001, B11111
};
byte block[8] =
{
  B00001, B00001, B00001, B00001, B00001, B00001, B00001, B11111
};



void custom0(int num)
{ // uses segments to build the number 0
  lcd.setCursor(num, 0); // set cursor to column 0, line 0 (first row)
  lcd.write(0);          // call each segment to create
  lcd.setCursor(num, 1); // set cursor to colum 0, line 1 (second row)
  lcd.write(1);          // call each segment to create
}

void custom1(int num)
{
  lcd.setCursor(num, 0);
  lcd.write(2);
  lcd.setCursor(num, 1);
  lcd.write(2);
}

void custom2(int num)
{
  lcd.setCursor(num, 0);
  lcd.write(5);
  lcd.setCursor(num, 1);
  lcd.write(4);
}

void custom3(int num)
{
  lcd.setCursor(num, 0);
  lcd.write(3);
  lcd.setCursor(num, 1);
  lcd.write(7);
}

void custom4(int num)
{
  lcd.setCursor(num, 0);
  lcd.write(1);
  lcd.setCursor(num, 1);
  lcd.write(2);
}

void custom5(int num)
{
  lcd.setCursor(num, 0);
  lcd.write(4);
  lcd.setCursor(num, 1);
  lcd.write(7);
}

void custom6(int num)
{
  lcd.setCursor(num, 0);
  lcd.write(4);
  lcd.setCursor(num, 1);
  lcd.write(1);
}

void custom7(int num)
{
  lcd.setCursor(num, 0);
  lcd.write(5);
  lcd.setCursor(num, 1);
  lcd.write(2);
}

void custom8(int num)
{
  lcd.setCursor(num, 0);
  lcd.write(6);
  lcd.setCursor(num, 1);
  lcd.write(1);
}

void custom9(int num)
{
  lcd.setCursor(num, 0);
  lcd.write(6);
  lcd.setCursor(num, 1);
  lcd.write(7);
}

void clearnumber()
{ // clears the area the custom number is displayed in
  lcd.setCursor(0, 0);
  lcd.print("   ");
  lcd.setCursor(0, 1);
  lcd.print("   ");
}

void getnumber(int colunm, char num)
{
  switch (num)
  {
    case '0':
      custom0(colunm);
      break;
    case '1':
      custom1(colunm);
      break;
    case '2':
      custom2(colunm);
      break;
    case '3':
      custom3(colunm);
      break;
    case '4':
      custom4(colunm);
      break;
    case '5':
      custom5(colunm);
      break;
    case '6':
      custom6(colunm);
      break;
    case '7':
      custom7(colunm);
      break;
    case '8':
      custom8(colunm);
      break;
    case '9':
      custom9(colunm);
      break;
    case '.':
      lcd.setCursor(colunm, 1);
      lcd.print(".");
      break;
    case ':':
      lcd.setCursor(colunm, 0);
      lcd.print(".");
      lcd.setCursor(colunm, 1);
      lcd.print(".");
      break;
  }
}

void custom_print(int colunm, char var[16])
{

  for (int i = 0; i <= 16; i++)
  {
    getnumber(colunm + i, var[i]);
  }
}


void checkunits() {
  unit = atof(unitbuf);
  if (unit >= 10) {
    digitalWrite(led, HIGH);
  }
  else digitalWrite(led, LOW);
}

void setup() {
  // Serial.begin(9600);
  pinMode(led, OUTPUT);
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, LT);
  lcd.createChar(1, UB);
  lcd.createChar(2, RT);
  lcd.createChar(3, LL);
  lcd.createChar(4, LB);
  lcd.createChar(5, LR);
  lcd.createChar(6, MB);
  lcd.createChar(7, block);
  radio.begin();
  radio.openWritingPipe(addresses[0]); // 00001
  radio.openReadingPipe(1, addresses[1]); // 00002
  radio.setPALevel(RF24_PA_MIN);

  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("CONNECTING...");
  delay(1000);
  lcd.noBacklight();
  lcd.clear();
}
void loop() {
  checkunits();
  delay(5);
  radio.startListening();
  if ( radio.available()) {
    while (radio.available()) {
      radio.read(&val, sizeof(val));
      //      Serial.print("Received date/time from MEGA: ");
      //      Serial.println(val);
    }
    getValue(val, ' ', 0).toCharArray(datebuf, 11);
    getValue(val, ' ', 1).toCharArray(timesbuf, 9);
    getValue(val, ' ', 2).toCharArray(unitbuf, 6);
    delay(5);
    radio.stopListening();
    if (physicalPaymentState == 1) {
      radio.write(&tokVal, sizeof(tokVal));
      physicalPaymentState = 0;
      }
  }
  customKey = customKeypad.getKey();
  if (customKey && lcdState == 0) {
    if (customKey == '*') {
      lcdState = 1;
    }
    else {
      lcdState = 0;
      offline_payment();
    }
  }
  if (lcdState == 1)home_display();
  if (lcdState == 1 && lcdChrono.hasPassed(21)) {
    lcd.noBacklight();
    lcd.clear();
    lcdState = 0;
    lcdChrono.restart();
  }
  
}
void home_display() {
  lcd.backlight();
  if (dateState == 0 && timeState == 1 && unitState == 1 && displayChrono.hasPassed(3000)) {
    lcd.clear();
    lcd.print("DATE");

    custom_print(4, datebuf);
    dateState = 1;
    timeState = 0;
    displayChrono.restart();
  }
  if (timeState == 0 && dateState == 1 && unitState == 1 && displayChrono.hasPassed(3000)) {
    lcd.clear();
    lcd.print("TIME");

    custom_print(4, timesbuf);
    unitState = 0;
    timeState = 1;
    displayChrono.restart();
  }
  if (unitState == 0 && dateState == 1 && timeState == 1 && displayChrono.hasPassed(3000)) {
    lcd.clear();
    lcd.print("kwH");

    custom_print(5, unitbuf);
    unitState = 1;
    dateState = 0;
    displayChrono.restart();
  }
}

String getValue(String data, char separator, int index)
{
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
float getrecharge(int input) {
  //  return map(input, 1026, 9054, 0, 1000);
  if (input >= 1026 && input <= 9054) {
    return mapFloat(input, 1026.0, 9054.0, 0.0, 1000.0);
  }
  else
    return 0.0;
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


void offline_payment() {
  // upload online that payments have been made
  // if payment is made

  if (customKey == '#') {
    i = 0;
    lcd.clear();
    lcd.noBacklight();
    return;
  }
  else if (customKey == 'A' || customKey == 'B' || customKey == 'C' || customKey == 'D') {

  }
  else {
    lcd.backlight();
    if (i <= 19) {
      token[i] = customKey;
      lcddisplay[i] = customKey;
      if (i >= 1)lcddisplay1[i - 1] = customKey;
      if (i >= 2)lcddisplay2[i - 2] = customKey;
      if (i >= 3)lcddisplay3[i - 3] = customKey;
      if (i >= 4)lcddisplay4[i - 4] = customKey;

      if (i == 16)custom_print(0, lcddisplay1);
      else if (i == 17)custom_print(0, lcddisplay2);
      else if (i == 18)custom_print(0, lcddisplay3);
      else if (i == 19)custom_print(0, lcddisplay4);
      else custom_print(0, lcddisplay);

      i++;

      //         if (i == 16) {
      //        lcd.setCursor(0, 1);
      //      }
      //      i++;
      //      lcd.print(customKey);

    }
  }
  if (i == 20) {
    lcd.clear();
    lcd.print("PROCESSING...");
    delay(1000);
    if (rearrangetoken(token) == 1) {
      lcd.clear();
      lcd.print("VALID TOKEN");
      delay(1000);
      String date = String(variable.date);
      String times = String(variable.rechargetime);
      String dt = date + times;
      long merger = dt.toInt();
      long mem;
      for (int j = 0; j <=  500; j++) {
        EEPROM.get(j * sizeof(mem), mem);
        if (mem == merger) {
          lcd.clear();
          lcd.print("USED TOKEN");
          delay(1000);
          fraudState = 1;
          break;
        }
        else {
          fraudState = 0;

        }
      }
      if (fraudState == 0) {
        fraudState = 1;
        tokVal = getrecharge(variable.token);
        if (tokVal != 0) {
          // TURN ENERGY TO ZERO HERE AND UPDATE ALL MAN
          physicalPaymentState = 1;
          payState = 1;
          // PRINT SUCCESSFUL ON LCD
          lcd.clear();
          lcd.print("RECHARGE SUCCESSFUL");
          delay(1000);
        }
        else {
          lcd.clear();
          lcd.print("FAILED");
          delay(1000);
          payState = 0;
          //          break; // check this line later.....................
        }
      }
      if (payState == 1) {
        lcd.clear();
        lcd.print("SAVE TO MEMORY");
        delay(1000);
        payState = 0;
        long info2;
        for (int address = 0; address <=  500; address++) {
          EEPROM.get(address * sizeof(info2), info2);
          if (info2 == 0) {
            EEPROM.put(address * sizeof(merger), merger);
            count++;
            if (count > 998) {
              int clearVal = 0;
              for (int address = 0; address <=  500; address++) {
                EEPROM.put(address * sizeof(clearVal), clearVal);
              }
              count = 0;
            }
            int address = 2 * sizeof(count);
            EEPROM.put(address, count);
            break;
          }
        }
      }
      lcd.clear();
      lcd.print(tokVal);
      delay(1000);
    }
    else {
      lcd.clear();
      lcd.print("INVALID TOKEN!");
      delay(1000);
    }
    memset(token, 0, sizeof(token));
    memset(lcddisplay, 0, sizeof(lcddisplay));
    i = 0;
    lcdState = 1;
  }
}
