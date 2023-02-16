const char* decode(const char* input)
{
  char tokenarray[17];
  static char arrtoken[15];
  char datacollector[5];
  char paritybit[3];

  String inp = String(input);
  if (inp.length() > 15) {

    strncpy(tokenarray, input, sizeof(tokenarray));
    memset(input, 0, sizeof(input));
    //  QRSerial.print("ARRay TOKEN: ");
    //  QRSerial.println(tokenarray);
    //int value = 0;
    paritybit[0] = tokenarray[0];
    paritybit[1] = tokenarray[1];
    paritybit[2] = '\0';
    int parity = atoi(paritybit);// convert to integar
    //  QRSerial.println(parity);
    for (int line = 0; line < 14; line++)// loop around 15 times
    {
      if (parity > 14)// if parity has reached 19 then
      {
        arrtoken[line] = tokenarray[(parity - 14) + 1];// collect the value at parity - 19 + 1 [20 - 19 + 1 = 2 which corresponds to 9]
        //      QRSerial.print("PARITY >>:  ");
        //      QRSerial.println(parity);
        //      QRSerial.print(F("ARRAY:  "));
        //      QRSerial.println(arrtoken[line]);
        parity = parity + 1;
      }

      if (parity <= 14)// each time check if parity num has reached 19 (ie from 12 to 19)if yes then:
      {
        arrtoken[line] = tokenarray[(parity + 1)];//collect the value at the position of parity + 1 [4244901]
        //      QRSerial.print("PARITY <<:  ");
        //      QRSerial.println(parity);
        //      QRSerial.print(F("ARRAY:  "));
        //      QRSerial.println(arrtoken[line]);
        parity = parity + 1;// increment parity
      }

      delay(10);
    }
    arrtoken[14] = '\0';
    Serial.print("\n");
    Serial.print("ARRANGED TOKEN: ");
    Serial.println(arrtoken);
    return arrtoken;

//    //  QRSerial.print("Troubleshoot 1: ");
//    //  QRSerial.println(datacollector);
//    memset(datacollector, 0, sizeof(datacollector));
//
//    for (int i = 0; i < 4; i++)
//      datacollector[i] = arrtoken[i];
//    //  QRSerial.print("Troubleshoot UID: ");
//    //  QRSerial.println(datacollector);
//    data.User_ID = atoi(datacollector);
//    memset(datacollector, 0, sizeof(datacollector));
//
//    for (int i = 4; i < 8; i++)
//      datacollector[i - 4] = arrtoken[i];
//    datacollector[4] = '\0';
//    //  QRSerial.print("Troubleshoot TOKEN: ");
//    //  QRSerial.println(datacollector);
//    data.token = atoi(datacollector);
//    memset(datacollector, 0, sizeof(datacollector));
//
//    for (int i = 8; i < 11; i++)
//      datacollector[i - 8] = arrtoken[i];
//    datacollector[3] = '\0';
//    data.tokpair = atoi(datacollector);
//    memset(datacollector, 0, sizeof(datacollector));

  }
}
