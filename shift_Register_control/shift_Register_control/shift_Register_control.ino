// Define the number of shift registers
#define NUM_SHIFT_REGISTERS 3

//Pin connected to ST_CP of 74HC595
#define dataPin PA1
#define clock PA3
#define latchpin PA2

// Array to store outputbytes for each shift register
uint8_t outputbytes[NUM_SHIFT_REGISTERS] = { 0 };

void displayRegister() {
  digitalWrite(latchpin, HIGH);

  for (int i = NUM_SHIFT_REGISTERS - 1; i >= 0; i--) {
    shiftOut(dataPin, clock, MSBFIRST, outputbytes[i]);
  }

  digitalWrite(latchpin, LOW);
}

void setLatchChannelOn(uint8_t channelId) {
  uint8_t shieldOutput = (channelId % 8 == 0) ? 8 : (channelId % 8);
  int registerIndex = (shieldOutput == 8) ? (channelId / 8 - 1) : (channelId / 8);
  uint8_t channelMask = 1 << (shieldOutput - 1);
  outputbytes[registerIndex] = outputbytes[registerIndex] | channelMask;
  displayRegister();
}

void setLatchChannelOff(uint8_t channelId) {
  uint8_t shieldOutput = (channelId % 8 == 0) ? 8 : (channelId % 8);
  int registerIndex = (shieldOutput == 8) ? (channelId / 8 - 1) : (channelId / 8);
  uint8_t channelMask = 255 - (1 << (shieldOutput - 1));
  outputbytes[registerIndex] = outputbytes[registerIndex] & channelMask;
  displayRegister();
}

void setup() {

  //set pins to output because they are addressed in the main loop

  pinMode(dataPin, OUTPUT);
  pinMode(clock, OUTPUT);
  pinMode(latchpin, OUTPUT);
}

void loop() {

  // first register has channels from 1 to 8
  // second register has channels from 9 to 16 and so on.
  // Example: Turn on channel 1 on the first shift register
  setLatchChannelOn(10);
  delay(500);

  // Example: Turn off channel 1 on the first shift register
  setLatchChannelOff(10);
  delay(500);
}