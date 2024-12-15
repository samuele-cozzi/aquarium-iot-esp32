#include <SPI.h>
#include <Wire.h> 
#include <RTClib.h>
#include <FastLED.h>

RTC_DS3231 rtc;

#define NUM_LEDS 14
#define DATA_PIN 2
CRGB leds[NUM_LEDS];

void printTwoDigits(int);

void setup() {
  Serial.begin(115200);
  delay(2000);
  if (! rtc.begin()) {
    Serial.println("Could not find RTC! Check circuit.");
    //while (1);
  }
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));


  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);

  Serial.print("Setup Finished!");
  delay(10000);
}

void loop() {
  DateTime now = rtc.now();
  Serial.print("Loop!");

  Serial.print("Current Date: ");
  Serial.print(now.year(), DEC);
  Serial.print("/");
  printTwoDigits(now.month());
  Serial.print("/");
  printTwoDigits(now.day());
  Serial.print(" Current Time: ");
  printTwoDigits(now.hour());
  Serial.print(":");
  printTwoDigits(now.minute());
  Serial.print(":");
  printTwoDigits(now.second());
  Serial.println();

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::White;
  }
  FastLED.show();

  // put your main code here, to run repeatedly:
  delay(10000);
}

void printTwoDigits(int number) {

  if (number < 10) {
    Serial.print("0"); // Add a leading zero for single-digit numbers
  }
  Serial.print(number);

}