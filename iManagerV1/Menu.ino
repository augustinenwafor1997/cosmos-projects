void menu_controls()
{
  /*
     menu code below
  */
  if (Debounce(&buttons[2]) == HIGH && page == HOME){
    page = MENU;
    digitalWrite(lcdBacklight, LOW);
    lcdChrono.restart();
  }
  if (page == MENU)
  {
    // struct Timer * all;
    uint8_t select = menufunc(7, 4, mainmenu, 0, cusor, All_Str);
    switch (select)
    {
      case 1:
        page = MODE;
        cusor = 1;
        lcdChrono.restart();
        break;
      case 2:
        page = CALENDAR;
        cusor = 1;
        lcdChrono.restart();
        break;
      case 3:
        page = OUTLET;
        cusor = 1;
        lcdChrono.restart();
        break;
      case 4:
        page = SENSOR;
        cusor = 1;
        lcdChrono.restart();
        break;
      case 5:
        page = NETWORK;
        cusor = 1;
        lcdChrono.restart();
        break;
      case 6:
        // RESET ALL TIMERS***
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
        page = HOME;
        cusor = 1;
        lcdChrono.restart();
        break;
      case 7:
        // EXIT MENU
        page = HOME;
        cusor = 1;
        lcdChrono.restart();
        break;
      default:
        cusor = select;
        break;
    }
  }
  else if (page == MODE)
  {
    uint8_t select = menufunc(4, 4, mode, 0, cusor, All_Str);
    switch (select)
    {
      case 1:
        page = CYCLE;
        cusor = 1;
        lcdChrono.restart();
        break;
      case 2:
        page = TIMER;
        cusor = 1;
        lcdChrono.restart();
        break;
      case 3:
        // SAVE MANUAL MODE HERE & EXIT
        if (irr_mode != MANUAL)
        {
          irr_mode = MANUAL;
          EEPROM.put(0 * sizeof(irr_mode), irr_mode);
        }
        // stop running
        closeV();
        page = HOME;
        cusor = 1;
        lcdChrono.restart();
        break;
      case 4:
        // RETURN TO PREVIOUS MENU
        page = MENU;
        cusor = 1;
        lcdChrono.restart();
        break;
      default:
        cusor = select;
        break;
    }
  }
  else if (page == CYCLE)
  {
    uint8_t select = menufunc(4, 4, cycle, 0, cusor, All_Str);
    switch (select)
    {
      case 3:
        // set: save & exit cycle**
        // stop running
        closeV();
        if (irr_mode != LOOP)
        {
          irr_mode = LOOP;
          EEPROM.put(0 * sizeof(irr_mode), irr_mode);
        }
        // const uint8_t offset = 2;
        EEPROM.put(2 * sizeof(All_Str.cyc), All_Str.cyc);
        firstRun = true;
        loopState = REST;
        page = HOME;
        cusor = 1;
        lcdChrono.restart();
        break;
      case 4:
        // back to previous menu
        page = MODE;
        cusor = 1;
        lcdChrono.restart();
        break;
      case 10:
        // set All_Str.tim.Runtime increment
        cusor = select / 10;
        lcdChrono.restart();
        if (All_Str.cyc.Run >= 10)
          All_Str.cyc.Run += 10;
        else
          All_Str.cyc.Run++;
        if (All_Str.cyc.Run > 100)
          All_Str.cyc.Run = 1;
        break;
      case 100:
        // set All_Str.tim.Runtime decrement
        lcdChrono.restart();
        cusor = select / 100;
        if (All_Str.cyc.Run <= 10)
          All_Str.cyc.Run--;
        else
          All_Str.cyc.Run -= 10;
        if (All_Str.cyc.Run < 0)
          All_Str.cyc.Run = 100;
        break;
      case 20:
        // set resttime increment
        lcdChrono.restart();
        cusor = select / 10;
        if (All_Str.cyc.Rest >= 10)
          All_Str.cyc.Rest += 10;
        else
          All_Str.cyc.Rest++;
        if (All_Str.cyc.Rest > 120)
          All_Str.cyc.Rest = 1;
        break;
      case 200:
        // set resttime decrement
        lcdChrono.restart();
        cusor = select / 100;
        if (All_Str.cyc.Rest <= 10)
          All_Str.cyc.Rest--;
        else
          All_Str.cyc.Rest -= 10;
        if (All_Str.cyc.Rest < 0)
          All_Str.cyc.Rest = 120;
        break;
      default:
        cusor = select;
        break;
    }
  }
  else if (page == TIMER)
  {
    lcdChrono.restart();
    const uint8_t MUX = 6;
    uint8_t startdate = 0;
    static bool EEPROM_FLAG = false;
    //static struct Timer Programs[PROG_SIZE];//test
    //EEPROM.get(5 * sizeof(All_Str.prog_count), Programs);
    // static uint8_t prog_count;
    if (irr_mode != PROGS)
      All_Str.prog_count = 0;
    uint16_t select = menufunc(9, 4, timer, 0, cusor, All_Str);
    switch (select)
    {
      case 7:
        // SET: SAVE CURRENT TIMER
        // DO ANALYSIS OF STRUCT HERE
        if (irr_mode != PROGS)
        { // save irrigation mode only once
          irr_mode = PROGS;
          EEPROM.put(0 * sizeof(irr_mode), irr_mode);
        }

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

        futureTime(startdate);                          //get the day to start irrigation. Normally starts today calc future day, month and year
        Programs[(All_Str.tim.Prog - 1)] = All_Str.tim; //add new program to struct array
        EEPROM_FLAG = true;
        cusor = 1;
        lcdChrono.restart();
        break;
      case 8:
        // COMMIT & EXIT TO HOME
        if (EEPROM_FLAG == true)
        {
          // stop running
          closeV();
          EEPROM_FLAG = false;
          EEPROM.put(5 * MUX, Programs);
          next_irr_flag = true; //test line
        }
        page = HOME;
        cusor = 1;
        lcdChrono.restart();
        break;
      case 9:
        // back to previous menu
        if (EEPROM_FLAG == true)
        {
          // stop running
          closeV();
          EEPROM_FLAG = false;
          EEPROM.put(5 * MUX, Programs);
          next_irr_flag = true; //test line
        }
        page = MODE;
        cusor = 1;
        lcdChrono.restart();
        break;
      case 10:
        // PROG increment
        lcdChrono.restart();
        cusor = select / 10;
        All_Str.tim.Prog++;
        if (All_Str.tim.Prog > 12)
          All_Str.tim.Prog = 1;
        break;
      case 100:
        // PROG decrement
        lcdChrono.restart();
        cusor = select / 100;
        All_Str.tim.Prog--;
        if (All_Str.tim.Prog < 1)
          All_Str.tim.Prog = 12;
        break;
      case 20:
        // START HOUR increment
        lcdChrono.restart();
        cusor = select / 10;
        All_Str.tim.StartHr++;
        if (All_Str.tim.StartHr > 23)
          All_Str.tim.StartHr = 0;
        break;
      case 200:
        // START HOUR decrement
        lcdChrono.restart();
        cusor = select / 100;
        All_Str.tim.StartHr--;
        if (All_Str.tim.StartHr < 0)
          All_Str.tim.StartHr = 23;
        break;
      case 30:
        // START MINUTE increment
        lcdChrono.restart();
        cusor = select / 10;
        All_Str.tim.StartMin++;
        if (All_Str.tim.StartMin > 59)
          All_Str.tim.StartMin = 0;
        break;
      case 300:
        // START MINUTE decrement
        lcdChrono.restart();
        cusor = select / 100;
        All_Str.tim.StartMin--;
        if (All_Str.tim.StartMin < 0)
          All_Str.tim.StartMin = 59;
        break;
      case 40:
        // DURATION increment
        lcdChrono.restart();
        cusor = select / 10;
        if (All_Str.tim.Runtime >= 10)
          All_Str.tim.Runtime += 5;
        else
          All_Str.tim.Runtime++;
        if (All_Str.tim.Runtime > 720)
          All_Str.tim.Runtime = 0;
        break;
      case 400:
        // DURATION decrement
        lcdChrono.restart();
        cusor = select / 100;
        if (All_Str.tim.Runtime <= 10)
          All_Str.tim.Runtime--;
        else
          All_Str.tim.Runtime -= 5;
        if (All_Str.tim.Runtime < 0)
          All_Str.tim.Runtime = 720;
        break;
      case 50:
        // INTERVAL increment
        lcdChrono.restart();
        cusor = select / 10;
        All_Str.tim.Interval++;
        if (All_Str.tim.Interval > 7)
          All_Str.tim.Interval = 1;
        break;
      case 500:
        // INTERVAL decrement
        lcdChrono.restart();
        cusor = select / 100;
        All_Str.tim.Interval--;
        if (All_Str.tim.Interval < 1)
          All_Str.tim.Interval = 7;
        break;
      case 60:
        // VALVE increment
        lcdChrono.restart();
        cusor = select / 10;
        All_Str.tim.Valve++;
        if (All_Str.tim.Valve > 4)
          All_Str.tim.Valve = 0;
        break;
      case 600:
        // VALVE decrement
        lcdChrono.restart();
        cusor = select / 100;
        All_Str.tim.Valve--;
        if (All_Str.tim.Valve < 0)
          All_Str.tim.Valve = 4;
        break;
      default:
        cusor = select;
        break;
    }
  }
  else if (page == CALENDAR)
  {
    uint8_t select = menufunc(3, 4, calendar, 0, cusor, All_Str);
    switch (select)
    {
      case 1:
        page = DATE;
        cusor = 1;
        lcdChrono.restart();
        break;
      case 2:
        page = TIME;
        cusor = 1;
        lcdChrono.restart();
        break;
      case 3:
        // EXIT
        page = HOME;
        cusor = 1;
        lcdChrono.restart();
        break;
      default:
        cusor = select;
        break;
    }
  }
  else if (page == DATE)
  {
    int select = menufunc(3, 3, setdate, 1, cusor, All_Str);
    switch (select)
    {
      case 1:
        // SET: SAVE CURRENT DATE
        lcdChrono.restart();
        rtc.setDate(All_Str.cal.DY, All_Str.cal.MTH, All_Str.cal.YR);
        page = CALENDAR;
        cusor = 1;
        break;
      case 2:
        // SET: SAVE CURRENT DATE
        lcdChrono.restart();
        rtc.setDate(All_Str.cal.DY, All_Str.cal.MTH, All_Str.cal.YR);
        page = CALENDAR;
        cusor = 1;
        break;
      case 3:
        // SET: SAVE CURRENT DATE
        lcdChrono.restart();
        rtc.setDate(All_Str.cal.DY, All_Str.cal.MTH, All_Str.cal.YR);
        page = CALENDAR;
        cusor = 1;
        break;
      case 100:
        // page = 2;
        lcdChrono.restart();
        cusor = select / 100;
        All_Str.cal.YR++;
        break;
      case 10:
        // page = 2;
        lcdChrono.restart();
        cusor = select / 10;
        All_Str.cal.YR--;
        if (All_Str.cal.YR < 22)
          All_Str.cal.YR = 22;
        break;
      case 200:
        // page = 2;
        lcdChrono.restart();
        cusor = select / 100;
        All_Str.cal.MTH++;
        if (All_Str.cal.MTH > 12)
          All_Str.cal.MTH = 1;
        break;
      case 20:
        // page = 2;
        lcdChrono.restart();
        cusor = select / 10;
        All_Str.cal.MTH--;
        if (All_Str.cal.MTH < 1)
          All_Str.cal.MTH = 12;
        break;
      case 300:
        // page = 2;
        lcdChrono.restart();
        cusor = select / 100;
        All_Str.cal.DY++;
        if (All_Str.cal.DY > 31)
          All_Str.cal.DY = 1;
        break;
      case 30:
        // page = 2;
        lcdChrono.restart();
        cusor = select / 10;
        All_Str.cal.DY--;
        if (All_Str.cal.DY < 1)
          All_Str.cal.DY = 31;
        break;
      default:
        cusor = select;
        break;
    }
  }
  else if (page == TIME)
  {
    //    static uint8_t All_Str.cal.HR = 20, All_Str.cal.MIN = 1, All_Str.cal.SEC = 1;
    uint16_t select = menufunc(3, 3, settime, 1, cusor, All_Str);
    switch (select)
    {
      case 1:
        // SET: SAVE CURRENT TIME
        lcdChrono.restart();
        rtc.setTime(All_Str.cal.HR, All_Str.cal.MIN, All_Str.cal.SEC);
        page = CALENDAR;
        cusor = 1;
        break;
      case 2:
        // SET: SAVE CURRENT TIME
        lcdChrono.restart();
        rtc.setTime(All_Str.cal.HR, All_Str.cal.MIN, All_Str.cal.SEC);
        page = CALENDAR;
        cusor = 1;
        break;
      case 3:
        // SET: SAVE CURRENT TIME
        lcdChrono.restart();
        rtc.setTime(All_Str.cal.HR, All_Str.cal.MIN, All_Str.cal.SEC);
        page = CALENDAR;
        cusor = 1;
        break;
      case 100:
        cusor = select / 100;
        All_Str.cal.HR++;
        if (All_Str.cal.HR > 23)
          All_Str.cal.HR = 0;
        break;
      case 10:
        cusor = select / 10;
        All_Str.cal.HR--;
        if (All_Str.cal.HR < 0)
          All_Str.cal.HR = 23;
        break;
      case 200:
        // page = 2;
        cusor = select / 100;
        All_Str.cal.MIN++;
        if (All_Str.cal.MIN > 59)
          All_Str.cal.MIN = 0;
        break;
      case 20:
        // page = 2;
        cusor = select / 10;
        All_Str.cal.MIN--;
        if (All_Str.cal.MIN < 0)
          All_Str.cal.MIN = 59;
        break;
      case 300:
        // page = 2;
        cusor = select / 100;
        All_Str.cal.SEC++;
        if (All_Str.cal.SEC > 59)
          All_Str.cal.SEC = 0;
        break;
      case 30:
        // page = 2;
        cusor = select / 10;
        All_Str.cal.SEC--;
        if (All_Str.cal.SEC < 0)
          All_Str.cal.SEC = 59;
        break;
      default:
        cusor = select;
        break;
    }
  }
  else if (page == OUTLET)
  {
    uint8_t select = menufunc(3, 3, outlet, 0, cusor, All_Str);
    switch (select)
    {
      case 1:
        // SET: SAVE VALVE***
        wOutlet = VALVE;
        EEPROM.put(1 * sizeof(wOutlet), wOutlet);
        page = HOME;
        cusor = 1;
        lcdChrono.restart();
        break;
      case 2:
        // SET: SAVE PUMP***
        wOutlet = PUMP;
        EEPROM.put(1 * sizeof(wOutlet), wOutlet);
        page = HOME;
        cusor = 1;
        lcdChrono.restart();
        break;
      case 3:
        // back to previous menu
        page = MENU;
        cusor = 1;
        lcdChrono.restart();
        break;
      default:
        cusor = select;
        break;
    }
  }
  else if (page == SENSOR)
  {
    uint8_t select = menufunc(3, 3, sensor, 0, cusor, All_Str);
    switch (select)
    {
      case 1:
        // GO TO HUMIDITY page
        page = HUMIDITY;
        cusor = 1;
        lcdChrono.restart();
        break;
      case 2:
        // SET: SAVE SENSOR OFF***
        rSense = OFF;
        EEPROM.put(2 * sizeof(rSense), rSense);
        page = HOME;
        cusor = 1;
        lcdChrono.restart();
        break;
      case 3:
        // back to previous menu
        page = MENU;
        cusor = 1;
        lcdChrono.restart();
        break;
      default:
        cusor = select;
        break;
    }
  }
  else if (page == HUMIDITY)
  {
    uint8_t select = menufunc(3, 3, humidity, 0, cusor, All_Str);
    switch (select)
    {
      case 2:
        // SET: SAVE AND EXIT**
        rSense = ON;
        EEPROM.put(2 * sizeof(rSense), rSense);
        EEPROM.put(2 * sizeof(All_Str.hum), All_Str.hum);
        page = HOME;
        cusor = 1;
        lcdChrono.restart();
        break;
      case 3:
        // back to previous menu
        page = SENSOR;
        cusor = 1;
        lcdChrono.restart();
        break;
      case 10:
        // setpoint increment
        lcdChrono.restart();
        cusor = select / 10;
        All_Str.hum += 5;
        if (All_Str.hum > 100)
          All_Str.hum = 0;
        break;
      case 100:
        // setpoint decrement
        cusor = select / 100;
        All_Str.hum -= 5;
        if (All_Str.hum < 0)
          All_Str.hum = 100;
        break;
      default:
        cusor = select;
        break;
    }
  }
  else if (page == NETWORK)
  {
    uint8_t select = menufunc(4, 4, network, 0, cusor, All_Str);
    switch (select)
    {
      case 1:
        // SET: SAVE NETWORK SMS***
        gsmMode = SMS;
        EEPROM.put(3 * sizeof(gsmMode), gsmMode);
        page = HOME;
        cusor = 1;
        lcdChrono.restart();
        break;
      case 2:
        // SET: SAVE NETWORK GPRS***
        gsmMode = GPRS;
        EEPROM.put(3 * sizeof(gsmMode), gsmMode);
        page = HOME;
        cusor = 1;
        lcdChrono.restart();
        break;
      case 3:
        // SET: SAVE NETWORK OFF***
        gsmMode = NaN;
        EEPROM.put(3 * sizeof(gsmMode), gsmMode);
        simStatus = WAIT;
        page = HOME;
        cusor = 1;
        lcdChrono.restart();
        break;
      case 4:
        // back to previous menu
        page = MENU;
        cusor = 1;
        lcdChrono.restart();
        break;
      default:
        cusor = select;
        break;
    }
  }
  // return page;
}
