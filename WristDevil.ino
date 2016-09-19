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
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

//Variables/consts for led pin
//TODO: int LED //Assign ~PWN pin from board.
//TODO: unsigned long brightness //Init to 0.
//TODO: unsigned long fadeAmount //TODO calculate fadeAmount. NOTE: may not need this variable.

void setup()
{
  //pinMode(LED, OUTPUT);
  Serial.begin(115200);
}

void loop()
{
  analogWrite(LED, brightness);

  int uS = sonar.ping_cm();
  brightness = uS;

  //Print statements for debug.
  Serial.print("Ping: "); Serial.print(uS); Serial.println("cm :: ");
  Serial.print("LED Brightness: "); Serial.println(brightness);

}
