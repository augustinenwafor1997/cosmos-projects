void printQRcode(const char *data) {
  // Create the QR code
  QRCode qrcode;
  uint8_t qrcodeData[qrcode_getBufferSize(3)];
  qrcode_initText(&qrcode, qrcodeData, 3, 0, data);
  // Serial.print("DATA:  ");
  // Serial.println(data);
  // Serial.println("done");
  for (uint8_t y = 0; y < qrcode.size; y++) {

    // Left quiet zone
    Serial.print("        ");

    // Each horizontal module
    for (uint8_t x = 0; x < qrcode.size; x++) {

      // Print each module (UTF-8 \u2588 is a solid block)
      // Serial.print(qrcode_getModule(&qrcode, x, y) ? "\u2588\u2588": "  ");
      int qr = qrcode_getModule(&qrcode, x, y) ? 1 : 0;
      //Serial.print(qr ? "\u2588\u2588": "  ");
      array[y][x] = qr;
      Serial.print(array[y][x]);
      Serial.print(" ");
      //printer.write(qrcode_getModule(&qrcode, x, y) ? 0 : 1);
    }

    Serial.print("\n");
    //printer.write('\n');
  }
  //Serial.print(qrcode.size);
}
