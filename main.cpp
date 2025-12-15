#include <Arduino.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include <SPI.h>

// HARDWARE DEFINITIONS
// Dual SPI Pinout
#define TOUCH_CLK  25
#define TOUCH_MOSI 32
#define TOUCH_MISO 39
#define TOUCH_CS   33
#define TOUCH_IRQ  36

// OBJECTS
TFT_eSPI tft = TFT_eSPI(); 

// Create a separate SPI bus instance for the Touch Controller
SPIClass touchSPI(HSPI); 
XPT2046_Touchscreen touch(TOUCH_CS, TOUCH_IRQ);

void setup() {
  Serial.begin(115200);
  
  // Start the Second SPI Bus for touch
  // Use HSPI because VSPI is likely used by the Screen
  touchSPI.begin(TOUCH_CLK, TOUCH_MISO, TOUCH_MOSI, TOUCH_CS);
  
  // Start Touch Controller using that specific bus
  if (!touch.begin(touchSPI)) {
    Serial.println("Touch Controller failed to start!");
  } else {
    Serial.println("Touch Controller Started.");
  }
  touch.setRotation(1); // Landscape

  // Start Screen
  tft.init();
  tft.setRotation(1); 
  tft.fillScreen(TFT_BLACK);
  
  // UI
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println("CYD: Dual SPI Mode");
  
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setCursor(10, 50);
  tft.println("Touch Works Now!");
}

void loop() {
  // Check touch
  if (touch.touched()) {
    TS_Point p = touch.getPoint();
    
    // CALIBRATION MAPPING
    // Using the raw XPT library, we map manually.
    // These values are typical for this screen.
    // X: Left=200, Right=3700
    // Y: Top=240, Bottom=3800
    int x = map(p.x, 200, 3700, 0, 320);
    int y = map(p.y, 240, 3800, 0, 240);
    
    // Constrain to keep dots inside screen
    if(x < 0) x = 0; if(x > 320) x = 320;
    if(y < 0) y = 0; if(y > 240) y = 240;

    // Draw
    tft.fillCircle(x, y, 2, TFT_CYAN);
    
    // Debug
    Serial.printf("Raw: %d,%d  -> Screen: %d,%d\n", p.x, p.y, x, y);
    
    // Update Text
    tft.fillRect(0, 100, 320, 30, TFT_BLACK);
    tft.setCursor(10, 100);
    tft.print("X: "); tft.print(x);
    tft.print(" Y: "); tft.println(y);
  }
}
