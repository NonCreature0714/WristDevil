#include <NewPing.h>
//TODO: #include <WristDevil.h> ... make the library!!!

/*
 * WristDevil.
 *
 * First Generation.
 **/

/*
 * Notes on Functionality.
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

//Ultrasonic Detector
const int TRIGGER_PIN = 12;
const int ECHO_PIN = 11;
const int MAX_DISTANCE = 255;
/////////////////////

//Instantiate NewPing object.
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
/////////////////////////////

//Simulated sample set for rolling average.
const int SAMPLE_SIZE = 5;
////////////////////////////////////////////

//Feedback setup.
int FEEDBACK_PIN_1 = 6;
int brightness = 0;
const int MAX_BRIGHT = 255;
const int DIMMING_STEP = 5;
const int MAX_READ_ATTEMPTS = 5;
/////////////////

//Variables to retain data.
int newRawSonar = 0;
int oldRawSonar = 0;
int rollAvg = 0;
//////////////////////////

//Values for sensor control.
const int FAR_DISTANCE_LIMIT = 250;
const int NEAR_DISTANCE_LIMIT = 1;
const int MAIN_LOOP_DELAY = 30;
const int BAUD_RATE = 115200;
////////////////////////////////

int rollingAverage(int & average, const int & newSample){
  average -= average/SAMPLE_SIZE;
  if(average < 0)
    return 0;
  average += newSample/SAMPLE_SIZE;
  return average;
}//End of rollingAverage.

int convertRawSonarToRGBLevel(const int & fromSig){
  int bright = MAX_BRIGHT-fromSig;
  return bright;
}//End of convertRawSonarToRGBLevel().

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
  brightness = convertRawSonarToRGBLevel(newRawSonar);
  analogWrite(FEEDBACK_PIN_1, brightness);

  delay(MAIN_LOOP_DELAY); //need to delay long enouch for the sonic echo to send an receive.

  Serial.print("Ping: ");Serial.print(newRawSonar); Serial.println("cm");
  Serial.print("FEEDBACK_PIN_1 Brightness: "); Serial.println(brightness);
}//End of loop().
