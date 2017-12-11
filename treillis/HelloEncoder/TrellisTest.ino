#include <Wire.h>
#include "Adafruit_Trellis.h"
#include "ClickEncoder.h"
#include "TimerOne.h"

#define MOMENTARY 0
#define LATCHING 1
#define MODE LATCHING 
#define NUMTRELLIS 1
#define numKeys (NUMTRELLIS * 16)
#define INTPIN A2

#define D8  8
#define D9  9

#define D10 10
#define D11 11

#define D2  2
#define D3  3

int a1,a2,a3;
int16_t last1, value1;
int16_t last2, value2;
ClickEncoder *encoder1;
ClickEncoder *encoder2;


Adafruit_Trellis matrix0 = Adafruit_Trellis();
Adafruit_TrellisSet trellis =  Adafruit_TrellisSet(&matrix0);



void timerIsr()
{
  a1++;
  encoder1->service();
  a2++;
  encoder2->service();
  a3++;
}


void setup_encoder()
{
  a1=a2=a3=0;
  encoder1 = new ClickEncoder(D8,  D9,  D2);
  encoder2 = new ClickEncoder(D10, D11, D3);

  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr); 
  
  last1 = -1;
  last2 = -1;

}


void setup_treillis()
{
  // INT pin requires a pullup
  pinMode(INTPIN, INPUT);
  digitalWrite(INTPIN, HIGH);
  
  // begin() with the addresses of each panel in order
  // I find it easiest if the addresses are in order
  trellis.begin(0x70);  // only one
  // trellis.begin(0x70, 0x71, 0x72, 0x73);  // or four!

  // light up all the LEDs in order
  for (uint8_t i=0; i<numKeys; i++)
    {
      trellis.setLED(i);
      trellis.writeDisplay();    
      delay(50);
    }
  // then turn them off
  for (uint8_t i=0; i<numKeys; i++)
    {
      trellis.clrLED(i);
      trellis.writeDisplay();    
      delay(50);
    }
}


void setup() {
  Serial.begin(9600);
  Serial.println("Trellis Demo");
  setup_treillis();
  setup_encoder();
}


void loop() {
  delay(30); // 30ms delay is required, dont remove me!

  static char buffer[64];
  value1 += encoder1->getValue();
  value2 += encoder2->getValue();
  
  if (value1 != last1 ||
      value2 != last2)
    {
      last1 = value1;
      last2 = value2;
      sprintf(buffer,"DE1:%.4d DE2:%.4d",value1,value2);
      Serial.println(buffer);
    }

  
  if (MODE == MOMENTARY) {
    // If a button was just pressed or released...
    if (trellis.readSwitches()) {
      // go through every button
      for (uint8_t i=0; i<numKeys; i++) {
	// if it was pressed, turn it on
	if (trellis.justPressed(i)) {
	  Serial.print("v"); Serial.println(i);
	  trellis.setLED(i);
	} 
	// if it was released, turn it off
	if (trellis.justReleased(i)) {
	  Serial.print("^"); Serial.println(i);
	  trellis.clrLED(i);
	}
      }
      // tell the trellis to set the LEDs we requested
      trellis.writeDisplay();
    }
  }

  if (MODE == LATCHING) {
    // If a button was just pressed or released...
    if (trellis.readSwitches()) {
      // go through every button
      for (uint8_t i=0; i<numKeys; i++) {
        // if it was pressed...
	if (trellis.justPressed(i)) {
	  Serial.print("v"); Serial.println(i);
	  // Alternate the LED
	  if (trellis.isLED(i))
	    trellis.clrLED(i);
	  else
	    trellis.setLED(i);
        } 
      }
      // tell the trellis to set the LEDs we requested
      trellis.writeDisplay();
    }
  }


  
  ClickEncoder::Button b1 = encoder1->getButton();
  if (b1 != ClickEncoder::Open)
    {
      Serial.print("Button b1: ");
#define VERBOSECASE(label) case label: Serial.println(#label); break;
      switch (b1) {
	VERBOSECASE(ClickEncoder::Pressed);
	VERBOSECASE(ClickEncoder::Held);
	VERBOSECASE(ClickEncoder::Released);
	VERBOSECASE(ClickEncoder::Clicked);
      case ClickEncoder::DoubleClicked:
	Serial.println("ClickEncoder::DoubleClicked");
	encoder1->setAccelerationEnabled(!encoder1->getAccelerationEnabled());
	Serial.print("  Acceleration is ");
	Serial.println((encoder1->getAccelerationEnabled()) ? "enabled" : "disabled");
        break;
      }
    }    

  ClickEncoder::Button b2 = encoder2->getButton();
  if (b2 != ClickEncoder::Open)
    {
      Serial.print("Button b2: ");
#define VERBOSECASE(label) case label: Serial.println(#label); break;
      switch (b2) {
	VERBOSECASE(ClickEncoder::Pressed);
	VERBOSECASE(ClickEncoder::Held);
	VERBOSECASE(ClickEncoder::Released);
	VERBOSECASE(ClickEncoder::Clicked);
      case ClickEncoder::DoubleClicked:
	Serial.println("ClickEncoder::DoubleClicked");
	encoder2->setAccelerationEnabled(!encoder2->getAccelerationEnabled());
	Serial.print("  Acceleration is ");
	Serial.println((encoder2->getAccelerationEnabled()) ? "enabled" : "disabled");
        break;
      }
    }    

  
}

