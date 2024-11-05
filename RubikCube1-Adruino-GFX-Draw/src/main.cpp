#include <Arduino.h>
#include <Arduino_GFX_Library.h>

Arduino_DataBus *bus = new Arduino_HWSPI(27, 14, 18, 23, 19);
Arduino_GFX *tft = new Arduino_ILI9342(bus, 33, 0, true);

unsigned long lastMs = 0;
#define PIN_SPEAKER 25

#define TFT_BL 32
#define LCD_BL_PWM 5

void disable_speaker() {
  pinMode(PIN_SPEAKER, OUTPUT);
  digitalWrite(PIN_SPEAKER, LOW);
}

void setup() {
  disable_speaker();
  Serial.begin(115200);
  tft->begin();
  tft->setRotation(2);
  tft->fillScreen(RED);
  pinMode(TFT_BL, OUTPUT);
  analogWriteResolution(10);
  analogWriteFrequency(25000);
  analogWrite(TFT_BL, 1023 - (LCD_BL_PWM * 10));
  tft->setCursor(10, 10);
  tft->setTextColor(GREEN);
  tft->setTextSize(2);
  tft->println("Hello World!");
  tft->drawCircle(120, 120, 40, YELLOW);
  tft->drawRect(30, 30, 40, 50, BLUE);
  tft->drawRoundRect(0, 0, 320, 240, 10, WHITE);
}

void loop() {
  if (millis() - lastMs >= 1000) {
    lastMs = millis();
  }
}
