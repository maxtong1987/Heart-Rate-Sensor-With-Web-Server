#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <string>
#include <vector>

namespace SSD1306Display {

enum class Code { OK, FAIL };

// Display settings
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 32  // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET -1  // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS \
  0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// 16x16px
#define LOGO_HEIGHT 16
#define LOGO_WIDTH 16
static const unsigned char PROGMEM heart_bmp[] = {
    0x00, 0x00, 0x3e, 0x7c, 0x7f, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0xfe, 0x7f, 0xfe, 0x3f, 0xfc,
    0x1f, 0xf8, 0x0f, 0xf0, 0x03, 0xc0, 0x01, 0x80, 0x00, 0x00};

Code setup() {
  // Initialize display
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    return Code::FAIL;
  }

  display.display();
  delay(2000);  // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  display.setTextSize(2);               // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);  // Draw white text
  display.setCursor(0, 0);              // Start at top-left corner
  display.cp437(true);  // Use full 256 char 'Code Page 437' font

  return Code::OK;
}

void showNoFinger() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("---");
  display.display();
}

void showHeartBeat(int heartBeat) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.drawBitmap(1, 0, heart_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.setCursor(20, 0);
  display.printf("%d", heartBeat);
  display.display();
}

};  // namespace SSD1306Display