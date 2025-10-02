void closeV()
{
  loadState = true;
  loadChrono.restart();
  if (irr_valve == RUNNING)
  {
    char valve_A[4] = {valve_1A, valve_2A, valve_3A, valve_4A};
    char valve_B[4] = {valve_1B, valve_2B, valve_3B, valve_4B};
    // stop running

    irr_valve = SHUTOFF;
    for (int i = 0; i < sizeof(valve_A); i++)
    {
      digitalWrite(valve_A[i], LOW);
      digitalWrite(valve_B[i], HIGH);
    }
    delay(100); // clarify actual delay time
    for (int i = 0; i < sizeof(valve_A); i++)
    {
      digitalWrite(valve_A[i], LOW);
      digitalWrite(valve_B[i], LOW);
    }
  }
  else if (wOutlet == PUMP && digitalRead(pumpPin))
  {
    digitalWrite(pumpPin, LOW);
    // digitalWrite (PC13, HIGH);//test
  }
}

void sms()
{
  static bool errorFlag = 0, flag = 0, cycleFlag = 0, timerFlag = 0, phoneFlag = 0, passFlag = 0;
  static uint8_t startdate = 0;
  static enum manual_state {DEF, MSTART, MSTOP} mState;
  static bool sMode = 0, mMode = 0;
  static unsigned long sMills = 0UL;
  /**************** RESTART SIM800 ***************/
  if (simStatus == EROR)
  {
    errorFlag = 1;
    //static bool flag = false;
    static unsigned long erorMills = millis();
    if (!flag && millis() - erorMills >= 1000)
    {
      mySerial.println("AT+CFUN=0"); // turn on flight mode
      erorMills = millis();
      flag = true;
    }
    if (flag && millis() - erorMills >= 300)
    {
      mySerial.println("AT+CFUN=1"); // turn off flight mode
      erorMills = millis();
      flag = false;
    }
  }
  else if (simStatus == READY)
  {
    errorFlag = 0;
  }
  /**********  CHECK FOR SIMCARD  **************/
  if ((gsmMode == SMS || gsmMode == GPRS) && (errorFlag == 0 || flag == 0) && sMode == 0 && mMode == 0)
  {
    static unsigned long buff_time = millis();

    if ((millis() - buff_time >= 7000) /* && buff_check == true*/)
    {
      mySerial.println("AT+CPIN?");
      buff_time = millis();
      mySerial.flush();
    }
  }

  /**********  GET SIGNAL QUALITY  **************/
  if (simStatus == READY  && sMode == 0 && mMode == 0)
  {
    static unsigned long csq = millis();
    if ((millis() - csq >= 20000))
    {
      mySerial.println("AT+CSQ");
      csq = millis();
      mySerial.flush();
    }
  }

  /**********  CHECK SIM OWNER  **************/
  if (simStatus == READY && gsmMode == GPRS)
  {
    static bool netCheck = false;
    if (!netCheck)
    {
      mySerial.println("AT+COPS?");
    }
    netCheck = true;
  }

  /**********  GET NETWORK TIME  **************/
  static bool clkSave = false;
  if (simStatus == READY)
  {
    static bool clkCheck = false, synchSet = false;
    static unsigned long saveMills = millis();
    if (!synchSet)
    {
      mySerial.println("AT+CLTS=1");
      synchSet = true;
      saveMills = millis();
    }
    if (synchSet == true && clkSave == false && millis() - saveMills >= 100)
    {
      mySerial.println("AT&W");
      clkSave = true;
      saveMills = millis();
    }
    if (!clkCheck && clkSave == true && millis() - saveMills >= 100)
    {
      mySerial.println("AT+CCLK?");
      clkCheck = true;
      saveMills = 0UL;
    }
  }

  /*** CONFIGURE TEXT MODE  ***/
  if (gsmMode == SMS && simStatus == READY && clkSave == true)
  {
    static bool config = false, del = false, tmode = false;
    static unsigned long configMills = millis();
    if (!tmode && millis() - configMills >= 15000)
    {
      mySerial.println("AT+CMGF=1");
      tmode = true;
      configMills = millis();
    }
    if (tmode == true && config == false && millis() - configMills >= 100)
    {
      mySerial.println("AT+CNMI=1,2,0,0,0");
      config = true;
      configMills = millis();
    }
    if (config == true && ((del == false && millis() - configMills >= 100) || millis() - configMills >= 86400000))
    {
      mySerial.println("AT+CMGDA=\"DEL ALL\"");
      del = true;
      configMills = millis();
    }
    //mySerial.flush();
  }

  recvSMS(); // PROCCESSES YOUR MESSAGEBODY
  if (gsmMode == SMS)
  {
    if (messagestatus)
    { //messagestatus says when sms is recieved
      messageBody.toLowerCase();
      messageBody.replace(" ", "");
      messageBody.replace("\r", "");
      messageBody.replace("\n", "");
      homeScr = messageBody;

      /*****NORMAL SETTINGS MODE*****/
      if (messageBody.startsWith("pwd"))
      {
        String pass = getValue(getValue(messageBody, ':', 1), ',', 0);
        //homeScr = pass;
        // ySerial.println("PASSWORD:");
        // ySerial.println(pass);
        /*****VALID PASSWORD DETECTION*****/
        if (pass.length() == 4)
        {
          //compare password here
          if (pass == password)
          {

            /*****MODE SELECTION*****/
            if (strstr(messageBody.c_str(), "mode") != NULL)
            {

              /*****CYCLE IRRIGATION SETTINGS*****/
              if (strstr(messageBody.c_str(), "cycle") != NULL)
              {
                // ySerial.println("CYCLE FOUND");
                if (irr_mode != LOOP) {
                  irr_mode = LOOP;
                  EEPROM.put(0 * sizeof(irr_mode), irr_mode);
                  closeV();
                }
                if (strstr(messageBody.c_str(), "run") != NULL)
                {
                  //collect run time
                  String run = smsVal(messageBody, "run:", ',');
                  uint16_t runn = run.toInt();
                  if (runn > 0)
                  {
                    All_Str.cyc.Run = runn;
                  }
                }
                if (strstr(messageBody.c_str(), "rest") != NULL)
                {
                  //collect rest time
                  String rest = smsVal(messageBody, "rest:", ',');
                  uint16_t rstt = rest.toInt();
                  if (rstt > 0)
                  {
                    All_Str.cyc.Rest = rstt;
                  }
                }
                EEPROM.put(2 * sizeof(All_Str.cyc), All_Str.cyc);
                firstRun = true;
                loopState = REST;
                cycleFlag = true;// flag to send reply
              }


              /*****TIMER IRRIGATION SETTINGS*****/
              if (strstr(messageBody.c_str(), "timer") != NULL)// if mode is timer
              {
                //Serial.println("timer");
                if (strstr(messageBody.c_str(), "prog") != NULL)
                {
                  String prog = smsVal(messageBody, "prog:", ',');
                  if (prog != "")// if program is valid
                  {
                    if (irr_mode != PROGS)
                    { // save irrigation mode only once
                      All_Str.prog_count = 0;
                      irr_mode = PROGS;
                      EEPROM.put(0 * sizeof(irr_mode), irr_mode);
                      closeV();
                    }
                    uint8_t prg = prog.toInt();// convert to int
                    if (prg > 0 && prg < 13)//check if it is valid
                    {
                      All_Str.tim.Prog = prg;//store new prog


                      if (strstr(messageBody.c_str(), "hour") != NULL)// if hour is set
                      {
                        String hou = smsVal(messageBody, "hour:", ',');
                        int sthr = hou.toInt();// convert to int
                        if (sthr > 0)//check if it is valid
                        {
                          All_Str.tim.StartHr = sthr;//store start hour
                        }

                      }
                      if (strstr(messageBody.c_str(), "min") != NULL)
                      {
                        String mint = smsVal(messageBody, "min:", ',');
                        int stmn = mint.toInt();// convert to int
                        if (stmn > 0)//check if it is valid
                        {
                          All_Str.tim.StartMin = stmn;//store start min
                        }
                      }

                      //**************duration*******************\\
                      if (strstr(messageBody.c_str(), "duration") != NULL)
                      {
                        String durations = smsVal(messageBody, "duration:", ',');
                        int rtim = durations.toInt();// convert to int
                        if (rtim > 0)//check if it is valid
                        {
                          All_Str.tim.Runtime = rtim;//store runtime
                        }
                      }

                      //*****************interval*******************\\
                      if (strstr(messageBody.c_str(), "interval") != NULL)
                      {
                        String intaval = smsVal(messageBody, "interval:", ',');
                        uint8_t intv = intaval.toInt();// convert to int
                        if (intv > 0)//check if it is valid
                        {
                          All_Str.tim.Interval = intv;//store interval
                        }
                      }


                      if (strstr(messageBody.c_str(), "zone") != NULL)
                      {
                        String valve = smsVal(messageBody, "zone:", ',');
                        static const char *valve_value[5] = {"1", "2", "3", "4", "all"};
                        for (int i = 0; i < 5; i++)
                        {
                          if (valve == valve_value[i])
                            All_Str.tim.Valve = i;
                        }
                      }
                      const uint8_t MUX = 6;
                      if (All_Str.tim.Prog > All_Str.prog_count)
                      { // keep record of number of Programs set
                        All_Str.prog_count = All_Str.tim.Prog;
                        EEPROM.put(4 * sizeof(All_Str.prog_count), All_Str.prog_count);
                      }
                      /*
                               below block of if statement is to handle the exception of setting irrigation time that has passed
                               if the set time has elapsed, it automatically postpones the irrigation to the next day
                      */
                      if ((All_Str.tim.StartHr < rtc.getHours()) || (All_Str.tim.StartHr == rtc.getHours() && All_Str.tim.StartMin < rtc.getMinutes()))
                        startdate = 1;
                      else
                        startdate = 0;

                      futureTime(startdate); //get the day to start irrigation. Normally starts today calc future day, month and year
                      Programs[(All_Str.tim.Prog - 1)] = All_Str.tim; //add new program to struct array
                      EEPROM.put(5 * MUX, Programs);
                      next_irr_flag = true;
                      timerFlag = true;
                    }
                  }
                }
              }

              /*****MANUAL IRRIGATION*****/
              if (strstr(messageBody.c_str(), "manual") != NULL)
              {
                if (irr_mode != MANUAL)
                {
                  irr_mode = MANUAL;
                  EEPROM.put(0 * sizeof(irr_mode), irr_mode);
                }
                if (strstr(messageBody.c_str(), "start") != NULL)
                {
                  loadState = true;
                  loadChrono.restart();
                  mState = MSTART;
                  char valve_A[4] = {valve_1A, valve_2A, valve_3A, valve_4A};
                  char valve_B[4] = {valve_1B, valve_2B, valve_3B, valve_4B};
                  if (wOutlet == VALVE)
                  {
                    irr_valve = RUNNING;
                    for (int i = 0; i < sizeof(valve_A); i++)
                    {
                      digitalWrite(valve_A[i], HIGH);
                      digitalWrite(valve_B[i], LOW);
                    }
                    delay(100); // clarify actual delay time
                    for (int i = 0; i < sizeof(valve_A); i++)
                    {
                      digitalWrite(valve_A[i], LOW);
                      digitalWrite(valve_B[i], LOW);
                    }
                  }
                  else if (wOutlet == PUMP)
                  {
                    digitalWrite(pumpPin, HIGH);
                    // digitalWrite (PC13, LOW);//test
                  }
                }
                else if (strstr(messageBody.c_str(), "stop") != NULL)
                {
                  mState = MSTOP;
                  closeV();
                  // stop running
                  //add flag later*****************************************
                }
              }
            }

            /*****NETWORK SETTING*****/
            if (strstr(messageBody.c_str(), "network") != NULL)
            {
              //Serial.print("network mode: ");
              if (strstr(messageBody.c_str(), "sms") != NULL)
              {
                gsmMode = SMS;
                EEPROM.put(3 * sizeof(gsmMode), gsmMode);
              }
              else if (strstr(messageBody.c_str(), "gprs") != NULL)
              {
                gsmMode = GPRS;
                EEPROM.put(3 * sizeof(gsmMode), gsmMode);
              }
              else if (strstr(messageBody.c_str(), "off") != NULL)
              {
                gsmMode = NaN;
                EEPROM.put(3 * sizeof(gsmMode), gsmMode);
                simStatus = WAIT;
              }
            }

            /*****SYSTEM RESET*****/
            if (strstr(messageBody.c_str(), "reset") != NULL)
            {
              for (int i = 16 ; i < 320 ; i++)//delete cycle & timer settings
              {
                EEPROM.write(i, 0);
              }
              EEPROM.commit();

              irr_mode = MANUAL;
              EEPROM.put(0 * sizeof(irr_mode), irr_mode);
              All_Str.prog_count = 0;
              EEPROM.put(4 * sizeof(All_Str.prog_count), All_Str.prog_count);
              // stop running
              closeV();
            }
            /********PHONE NUMBER UPDATE*********/
            if (strstr(messageBody.c_str(), "phone") != NULL)
            {
              String phonenum = smsVal(messageBody, "phone:", ',');
              if (phonenum.charAt(0) == '0' && phonenum.length() == 11)
              {
                phone  = "+234" + phonenum.substring(1);
                EEPROMwrite(327, phone);
                phoneFlag = true;
                homeScr = phone;
              }
            }
          }
        }
      }

      // /*****PASSWORD RESET*****/
      else if (messageBody.startsWith("rstpwd"))
      {
        if (strstr(messageBody.c_str(), "oldpwd") != NULL)
        {
          String oldpwd = smsVal(messageBody, "oldpwd:", ',');
          if (strstr(messageBody.c_str(), "newpwd") != NULL)
          {
            String newpwd = smsVal(messageBody, "newpwd:", ',');
            if (oldpwd == password)
            {
              password = newpwd;
              EEPROMwrite(320, password);
              passFlag = true;
              homeScr = password;
            }
          }
        }
      }
      messagestatus = false;
    }
  }


  if (cycleFlag)
  {
    if (!sMode && !mMode)
    {
      mySerial.println("AT+CMGS=\"" + phone + "\"");
      sMills = millis();
      sMode = true;
    }

    if (sMode && !mMode && millis() - sMills >= 100)
    {
      mySerial.print("Cycle mode set to run for " + String(All_Str.cyc.Run) + " mins and rest for " + String(All_Str.cyc.Rest) + " mins."); //text content
      mMode = true;
      sMills = millis();
    }

    if (sMode && mMode && millis() - sMills >= 100)
    {
      mySerial.write(26);
      sMode = false;
      mMode = false;
      cycleFlag = false;
      sMills = millis() + 100000;
      mySerial.flush();
    }

  }
  if (timerFlag)
  {
    if (!sMode && !mMode)
    {
      mySerial.println("AT+CMGS=\"" + phone + "\"");
      sMills = millis();
      sMode = true;
    }

    if (sMode && !mMode && millis() - sMills >= 100)
    {
      String sDay;
      if (startdate == 0)
        sDay = "today";
      else
        sDay = "tomorrow";
      mySerial.print("Program " + String(All_Str.tim.Prog) + " set to start at " + String(All_Str.tim.StartHr) + ":" + String(All_Str.tim.StartMin) + " " + sDay + "."); //text content
      //mySerial.print("Hr: " + String(All_Str.tim.StartHr) + "\nMin: " + String(All_Str.tim.StartMin) + "\nRun: " + String(All_Str.tim.Runtime) + "\nInt: " + String(All_Str.tim.Interval) + "\nZone: " + String(All_Str.tim.Valve) + "\nProg: " + String(All_Str.tim.Prog) + "\nDy: " + String(All_Str.tim.futureDay) + "\nMth: " + String(All_Str.tim.futureMonth) + "\nYr: " + String(All_Str.tim.futureYr) + "\nWk: " + String(All_Str.tim.futureWkDay)); //text content
      mMode = true;
      sMills = millis();
    }

    if (sMode && mMode && millis() - sMills >= 100)
    {
      mySerial.write(26);
      sMode = false;
      mMode = false;
      timerFlag = false;
      sMills = millis() + 100000;
    }
  }

  if (mState == MSTART || mState == MSTOP)
  {
    if (!sMode && !mMode)
    {
      mySerial.println("AT+CMGS=\"" + phone + "\"");
      sMills = millis();
      sMode = true;
    }

    if (sMode && !mMode && millis() - sMills >= 100)
    {
      String sDay;
      if (mState == MSTART)
        sDay = "started";
      else
        sDay = "stopped";
      mySerial.print("Manual irrigation " + sDay + "."); //text content
      //mySerial.print("Hr: " + String(All_Str.tim.StartHr) + "\nMin: " + String(All_Str.tim.StartMin) + "\nRun: " + String(All_Str.tim.Runtime) + "\nInt: " + String(All_Str.tim.Interval) + "\nZone: " + String(All_Str.tim.Valve) + "\nProg: " + String(All_Str.tim.Prog) + "\nDy: " + String(All_Str.tim.futureDay) + "\nMth: " + String(All_Str.tim.futureMonth) + "\nYr: " + String(All_Str.tim.futureYr) + "\nWk: " + String(All_Str.tim.futureWkDay)); //text content
      mMode = true;
      sMills = millis();
    }

    if (sMode && mMode && millis() - sMills >= 100)
    {
      mySerial.write(26);
      sMode = false;
      mMode = false;
      mState = DEF;
      sMills = millis() + 100000;
    }
  }


  if (phoneFlag)
  {
    if (!sMode && !mMode)
    {
      mySerial.println("AT+CMGS=\"" + phone + "\"");
      sMills = millis();
      sMode = true;
    }

    if (sMode && !mMode && millis() - sMills >= 100)
    {
      //      mySerial.print("Timer set for " + String(All_Str.tim.StartHr) + " o' clock."); //text content
      mySerial.print("User phone number changed to: " + phone + " Successfully."); //text content
      mMode = true;
      sMills = millis();
    }

    if (sMode && mMode && millis() - sMills >= 100)
    {
      mySerial.write(26);
      sMode = false;
      mMode = false;
      phoneFlag = false;
      sMills = millis() + 100000;
    }
  }
  if (passFlag)
  {
    if (!sMode && !mMode)
    {
      mySerial.println("AT+CMGS=\"" + phone + "\"");
      sMills = millis();
      sMode = true;
    }

    if (sMode && !mMode && millis() - sMills >= 100)
    {
      //      mySerial.print("Timer set for " + String(All_Str.tim.StartHr) + " o' clock."); //text content
      mySerial.print("Password changed successfully."); //text content
      mMode = true;
      sMills = millis();
    }

    if (sMode && mMode && millis() - sMills >= 100)
    {
      mySerial.write(26);
      sMode = false;
      mMode = false;
      passFlag = false;
      sMills = millis() + 100000;
    }
  }
}


