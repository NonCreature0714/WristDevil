//Copyright (c) 2016 Bill Brubaker (noncreature0714@gmail.com), Stehpen Byrne, Meason Wiley
//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

//The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE  WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include <NewPing.h>
//TODO: #include <WristDevil.h> ... make the library!!!

/*
 * WristDevil.
 * https://github.com/NonCreature0714/WristDevil
 * First Generation.
 *
 * Purpose:
 *  To give people with sight impairment the ability
 *  to "sense" their envirment better with a wrist
 *  worn haptic (vibration) sensor, which vibrates
 *  more forcefully as it gets in closer proximity
 * to obstacles.
 **/

/*
 * Notes on Functionality.
 *
 * This iteration of WD (WristDevil) is designed
 *to work with one sonic distacne detector, the
 * HC-SR04, and is dependant NewPing library.
 * https://github.com/PaulStoffregen/NewPing
 **/

/*
 * Sept 19, 2016.
 *  Author: Bill Brubaker
 *  LED test for WristDevil.
 *  At this time, the Batlab has no haptic motors.
 *  So in the meantime, we are going to use an LED
 *  to text the program logic/responsiveness.
 **/

/*
 * Oct 2, 2016.
 *  Author: Bill Brubaker
 *  Stephen Byrne suggested that the sonic distance
 *  sensor shouldn't return 0 values, which which causes
 *  input generated from the sensor cause errors in the
 *  distance detection algorithms.
 *  Meason Wiley recommended that this logic happen in
 *  a function separate from Arduino's `loop()`.
 *
 *  Changed LED to FEEDBACK_PIN_1 anticipating change
 *  for haptic motor addition.
 **/

/*
 * October 3, 2016.
 *  Author: Bill Brubaker
 *  Corrected use of #define because:
 *    1. const was taking up too much space, so the
 *       compiler was complaining about space
 *    2. I wasn't using #define correctly anyway
 **/

 /*
  * October 7, 2016.
  *  Author: Bill Brubaker
  *  Successfully added haptic motor to current build!!
  *  Rewrote function names to be more generic, and not
  *  just for LEDs or haptic motors.
  **/

//Ultrasonic Detector
#define TRIGGER_PIN 12
#define ECHO_PIN 11
#define MAX_DISTANCE 255
/////////////////////

//Instantiate NewPing object.
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
/////////////////////////////

//Simulated sample set for rolling average.
#define SAMPLE_SIZE 20
////////////////////////////////////////////

//Feedback setup.
#define FEEDBACK_PIN_1 6
#define MAX_BRIGHT 255
#define DIMMING_STEP 5
#define MAX_READ_ATTEMPTS 10
/////////////////

//Variables to retain data.
//TODO: convert values to uint8-16_t.
int brightness = 0;
int newRawSonar = 0;
int oldRawSonar = 0;
int rollAvg = 0;
//////////////////////////

//Values for sensor control.
#define FAR_DISTANCE_LIMIT 250
#define NEAR_DISTANCE_LIMIT 1
#define MAIN_LOOP_DELAY 30
#define BAUD_RATE 115200
////////////////////////////////

int rollingAverage(int & average, const int & newSample){
  average -= average/SAMPLE_SIZE;
  if(average < 0)
    return 0;
  average += newSample/SAMPLE_SIZE;
  return average;
}//End of rollingAverage.

int convertRawSonarToFeedback(const int & fromSig){
  int bright = MAX_BRIGHT-fromSig;
  return bright;
}//End of convertRawSonarToFeedback().

int interpretDataFrom(int & sensorValue){//Control behavior of feedback.
  if(rollAvg > FAR_DISTANCE_LIMIT){
    rollAvg = rollingAverage(rollAvg, sensorValue);
    return sensorValue -= sensorValue-DIMMING_STEP;
  }
  rollAvg = rollingAverage(rollAvg,sensorValue);
  return sensorValue;
}//End of interpretDataFrom().

int sensorRead(){
  int numTries = 0;
  int uSPing = 0;
  do {
    ++numTries;
    uSPing = sonar.ping_cm();
    if(numTries > MAX_READ_ATTEMPTS)
      return MAX_DISTANCE; //If more than 5 tries, return max range.
  } while (uSPing == 0);
  return uSPing;
}//End of sensorRead().

void setup()
{
  pinMode(FEEDBACK_PIN_1, OUTPUT);
  Serial.begin(BAUD_RATE);
}//End of setup().

void loop()
{
  newRawSonar = sensorRead();
  newRawSonar = interpretDataFrom(newRawSonar);
  brightness = convertRawSonarToFeedback(newRawSonar);
  analogWrite(FEEDBACK_PIN_1, brightness);

  delay(MAIN_LOOP_DELAY); //need to delay long enouch for the sonic echo to send an receive.

  Serial.print("Ping: ");Serial.print(newRawSonar); Serial.println("cm");
  Serial.print("FEEDBACK_PIN_1 Brightness: "); Serial.println(brightness);
}//End of loop().
//End of WristDevil.
