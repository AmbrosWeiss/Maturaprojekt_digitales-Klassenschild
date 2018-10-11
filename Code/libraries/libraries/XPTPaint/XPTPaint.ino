#include <Arduino.h>
#include <SPI.h>

#include <Adafruit_ILI9341esp.h>
#include <Adafruit_GFX.h>
#include <XPT2046.h>

// Modify the following two lines to match your hardware
// Also, update calibration parameters below, as necessary

// For the esp shield, these are the default.
#define TFT_DC D4
#define TFT_CS D8
#define ESP_SPI_FREQ 4000000



Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
XPT2046 touch(/*cs=*/ D2, /*irq=*/ D0); // irq=0 ... kein IRQ

Adafruit_GFX_Button button;

void setup() {
  delay(1000);
  
  Serial.begin(115200);
  SPI.setFrequency(ESP_SPI_FREQ);

  tft.begin();
  touch.begin(tft.width(), tft.height());  // Must be done before setting rotation
  touch.setRotation(touch.ROT360);
  Serial.print("tftx ="); Serial.print(tft.width()); Serial.print(" tfty ="); Serial.println(tft.height());
  tft.fillScreen(ILI9341_BLACK);
  
  // Replace these for your screen module
  touch.setCalibration(300, 229, 2320, 1320 );
  button.initButton(&tft, 37, 22, 70, 40, ILI9341_DARKCYAN, ILI9341_BLUE, ILI9341_GREENYELLOW, "Clear", 2);
  button.drawButton();
  
}

static uint16_t prev_x = 0xffff, prev_y = 0xffff;

void loop() {
	uint16_t x, y;
  if (touch.isTouching()) {
    touch.getPosition(x, y);
//	Serial.print("x ="); Serial.print(x); Serial.print(" y ="); Serial.println(y);
    if (prev_x == 0xffff) {
      tft.drawPixel(x, y,ILI9341_BLUE);
    } else {
      tft.drawLine(prev_x, prev_y, x, y,ILI9341_BLUE);
    }
    prev_x = x;
    prev_y = y;
  } else {
    prev_x = prev_y = 0xffff;
  }
  
  
  button.press(button.contains(x, y)); // tell the button it is pressed
  

// now we can ask the buttons if their state has changed
	if (button.justReleased()) {
		tft.fillScreen(ILI9341_BLACK);
		button.drawButton(); // draw normal
	}

	if (button.justPressed()) {
		button.drawButton(true); // draw invert!
	}

  delay(20);
}
