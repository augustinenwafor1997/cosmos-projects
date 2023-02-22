void printTicket() {
  if (printButton.checkClick() == LOW)
  {
    printer.wake();

    printer.setFont('B');
    printer.justify('C');
    printer.doubleHeightOn();
    printer.println("SUCCESSFUL");
    printer.doubleHeightOff();
    printQRcode(encode());
    printer.setSize('S');
    printer.println(F("QR code is only valid for one day"));

    printer.feed(2);
    printer.setDefault();
    printer.sleep();
  }

}
