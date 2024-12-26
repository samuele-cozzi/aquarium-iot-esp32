#include <SPI.h>
#include <Wire.h> 
#include <RTClib.h>
#include <FastLED.h>
#include <ESP32Servo.h>

// DS3231 Timer
RTC_DS3231 rtc;

// LED STRIP
#define NUM_LEDS 60
#define DATA_PIN 2
CRGB leds[NUM_LEDS];

// SERVO
int servoPin = 19;
int posVal = 0;    // variable to store the servo position
Servo myservo;

// FUNCTIONS
int green = 0;
int blue = 0;
int red = 0;
int step_green_red = 21; // 253/ 12
int step_blue = 17; // 211 / 12

void printTwoDigits(int);

void setup() {
  Serial.begin(115200);
  delay(2000);

  // Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	myservo.setPeriodHertz(50);    // standard 50 hz servo
	myservo.attach(servoPin, 500, 2500); // attaches the servo on pin to the servo object


  if (! rtc.begin()) {
    Serial.println("Could not find RTC! Check circuit.");
    //while (1);
  }
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  //rtc.adjust(DateTime(2024, 12, 26, 18, 53, 30));

  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);

  Serial.print("Setup Finished!");
  delay(10000);
}

void loop() {
  DateTime now = rtc.now();

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

  // LIGHTS

  if (now.hour() < 7 || now.hour() > 17) {
    // NIGHT
    Serial.println("NIGHT");
    green = 0; red = 0; blue = 5; 
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB(red,green,blue);
    }
  }
  else if (now.hour() == 7) {
    // MORNING
    Serial.println("MORNING");
    green += step_green_red; red += step_green_red; blue += step_blue;
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB(red,green,blue);
    }
  }
  else if (now.hour() == 17){
    // EVENING
    Serial.println("EVENING");
    green -= step_green_red; red -= step_green_red; blue -= step_blue;
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB(red,green,blue);
    }
  }
  else
  {
    // DAY
    Serial.println("DAY");
    green = 253; red = 253; blue = 211; 
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB(red,green,blue);
    }
  }
  FastLED.show();

  // FOOD
  if (now.hour() == 13 && now.minute() > 0 && now.minute() <= 5 && posVal < 180){
  //if (posVal < 180){
    for (int move = 0; move <= 60; move += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      posVal = posVal + 1;
      myservo.write(posVal);       // tell servo to go to position in variable 'pos'
      delay(15);                   // waits 15ms for the servo to reach the position
      Serial.println(posVal);
    }
  }
  else
  {
    posVal = 0;
    myservo.write(posVal);
  }

  // delay 5 minutes
  delay(300000);
  //delay(5000);
}

void printTwoDigits(int number) {

  if (number < 10) {
    Serial.print("0"); // Add a leading zero for single-digit numbers
  }
  Serial.print(number);

}