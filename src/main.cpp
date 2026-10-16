#include <Arduino.h>
const int led=4;
const int pir=5;

unsgned long now=0;
volatile unsigned long lastTrigger=0;
volatile bool startTimer=0;
bool printMotion=false;
const unsigned long timeSeconds=20000;
void ARDUINO_ISR_ATTR motionisr(){
  lastTrigger=millis();
  startTimer=true;
}
void setup(){
  Serial.begin(115200);
  pinMode(led,OUTPUT);
  pinMode(pir,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pir,motionisr,RISING));
  digitalWrite(led,LOW);
}
void loop(){
  now=millis();
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

