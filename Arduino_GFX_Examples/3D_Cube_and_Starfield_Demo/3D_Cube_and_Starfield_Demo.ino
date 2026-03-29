#include <Arduino_GFX_Library.h>

// Colors
#define BLACK   0x0000
#define WHITE   0xFFFF
#define GREEN   0x07E0

// SPI bus
Arduino_DataBus *bus = new Arduino_ESP32SPI(-1, 5, 18, 23);

// ✅ IMPORTANT: Set width & height manually
Arduino_GFX *gfx = new Arduino_ST7789(
  bus,
  4,        // RST
  0,        // rotation
  false,    // IPS
  240,      // width
  320       // height
);

// ⭐ Starfield
const int numStars = 70;
float starX[numStars], starY[numStars], starZ[numStars];

// 🧊 Cube
float angle = 0;

struct Point3D { float x, y, z; };

Point3D cube[8] = {
  {-30,-30,30}, {30,-30,30}, {30,30,30}, {-30,30,30},
  {-30,-30,-30}, {30,-30,-30}, {30,30,-30}, {-30,30,-30}
};

Point3D proj[8], prevProj[8];

// ✅ Center for 240x320
int cx = 120;
int cy = 160;

// ===================== SETUP =====================
void setup() {
  Serial.begin(115200);

  if (!gfx->begin(40000000)) {
    Serial.println("Init Failed!");
  }

  gfx->fillScreen(BLACK);

  // ⭐ Init stars
  for(int i=0;i<numStars;i++) {
    starX[i] = random(-120,120);
    starY[i] = random(-160,160);
    starZ[i] = random(1,255);
  }
}

// ===================== STARFIELD =====================
void drawStars() {
  for(int i=0;i<numStars;i++) {

    // erase old
    int oldX = (starX[i] * 128 / starZ[i]) + cx;
    int oldY = (starY[i] * 128 / starZ[i]) + cy;

    if(oldX>=0 && oldX<240 && oldY>=0 && oldY<320)
      gfx->drawPixel(oldX, oldY, BLACK);

    // move forward
    starZ[i] -= 3;

    if(starZ[i] <= 1) {
      starX[i] = random(-120,120);
      starY[i] = random(-160,160);
      starZ[i] = 255;
    }

    // draw new
    int x = (starX[i] * 128 / starZ[i]) + cx;
    int y = (starY[i] * 128 / starZ[i]) + cy;

    if(x>=0 && x<240 && y>=0 && y<320) {
      uint16_t c = (starZ[i] < 80) ? WHITE :
                   (starZ[i] < 160) ? 0xAAAA : 0x5555;

      gfx->drawPixel(x, y, c);
    }
  }
}

// ===================== CUBE =====================
void projectCube() {
  for(int i=0;i<8;i++) {

    float x = cube[i].x;
    float y = cube[i].y;
    float z = cube[i].z;

    // Rotate Y
    float rx = x * cos(angle) - z * sin(angle);
    float rz = x * sin(angle) + z * cos(angle);

    // Rotate X
    float ry = y * cos(angle) - rz * sin(angle);
    rz = y * sin(angle) + rz * cos(angle);

    // Perspective
    float scale = 240 / (rz + 120);

    proj[i].x = rx * scale + cx;
    proj[i].y = ry * scale + cy;
  }
}

void drawCube(uint16_t color, Point3D *p) {
  for(int i=0;i<4;i++) {
    gfx->drawLine(p[i].x, p[i].y, p[(i+1)%4].x, p[(i+1)%4].y, color);
    gfx->drawLine(p[i+4].x, p[i+4].y, p[((i+1)%4)+4].x, p[((i+1)%4)+4].y, color);
    gfx->drawLine(p[i].x, p[i].y, p[i+4].x, p[i+4].y, color);
  }
}

// ===================== LOOP =====================
void loop() {

  // ⭐ Draw starfield
  drawStars();

  // ❌ erase previous cube
  drawCube(BLACK, prevProj);

  // 🔄 rotate
  angle += 0.04;

  // 🧠 project
  projectCube();

  // ✅ draw cube
  drawCube(GREEN, proj);

  // save for next frame
  memcpy(prevProj, proj, sizeof(proj));

  // ⚡ high FPS
  delay(1);
}