void turnOffBacklight() {
  if (lcdChrono.hasPassed(10)) {
    digitalWrite(lcdBacklight, HIGH);
    //  lcdState = 0;
  }
}

void turnOnBacklight() {
  if (page == HOME) {
    for (int i = 0; i < 5; i++) {
      if (i != 2) {
        if (Debounce(&buttons[i]) == HIGH) {
          digitalWrite(lcdBacklight, LOW);
          lcdChrono.restart();
        }
      }
    }
  }
}
