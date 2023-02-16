#ifndef _DECLARATIONS_H_
#define _DECLARATIONS_H_

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Chrono.h>
Chrono entryChrono(Chrono::SECONDS);
Chrono exitChrono(Chrono::SECONDS);
#include "RTClib.h"

RTC_DS3231 rtc;

const int chipSelect = 53;
const int entrySolenoid = 4;
const int exitSolenoid = 5;
const int entrySwitch = 6;
const int exitSwitch = 7;
const int redLed = 8;
const int amberLed = 9;
const int greenLed = LED_BUILTIN;
const int buzzer = 11;
const int exitSensor = A3;
const int exitAmberLed = 12;
const int exitGreenLed = LED_BUILTIN;
const int exitBuzzer = 13;
//const int exitSolenoid = 5;
#define QRSerial Serial
#define ESPSerial Serial2

enum doorState {closed, open} entryDoorState;
doorState exitDoorState;
//struct breakup
//{
//  int User_ID;
//  int token;
//  int tokpair;
//};
//breakup data;

#endif
