#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

/*    Arduino Long Range Wireless Communication using HC-12
                      Example 01
   by Dejan Nedelkovski, www.HowToMechatronics.com
*/

#include <SoftwareSerial.h>
//#include <Adafruit_SSD1306.h> // Vorher hinzugefügte Bibliothek hochladen

#define OLED_RESET 0
Adafruit_SSD1306 display(OLED_RESET);

const byte HC12RxdPin = 4;                  // Recieve Pin on HC12
const byte HC12TxdPin = 5;                  // Transmit Pin on HC12

SoftwareSerial HC12(HC12TxdPin,HC12RxdPin); // Create Software Serial Port


void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //Im Setup wird der LCD gestartet
  display.clearDisplay();
  display.display();

  HC12.print("AT+RX");

 /* Serial.begin(9600);                       // Open serial port to computer //Hardware Schnittstelle
  HC12.begin(9600);                         // Open serial port to HC12     //Software Schnittstelle
*/
}

void loop() {
  // ==== Storing the incoming data into a String variable
  display.setTextSize(1);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(0,0);
  display.println("Hello there!");
  display.display();

    if(HC12.available()){                     // If Arduino's HC12 rx buffer has data
    Serial.write(HC12.read());              // Send the data to the computer
    }
  if(Serial.available()){                   // If Arduino's computer rx buffer has data
    HC12.write(Serial.read());              // Send that data to serial
  }

  


}
