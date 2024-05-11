#include "lgfx.h"
#include <Arduino.h>

static LGFX lcd;
static LGFX_Sprite sprite(&lcd);

unsigned long lastMs = 0;
#define PIN_SPEAKER 25

void disable_speaker() {
  pinMode(PIN_SPEAKER, OUTPUT);
  digitalWrite(PIN_SPEAKER, LOW);
}

void setup() {
  disable_speaker();
  Serial.begin(115200);
  lcd.init();
  lcd.setBrightness(10);
  delay(1000);

  lcd.setBrightness(20);
  lcd.setColor(TFT_BLUE);
  lcd.fillCircle(100, 50, 30);

  sprite.setColorDepth(24);
  sprite.createSprite(65, 65);
  sprite.drawRect(0, 0, 65, 65, TFT_CYAN);
  sprite.drawCircle(30, 20, 20, TFT_GOLD);
  sprite.pushSprite(160, 80);
  sprite.deleteSprite();
}

void loop() {
  if (millis() - lastMs >= 1000) {
    lastMs = millis();
  }
}
