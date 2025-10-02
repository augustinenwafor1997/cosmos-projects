/////////////////////////// selectBox  ////////////////////////////////////////////////////////////////

void selectBox(uint8_t menu, uint8_t controlType)
{
  if (controlType == 0)
  {
    u8g2.setDrawColor(2);
    u8g2.drawBox(0, (1 + (12 * menu)), u8g2.getDisplayWidth(), 12);
  }
  else if (controlType == 1)
  {
    u8g2.setDrawColor(2);
    u8g2.drawBox((0 + (45 * (menu - 1))), 11, 43, 10);
  }
  else
  {
    /* code */
  }
}

//////////////////////////// xml components //////////////////////////////////////////////////////////
void heading(uint8_t xpos, uint8_t ypos, const char *title)
{ // to reduce repetition
  u8g2.setFont(u8g2_font_timB10_tr);
  u8g2.setFontMode(1);
  u8g2.drawStr(xpos, ypos, title); // u8g2_font_u8glib_4_tf
  u8g2.drawLine(0, 11, 127, 11);
  u8g2.setFont(u8g2_font_crox1hb_tr);
}
void mainmenu(uint8_t marker, uint8_t frame, uint8_t controlType, struct All all_str)
{

  u8g2.firstPage();
  do
  {
    heading(44, 10, "MENU");
    u8g2.drawStr(6, height(frame, 12, 1), "IRRIGATION MODE");
    u8g2.drawStr(6, height(frame, 12, 2), "SET DATE/TIME");
    u8g2.drawStr(6, height(frame, 12, 3), "WATER OUTLET");
    u8g2.drawStr(6, height(frame, 12, 4), "RAIN SENSE");
    u8g2.drawStr(6, height(frame, 12, 5), "NETWORK");
    u8g2.drawStr(6, height(frame, 12, 6), "RESET");
    u8g2.drawStr(6, height(frame, 12, 7), "EXIT");
    //    u8g2.drawStr(6, height(frame, 12, 8), "GAMES");
    //    u8g2.drawStr(6, height(frame, 12, 9), "EXIT");
    selectBox(marker, controlType);
  } while (u8g2.nextPage());
}

void mode(uint8_t marker, uint8_t frame, uint8_t controlType, struct All all_str)
{

  u8g2.firstPage();
  do
  {
    heading(44, 10, "MODE");
    u8g2.drawStr(6, height(frame, 12, 1), "CYCLE");
    u8g2.drawStr(6, height(frame, 12, 2), "TIMER");
    u8g2.drawStr(6, height(frame, 12, 3), "MANUAL");
    u8g2.drawStr(6, height(frame, 12, 4), "BACK");
    selectBox(marker, controlType);
  } while (u8g2.nextPage());
}

void cycle(uint8_t marker, uint8_t frame, uint8_t controlType, struct All all_str)
{

  u8g2.firstPage();
  do
  {
    heading(40, 10, "CYCLE");
    u8g2.drawStr(6, height(frame, 12, 1), "RUN<      >MINS");
    u8g2.drawStr(6, height(frame, 12, 2), "REST<      >MINS");
    u8g2.drawStr(6, height(frame, 12, 3), "SET");
    u8g2.drawStr(6, height(frame, 12, 4), "BACK");
    u8g2.drawStr(41, height(frame, 12, 1), u8x8_u16toa(all_str.cyc.Run, 3));
    u8g2.drawStr(47, height(frame, 12, 2), u8x8_u16toa(all_str.cyc.Rest, 3));
    selectBox(marker, controlType);
  } while (u8g2.nextPage());
}

void timer(uint8_t marker, uint8_t frame, uint8_t controlType, struct All all_str)
{
  static const char *valve_value[5] = {"1 >", "2 >", "3 >", "4 >", "ALL >"};
  u8g2.firstPage();
  do
  {
    heading(40, 10, "TIMER");
    u8g2.drawStr(1, height(frame, 12, 1), "PROG<    >");
    u8g2.drawStr(1, height(frame, 12, 2), "START HOUR<    >");
    u8g2.drawStr(1, height(frame, 12, 3), "START MIN<    >");
    u8g2.drawStr(1, height(frame, 12, 4), "DURATION<      MINS");
    u8g2.drawStr(1, height(frame, 12, 5), "INTERVAL<   >DAYS");
    u8g2.drawStr(1, height(frame, 12, 6), "ZONE<");
    u8g2.drawStr(1, height(frame, 12, 7), "SET");
    u8g2.drawStr(1, height(frame, 12, 8), "EXIT");
    u8g2.drawStr(1, height(frame, 12, 9), "BACK");
    u8g2.drawStr(43, height(frame, 12, 1), u8x8_u16toa(all_str.tim.Prog, 2));
    u8g2.drawStr(89, height(frame, 12, 2), u8x8_u16toa(all_str.tim.StartHr, 2));
    u8g2.drawStr(76, height(frame, 12, 3), u8x8_u16toa(all_str.tim.StartMin, 2));
    u8g2.drawStr(73, height(frame, 12, 4), u8x8_u16toa(all_str.tim.Runtime, 3));
    u8g2.drawStr(70, height(frame, 12, 5), u8x8_u16toa(all_str.tim.Interval, 1));
    u8g2.drawStr(44, height(frame, 12, 6), valve_value[all_str.tim.Valve]);
    selectBox(marker, controlType);
  } while (u8g2.nextPage());
}

