
#include <Arduino.h>
#include <ArduinoBLE.h>
const int  buttonPin = 13;    // the pin that the pushbutton is attached to
const int espressoPin = 12;       // the pin that the LED is attached to
bool on; // boolean to keep track of on/off
int timer = 0; //counts how long the button is pushed
BLEService esspressoService("fe8e9fb9-e229-4df3-975d-8b92b7a31c9c");
BLEByteCharacteristic switchCharacteristic("fe8e9fb9-e229-4df3-975d-8b92b7a31c9c", BLERead | BLEWrite);


// central connected event handler
void blePeripheralConnectHandler(BLEDevice central) {
  Serial.print("Connected event, central: ");
}

// central disconnected event handler
void blePeripheralDisconnectHandler(BLEDevice central) {
  Serial.print("Disconnected event, central: ");
  Serial.println(central.address());
}

void switchCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {
  // Setup wifi disconnect bluetooth
  Serial.print("Characteristic event, written: ");

  if (central.address()) {
    Serial.println("LED on");
    digitalWrite(espressoPin, HIGH);
   } /*else {
  //   Serial.println("LED off");
  //   digitalWrite(espressoPin, LOW);
  // }*/
}


void setup() {
  Serial.begin(9600);
  while (!Serial);
  pinMode(espressoPin,OUTPUT);
  pinMode(buttonPin,INPUT);

  if (!BLE.begin()) {
    Serial.println("starting BLE failed!"); 
    while (1);
  }
  // set the local name peripheral advertises
  BLE.setLocalName("Rocket");
  // set the UUID for the service this peripheral advertises
  BLE.setAdvertisedService(esspressoService);
  // add the characteristic to the service
  esspressoService.addCharacteristic(switchCharacteristic);

  BLE.addService(esspressoService);

  BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);

  // assign event handlers for characteristic
  switchCharacteristic.setEventHandler(BLEWritten, switchCharacteristicWritten);
  // set an initial value for the characteristic
  switchCharacteristic.setValue(0);
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
      Serial.println(("Bluetooth device active, waiting for connections..."));
    }
      //  if timer is less than 5 ms check to see if espressoPin is high or low
      //  if it's low turn it on
    else if(!on)
    {
      on = true;
      digitalWrite(espressoPin,HIGH);
      Serial.println("ON");
      delay(500);
    }
      // if its high pull it low
    else if(on)
    {
      on = false;
      digitalWrite(espressoPin,LOW);
      Serial.println("OFF");
      delay(500);
    }
    // reset timer
    timer = 0;

  }
} 