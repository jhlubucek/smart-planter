#include <Arduino.h>
#include "config.h"
#include "planter/planter.h"

Planter pl;

void setup() {
  Serial.begin(9600);
  Serial.println("test");
  pl.connect();
  pinMode(LED_PIN, OUTPUT);
  pinMode(34, INPUT);
}

void loop() {
  Serial.println("loop");
  pl.publish();
  pl.loopClient();
  delay(1000);
};