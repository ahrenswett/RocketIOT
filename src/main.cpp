
#include <Arduino.h>
#include <ArduinoBLE.h>
const int  buttonPin = 13;    // the pin that the pushbutton is attached to
const int espressoPin = 12;       // the pin that the LED is attached to
// boolean to keep track of on/off
bool on;

void setup() {
  // initialize the button pin as a input:
  pinMode(buttonPin, INPUT);
  // initialize the LED as an output:
  pinMode(espressoPin, OUTPUT);
  // initialize serial communication:
  Serial.begin(9600);
}


void loop() {
  // read the pushbutton input pin:
  Serial.println("Waiting for button press/n");
  Serial.println(digitalRead(buttonPin));
  
  if (digitalRead(buttonPin) == HIGH)
  {
  //  check to see if light is on or not
    // if it's  not turn it on
    if(!on)
    {
      on = true;
      digitalWrite(espressoPin,HIGH);
      Serial.println("ON");
      delay(1000);
    }
    // if it is turn it off
    else
    {
      on = false;
      digitalWrite(espressoPin,LOW);
      Serial.println("OFF");
      delay(1000);
    }
  }
} 