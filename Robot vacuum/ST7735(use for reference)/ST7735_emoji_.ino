#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

// Pin configuration for NodeMCU and ST7735
#define TFT_CS    D2  // GPIO 4
#define TFT_RST   D3  // GPIO 0
#define TFT_DC    D1  // GPIO 5
#define TFT_SDA   D7  // GPIO 13
#define TFT_SCK   D5  // GPIO 14
#define TFT_LED   D0  // GPIO 16 (D0 is used as LED control)

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  Serial.begin(115200);

  // Initialize TFT display
  tft.initR(INITR_BLACKTAB);  // Use this initializer if your display has a black tab
  pinMode(TFT_LED, OUTPUT);
  analogWrite(TFT_LED, 200); // Adjust the brightness (0-255)

  // Set landscape mode
  tft.setRotation(3);  // Adjust the rotation value as needed

  // Fill the screen with a solid color
  tft.fillScreen(ST7735_BLACK);

  // Set text properties
  tft.setTextSize(1);
  tft.setTextColor(ST7735_WHITE);
  tft.setCursor(10, 10);

  // Display a simple text message
  String displayText = "Hello, NodeMCU! This is a test.";

  tft.print(displayText);
}

void loop() {
  // Leave the loop empty
}
