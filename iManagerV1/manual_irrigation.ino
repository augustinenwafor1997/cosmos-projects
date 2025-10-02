void manual_irrigate() {
  //static bool buttState = false;
  char valve_A[4] = {valve_1A, valve_2A, valve_3A, valve_4A};
  char valve_B[4] = {valve_1B, valve_2B, valve_3B, valve_4B};

  if (irr_mode == MANUAL)
  {
    if (manual.checkClick() == LOW)
    {
      digitalWrite(lcdBacklight, LOW);
      lcdChrono.restart();
      loadState = true;
      loadChrono.restart();
      if (irr_valve == RUNNING || (wOutlet == PUMP && digitalRead(pumpPin))) {
        // stop running
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
      }
      else {
        //start running
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
      //      buttState = !buttState;
      //      if (buttState == true) {
      //        //start running
      //        if (wOutlet == VALVE)
      //        {
      //          irr_valve = RUNNING;
      //          for (int i = 0; i < sizeof(valve_A); i++)
      //          {
      //            digitalWrite(valve_A[i], HIGH);
      //            digitalWrite(valve_B[i], LOW);
      //          }
      //          delay(100); // clarify actual delay time
      //          for (int i = 0; i < sizeof(valve_A); i++)
      //          {
      //            digitalWrite(valve_A[i], LOW);
      //            digitalWrite(valve_B[i], LOW);
      //          }
      //        }
      //        else if (wOutlet == PUMP)
      //        {
      //          digitalWrite(pumpPin, HIGH);
      //          // digitalWrite (PC13, LOW);//test
      //        }
      //      }
      //      else {
      //        // stop running
      //        if (wOutlet == VALVE)
      //        {
      //          irr_valve = SHUTOFF;
      //          for (int i = 0; i < sizeof(valve_A); i++)
      //          {
      //            digitalWrite(valve_A[i], LOW);
      //            digitalWrite(valve_B[i], HIGH);
      //          }
      //          delay(100); // clarify actual delay time
      //          for (int i = 0; i < sizeof(valve_A); i++)
      //          {
      //            digitalWrite(valve_A[i], LOW);
      //            digitalWrite(valve_B[i], LOW);
      //          }
      //        }
      //        else if (wOutlet == PUMP)
      //        {
      //          digitalWrite(pumpPin, LOW);
      //          // digitalWrite (PC13, HIGH);//test
      //        }
      //      }
    }
  }
}
