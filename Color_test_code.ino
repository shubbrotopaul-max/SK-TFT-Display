#include <Arduino_GFX_Library.h>

// Standard 16-bit color definitions
#define BLACK   0x0000
#define WHITE   0xFFFF
#define RED     0xF800
#define GREEN   0x07E0
#define BLUE    0x001F

// Setup 9-bit Bus (No DC pin)
Arduino_DataBus *bus = new Arduino_ESP32SPI(-1 /* DC */, 5 /* CS */, 18 /* SCK */, 23 /* MOSI */);

// Driver for BXHL_D995 (ST7789)
Arduino_GFX *gfx = new Arduino_ST7789(bus, 4 /* RST */, 0 /* Rotation */, false /* IPS */);

void setup() {
  Serial.begin(115200);
  
  // Use fast speed (40MHz) for testing.
  if (!gfx->begin(40000000)) { 
    Serial.println("Display Init Failed!");
  }

  gfx->fillScreen(RED);   delay(1000);
  gfx->fillScreen(GREEN); delay(1000);
  gfx->fillScreen(BLUE);  delay(1000);
  gfx->fillScreen(BLACK);

  gfx->setCursor(10, 80);
  gfx->setTextColor(WHITE);
  gfx->setTextSize(2);
  gfx->println("9-BIT MODE OK");
}

void loop() {
  // CORRECTED: fillRect (not drawFillRect)
  gfx->fillRect(100, 100, 40, 40, GREEN);
  delay(500);
  gfx->fillRect(100, 100, 40, 40, BLACK);
  delay(500);
}