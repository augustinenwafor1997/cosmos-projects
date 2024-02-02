#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

#include <SimpleModbusMaster.h>
//#include <Arduino_JSON.h>

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(PA10, PA9);  // nRF24L01 (CE,CSN)
// const byte address[][7] = { "00001A", "00001B", "00001C", "00001D", "00001E" };
uint8_t address[][6] = { "1Node", "2Node", "3Node", "4Node", "5Node" };


//////////////////// Voltage quality information ///////////////
#define HIGH_VOLTAGE_THRESHOLD 220    // Set your high voltage threshold
#define LOW_VOLTAGE_THRESHOLD 180     // Set your low voltage threshold
#define PHASE_IMBALANCE_THRESHOLD 10  // Set your phase imbalance threshold
#define PHASE_FAILURE_THRESHOLD 100   // Set your phase failure threshold

//////////////transmission window //////////////////
unsigned long previousMillis = 0;
const long transmissionInterval = 3000;  // Interval for transmitting states (in milliseconds)


////////////////////Modbus Port information ///////////////////
#define baud 9600
#define timeout 1000
#define polling 200  // the scan rate
#define retry_count 20

// used to toggle the receive/transmit pin on the driver
#define TxEnablePin PB1
#define LED_BUILTIN PC13
#define channPin1 PB6
#define channPin2 PB7
#define channPin3 PB8
#define channPin4 PB9
#define addPin1 PA15
#define addPin2 PA11
#define addPin3 PA12

const int numTransmitters = 5;                // Number of transmitters
const unsigned long timeSlotDuration = 4000;  // Time slot duration in milliseconds
bool highVoltage, lowVoltage, phaseImbalance, phaseFailure = false;
uint8_t channel = 0, addrDec = 0, sendCount = 0;


// The total amount of available memory on the master to store data
#define TOTAL_NO_OF_REGISTERS 13

// This is the easiest way to create new packets
// Add as many as you want. TOTAL_NO_OF_PACKETS
// is automatically updated.
enum {
  PACKET1,
  PACKET2,
  PACKET3,
  PACKET4,
  TOTAL_NO_OF_PACKETS  // leave this last entry
};

// Create an array of Packets to be configured
Packet packets[TOTAL_NO_OF_PACKETS];

// Masters register array
unsigned int regs[TOTAL_NO_OF_REGISTERS];

// get voltage from modbus RTU
float convertToVoltage(int input) {
  // Combine two 16-bit values into a 32-bit unsigned integer
  unsigned long combinedValue = (((unsigned long)regs[input] << 16) | regs[input + 1]);

  // Convert the combined value to a float
  float Voltage;
  memcpy(&Voltage, &combinedValue, sizeof(float));

  return Voltage;
}

/////////////////////// voltage compare components ////////////////////

bool checkHighVoltage(float phase1, float phase2, float phase3) {
  return (phase1 > HIGH_VOLTAGE_THRESHOLD || phase2 > HIGH_VOLTAGE_THRESHOLD || phase3 > HIGH_VOLTAGE_THRESHOLD);
}

bool checkLowVoltage(float phase1, float phase2, float phase3) {
  return (phase1 < LOW_VOLTAGE_THRESHOLD || phase2 < LOW_VOLTAGE_THRESHOLD || phase3 < LOW_VOLTAGE_THRESHOLD);
}

bool checkPhaseImbalance(float phase1, float phase2, float phase3) {
  float maxVoltage = max(max(phase1, phase2), phase3);
  float minVoltage = min(min(phase1, phase2), phase3);
  return (maxVoltage - minVoltage > PHASE_IMBALANCE_THRESHOLD);
}

bool checkPhaseFailure(float phase1, float phase2, float phase3) {
  return (phase1 < PHASE_FAILURE_THRESHOLD || phase2 < PHASE_FAILURE_THRESHOLD || phase3 < PHASE_FAILURE_THRESHOLD);
}

