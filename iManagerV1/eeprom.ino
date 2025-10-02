void readEEPROM()
{
  const uint8_t MUX = 6;
  static bool initialised = false;
  if (!initialised)
  {
    pump_init_flag = true;
    EEPROM.get(0 * sizeof(irr_mode), irr_mode);
    EEPROM.get(1 * sizeof(wOutlet), wOutlet);
    EEPROM.get(2 * sizeof(rSense), rSense);
    EEPROM.get(3 * sizeof(gsmMode), gsmMode);
    EEPROM.get(4 * sizeof(All_Str.prog_count), All_Str.prog_count);
    if (irr_mode == LOOP)
    {
      EEPROM.get(2 * sizeof(All_Str.cyc), All_Str.cyc);//16
      firstRun = true;
      loopState = REST;
    }
    else if (irr_mode == PROGS)
    {
      EEPROM.get(5 * MUX, Programs);//30
      next_irr_flag = true;
    }
    EEPROM.get(2 * sizeof(All_Str.hum), All_Str.hum);//8
    password = EEPROMread(320);//320
    phone = EEPROMread(327);//327
    APN = EEPROMread(345);//345
  }
  initialised = true;
}


void EEPROMwrite(int addrOffset, String strToWrite)
{
  byte len = strToWrite.length();
  EEPROM.write(addrOffset, len);
  for (int i = 0; i < len; i++)
  {
    EEPROM.write(addrOffset + 1 + i, strToWrite[i]);
  }
  EEPROM.commit();
}


String EEPROMread(int addrOffset)
{
  byte newStrLen = EEPROM.read(addrOffset);
  char data[newStrLen + 1];
  for (int i = 0; i < newStrLen; i++)
  {
    data[i] = EEPROM.read(addrOffset + 1 + i);
  }
  data[newStrLen] = '\0'; // !!! NOTE !!! Remove the space between the slash "/" and "0" (I've added a space because otherwise there is a display bug)
  return String(data);
}
