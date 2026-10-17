#include <Arduino.h>

const int led = 4;
const int pir = 5;

unsigned long now = 0;                  // Fixed typo: unsgned -> unsigned
volatile unsigned long lastTrigger = 0;
volatile bool startTimer = false;       // Use false instead of 0 for clarity
bool printMotion = false;
const unsigned long timeSeconds = 20000;

void IRAM_ATTR motionisr() {            // Standard ESP32 attribute for ISRs
  lastTrigger = millis();
  startTimer = true;
}

void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  pinMode(pir, INPUT_PULLUP);
  
  // Fixed syntax: motionisr and RISING moved outside digitalPinToInterrupt
  attachInterrupt(digitalPinToInterrupt(pir), motionisr, RISING);
  
  digitalWrite(led, LOW);
}

void loop() {
  now = millis();
  
  if (startTimer && !printMotion) {
    digitalWrite(led, HIGH);
    Serial.println("MOTION DETECTED!!!");
    printMotion = true;
  }

  // Turn off the LED after timeout
  if (startTimer && (now - lastTrigger > timeSeconds)) {
    Serial.println("Motion stopped...");
    digitalWrite(led, LOW);
    startTimer = false;
    printMotion = false;
  }
}