// Implement your transmission function here
void transmitStates(bool highVoltage, bool lowVoltage, bool phaseImbalance, bool phaseFailure) {
  // Implement your transmission logic based on your communication module 
  //TEST CODE
  JsonDocument data;

  data["comm"] = addrDec;
  data["f"] = phaseFailure ? 1 : 0;
  data["d"] = phaseImbalance ? 1 : 0;
  data["hv"] = highVoltage ? 1 : 0;

  String dataString; 
  serializeJson(data, dataString);

  unsigned long currentTime = millis();
  int currentSlot = currentTime / timeSlotDuration;  // Calculate current time slot
  // uint8_t currentAddr = currentSlot % numTransmitters;

  // Serial.print("CURRENT SLOT: ");
  // Serial.println(currentAddr + 1);
  if (currentSlot % numTransmitters == addrDec - 1) {  // Check if it's this transmitter's slot
    // Transmit data
    if (sendCount < 3) {
      Serial.print("SLOT: ");
      Serial.println(addrDec);
      radio.powerUp();
      radio.write(&dataString[0], dataString.length());
      sendCount++;
      Serial.println(dataString);
    }
  } else {
    // Wait for the next time slot
    //Serial.println("wait");
    radio.powerDown();  // Conserve power during inactive slots
    sendCount = 0;
  }
}

//                      RX    TX
//HardwareSerial Serial2(PA3, PA2);

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  pinMode(channPin1, INPUT_PULLDOWN);
  pinMode(channPin2, INPUT_PULLDOWN);
  pinMode(channPin3, INPUT_PULLDOWN);
  pinMode(channPin4, INPUT_PULLDOWN);
  pinMode(addPin1, INPUT_PULLDOWN);
  pinMode(addPin2, INPUT_PULLDOWN);
  pinMode(addPin3, INPUT_PULLDOWN);
  //Serial.begin(9600);
  // Initialize each packet
  modbus_construct(&packets[PACKET1], 1, READ_HOLDING_REGISTERS, 512, 2, 1);
  modbus_construct(&packets[PACKET2], 1, READ_HOLDING_REGISTERS, 514, 2, 3);
  modbus_construct(&packets[PACKET3], 1, READ_HOLDING_REGISTERS, 516, 2, 5);

  // Initialize the Modbus Finite State Machine
  modbus_configure(&Serial2, baud, SERIAL_8E1, timeout, polling, retry_count, TxEnablePin, packets, TOTAL_NO_OF_PACKETS, regs);

  //settup channels and addresses
  channel += digitalRead(channPin4) * 8;  // 2^3
  channel += digitalRead(channPin3) * 4;  // 2^2
  channel += digitalRead(channPin2) * 2;  // 2^1
  channel += digitalRead(channPin1) * 1;  // 2^0
  // Serial.print("Channel: ");
  // Serial.println(channel);
  
  // if(channel == 1){
  //    digitalWrite(LED_BUILTIN, LOW);
  //}
  while (channel < 1) {
    // code does not run until channel is set
  }
  addrDec += digitalRead(addPin1) * 4;
  addrDec += digitalRead(addPin2) * 2;
  addrDec += digitalRead(addPin3) * 1;
  // Serial.print("Address  in decimal: ");
  // Serial.println(addrDec);
  // if(addrDec == 1){
  //   digitalWrite(LED_BUILTIN, LOW);
  // }
  while (addrDec < 1 || addrDec > 5) {
    // code does not if address is not set
  }
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(channel);
  radio.openWritingPipe(address[addrDec - 1]);
  radio.stopListening();

}

void loop() {
  //digitalWrite(LED, LOW);
  // digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  // delay(1000);
  // digitalWrite(LED_BUILTIN, HIGH);   // turn the LED off by making the voltage LOW
  // delay(1000);
  unsigned long currentMillis = millis();
  modbus_update();

  if (currentMillis - previousMillis >= transmissionInterval) {
    previousMillis = currentMillis;  // Save the last time a transmission occurred
    float phase1Voltage = convertToVoltage(1);
    float phase2Voltage = convertToVoltage(3);
    float phase3Voltage = convertToVoltage(5);


    highVoltage = checkHighVoltage(phase1Voltage, phase2Voltage, phase3Voltage);
    lowVoltage = checkLowVoltage(phase1Voltage, phase2Voltage, phase3Voltage);
    phaseImbalance = checkPhaseImbalance(phase1Voltage, phase2Voltage, phase3Voltage);
    phaseFailure = checkPhaseFailure(phase1Voltage, phase2Voltage, phase3Voltage);

    // Print results to Serial Monitor
    // Serial.print("PHASE 1 Voltage: ");
    // Serial.println(phase1Voltage);
    // Serial.print("PHASE 2 Voltage: ");
    // Serial.println(phase2Voltage);
    // Serial.print("PHASE 3 voltage: ");
    // Serial.println(phase3Voltage);
  }

  // Implement your transmission function here
  transmitStates(highVoltage, lowVoltage, phaseImbalance, phaseFailure);
}
