#include <NewPing.h>

/*
 * Sept 19, 2016.
 *  Author: Bill Brubaker
 *  LED test for WristDevil.
 *  At this time, the Batlab has no haptic motors.
 *  So in the meantime, we are going to use an LED
 *  to text the program logic/responsiveness.
 */

// Ultrasonic Detector
const int TRIGGER_PIN = 12;
const int ECHO_PIN = 11;
const int MAX_DISTANCE = 255; //Set to 255 for LED dimming test.
const int MAX_BRIGHT = 255;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
const int SAMPLE_SIZE = 20;

int avg = 1;
int LED = 6;
int brightness = 0;
const int MAX_BRIGHT = 255;

//Variables/consts for led pin
//TODO: int LED //Assign ~PWN pin from board.
//TODO: unsigned long brightness //Init to 0.
//TODO: unsigned long fadeAmount //TODO calculate fadeAmount. NOTE: may not need this variable.

int rollingAverage(int average, int newSample){
  average -= average/SAMPLE_SIZE;
  average += newSample/SAMPLE_SIZE;
  return average;
}

int rollingAverage(int average, int newSample){
  average -= average/SAMPLE_SIZE;
  average += newSample/SAMPLE_SIZE;
  return average;
}

//TODO: work on light conversion.
int convertToLightLevel(int average){
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

void setup()
{
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
}

void loop()
{
  analogWrite(LED, avg);
  
  int uS = sonar.ping_cm();
  brightness = MAX_BRIGHT - uS;
  avg = rollingAverage(avg, brightness);
  
  delay(30);

  Serial.print("Ping: ");Serial.print(sonar.ping_cm()); Serial.println("cm");
  Serial.print("LED Brightness: "); Serial.println(avg);
}
