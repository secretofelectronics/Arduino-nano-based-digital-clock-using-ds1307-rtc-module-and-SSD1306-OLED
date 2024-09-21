#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED display size
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Initialize the OLED display using I2C
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Initialize the RTC
RTC_DS1307 rtc;

void setup() {
  // Initialize Serial communication
  Serial.begin(9600);

  // Initialize the OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  
  // Initialize the RTC
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  
  // If RTC is not running, set the current time (only needed once)
  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Set RTC to compile time
  }

  // Clear display buffer
  display.clearDisplay();
}

void loop() {
  // Get the current time from the RTC
  DateTime now = rtc.now();
  
  // Draw background
  drawBackground();

  // Display time (HH:MM:SS)
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(20, 20);
  display.print(now.hour(), DEC);
  display.print(":");
  display.print(now.minute(), DEC);
  display.print(":");
  display.print(now.second(), DEC);

  // Display date (DD/MM/YYYY)
  display.setTextSize(1);
  display.setCursor(20, 45);
  display.print(now.day(), DEC);
  display.print("/");
  display.print(now.month(), DEC);
  display.print("/");
  display.print(now.year(), DEC);

  // Display an icon
  drawClockIcon(0, 0);

  // Show everything on the display
  display.display();
  
  // Wait 1 second before updating
  delay(1000);
}

void drawBackground() {
  // Fill the screen with a black background
  display.clearDisplay();
  display.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_BLACK);

  // Add a frame or a line
  display.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);  // Border
}

void drawClockIcon(int x, int y) {
  // Example: Draw a small clock icon at position (x, y)
  display.drawCircle(x + 8, y + 8, 8, SSD1306_WHITE);  // Outer circle
  display.drawLine(x + 8, y + 8, x + 8, y + 4, SSD1306_WHITE);  // Minute hand
  display.drawLine(x + 8, y + 8, x + 12, y + 8, SSD1306_WHITE); // Hour hand
}