/***************** GSM DATA PROCESSOR FUNCTION ************************/
void recvSMS(void)
{
  // serialEvent();
  if (stringComplete)
  {
    //homeScr = inputString;
    // ySerial.println(inputString);
    if (getValue(getValue(inputString, '+', 1), ':', 0) == "CMT")
    {
      messageBody = getValue(inputString, '\n', 2);
      messagestatus = true;
    }
    if ((getValue(getValue(inputString, '+', 1), '?', 0)) == "CCLK")
    {
      // Serial.println("NETWORK TIME:");
      // Serial.println(getValue(getValue(inputString, '"', 1), '+', 0)); // DATE AND TIME
      uint8_t yr = getValue(getValue(inputString, '"', 1), '/', 0).toInt();
      uint8_t mth = getValue(getValue(inputString, '/', 1), '/', 0).toInt();
      uint8_t dy = getValue(getValue(inputString, '/', 2), ',', 0).toInt();
      uint8_t hr = getValue(getValue(inputString, ',', 1), ':', 0).toInt();
      uint8_t mint = getValue(getValue(inputString, ':', 2), ':', 0).toInt();
      uint8_t secs = getValue(getValue(inputString, ':', 3), '+', 0).toInt();
      if (yr != rtc.getYear() || mth != rtc.getMonth() || dy != rtc.getDay() || hr != rtc.getHours() || mint != rtc.getMinutes())
      {
        rtc.setDate(dy, mth, yr);
        rtc.setTime(hr, mint, secs);
      }
    }
    if ((getValue(getValue(inputString, '+', 1), '?', 0)) == "COPS")
    {
      // Serial.println("SIM OWNER IDENTIFIED:");
      // Serial.println(getValue(getValue(inputString, '\n', 1), '\r', 0));
      // Serial.println("SIM IS :");
      // Serial.println(getValue(getValue(inputString, '"', 1), '"', 0));   //mtn nigeria
      String sim_name = getValue(getValue(inputString, '"', 1), '"', 0); //eg: mtn nigeria
      sim_name.toLowerCase();
      // String APN, USER, PASS;
      if (sim_name.startsWith("mtn"))
      {
        APN = "web.gprs.mtnnigeria.net";
        USER = "";
        PASS = "";
      }
      else if (sim_name.startsWith("glo"))
      {
        APN = "gloflat";
        USER = "flat";
        PASS = "flat";
      }
      else if (sim_name.startsWith("airtel"))
      {
        APN = "internet.ng.airtel.com";
        USER = "";
        PASS = "";
      }
      else if (sim_name.startsWith("9mobile"))
      {
        APN = "9mobile";
        USER = "";
        PASS = "";
      }
    }
    if ((getValue(getValue(inputString, '+', 1), '\r', 0)) == "CSQ")
    {
      //      Serial.println("received signal quality:");
      //      Serial.println(getValue(getValue(inputString, '\n', 1), '\r', 0));
      //      Serial.println("quality is :");
      //      Serial.println(getValue(getValue(inputString, ' ', 1), ',', 0));
      // Serial.print("integar: ");
      // Serial.println(getValue(getValue(inputString, ' ', 1), ',', 0).toInt()); //signal quality integar
      //convert to int here
      sigStrength = getValue(getValue(inputString, ' ', 1), ',', 0).toInt(); //signal quality integar
    }
    if ((getValue(getValue(inputString, '+', 1), '?', 0)) == "CREG")
    {
      //      Serial.println("creg found:");
      //      Serial.println(getValue(getValue(inputString, '\n', 1), '\r', 0));
      //homeScr = getValue(getValue(inputString, '\n', 1), '\r', 0);
      if ((getValue(getValue(inputString, ',', 1), '\r', 0)) == "1")
      {
        //homeScr = "REGISTERED";
        // Serial.println("sim registered");
        // Serial.println(getValue(getValue(inputString, ',', 1), '\n', 0));
        //        Serial.println("destiny");
        simStatus = READY;
        // csq = millis();
      }
    }
    if ((getValue(getValue(inputString, '+', 1), '?', 0)) == "CPIN")
    {
      //      Serial.println("received data correctly");
      //      Serial.println(getValue(getValue(inputString, '\n', 1), '\r', 0));
      //homeScr = getValue(getValue(inputString, '\n', 1), '\r', 0);
      if ((getValue(getValue(inputString, '\n', 1), '\r', 0)) == "ERROR")
      {
        simStatus = EROR;
      }
      else if ((getValue(getValue(getValue(inputString, '\n', 1), ' ', 1), '\r', 0)) == "READY")
      {
        // Serial.println("cpin returns:");
        // Serial.println(getValue(getValue(getValue(inputString, '\n', 1), ' ', 1), '\r', 0));
        if (simStatus != READY)
        {
          simStatus = WAIT;
          mySerial.println("AT+CREG?");
        }
        //simStatus = READY;
      }
      //locationString = getValue(inputString, ':', 1);
    }
    inputString = "";
    stringComplete = false;
    //nxl = 0;
  }
}

