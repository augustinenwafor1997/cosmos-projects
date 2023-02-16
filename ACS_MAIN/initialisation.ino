void initialisation() {
  QRSerial.begin(115200);
  ESPSerial.begin(115200);

  pinMode(entrySolenoid, OUTPUT);
  pinMode(exitSolenoid, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(amberLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(exitAmberLed, OUTPUT);
  pinMode(exitGreenLed, OUTPUT);
  pinMode(exitBuzzer, OUTPUT);
  pinMode(entrySwitch, INPUT_PULLUP);
  pinMode(exitSwitch, INPUT_PULLUP);
  Wire.begin();
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  if (!SD.begin(chipSelect)) {
    Serial.println("Initialization failed!");
    return;
  }
  digitalWrite(entrySolenoid, HIGH);//lock
  digitalWrite(amberLed, HIGH);

  digitalWrite(exitSolenoid, HIGH);//lock
  digitalWrite(exitAmberLed, HIGH);

//  HardwareTimer *MyTim = new HardwareTimer(TIM1);
//  MyTim->pause();
//  MyTim->setOverflow(10000, HERTZ_FORMAT);
//  MyTim->attachInterrupt(exitControls);
//  MyTim->resume();
}
