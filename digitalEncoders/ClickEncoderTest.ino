#include "ClickEncoder.h"
#include "TimerOne.h"

ClickEncoder *encoder;
int16_t last, value;

#define D8 8
#define D9 9
#define D2 2


void timerIsr()
{
  encoder->service();
}

void setup()
{
  Serial.begin(9600);
  encoder = new ClickEncoder(D8, D9, D2);

  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr); 
  
  last = -1;
}

void loop()
{  
  value += encoder->getValue();
  
  if (value != last)
    {
      last = value;
      Serial.print("Encoder Value: ");
      Serial.println(value);
    }
  
  ClickEncoder::Button b = encoder->getButton();
  if (b != ClickEncoder::Open)
    {
      Serial.print("Button: ");
#define VERBOSECASE(label) case label: Serial.println(#label); break;
      switch (b) {
	VERBOSECASE(ClickEncoder::Pressed);
	VERBOSECASE(ClickEncoder::Held);
	VERBOSECASE(ClickEncoder::Released);
	VERBOSECASE(ClickEncoder::Clicked);
      case ClickEncoder::DoubleClicked:
	Serial.println("ClickEncoder::DoubleClicked");
	encoder->setAccelerationEnabled(!encoder->getAccelerationEnabled());
	Serial.print("  Acceleration is ");
	Serial.println((encoder->getAccelerationEnabled()) ? "enabled" : "disabled");
        break;
      }
    }    
}