/**************************************** DATA COLLECTION FUNCTIONS ******************************/
void serialEvent()
{
  while (mySerial.available())
  {
    collectSmsData((char)mySerial.read()); // get the new byte:
    //    Serial.print("received: ");
    //  Serial.println(mySerial.readString());
  }
}

void collectSmsData(char inChar)
{
  static uint8_t nxl = 0;
  //  // add it to the inputString:
  if (inChar == '\r')
  {
    nxl++;
  }
  if (inChar == '\r' && nxl >= 3)
  {
    stringComplete = true;
    nxl = 0;
    // Serial.println("complete sms: ");
    // Serial.println(inputString);
  }
  else
  {
    inputString += inChar;
  }
}

/************************ DATA PROCESSING FUNCTION ***************************/
/*****GET VALUE TO KEY*****/
String smsVal(String haystack, const char *needle, char seperator)
{
  haystack += seperator;
  //  int len = String(needle).length();
  //  int pos = haystack.indexOf(needle);
  int pos = haystack.indexOf(needle) + String(needle).length();
  for (int i = 1; i <= haystack.length(); i++)
  {
    if (haystack.charAt(pos + i) == seperator)
    {
      return haystack.substring(pos, pos + i);
    }
  }
  return "";
}

/****************** DATA FORMATING FUNCTION ***********************/
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++)
  {
    if (data.charAt(i) == separator || i == maxIndex)
    {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
