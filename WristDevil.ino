#include <NewPing.h>
//TODO: #include <WristDevil.h> ... make the library!!!

/*
 * Sept 19, 2016.
 *  Author: Bill Brubaker
 *  LED test for WristDevil.
 *  At this time, the Batlab has no haptic motors.
 *  So in the meantime, we are going to use an LED
 *  to text the program logic/responsiveness.
 */

/*
 * Oct 2, 2016.
 *  Author: Bill Brubaker
 *  Stephen Byrne suggested that the sonic distance
 *  sensor shouldn't return 0 values, which which causes
 *  input generated from the sensor cause errors in the
 *  distance detection algorithms.
 *  Meason Wiley recommended that this process happen in
 *  a loop separate from Arduino's `loop()`.
 */

//Ultrasonic Detector
const int TRIGGER_PIN = 12;
const int ECHO_PIN = 11;
const int MAX_DISTANCE = 255;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
const int SAMPLE_SIZE = 5;

int avg = 1;
int LED = 6;
int brightness = 0;
const int MAX_BRIGHT = 255;

int newRawSonar = 0;
int oldRawSonar = 0;

int rollingAverage(int average, int newSample){
  average -= average/SAMPLE_SIZE;
  if(average < 0)
    return 0;
  average += newSample/SAMPLE_SIZE;
  return average;
}

int convertToLightLevel(int average){ //Intended a custom map() function.
  int light;
  int range = 238 - 205;

  if(average >=238)
    return 255;
  else if (average <= 205)
    return 0;
  else{
    //1-254 range return,
    //based on range of 33
    return average/range;
  }
}

int convertRawSonar(int rawSig){
  int bright = MAX_BRIGHT-rawSig;
  return bright;
}

int smoothZeroes(int oldValue, int newValue){
  if(newValue == 0 && oldValue != 0)
    return oldValue;
  return newValue;
}

int sensorRead(){
  int uSPing = 0;
  do {
    uSPing = sonar.ping_cm();
  } while (uSPing == 0);
  return uSPing;
}

void setup()
{
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
}

void loop()
{
  newRawSonar = sensorRead();
  //int uS = smoothZeroes(oldRawSonar, newRawSonar); //TODO: move to function which supports sensorRead().
  oldRawSonar = newRawSonar; //TODO: not needed here. Move to function which supports sensorRead().
  brightness = convertToLightLevel(uS);

  //analogWrite(LED, convertToLightLevel(avg));
  analogWrite(LED, brightness);

  avg = rollingAverage(avg, brightness); //TODO: not needed here. Move to sensorRead().

  delay(30); //need to delay long enouch for the sonic echo to send an receive.

  Serial.print("Ping: ");Serial.print(uS); Serial.println("cm");
  Serial.print("LED Brightness: "); Serial.println(brightness);
}
