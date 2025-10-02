/* calculate a future date */
void futureTime(int addValue)
{ //
  unsigned long futureEpochTime = (rtc.getEpoch() + (addValue * 86400));
  All_Str.tim.futureDay = day(futureEpochTime);
  All_Str.tim.futureMonth = month(futureEpochTime);
  All_Str.tim.futureYr = year(futureEpochTime) - 2000;
  All_Str.tim.futureWkDay = weekday(futureEpochTime);
}
/**Program to empty struct array**/
void emptystruct()
{
  struct Timer timers;
  timers = {
    .Prog = 0,
    .StartHr = 0,
    .StartMin = 0,
    .Runtime = 0,
    .Interval = 0,
    .Valve = 0,
    .futureYr = 0,
    .futureMonth = 0,
    .futureDay = 0,
    .futureWkDay = 0
  };
  for (int i = 0; i < PROG_SIZE; i++)
  {
    Programs[i] = timers;
  }
}
/***Irrigating function for timers***/
void timer_irrigate()
{
  const uint8_t MUX = 6;
  if (irr_mode == PROGS)
  { //if we are in timer mode
    static uint8_t curr_prog = 0, curr_clash_count = 0;
    static int curr_clash_progs[PROG_SIZE];
    static bool mtimerState[PROG_SIZE];
    /***Below block gets next program on startup, menu setting or current program running***/
    if (next_irr_flag == true)
    {
      //EEPROM.get(5 * sizeof(All_Str.prog_count), Programs);
      next_prog = next_irrigation();
      next_irr_flag = false;
    }
    /***Below block handles single timer irrigation***/
    if (next_prog > 0 && next_prog < 13)
    {
      //irrState = 1;//test
      if ((fYr(&Programs[next_prog - 1]) == rtc.getYear()) && (fMth(&Programs[next_prog - 1]) == rtc.getMonth()) && (fDay(&Programs[next_prog - 1]) == rtc.getDay()) && (starthour(&Programs[next_prog - 1]) == rtc.getHours()) && (startmin(&Programs[next_prog - 1]) == rtc.getMinutes()))
      {
        //irrState = 1;                                 // test
        futureTime(Programs[next_prog - 1].Interval); //calculate future time
        /***store the next irrigation date***/
        Programs[next_prog - 1].futureWkDay = All_Str.tim.futureWkDay;
        Programs[next_prog - 1].futureDay = All_Str.tim.futureDay;
        Programs[next_prog - 1].futureMonth = All_Str.tim.futureMonth;
        Programs[next_prog - 1].futureYr = All_Str.tim.futureYr;
        EEPROM.put(5 * MUX, Programs);
        // emptystruct();                                        //test
        // EEPROM.get(5 * sizeof(All_Str.prog_count), Programs); //test
        next_irr_flag = true;                                 // raise flag
        curr_prog = next_prog;                                // flag current program
        curr_clash_count = 0;
        if (rSense == ON)
        { // if soil moisture sensor is connected
          int sensor_val = readSoilHumidity();
          if (sensor_val < All_Str.hum)
          { // if it has not rained
            loadState = true;
            loadChrono.restart();
            if (wOutlet == PUMP)
            { //pump irrigation
              digitalWrite(pumpPin, HIGH);
              mtimerState[curr_prog - 1] = true;
              mtimerChrono[curr_prog - 1].restart();
            }
            else if (wOutlet == VALVE)
            { //valve irrigation
              irr_valve = RUNNING;
              char valve_A[4] = {valve_1A, valve_2A, valve_3A, valve_4A};
              char valve_B[4] = {valve_1B, valve_2B, valve_3B, valve_4B};
              uint8_t valve = Programs[curr_prog - 1].Valve; // get valve setting
              if (valve == 0)
              { //valve 1
                digitalWrite(valve_1A, HIGH);
                digitalWrite(valve_1B, LOW);
                delay(100);
                digitalWrite(valve_1A, LOW);
                digitalWrite(valve_1B, LOW);
                mtimerState[curr_prog - 1] = true;
                mtimerChrono[curr_prog - 1].restart();
              }
              else if (valve == 1)
              { //valve 2
                digitalWrite(valve_2A, HIGH);
                digitalWrite(valve_2B, LOW);
                delay(100);
                digitalWrite(valve_2A, LOW);
                digitalWrite(valve_2B, LOW);
                mtimerState[curr_prog - 1] = true;
                mtimerChrono[curr_prog - 1].restart();
              }
              else if (valve == 2)
              { //valve 3
                digitalWrite(valve_3A, HIGH);
                digitalWrite(valve_3B, LOW);
                delay(100);
                digitalWrite(valve_3A, LOW);
                digitalWrite(valve_3B, LOW);
                mtimerState[curr_prog - 1] = true;
                mtimerChrono[curr_prog - 1].restart();
              }
              else if (valve == 3)
              { //valve 4
                digitalWrite(valve_4A, HIGH);
                digitalWrite(valve_4B, LOW);
                delay(100);
                digitalWrite(valve_4A, LOW);
                digitalWrite(valve_4B, LOW);
                mtimerState[curr_prog - 1] = true;
                mtimerChrono[curr_prog - 1].restart();
              }
              else if (valve == 4)
              { //all valves
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
                mtimerState[curr_prog - 1] = true;
                mtimerChrono[curr_prog - 1].restart();
              }
            }
          }
        }
        else
        { // soil moisture sensor is not connected
          loadState = true;
          loadChrono.restart();
          if (wOutlet == PUMP)
          {
            digitalWrite(pumpPin, HIGH);
            digitalWrite(PC13, LOW); //test
            mtimerState[curr_prog - 1] = true;
            mtimerChrono[curr_prog - 1].restart();
          }
          else if (wOutlet == VALVE)
          {
            irr_valve = RUNNING;
            char valve_A[4] = {valve_1A, valve_2A, valve_3A, valve_4A};
            char valve_B[4] = {valve_1B, valve_2B, valve_3B, valve_4B};
            uint8_t valve = Programs[curr_prog - 1].Valve;
            if (valve == 0)
            {
              digitalWrite(valve_1A, HIGH);
              digitalWrite(valve_1B, LOW);
              delay(100);
              digitalWrite(valve_1A, LOW);
              digitalWrite(valve_1B, LOW);
              mtimerState[curr_prog - 1] = true;
              mtimerChrono[curr_prog - 1].restart();
            }
            else if (valve == 1)
            {
              digitalWrite(valve_2A, HIGH);
              digitalWrite(valve_2B, LOW);
              delay(100);
              digitalWrite(valve_2A, LOW);
              digitalWrite(valve_2B, LOW);
              //digitalWrite(PB11, LOW);
              mtimerState[curr_prog - 1] = true;
              mtimerChrono[curr_prog - 1].restart();
            }
            else if (valve == 2)
            {
              digitalWrite(valve_3A, HIGH);
              digitalWrite(valve_3B, LOW);
              delay(100);
              digitalWrite(valve_3A, LOW);
              digitalWrite(valve_3B, LOW);
              mtimerState[curr_prog - 1] = true;
              mtimerChrono[curr_prog - 1].restart();
            }
            else if (valve == 3)
            {
              digitalWrite(valve_4A, HIGH);
              digitalWrite(valve_4B, LOW);
              delay(100);
              digitalWrite(valve_4A, LOW);
              digitalWrite(valve_4B, LOW);
              mtimerState[curr_prog - 1] = true;
              mtimerChrono[curr_prog - 1].restart();
            }
            else if (valve == 4)
            {
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
              mtimerState[curr_prog - 1] = true;
              mtimerChrono[curr_prog - 1].restart();
            }
          }
        }
      }
    }

    /**Irrigation for clashing times**/
    if (next_prog == 13)
    {
      if ((fYr(&Programs[clashProgs[0]]) == rtc.getYear()) && (fMth(&Programs[clashProgs[0]]) == rtc.getMonth()) && (fDay(&Programs[clashProgs[0]]) == rtc.getDay()) && (starthour(&Programs[clashProgs[0]]) == rtc.getHours()) && (startmin(&Programs[clashProgs[0]]) == rtc.getMinutes()))
      {
        for (int i = 0; i < clashCount; i++)
        {
          futureTime(Programs[clashProgs[i]].Interval); //calculate future time
          /***store the next irrigation date***/
          Programs[clashProgs[i]].futureWkDay = All_Str.tim.futureWkDay;
          Programs[clashProgs[i]].futureDay = All_Str.tim.futureDay;
          Programs[clashProgs[i]].futureMonth = All_Str.tim.futureMonth;
          Programs[clashProgs[i]].futureYr = All_Str.tim.futureYr;
        }
        EEPROM.put(5 * MUX, Programs);
        next_irr_flag = true;
        curr_clash_count = clashCount;
        curr_prog = 0;
        for (int i = 0; i < curr_clash_count; i++)
        {
          curr_clash_progs[i] = clashProgs[i];
        }
        if (rSense == ON)
        {
          int sensor_val = readSoilHumidity();
          if (sensor_val < All_Str.hum)
          { // if it has not rained
            loadState = true;
            loadChrono.restart();
            if (wOutlet == VALVE)
            { //valve irrigation
              irr_valve = RUNNING;
              for (int i = 0; i < curr_clash_count; i++)
              {
                uint8_t valve = Programs[curr_clash_progs[i]].Valve; // get valve setting
                if (valve == 0)
                { //valve 1
                  digitalWrite(valve_1A, HIGH);
                  digitalWrite(valve_1B, LOW);
                  delay(100);
                  digitalWrite(valve_1A, LOW);
                  digitalWrite(valve_1B, LOW);
                  mtimerState[curr_clash_progs[i]] = true;
                  mtimerChrono[curr_clash_progs[i]].restart();
                }
                else if (valve == 1)
                { //valve 2
                  digitalWrite(valve_2A, HIGH);
                  digitalWrite(valve_2B, LOW);
                  delay(100);
                  digitalWrite(valve_2A, LOW);
                  digitalWrite(valve_2B, LOW);
                  //digitalWrite(PB11, LOW);
                  mtimerState[curr_clash_progs[i]] = true;
                  mtimerChrono[curr_clash_progs[i]].restart();
                }
                else if (valve == 2)
                { //valve 3
                  digitalWrite(valve_3A, HIGH);
                  digitalWrite(valve_3B, LOW);
                  delay(100);
                  digitalWrite(valve_3A, LOW);
                  digitalWrite(valve_3B, LOW);
                  mtimerState[curr_clash_progs[i]] = true;
                  mtimerChrono[curr_clash_progs[i]].restart();
                }
                else if (valve == 3)
                { //valve 4
                  digitalWrite(valve_4A, HIGH);
                  digitalWrite(valve_4B, LOW);
                  delay(100);
                  digitalWrite(valve_4A, LOW);
                  digitalWrite(valve_4B, LOW);
                  mtimerState[curr_clash_progs[i]] = true;
                  mtimerChrono[curr_clash_progs[i]].restart();
                }
              }
            }
          }
        }
        else
        {
          loadState = true;
          loadChrono.restart();
          if (wOutlet == VALVE)
          { //valve irrigation
            irr_valve = RUNNING;
            for (int i = 0; i < curr_clash_count; i++)
            {
              uint8_t valve = Programs[curr_clash_progs[i]].Valve; // get valve setting
              if (valve == 0)
              { //valve 1
                digitalWrite(valve_1A, HIGH);
                digitalWrite(valve_1B, LOW);
                delay(100);
                digitalWrite(valve_1A, LOW);
                digitalWrite(valve_1B, LOW);
                mtimerState[curr_clash_progs[i]] = true;
                mtimerChrono[curr_clash_progs[i]].restart();
              }
              else if (valve == 1)
              { //valve 2
                digitalWrite(valve_2A, HIGH);
                digitalWrite(valve_2B, LOW);
                delay(100);
                digitalWrite(valve_2A, LOW);
                digitalWrite(valve_2B, LOW);
                //digitalWrite(PB11, LOW);
                mtimerState[curr_clash_progs[i]] = true;
                mtimerChrono[curr_clash_progs[i]].restart();
              }
              else if (valve == 2)
              { //valve 3
                digitalWrite(valve_3A, HIGH);
                digitalWrite(valve_3B, LOW);
                delay(100);
                digitalWrite(valve_3A, LOW);
                digitalWrite(valve_3B, LOW);
                mtimerState[curr_clash_progs[i]] = true;
                mtimerChrono[curr_clash_progs[i]].restart();
              }
              else if (valve == 3)
              { //valve 4
                digitalWrite(valve_4A, HIGH);
                digitalWrite(valve_4B, LOW);
                delay(100);
                digitalWrite(valve_4A, LOW);
                digitalWrite(valve_4B, LOW);
                mtimerState[curr_clash_progs[i]] = true;
                mtimerChrono[curr_clash_progs[i]].restart();
              }
            }
          }
        }
      }
    }

    /***Below block stops irrigation when completed***/

    /*Stop irrigation*/
    for (int i = 0; i < PROG_SIZE; i++)
    {
      if (mtimerState[i] == true && mtimerChrono[i].hasPassed(Programs[i].Runtime * 60000))
      {
        loadState = true;
        loadChrono.restart();
        if (wOutlet == PUMP)
        {
          digitalWrite(pumpPin, LOW);
          digitalWrite(PC13, HIGH); //test
          //irrState = 0;             // test
          mtimerState[i] = false;
        }
        else if (wOutlet == VALVE)
        {

          uint8_t valve = Programs[i].Valve;
          char valve_A[4] = {valve_1A, valve_2A, valve_3A, valve_4A};
          char valve_B[4] = {valve_1B, valve_2B, valve_3B, valve_4B};
          if (valve == 0)
          {
            digitalWrite(valve_1A, LOW);
            digitalWrite(valve_1B, HIGH);
            delay(100);
            digitalWrite(valve_1A, LOW);
            digitalWrite(valve_1B, LOW);
            mtimerState[i] = false;
          }
          else if (valve == 1)
          {
            digitalWrite(valve_2A, LOW);
            digitalWrite(valve_2B, HIGH);
            delay(100);
            digitalWrite(valve_2A, LOW);
            digitalWrite(valve_2B, LOW);
            //digitalWrite(PB11, HIGH);
            mtimerState[i] = false;
          }
          else if (valve == 2)
          {
            digitalWrite(valve_3A, LOW);
            digitalWrite(valve_3B, HIGH);
            delay(100);
            digitalWrite(valve_3A, LOW);
            digitalWrite(valve_3B, LOW);
            mtimerState[i] = false;
          }
          else if (valve == 3)
          {
            digitalWrite(valve_4A, LOW);
            digitalWrite(valve_4B, HIGH);
            delay(100);
            digitalWrite(valve_4A, LOW);
            digitalWrite(valve_4B, LOW);
            mtimerState[i] = false;
          }
          else if (valve == 4)
          {
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
            mtimerState[i] = false;
          }
          if (curr_prog > 0) {
            irr_valve = SHUTOFF;
          }
          else if (curr_clash_count > 0) {
            uint8_t count = 0;
            for (int i = 0; i < curr_clash_count; i++)
            {
              if (mtimerState[curr_clash_progs[i]] == false) {
                count++;
              }
            }
            if (count == curr_clash_count)
              irr_valve = SHUTOFF;
            else
              irr_valve = RUNNING;
            count = 0;
          }
        }
      }
    }
  }
}
