
#include <ArduinoBLE.h>

void blePeripheralConnectHandler(BLEDevice central);
void blePeripheralDisconnectHandler(BLEDevice central);
void switchCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic);

const int  buttonPin = 13;    // the pin that the pushbutton is attached to
const int espressoPin = 12;       // the pin that the LED is attached to
bool on; // boolean to keep track of on/off
int timer = 0; //counts how long the button is pushed
BLEService esspressoService("fe8e9fb9-e229-4df3-975d-8b92b7a31c9c");
BLEUnsignedIntCharacteristic switchCharacteristic("fe8e9fb9-e229-4df3-975d-8b92b7a31c9c", BLERead | BLEWrite);


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

  if (switchCharacteristic.value()) {
    Serial.println("LED on");
    digitalWrite(espressoPin, HIGH);
   } else {
    Serial.println("LED off");
    digitalWrite(espressoPin, LOW);
  }
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
  BLE.advertise();
}


void loop() {
  BLE.poll();
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
      Serial.println("Bluetooth device active, waiting for connections...");
    }
      //  if timer is less than 5s check to see if espressoPin is high or low
      //  if it's low turn it on
    else if(!on)
    {
      on = true;
      digitalWrite(espressoPin,HIGH);
      Serial.println("ON");
      delay(500);
    }
      // if its high pull it low
    else
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

/*
  Callback LED
  This example creates a BLE peripheral with service that contains a
  characteristic to control an LED. The callback features of the
  library are used.
  The circuit:
  - Arduino MKR WiFi 1010, Arduino Uno WiFi Rev2 board, Arduino Nano 33 IoT,
    Arduino Nano 33 BLE, or Arduino Nano 33 BLE Sense board.
  You can use a generic BLE central app, like LightBlue (iOS and Android) or
  nRF Connect (Android), to interact with the services and characteristics
  created in this sketch.
  This example code is in the public domain.
*/
 
// BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214"); // create service
// // create switch characteristic and allow remote device to read and write
// BLEUnsignedIntCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
// const int ledPin = LED_BUILTIN; // pin to use for the LED


// void setup() {
//   Serial.begin(9600);
//   while (!Serial);
//   pinMode(ledPin, OUTPUT); // use the LED pin as an output
//   //start BLE
//   if (!BLE.begin()) {
//     Serial.println("starting BLE failed!");

//     while (1);
//   }
//   // set the local name peripheral advertises
//   BLE.setLocalName("LEDCallback");
//   // set the UUID for the service this peripheral advertises
//   BLE.setAdvertisedService(ledService);
//   // add the characteristic to the service
//   ledService.addCharacteristic(switchCharacteristic);
//   // add service
//   BLE.addService(ledService);
//   // assign event handlers for connected, disconnected to peripheral
//   BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
//   BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);
//   // assign event handlers for characteristic
//   switchCharacteristic.setEventHandler(BLEWritten, switchCharacteristicWritten);
//   // set an initial value for the characteristic
//   switchCharacteristic.setValue(0);
//   // start advertising
//   BLE.advertise();

//   Serial.println("Bluetooth device active, waiting for connections...");
// }

// void loop() {
//   // poll for BLE events
//   while(BLE.connected()){
//   BLE.poll();
//   }
// }


// void blePeripheralConnectHandler(BLEDevice central) {
//   // central connected event handler
//   Serial.print("Connected event, central: ");
//   Serial.println(central.address());
//   // //needs wi
//   // Serial.println(WiFi.status());
//   // Serial.println(WiFi.scanNetworks() + " Networks Found");
// }

// void blePeripheralDisconnectHandler(BLEDevice central) {
//   // central disconnected event handler
//   Serial.print("Disconnected event, central: ");
//   Serial.println(central.address());
// }

// void switchCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {
//   // central wrote new value to characteristic, update LED
//   Serial.print("Characteristic event, written: ");

//   if (switchCharacteristic.value()) {
//     Serial.println("LED on");
//     digitalWrite(ledPin, HIGH);
//   } else {
//     Serial.println("LED off");
//     digitalWrite(ledPin, LOW);
//   }
// }