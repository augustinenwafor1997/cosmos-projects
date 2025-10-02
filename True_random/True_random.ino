const int waitTime = 16;
unsigned int lastint = 0;

unsigned int leftStack = 0;
unsigned int rightStack = 0;


int rotate(int b, int r) {
  return (b << r) | (b >> (8 - r));
}

void pushLeftStack(int bitToPush) {
  leftStack = (leftStack << 1) ^ bitToPush ^ leftStack;
}
void pushRightStackRight(int bitToPush) {
  rightStack = (rightStack >> 1) ^ (bitToPush << 7) ^ rightStack;
}



unsigned int getTrueRotateRandomint() {
  unsigned int finalint = 0;

  unsigned int lastStack = leftStack ^ rightStack;

  for (int i = 0; i < 4; i++) {
    delayMicroseconds(waitTime);
    int leftBits = analogRead(1);

    delayMicroseconds(waitTime);
    int rightBits = analogRead(1);

    finalint ^= rotate(leftBits, i);
    finalint ^= rotate(rightBits, 7 - i);

    for (int j = 0; j < 8; j++) {
      int leftBit = (leftBits >> j) & 1;
      int rightBit = (rightBits >> j) & 1;

      if (leftBit != rightBit) {
        if (lastStack % 2 == 0) {
          pushLeftStack(leftBit);
        } else {
          pushRightStackRight(leftBit);
        }
      }
    }

  }
  lastint ^= (lastint >> 3) ^ (lastint << 5) ^ (lastint >> 4);
  lastint ^= finalint;

  return lastint ^ leftStack ^ rightStack;
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println(getTrueRotateRandomint());
}

void loop() {
  // put your main code here, to run repeatedly:
//Serial.println(getTrueRotateRandomint());
delay(100);
}
