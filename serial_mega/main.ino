#include <SoftwareSerial.h>

SoftwareSerial gtSerial(7, 8); // Arduino RX, Arduino TX

void setup()
{
  Serial.begin(9600);    // serial / USB port
  //Serial3.begin(9600);  // software serial port
  gtSerial.begin(9600);
}

byte rx_byte = 0;        // stores received byte

void loop()
{
  // check if byte available from USB port
  if (Serial.available())
    {
      rx_byte = Serial.read();
      // send a byte to the software serial port
      //Serial.write(rx_byte);
      gtSerial.write(rx_byte);
    }
  
  // check if byte available on the software serial port
  if (gtSerial.available())
    {
      // get the byte from the software serial port
      rx_byte = gtSerial.read();
      Serial.write(rx_byte);
    }
  
}
