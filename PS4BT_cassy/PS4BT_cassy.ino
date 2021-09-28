/*
 PS4-C64 - Connect a PS4 controller via bluetooth to your beloved Commodore C64
 Hardware: Mega2560 - USB-Host-Shield - supported Bluetooth-Dongle
 based on the example sketch for the PS4 Bluetooth library 
 
*/

#include <PS4BT.h>
#include <usbhub.h>

  bool up, down, left, right, fire;
  int potx, poty;
  int upPIN      = 44;  // IO-Pins of Mega2560 - directly connected to joystick port
  int downPIN    = 48;
  int leftPIN    = 42;
  int rightPIN   = 46;
  int firePIN    = 47;
  int potxPIN    = 45;
  int potyPIN    = 49;


// Satisfy the IDE, which needs to see the include statment in the ino too.
//#ifdef dobogusinclude
//#include <spi4teensy3.h>
//#endif
//#include <SPI.h>

USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside
BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so

/* You can create the instance of the PS4BT class in two ways */
// This will start an inquiry and then pair with the PS4 controller - you only have to do this once
// You will need to hold down the PS and Share button at the same time, the PS4 controller will then start to blink rapidly indicating that it is in pairing mode
//PS4BT PS4(&Btd, PAIR);

// After that you can simply create the instance like so and then press the PS button on the device
PS4BT PS4(&Btd);

bool printAngle, printTouch;
uint8_t oldL2Value, oldR2Value;

void setup() {

  up = false;
  down = false;
  right = false;
  left = false;
  fire = false;

  pinMode(upPIN, INPUT);     // virtual open collector  
  pinMode(downPIN, INPUT);      
  pinMode(leftPIN, INPUT);       
  pinMode(rightPIN, INPUT);     
  pinMode(firePIN, INPUT);      
  pinMode(potxPIN, INPUT);     
  pinMode(potyPIN, INPUT);    


  
  Serial.begin(115200);
// #if !defined(__MIPSEL__)
//  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
// #endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); // Halt
  }
  Serial.print(F("\r\nPS4 Bluetooth Library Started"));


}


void loop() {
  Usb.Task();

  if (PS4.connected()) {

    if (PS4.getAnalogButton(L2) || PS4.getAnalogButton(R2)) { // These are the only analog buttons on the PS4 controller
      Serial.print(F("\r\nL2: "));
      Serial.print(PS4.getAnalogButton(L2));
      Serial.print(F("\tR2: "));
      Serial.print(PS4.getAnalogButton(R2));
    }
    if (PS4.getAnalogButton(L2) != oldL2Value || PS4.getAnalogButton(R2) != oldR2Value) // Only write value if it's different
      PS4.setRumbleOn(PS4.getAnalogButton(L2), PS4.getAnalogButton(R2));
    oldL2Value = PS4.getAnalogButton(L2);
    oldR2Value = PS4.getAnalogButton(R2);

    if (PS4.getButtonClick(PS)) {
      Serial.print(F("\r\nPS"));
      PS4.disconnect();
    }
    else {

      up = false;
      down = false;
      right = false;
      left = false;
      fire = false;
 
      if ( PS4.getButtonPress(UP) || 
           PS4.getButtonPress(TRIANGLE) ||
           PS4.getAnalogHat(LeftHatY) < 60 ||
           PS4.getAnalogHat(RightHatY) < 60) { 
             up = true;  
      }
      if (PS4.getButtonPress(DOWN) || 
          PS4.getButtonPress(CROSS) ||
          PS4.getAnalogHat(LeftHatY) > 190 ||
          PS4.getAnalogHat(RightHatY) > 190) {  
            down = true; 
      }
      if (PS4.getButtonPress(LEFT) || 
          PS4.getButtonPress(SQUARE) || 
          PS4.getAnalogHat(LeftHatX) < 60 ||
          PS4.getAnalogHat(RightHatX) < 60) { 
            left = true; 
      }
      if (PS4.getButtonPress(RIGHT) || 
          PS4.getButtonPress(CIRCLE) || 
          PS4.getAnalogHat(LeftHatX) > 190 ||
          PS4.getAnalogHat(RightHatX) > 190) {  
             right = true; 
      }

      if (PS4.getButtonPress(L1) || 
          PS4.getButtonPress(L3) ||
          PS4.getButtonPress(R1) ||
          PS4.getButtonPress(R3)) { 
            fire = true; 
      }

      // after reading the buttons via bluetooth it's time to control the joystick port

      if ( up ) { 
          digitalWrite(upPIN, LOW);    
          pinMode(upPIN, OUTPUT);     // activate virtual open collector 
          PS4.setLed(Red);            // just for fun ;-)
        } else { 
          pinMode(upPIN, INPUT);     // release virtual open collector  
      }

      if ( down ) { 
          digitalWrite(downPIN, LOW);    
          pinMode(downPIN, OUTPUT);     // activate virtual open collector 
          PS4.setLed(Blue);            // just for fun ;-)
        } else { 
          pinMode(downPIN, INPUT);     // release virtual open collector  
      }

      if ( left ) { 
          digitalWrite(leftPIN, LOW);    
          pinMode(leftPIN, OUTPUT);     // activate virtual open collector 
          PS4.setLed(Green);            // just for fun ;-)
        } else { 
          pinMode(leftPIN, INPUT);     // release virtual open collector  
      }

      if ( right ) { 
          digitalWrite(rightPIN, LOW);    
          pinMode(rightPIN, OUTPUT);     // activate virtual open collector 
          PS4.setLed(Yellow);            // just for fun ;-)
        } else { 
          pinMode(rightPIN, INPUT);     // release virtual open collector  
      }

      if ( fire ) { 
          digitalWrite(firePIN, LOW);    
          pinMode(firePIN, OUTPUT);     // activate virtual open collector 
          PS4.setLed(Red);            // just for fun ;-)
        } else { 
          pinMode(firePIN, INPUT);     // release virtual open collector  
      }

      delayMicroseconds(250);
    }
  }
  delayMicroseconds(250);
}
