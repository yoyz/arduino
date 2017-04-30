#include "ClickEncoder.h"
#include "TimerOne.h"

ClickEncoder *encoder1;
ClickEncoder *encoder2;
int16_t last1, value1;
int16_t last2, value2;

#define D8  8
#define D9  9

#define D10 10
#define D11 11

#define D2  2
#define D3  3

int a1,a2,a3;

void timerIsr()
{
  a1++;
  encoder1->service();
  a2++;
  encoder2->service();
  a3++;
}

void setup()
{
  a1=a2=a3=0;
  Serial.begin(9600);
  encoder1 = new ClickEncoder(D8,  D9,  D2);
  encoder2 = new ClickEncoder(D10, D11, D3);

  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr); 
  
  last1 = -1;
  last2 = -1;
}

void loop()
{
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

