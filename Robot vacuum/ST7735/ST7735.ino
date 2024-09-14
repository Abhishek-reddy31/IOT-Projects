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
  Serial.begin(9600);

  // Initialize TFT display
  tft.initR(INITR_BLACKTAB);  // Use this initializer if your display has a black tab
  pinMode(TFT_LED, OUTPUT);
  analogWrite(TFT_LED, 200); // Adjust the brightness (0-255)

  // Set landscape mode
  tft.setRotation(3);  // Adjust the rotation value as needed

  // Fill the screen with a solid color
  tft.fillScreen(ST7735_BLACK);

  // Display the first text immediately
  String firstText = "Hello, NodeMCU! This is a test.";

  if (firstText.length() > 62) {
    // Set properties for font size 1
    tft.setTextSize(1);
  } else {
    // Set properties for font size 2
    tft.setTextSize(2);
  }

  tft.setTextColor(ST7735_WHITE);
  tft.setCursor(10, 10);
  tft.print(firstText);

  // Wait for 5 seconds
  delay(5000);

  // Clear the screen
  tft.fillScreen(ST7735_BLACK);

  // Display the second text after 5 seconds with more than 62 characters
  String secondText = "Second text displayed after 5 seconds. This string has more than 62 characters.";

  if (secondText.length() > 62) {
    // Set properties for font size 1
    tft.setTextSize(1);
  } else {
    // Set properties for font size 2
    tft.setTextSize(2);
  }

  tft.setCursor(10, 10);
  tft.print(secondText);
}

void loop() {
  // Leave the loop empty
}
