#include "declarations.h"
void setup() {
  // put your setup code here, to run once:
  initialisation();
}

void loop() {
  // put your main code here, to run repeatedly:
  processToken(decode(checkScanner()));
  entryControls();
  exitControls();
  espCommands();
}
