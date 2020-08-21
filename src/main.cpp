
#include <Arduino.h>
#include <ArduinoBLE.h>
const int  buttonPin = 13;    // the pin that the pushbutton is attached to
const int espressoPin = 12;       // the pin that the LED is attached to
bool on; // boolean to keep track of on/off
int timer = 0; //counts how long the button is pushed
void blePeripheralConnectHandler();

void setup() {
   if (!BLE.begin()) {
    Serial.println("starting BLE failed!"); 

    while (1);
  }
  BLE.setAdvertisedServiceUuid("fe8e9fb9-e229-4df3-975d-8b92b7a31c9c");
  // BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler;
  // BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);

  // initialize the button pin as a input:
  pinMode(buttonPin, INPUT);
  // initialize the LED as an output:
  pinMode(espressoPin, OUTPUT);
  // initialize serial communication:
  Serial.begin(9600);
}


void loop() {
    // read the pushbutton input pin:  
  if (digitalRead(buttonPin) == HIGH) 
  {
      // start a counter
    while (digitalRead(buttonPin) == HIGH)
    {
        delay(1000);
        timer++;
    }

      // if it is less than 5000 ms turn on /off machine else start bluetooth pairing
    if(timer >= 5)
    {
      // start pairing
      Serial.println("Pairing");
      BLE.advertise();
    }
      //  if timer is less than 5 ms check to see if espressoPin is high or low
      //  if it's low turn it on
    else if(!on)
    {
      on = true;
      digitalWrite(espressoPin,HIGH);
      Serial.println("ON");
      delay(1000);
    }
      // if its high pull it low
    else if(on)
    {
      on = false;
      digitalWrite(espressoPin,LOW);
      Serial.println("OFF");
      delay(1000);
    }
    // reset timer
    timer = 0;

  }
} 

// // central connected event handler
// void blePeripheralConnectHandler() {
//   Serial.print("Connected event, central: ");
// }

// // central disconnected event handler
// void blePeripheralDisconnectHandler(BLEDevice central) {
//   Serial.print("Disconnected event, central: ");
//   Serial.println(central.address());
// }