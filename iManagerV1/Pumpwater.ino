void pumpwater()
{
  if (wOutlet == VALVE)
  {
    if ((digitalRead(FS1) == LOW || digitalRead(FS2) == LOW) && pumpState == PUMP_OFF)
    {
      loadState = true;
      loadChrono.restart();
      pumpState = PUMP_ON;
      //EEPROM.put(13 * sizeof(pumpState), pumpState); // revise EEPROM ADDRESS
      digitalWrite(pumpPin, HIGH);
      //digitalWrite(PC13, LOW); //test
    }
    if (digitalRead(FS1) == HIGH && digitalRead(FS2) == HIGH && pumpState == PUMP_ON)
    {
      loadState = true;
      loadChrono.restart();
      pumpState = PUMP_OFF;
      //EEPROM.put(13 * sizeof(pumpState), pumpState);
      digitalWrite(pumpPin, LOW);
      //digitalWrite(PC13, HIGH);//TEST
    }
    // if (pumpState == PUMP_ON && pump_init_flag == true)
    // {
    //     pump_init_flag = false;
    //     digitalWrite(pumpPin, HIGH);
    //     digitalWrite(PC13, LOW); //test
    // }
  }
}
