#include <NewPing.h>

// Ultrasonic Detector
const int TRIGGER_PIN = 12;
const int ECHO_PIN = 11;
const int MAX_DISTANCE = 200;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

// Potentiometer
const int SENSOR_PIN = 0;
const int LED_PIN = 9;

int potValue = 0;

// Button
const int BUTTON_PIN = 2;

// Buzzer
const int BUZZER_PIN = 4;
char notes[] = "cdefg";


void setup()
{
  pinMode(BUTTON_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  Serial.begin(115200);
}

void loop()
{
  int buttonState;
  buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == LOW) // Button pushed
  {
    // Potentiometer value (0-1023)
    potValue = analogRead(SENSOR_PIN);
    
    int uS = sonar.ping();
    
    tone(BUZZER_PIN, 'a', potValue);
      
    delay(potValue);

    Serial.print("Ping: ");
    Serial.print(uS / US_ROUNDTRIP_CM); // Distance in cm
    Serial.println("cm");

    Serial.print("potValue == ");
    Serial.println(potValue, DEC);
  }
  else
  {
    Serial.println("OFF");
  }
  

}

