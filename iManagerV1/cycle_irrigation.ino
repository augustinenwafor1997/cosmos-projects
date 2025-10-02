void cycle_irrigate()
{
  if (irr_mode == LOOP)
  {
    char valve_A[4] = {valve_1A, valve_2A, valve_3A, valve_4A};
    char valve_B[4] = {valve_1B, valve_2B, valve_3B, valve_4B};
    if (loopState == REST && (firstRun == true || restChrono.hasPassed(All_Str.cyc.Rest * 60)))
    {
      //start running
      loadState = true;
      loadChrono.restart();
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
      //selTime = (All_Str.cyc.Run * 60000);// convert the set value from mins to milliseconds
      counter = millis() + (All_Str.cyc.Run * 60000);// add the timer value to the time that has elapsed since the system bootup
      runChrono.restart();
      loopState = RUN;
      firstRun = false;
    }
    if (loopState == RUN && runChrono.hasPassed(All_Str.cyc.Run * 60))
    {
      // stop running
      loadState = true;
      loadChrono.restart();
      if (wOutlet == VALVE)
      {
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
      else if (wOutlet == PUMP)
      {
        digitalWrite(pumpPin, LOW);
        // digitalWrite (PC13, HIGH);//test
      }
      counter = millis() + (All_Str.cyc.Rest * 60000);// add the timer value to the time that has elapsed since the system bootup
      restChrono.restart();
      loopState = REST;
    }
  }
}
