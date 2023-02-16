const char* checkScanner() {
  if (QRSerial.available()) {
    while (QRSerial.available()) {
      // processQRByte((char)QRSerial.read());
      char input_line [17];
      static char input [17];
      memset(input, 0, sizeof(input));
      static unsigned int input_pos = 0;

      if (input_pos < 16) {
        input_line [input_pos] = QRSerial.read();
        Serial.print(input_line [input_pos]);
        input_pos++;
      }

      if (input_pos == 16) {
        input_line [input_pos] = '\0';  // terminating null byte
        Serial.print("\n");
        Serial.print(input_line);
        //decode(input_line);
        input_pos = 0;
        strncpy(input, input_line, sizeof(input));
        return input;
      }
    }
  }
  // recvState = 0;
}

//void processQRByte (char inByte) {
//  static char input_line [17];
//  static unsigned int input_pos = 0;
//
//    if (input_pos < 16) {
//      input_line [input_pos] = inByte;
//      QRSerial.print(inByte);
//      input_pos++;
//    }
//
//    if (input_pos == 16) {
//    input_line [input_pos] = 0;  // terminating null byte
//    QRSerial.print("\n");
//    QRSerial.print(input_line);
//    decode(input_line);
////    incomingMessage = String(input_line);
////    // terminator reached! process input_line here ...
////    messState = 1;
//    // reset buffer for next time
//    input_pos = 0;
//  }
//
//} // end of processIncomingByte
