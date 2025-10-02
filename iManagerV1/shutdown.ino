void shutDown(void) {
  if (power.checkpress() == LOW)
  {
    holdCheck = true;
    //ledStateO = !ledStateO;
    //    gotosleep();
  }
  if (holdCheck) {
    delay(1000);
    u8g2.firstPage();
    do {
      u8g2.setFont(u8g2_font_fub17_tr);// u8g2_font_fub17_tr
      u8g2.drawStr(2, 40, "iMANAGER");
    } while ( u8g2.nextPage() );
    delay(5000);
    u8g2.firstPage();
    do {
      //      u8g2.setFont(u8g2_font_fub17_tr);// u8g2_font_fub17_tr
      //      u8g2.drawStr(0, 40, "iMANAGER");
    } while ( u8g2.nextPage() );
    gotosleep();
  }
}
void gotosleep() { // FUNCTION TO SEND THE CHIP TO STANDBY MODE
  //enable the PWR  control clock
  RCC -> APB1ENR |= (RCC_APB1ENR_PWREN);

  // set SLEEPDEEP-bit of cortex system control Register
  SCB -> SCR |= SCB_SCR_SLEEPDEEP_Msk;

  // Select standby mode
  PWR ->CR |= PWR_CR_PDDS;

  //clear wakeup flag
  PWR -> CR |= PWR_CR_CWUF;

  //enable wake up pin
  PWR -> CSR |= (PWR_CSR_EWUP);

  // request wait for interrupt
  __WFI();
}
void Checkoffsate() { //NOT NECCESSARY JUST USED IT TO TEST SOMETHING I USED IT TO DETECT WHEN I USE STANDBY AND WHEN I POWERED IT OFF USING SWITCH
  if (((PWR -> CSR) & (PWR_CSR_SBF)))//check standby flag to clear if just wake up from standby
  {
    // clear PWR wake up flag
    PWR -> CR |= PWR_CR_CWUF;
    PWR -> CR |= PWR_CR_CSBF;
    delay(2000);
    if (digitalRead(A0))
    {
      delay(1000);
      //holdCheck = true;
    }
    else{
      gotosleep();
    }
    
    // DO ANY WAKEUP FROM STANDBY FUNC
  }
  //    else
  //    {
  //        // DO ANY WAKEUP FROM POWER LOSS FUNC OR SWITCH USE
  //    }
}
