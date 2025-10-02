void homepage()
{
  if (page == HOME)
  {
#define TIME_HEIGHT 20 //35
#define DATE_HEIGHT 32 //47
    uint8_t width = 0, npercent =  batterypercentage(battPin);
    static uint8_t percent = npercent;
    u8g2.firstPage();
    do
    {
      if (!loadState && checkState) {
        if (!isBatteryCharging(chargePin)) {
          if (npercent < percent)
            percent = --percent;
        }
        else {
          if (npercent > percent)
            percent = ++percent;
        }
        checkState = false;
        checkChrono.restart();
      }
      if(loadState && loadChrono.hasPassed(5)){
        loadState = false;
      }
      if(!checkState && checkChrono.hasPassed(5)){
        checkState = true;
      }
      /***DISPLAY BATTERY PERCENT AND ICON***/
      // u8g2.setFont(u8g2_font_mozart_nbp_tr);
      u8g2.setFont(u8g2_font_trixel_square_tr); //  u8g2_font_tinytim_tr u8g2_font_trixel_square_tr u8g2_font_blipfest_07_tr
      if (percent == 100)
      {
        u8g2.drawStr(102, 6, u8x8_u16toa(percent, 3));
        /***DISPLAY CHARGING ICON***/
        if (isBatteryCharging(chargePin))
          u8g2.drawXBMP(94 /*118*/, 0 /*1*/, icons8_charging_width, icons8_charging_height, icons8_charging);

      }
      else if (percent < 100 && percent >= 10)
      {
        u8g2.drawStr(105, 6, u8x8_u16toa(percent, 2));
        /***DISPLAY CHARGING ICON***/
        if (isBatteryCharging(chargePin))
          u8g2.drawXBMP(97 /*118*/, 0 /*1*/, icons8_charging_width, icons8_charging_height, icons8_charging);

      }
      else if (percent < 10)
      {
        u8g2.drawStr(109, 6, u8x8_u16toa(percent, 1));
        /***DISPLAY CHARGING ICON***/
        if (isBatteryCharging(chargePin))
          u8g2.drawXBMP(101 /*118*/, 0 /*1*/, icons8_charging_width, icons8_charging_height, icons8_charging);

      }

      /***DISPLAY BATTERY ICON***/
      u8g2.drawXBMP(113 /*118*/, 0 /*1*/, icons8_empty_battery_32_2__width, icons8_empty_battery_32_2__height, icons8_empty_battery_32_2__bits);
      width = (percent * CHARGE_AREA_WIDTH) / 100;
      u8g2.drawBox(CHARGE_AREA_START_X, CHARGE_AREA_START_Y, width, CHARGE_AREA_HEIGHT);

      /***DISPLAY NETWORK BARS***/
      if (simStatus == READY)
      {
        if (sigStrength < 10)
        {
          u8g2.drawXBMP(84, 1, icons8_network_bar__width, icons8_network_bar__height, icons8_network_1_bar);
        }
        else if (sigStrength >= 10 && sigStrength < 15)
        {
          u8g2.drawXBMP(84, 1, icons8_network_bar__width, icons8_network_bar__height, icons8_network_2_bar);
        }
        else if (sigStrength >= 15 && sigStrength < 20)
        {
          u8g2.drawXBMP(84, 1, icons8_network_bar__width, icons8_network_bar__height, icons8_network_3_bar);
        }
        else if (sigStrength >= 20 && sigStrength < 25)
        {
          u8g2.drawXBMP(84, 1, icons8_network_bar__width, icons8_network_bar__height, icons8_network_4_bar);
        }
        else if (sigStrength >= 25)
        {
          u8g2.drawXBMP(84, 1, icons8_network_bar__width, icons8_network_bar__height, icons8_network_5_bar);
        }
      }

      /***DISPLAY CALENDAR***/
      u8g2.setFont(u8g2_font_6x13B_tr); // u8g2_font_8x13B_tr
      u8g2.drawStr(0, TIME_HEIGHT, u8x8_u16toa(rtc.getHours(), 2));
      u8g2.drawStr(13, (TIME_HEIGHT - 1), ":");
      u8g2.drawStr(20, TIME_HEIGHT, u8x8_u16toa(rtc.getMinutes(), 2));
      u8g2.drawStr(33, (TIME_HEIGHT - 1), ":");
      u8g2.drawStr(40, TIME_HEIGHT, u8x8_u16toa(rtc.getSeconds(), 2));
      u8g2.drawStr(0, DATE_HEIGHT, u8x8_u16toa(rtc.getDay(), 2));
      u8g2.drawStr(14, DATE_HEIGHT, "/");
      u8g2.drawStr(20, DATE_HEIGHT, u8x8_u16toa(rtc.getMonth(), 2));
      u8g2.drawStr(34, DATE_HEIGHT, "/");
      u8g2.drawStr(40, DATE_HEIGHT, u8x8_u16toa(rtc.getYear() + 2000, 4));

      /***DISPLAY PUMP ICON***/
      if (wOutlet == VALVE && digitalRead(pumpPin))
      {
        u8g2.drawXBMP(24, (DATE_HEIGHT + 5), icons8_pump_16_width, icons8_pump_16_height, icons8_pump_16_bits);
      }

      /***DISPLAY IRRIGATION ICON***/
      if (irr_valve == RUNNING || (wOutlet == PUMP && digitalRead(pumpPin)))
      {
        u8g2.drawXBMP(0, (DATE_HEIGHT + 3), icons8_shower_24_20_1__width, icons8_shower_24_20_1__height, icons8_shower_24_20_1__bits);
      }


      /**DISPLAY SIZE OF PROGRAMS STRUCT***/
      //      u8g2.setFont(u8g2_font_profont11_tr);
      //      u8g2.drawStr(0, 62, homeScr.c_str());
      //String pssword = EEPROMread(345);//24
      //u8g2.drawStr(0, 62, pssword.c_str());
      //      u8g2.drawStr(100, 62, u8x8_u16toa(sizeof(All_Str.cyc), 2));
      //u8g2.drawStr(100, 62, u8x8_u16toa(sigStrength, 2));

      /****CYCLE MODE***/
      if (irr_mode == LOOP)
      {

        /***DISPLAY LOOP***/
        u8g2.drawXBMP(0, 0, icons8_loop_width, icons8_loop_height, icons8_loop);

        /***DISPLAY SOIL MOISTURE***/
        if (rSense == ON)
        {
          uint8_t soilmoisture = readSoilHumidity();
          // display soil moisture
          u8g2.drawXBMP(86, 31, icons8_moisture_24_20_1__width, icons8_moisture_24_20_1__height, icons8_moisture_24_20_1__bits);
          u8g2.setFont(u8g2_font_profont11_tr); // u8g2_font_ordinarybasis_tr
          // u8g2.setFontMode(1);
          u8g2.setCursor(100, 41);
          u8g2.print(soilmoisture);
          u8g2.print("%");
        }

        /********DISPLAY COUNTDOWN****************/
        u8g2.setFont(u8g2_font_mozart_nbp_tr);
        unsigned long currentMillis = (counter - millis());
        uint8_t secs = (currentMillis / 1000) % 60;
        uint8_t mint = (currentMillis / (1000 * 60)) % 60;// conversion from miiliseconds-to-seconds-to-minutes
        uint8_t hor = (currentMillis / (1000 * 60 * 60)) % 24;
        if (hor > 0)
        {
          //u8g2.drawXBMP(91, (TIME_HEIGHT - 9), icons8_bell_alarm_width, icons8_bell_alarm_height, icons8_bell_alarm);
          u8g2.drawStr(66, TIME_HEIGHT + 1, u8x8_u16toa(hor, 2));
          u8g2.drawStr(79, (TIME_HEIGHT + 1), ":");
          u8g2.drawStr(83, TIME_HEIGHT + 1, u8x8_u16toa(mint, 2));
          u8g2.drawStr(96, (TIME_HEIGHT + 1), ":");
          u8g2.drawStr(100, TIME_HEIGHT + 1, u8x8_u16toa(secs, 2));

        }
        else {
          u8g2.drawStr(83, TIME_HEIGHT + 1, u8x8_u16toa(mint, 2));
          u8g2.drawStr(96, (TIME_HEIGHT + 1), ":");
          u8g2.drawStr(100, TIME_HEIGHT + 1, u8x8_u16toa(secs, 2));
        }
        //}
        if (irr_valve == RUNNING || (wOutlet == PUMP && digitalRead(pumpPin)))
        {

          /***DISPLAY RUN CYCLE***/
          u8g2.drawXBMP(112, (TIME_HEIGHT - 9), icons8_bell_alarm_width, icons8_bell_alarm_height, icons8_bell_alarm);
        }
        else
        {
          /***DISPLAY REST CYCLE***/
          u8g2.drawXBMP(112, (TIME_HEIGHT - 9), icons8_bell_snooze_width, icons8_bell_snooze_height, icons8_bell_snooze);
        }
      }

      /***TIMER MODE***/
      else if (irr_mode == PROGS)
      {
        /***DISPLAY TIMER***/
        u8g2.drawXBMP(0, 0, icons8_clock_width, icons8_clock_height, icons8_clock);

        /***DISPLAY NEXT IRRIGATION TIME***/
        u8g2.setFont(u8g2_font_mozart_nbp_tr);
        if (next_prog < 13)
        {
          u8g2.drawStr(80, TIME_HEIGHT, u8x8_u16toa(Programs[next_prog - 1].StartHr, 2));
          u8g2.drawStr(94, (TIME_HEIGHT - 1), ":");
          u8g2.drawStr(100, TIME_HEIGHT, u8x8_u16toa(Programs[next_prog - 1].StartMin, 2));
          u8g2.drawStr(90, (DATE_HEIGHT - 4), dayShortStr(Programs[next_prog - 1].futureWkDay));
        }
        else if (next_prog == 13)
        {
          u8g2.drawStr(80, TIME_HEIGHT, u8x8_u16toa(Programs[clashProgs[0]].StartHr, 2));
          u8g2.drawStr(94, (TIME_HEIGHT - 1), ":");
          u8g2.drawStr(100, TIME_HEIGHT, u8x8_u16toa(Programs[clashProgs[0]].StartMin, 2));
          u8g2.drawStr(90, (DATE_HEIGHT - 4), dayShortStr(Programs[clashProgs[0]].futureWkDay));
        }

        /***DISPLAY BELL ICON***/
        u8g2.drawXBMP(115, (TIME_HEIGHT - 3), icons8_bell_width, icons8_bell_height, icons8_bell);

        /***DISPLAY SOIL MOISTURE***/
        if (rSense == ON)
        {
          uint8_t soilmoisture = readSoilHumidity();
          // display soil moisture
          u8g2.drawXBMP(86, 31, icons8_moisture_24_20_1__width, icons8_moisture_24_20_1__height, icons8_moisture_24_20_1__bits);
          u8g2.setFont(u8g2_font_profont11_tr); // u8g2_font_ordinarybasis_tr
          // u8g2.setFontMode(1);
          u8g2.setCursor(100, 41);
          u8g2.print(soilmoisture);
          u8g2.print("%");
        }
        // u8g2.drawStr(2, 60, u8x8_u16toa(All_Str.prog_count, 2));
        // for (int i = 0; i < All_Str.prog_count; i++)
        // {
        //   int irr_time = starttime(&Programs[i]);
        //   int irr_min = startmin(&Programs[i]);
        //   const char *prog = ("PROG " + String(i + 1)).c_str();
        //   u8g2.drawStr(0, (i + 1) * 10, prog);
        //   u8g2.drawStr(45, (i + 1) * 10, u8x8_u16toa(irr_time, 2));
        //   u8g2.drawStr(57, (i + 1) * 10, ":");
        //   u8g2.drawStr(60, (i + 1) * 10, u8x8_u16toa(irr_min, 2));
        // }
        /*print future time*/
        // u8g2.drawStr(20, 60, "NEXT"); //
        // if (next_prog < 13)
        // {
        //   u8g2.drawStr(50, 60, u8x8_u16toa(Programs[next_prog - 1].StartHr, 2));
        //   u8g2.drawStr(65, 60, ":");
        //   //        u8g2.drawStr(70, 60, u8x8_u16toa(Programs[next_prog - 1].StartMin, 2));
        //   u8g2.drawStr(70, 60, u8x8_u16toa(startmin(&Programs[next_prog - 1]), 2));
        //   u8g2.drawStr(100, TIME_HEIGHT, u8x8_u16toa(Programs[next_prog - 1].futureDay, 2));
        //   u8g2.drawStr(115, DATE_HEIGHT, u8x8_u16toa(Programs[next_prog - 1].futureMonth, 2));
        //   u8g2.drawStr(105, TIME_HEIGHT - 12, u8x8_u16toa(Programs[next_prog - 1].futureYr, 4));
        // }
        // else if (next_prog == 13)
        // {
        //   u8g2.drawStr(50, 60, u8x8_u16toa(Programs[clashProgs[0]].StartHr, 2));
        //   u8g2.drawStr(65, 60, ":");
        //   //        u8g2.drawStr(70, 60, u8x8_u16toa(Programs[next_prog - 1].StartMin, 2));
        //   u8g2.drawStr(70, 60, u8x8_u16toa(startmin(&Programs[clashProgs[0]]), 2));
        //   u8g2.drawStr(100, TIME_HEIGHT, u8x8_u16toa(Programs[clashProgs[0]].futureDay, 2));
        //   u8g2.drawStr(115, DATE_HEIGHT, u8x8_u16toa(Programs[clashProgs[0]].futureMonth, 2));
        //   u8g2.drawStr(105, TIME_HEIGHT - 12, u8x8_u16toa(Programs[clashProgs[0]].futureYr, 4));
        // }
        // if (irrState == 1)
        //   u8g2.drawStr(100, 60, "SEEN");
        // //        u8g2.drawStr(100, 60, u8x8_u16toa(sizeof(Programs), 4));
        // /*print future day*/
        // u8g2.drawStr(67, TIME_HEIGHT, "DAY:");
        // u8g2.drawStr(67, DATE_HEIGHT, "MONTH:");
        // u8g2.drawStr(90, TIME_HEIGHT - 12, "Y:");

        // /*print next prog*/
        // u8g2.drawStr(75, 10, "PROG:");
        // u8g2.drawStr(110, 10, u8x8_u16toa(next_prog, 2));

        // u8g2.setFont(u8g2_font_6x13B_tr); // u8g2_font_8x13B_tr
        // u8g2.drawStr(0, TIME_HEIGHT, u8x8_u16toa(rtc.getHours(), 2));
        // u8g2.drawStr(13, (TIME_HEIGHT - 1), ":");
        // u8g2.drawStr(20, TIME_HEIGHT, u8x8_u16toa(rtc.getMinutes(), 2));
        // u8g2.drawStr(33, (TIME_HEIGHT - 1), ":");
        // u8g2.drawStr(40, TIME_HEIGHT, u8x8_u16toa(rtc.getSeconds(), 2));
        // u8g2.drawStr(0, DATE_HEIGHT, u8x8_u16toa(rtc.getDay(), 2));
        // u8g2.drawStr(14, DATE_HEIGHT, "/");
        // u8g2.drawStr(20, DATE_HEIGHT, u8x8_u16toa(rtc.getMonth(), 2));
        // u8g2.drawStr(34, DATE_HEIGHT, "/");
        // u8g2.drawStr(40, DATE_HEIGHT, u8x8_u16toa(rtc.getYear() + 2000, 4));
        //
        //        u8g2.drawStr(100, 10, u8x8_u16toa(count, 2));
        //        for (int i = 0; i < All_Str.prog_count; i++) {
        //          // int irr_time = starttime(&Programs[i]);
        //          // const char* prog = ("PROG " + String(i + 1)).c_str();
        //          // u8g2.drawStr(20, (i + 1) * 10, prog);
        //          u8g2.drawStr(i * 10, 50, u8x8_u16toa(starthr_store[i], 2));
        //        }
      }

      /***MANUAL MODE***/
      else if (irr_mode == MANUAL)
      {
        /***DISPLAY MANUAL***/
        u8g2.drawXBMP(0, 0, icons8_human_width, icons8_human_height, icons8_human);

        /***DISPLAY SOIL MOISTURE***/
        if (rSense == ON)
        {
          uint8_t soilmoisture = readSoilHumidity();
          // display soil moisture
          u8g2.drawXBMP(80, 20, icons8_moisture_24_20_1__width, icons8_moisture_24_20_1__height, icons8_moisture_24_20_1__bits);
          u8g2.setFont(u8g2_font_profont11_tr); // u8g2_font_ordinarybasis_tr
          // u8g2.setFontMode(1);
          u8g2.setCursor(95, 29);
          u8g2.print(soilmoisture);
          u8g2.print("%");
        }
      }

    } while (u8g2.nextPage());
  }
}
