/*
  AnalogReadSerial
  Reads an analog input on pin A0, prints the result to the serial monitor.
  This example code is in the public domain.
*/

// Redefine analog input
#define AN0 A0
#define AN1 A1

// Redefine digital input
#define BT0 8
#define BT1 9
#define BT2 10

// the setup routine runs once when you press reset:
void setup()
{
  // put the 3 digital input as INPUT_PULLUP
  // it allow to not use any resistor and use internal resistor
  // see : https://www.arduino.cc/en/Tutorial/InputPullupSerial
  pinMode(BT0, INPUT_PULLUP);
  pinMode(BT1, INPUT_PULLUP);
  pinMode(BT2, INPUT_PULLUP);
  // initialize serial communication at 9600 bits per second:  
  Serial.begin(9600);
}

int i=0; // use to know how many loop was done

// the loop routine runs over and over again forever:
void loop() {

  // read the input on analog pin 0:
  char buffer[64];
  int a0          = analogRead(AN0);
  int a1          = analogRead(AN1);
  int b0          = digitalRead(BT0);
  int b1          = digitalRead(BT1);
  int b2          = digitalRead(BT2);

  sprintf(buffer,"A0:%.4d A1:%.4d BT0:%d BT1:%d BT2:%d i:%d",a0,a1,b0,b1,b2,i);
  i++;
  Serial.println(buffer);
  delay(10);        // delay in between reads for stability
}
