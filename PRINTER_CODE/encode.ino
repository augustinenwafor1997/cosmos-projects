
int getParity() {
  unsigned int rd = getTrueRotateRandomint();
  //  Serial.print("random parity: ");
  //  Serial.println(rd);

  String rando = String(rd);
  char randVal[rando.length() + 1];
  rando.toCharArray(randVal, sizeof(randVal));
  rando = "";
  //  Serial.print("converted to array: ");
  //  Serial.println(randVal);
  String parity = "";
  bool found = false;
  int convRd;
  for (int i = 0; i < (sizeof(randVal) - 1); i++) {
    convRd = randVal[i] - '0';
    //Serial.println(convRd);
    if (convRd > 0 && convRd < 4) {
      parity = "1" + String(randVal[i]);
      found = true;
      //Serial.println("FOUND VALUE");
      break;
    }
  }
  if (found) {
    //Serial.print("return parity now: ");
    return parity.toInt();
  }
  else {
    // Serial.println("return 10");
    return 10;
  }
}
int getToken() {
  unsigned int rd = getTrueRotateRandomint();
  //  Serial.print("random token: ");
  //  Serial.println(rd);
  String rando = String(rd);
  if (rando.length() > 4) {
    rando = rando.substring(1, 5);
    if (rando.startsWith("0")) {
      rando.setCharAt(0, '2');
      // Serial.println("changed value from zero to two");
    }
    return rando.toInt();
  }
  else if (rando.length() == 4) {
    return rando.toInt();
  }
  else return 1000;
}

int getPairAndErr() {
  unsigned int rd = getTrueRotateRandomint();
  //  Serial.print("random pair and error: ");
  //  Serial.println(rd);
  String rando = String(rd);
  if (rando.length() > 3) {
    rando = rando.substring(1, 4);
    if (rando.startsWith("0")) {
      rando.setCharAt(0, '5');
      // Serial.println("changed value from zero to two");
    }
    return rando.toInt();
  }
  else if (rando.length() == 3) {
    return rando.toInt();
  }
  else return 100;
}


const char* encode() {
  /*what to when I revisit
     1. for parity, get random int. convert to string and then to char array, loop through and find num less than 4 and greater than 0. add 1 to it and convert to int otherwise use 10.
     2. UID is hardcoded for now
     3. for token, get random int, remove the last value if it is more than 4 and store the rest as token
     4. for pair, get random int, remove the last two values if it's 5 and remove only one if it's 4 and store the rest as pair
     5. for error, get random int, remove the last two values if it's 5 and remove only one if it's 4 and store the rest as error
  */
  int PARITY = getParity();
  int UID = 1000;
  int tok = getToken();
  int pair = getPairAndErr();
  int error = getPairAndErr();

  // Serial.print("\n\n\n\n");

  // Serial.print("RD PARITY: ");
  // Serial.println(PARITY);

  // Serial.print("RD TOKEN: ");
  // Serial.println(tok);

  // Serial.print("RD PAIR: ");
  // Serial.println(pair);

  // Serial.print("RD ERROR: ");
  // Serial.println(error);

  char lstr[19];
  sprintf(lstr, "%d%d%d%d", UID, tok, pair, error);

  //  Serial.print("ARRANGED: ");
  //  Serial.println(lstr);

  char str[3];
  dtostrf(PARITY, 0, 0, str);
  char firsarr[14];
  char secarr[14];

  for (int line = 0; line < 14; line++) {  // Iterate 20 times
    if (PARITY < 15) {
      secarr[line] = lstr[line];
      //      Serial.print("PARITY <<:  ");
      //      Serial.println(PARITY);
      //      Serial.print(F("VALUE:  "));
      //      Serial.println(lstr[line]);
      PARITY++;
    }
    else {
      firsarr[PARITY - 15] = lstr[line];
      if (PARITY == 15)
        secarr[line] = '\0';
      //      Serial.print("PARITY >>:  ");
      //      Serial.println(PARITY);
      //      Serial.print(F("VALUE:  "));
      //      Serial.println(lstr[line]);
      PARITY++;
    }
    delay (10);
  }
  firsarr[PARITY - 15] = '\0';
  //  delay (1000);
  //  Serial.print("FIRST ARRAY:  ");
  //Serial.println(firsarr);
  //  delay(1000);
  //  Serial.print("SECOND ARRAY:  ");
  //Serial.println(secarr);

  String arr = String(str) + String(firsarr) + String(secarr);
  //    Serial.print("arr:  ");
  //    Serial.println(arr);
  //  static char tokarr[arr.length() + 1];
  static char tokarr[19];
  arr.toCharArray(tokarr, sizeof(tokarr));
  arr = "";
  //  memcpy(token, tokarr, sizeof(tokarr));
  //
  //  Serial.print(F("FIRST ARRAY:  "));
  //  Serial.println(firsarr);
  //
  //  Serial.print(F("SECOND ARRAY:  "));
  //  Serial.println(secarr);
  //
  Serial.print(F("CONCAT ARRAY:  "));
  Serial.println(tokarr);
  return tokarr;
  //
  //  Serial.print(F("TOKEN:  "));
  //  Serial.println(token);
}
