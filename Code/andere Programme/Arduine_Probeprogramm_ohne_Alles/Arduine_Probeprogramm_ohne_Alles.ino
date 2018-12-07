#include <SoftwareSerial.h>

const byte HC12RxdPin = 4;                  // Recieve Pin on HC12
const byte HC12TxdPin = 5;                  // Transmit Pin on HC12

SoftwareSerial HC12(HC12TxdPin,HC12RxdPin); // Create Software Serial Port

void setup() {
  Serial.begin(9600);                       // Open serial port to computer // Hardware Schnittstelle 
  HC12.begin(9600);                         // Open serial port to HC12     // Software Schnittstelle 
}

void loop() {
  if(HC12.available()){                     // Wenn beim Funkmodull Daten sind 
    Serial.write(HC12.read());           // Senden der Daten zum PC (software SChnittstelle zu Hardware SChnittstelle und dan zum PC)
    }
  if(Serial.available()){                   // Wenn Daten vom Computer am Arduino sind (Hardware Schnittstelle über Software Schnittstelle zu Funkmodull) 
    HC12.write(Serial.read());              // Die Software Schnittstelle schreibt und die Hardware Schnittstelle liest
  }
        
}
