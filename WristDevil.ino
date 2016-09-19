#include <NewPing.h>

// Ultrasonic Detector
const int TRIGGER_PIN = 12;
const int ECHO_PIN = 11;
const int MAX_DISTANCE = 255; //Set to 255 for LED dimming test.
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

//TODO: variables/consts for led pin


void setup()
{
  Serial.begin(115200);
}

void loop()
{
  int uS = sonar.ping_cm();
  Serial.print("Ping: ");
  Serial.print(sonar.ping_cm(); // Distance in cm
  Serial.println("cm");
}