void calendar(uint8_t marker, uint8_t frame, uint8_t controlType, struct All all_str)
{

  u8g2.firstPage();
  do
  {
    heading(11, 10, "SET DATE/TIME");
    u8g2.drawStr(6, height(frame, 12, 1), "DATE");
    u8g2.drawStr(6, height(frame, 12, 2), "TIME");
    u8g2.drawStr(6, height(frame, 12, 3), "EXIT");
    selectBox(marker, controlType);
  } while (u8g2.nextPage());
}

void setdate(uint8_t marker, uint8_t frame, uint8_t controlType, struct All all_str)
{
  u8g2.firstPage();
  do
  {
    u8g2.setFont(u8g2_font_helvB08_tr); // u8g2_font_timB10_tr
    u8g2.setFontMode(1);
    u8g2.drawStr(38, 8, "SET DATE");
    u8g2.drawLine(0, 9, 127, 9);
    u8g2.setFont(u8g2_font_helvB08_tr); // u8g2_font_luBS08_tr// show menu items:
    u8g2.drawStr(5, 20, "YEAR");
    u8g2.drawStr(44, 20, "MONTH");
    u8g2.drawStr(100, 20, "DAY");
    u8g2.drawStr(9, 30, u8x8_u16toa((all_str.cal.YR + 2000), 4));
    u8g2.drawStr(59, 30, u8x8_u16toa(all_str.cal.MTH, 2));
    u8g2.drawStr(106, 30, u8x8_u16toa(all_str.cal.DY, 2));
    if (marker == 1)
    {
      u8g2.setDrawColor(2);
      u8g2.drawBox(0, 11, 40, 10);
    }
    if (marker == 2)
    {
      u8g2.setDrawColor(2);
      u8g2.drawBox(39, 11, 49, 10);
    }
    if (marker == 3)
    {
      u8g2.setDrawColor(2);
      u8g2.drawBox(95, 11, 33, 10);
    }
    //  selectBox(marker, controlType);
  } while (u8g2.nextPage());
}
void settime(uint8_t marker, uint8_t frame, uint8_t controlType, struct All all_str)
{
  u8g2.firstPage();
  do
  {
    u8g2.setFont(u8g2_font_helvB08_tr); // u8g2_font_timB10_tr
    u8g2.setFontMode(1);
    u8g2.drawStr(38, 8, "SET TIME");
    u8g2.drawLine(0, 9, 127, 9);
    u8g2.setFont(u8g2_font_helvB08_tr); // u8g2_font_luBS08_tr// show menu items:
    u8g2.drawStr(2, 20, "HOUR");
    u8g2.drawStr(36, 20, "MINUTE");
    u8g2.drawStr(83, 20, "SECOND");
    u8g2.drawStr(10, 30, u8x8_u16toa(all_str.cal.HR, 2));
    u8g2.drawStr(50, 30, u8x8_u16toa(all_str.cal.MIN, 2));
    u8g2.drawStr(98, 30, u8x8_u16toa(all_str.cal.SEC, 2));
    if (marker == 1)
    {
      u8g2.setDrawColor(2);
      u8g2.drawBox(0, 11, 32, 10);
    }
    if (marker == 2)
    {
      u8g2.setDrawColor(2);
      u8g2.drawBox(34, 11, 44, 10);
    }
    if (marker == 3)
    {
      u8g2.setDrawColor(2);
      u8g2.drawBox(81, 11, 47, 10);
    }
    //    selectBox(marker, controlType);
  } while (u8g2.nextPage());
}
void outlet(uint8_t marker, uint8_t frame, uint8_t controlType, struct All all_str)
{

  u8g2.firstPage();
  do
  {
    heading(36, 10, "OUTLET");
    u8g2.drawStr(6, height(frame, 12, 1), "VALVE");
    u8g2.drawStr(6, height(frame, 12, 2), "PUMP");
    u8g2.drawStr(6, height(frame, 12, 3), "BACK");
    selectBox(marker, controlType);
  } while (u8g2.nextPage());
}
void sensor(uint8_t marker, uint8_t frame, uint8_t controlType, struct All all_str)
{

  u8g2.firstPage();
  do
  {
    heading(25, 10, "RAIN SENSE");
    u8g2.drawStr(6, height(frame, 12, 1), "ON");
    u8g2.drawStr(6, height(frame, 12, 2), "OFF");
    u8g2.drawStr(6, height(frame, 12, 3), "BACK");
    selectBox(marker, controlType);
  } while (u8g2.nextPage());
}
void network(uint8_t marker, uint8_t frame, uint8_t controlType, struct All all_str)
{

  u8g2.firstPage();
  do
  {
    heading(29, 10, "NETWORK");
    u8g2.drawStr(6, height(frame, 12, 1), "SMS");
    u8g2.drawStr(6, height(frame, 12, 2), "GPRS");
    u8g2.drawStr(6, height(frame, 12, 3), "OFF");
    u8g2.drawStr(6, height(frame, 12, 4), "BACK");
    selectBox(marker, controlType);
  } while (u8g2.nextPage());
} //
void humidity(uint8_t marker, uint8_t frame, uint8_t controlType, struct All all_str)
{

  u8g2.firstPage();
  do
  {
    heading(29, 10, "SETPOINT");
    u8g2.drawStr(6, height(frame, 12, 1), "HUMIDITY<      >%");
    u8g2.drawStr(6, height(frame, 12, 2), "SET");
    u8g2.drawStr(6, height(frame, 12, 3), "BACK");
    u8g2.drawStr(75, height(frame, 12, 1), u8x8_u16toa(all_str.hum, 3));
    selectBox(marker, controlType);
  } while (u8g2.nextPage());
}
