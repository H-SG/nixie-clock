#include <Wire.h>
#include "RTClib.h"
#include "LinkedList.h"

// hardware vars and others
RTC_DS3231 rtc;
int brightP = 10;
int latchP = 11;
int clockP = 12;
int dataP = 13;
unsigned long current_time;
DateTime now;

// levels for brightness cycles, levels are precomputed
LinkedList<int> brightnessLevels = LinkedList<int>();
int b[50] = {128, 143, 159, 174, 189, 202, 215, 226, 235, 243, 
             248, 252, 254, 254, 252, 248, 242, 234, 225, 214,
             202, 188, 174, 158, 143, 127, 111, 95, 80, 65,
             52, 40, 29, 20, 12, 6, 3, 1, 1, 3,
             7, 13, 21, 31, 42, 54, 68, 82, 98, 113};

void setup() {
  rtc.begin();
  // unset this to update time from system
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  // set out pinmodes
  pinMode(latchP, OUTPUT);
  pinMode(clockP, OUTPUT);
  pinMode(dataP, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  // debug serial
  // Serial.begin(9600);

  // initialise linked list
  for (int i = 0; i < 50; i++) {
    brightnessLevels.add(b[i]);
  }

  // run antipoison as startup animation
  antiPoison(100, 2000);

  // set brightness to median
  analogWrite(brightP, 128);

  // initialise time
  current_time = millis();
  now = rtc.now();
}

void loop() {
  // get current time from rtc and write it out
  now = rtc.now();
  timeWrite(now);

  // check if millis has rolled over
  if (millis() < current_time) {
    current_time = millis();
  }

  // brightness pulse routine 40 ms delay should result in full pulse cycle over 2 seconds
  if (millis() - current_time >= 40) {
    int current_brightness = brightnessLevels.shift();
    brightnessLevels.add(current_brightness);
    analogWrite(brightP, current_brightness);
    current_time += 40;
  }


  // run anti-poison routine at 01:00:00
  if (now.hour() == 1) {
    if (now.minute() == 0) {
      if (now.second() == 0) {
        antiPoison(0, 300000);
        current_time += 300000;
      }
    }
  }
}

void timeWrite(DateTime passed_time){
  // disable output so we don't see flickering garbage
  digitalWrite(latchP, LOW);
  // each shiftout controls two digits of the clock
  shiftOut(dataP, clockP, MSBFIRST, numToShiftNum(passed_time.second()));
  shiftOut(dataP, clockP, MSBFIRST, numToShiftNum(passed_time.minute()));
  shiftOut(dataP, clockP, MSBFIRST, numToShiftNum(passed_time.hour()));
  // enable output now that shiftout is done
  digitalWrite(latchP, HIGH);
}

void antiPoison(unsigned int delayTime, unsigned long runTime){
  // implements carousel effect for cathode poisoning prevention
  // better would be to track on time for various digits and then 
  // run the rest as needed
  unsigned long start_time = millis();
  analogWrite(brightP, 255);
  while ((millis() - start_time) < runTime) {
    for (int i = 0; i <= 9; i++){
      int output = (i << 4) + i;
      digitalWrite(latchP, LOW);
      shiftOut(dataP, clockP, MSBFIRST, output);
      shiftOut(dataP, clockP, MSBFIRST, output);
      shiftOut(dataP, clockP, MSBFIRST, output);
      digitalWrite(latchP, HIGH);
      delay(delayTime);
    }

    // handle rollover behaviour
    if (millis() < start_time) {
      break;
    }
  }
  analogWrite(brightP, 1);
}

int numToShiftNum(unsigned int num){
  // shiftOut can do max 255, so we limit this to two digits (two 4 bit pairs)
  int digit2 = num % 10;
  int digit1 = (num - digit2)/10;
  int output = digit2 << 4;
  output += digit1;
  return output;
}
