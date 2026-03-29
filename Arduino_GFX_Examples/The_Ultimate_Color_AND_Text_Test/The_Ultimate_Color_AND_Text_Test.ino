#include <Arduino_GFX_Library.h>

/* 1. Hardware Bus Setup (9-bit SPI) */
Arduino_DataBus *bus = new Arduino_ESP32SPI(-1 /* DC */, 5 /* CS */, 18 /* SCK */, 23 /* MOSI */);

/* 2. Display Driver Setup (240x320 Portrait) */
Arduino_GFX *gfx = new Arduino_ST7789(
  bus, 4 /* RST */, 0 /* Rotation */, false /* IPS */, 
  240 /* width */, 320 /* height */
);

void setup() {
  Serial.begin(115200);
  if (!gfx->begin(40000000)) Serial.println("GFX Init Failed!");

  // Color Correction (IMPORTANT: Change 0x00 to 0x08 if Red/Blue swap)
  bus->sendCommand(0x36); 
  bus->sendData(0x00); 
}

void loop() {
  // --- TEST 1: PURE COLOR FILL ---
  testColor(0xF800, "PURE RED");
  testColor(0x07E0, "PURE GREEN");
  testColor(0x001F, "PURE BLUE");
  testColor(0xFFFF, "PURE WHITE");
  testColor(0x0000, "PURE BLACK");

  // --- TEST 2: TEXT SCALE & ALIGNMENT ---
  gfx->fillScreen(0x0000);
  gfx->setCursor(0, 0);
  
  // Border for alignment check
  gfx->drawRect(0, 0, 240, 320, 0x07E0); // Green border

  gfx->setTextColor(0xFFFF);
  
  for(int i = 1; i <= 5; i++) {
    gfx->setTextSize(i);
    gfx->print("Size "); gfx->println(i);
  }

  gfx->setTextSize(2);
  gfx->setTextColor(0xF800); gfx->println("\nRED TEXT");
  gfx->setTextColor(0x07E0); gfx->println("GREEN TEXT");
  gfx->setTextColor(0x001F); gfx->println("BLUE TEXT");
  gfx->setTextColor(0xFFE0); gfx->println("YELLOW TEXT");
  gfx->setTextColor(0xF81F); gfx->println("MAGENTA TEXT");
  gfx->setTextColor(0x07FF); gfx->println("CYAN TEXT");

  // --- TEST 3: GRADIENT (Bit Depth Check) ---
  for (int i = 0; i < 240; i++) {
    uint16_t gray = gfx->color565(i, i, i);
    gfx->drawFastVLine(i, 280, 40, gray);
  }
  gfx->setTextSize(1);
  gfx->setCursor(10, 270);
  gfx->setTextColor(0xFFFF);
  gfx->print("16-BIT GRAYSCALE GRADIENT");

  delay(5000); // Wait 5 seconds before repeating
}

void testColor(uint16_t color, const char* name) {
  gfx->fillScreen(color);
  gfx->setCursor(20, 150);
  gfx->setTextSize(3);
  
  // Contrast text: Black on bright colors, White on dark colors
  if (color == 0xFFFF || color == 0x07E0 || color == 0xFFE0) {
    gfx->setTextColor(0x0000);
  } else {
    gfx->setTextColor(0xFFFF);
  }
  
  gfx->println(name);
  delay(1500);
}