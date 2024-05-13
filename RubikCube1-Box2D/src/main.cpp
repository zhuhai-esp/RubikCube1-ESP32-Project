#include "MPU6886.h"
#include "TFTDebugDraw.h"
#include <Arduino.h>
#include <SPI.h>
#include <box2d/box2d.h>

LGFX lcd;
LGFX_Sprite sprite(&lcd);

b2World *myWorld = NULL;
unsigned long lastMs = 0;
I2C_MPU6886 imu;
#define PIN_SPEAKER 25

void disable_speaker() {
  pinMode(PIN_SPEAKER, OUTPUT);
  digitalWrite(PIN_SPEAKER, LOW);
}

void createSomeBall() {
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.allowSleep = true;
  auto r = (random() % 100) / 100.01;
  bodyDef.position.Set(16 + r, 4);
  auto *body = myWorld->CreateBody(&bodyDef);

  b2CircleShape shape;
  shape.m_radius = 0.7 + r;

  b2FixtureDef f;
  f.shape = &shape;
  f.density = 0.5;
  f.friction = 0.4;
  f.restitution = 0.5;

  body->CreateFixture(&f);
  body->ApplyForce(b2Vec2(10 + r, 6 + r), b2Vec2(0, 0), true);
}

void createSomeBox() {
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.allowSleep = true;
  auto w = (random() % 100) / 100.01;
  bodyDef.position.Set(17 + w, 3 + w);
  auto *body = myWorld->CreateBody(&bodyDef);

  b2PolygonShape shape;
  shape.SetAsBox(0.6 + w, 0.6 + w);

  b2FixtureDef f;
  f.shape = &shape;
  f.density = 0.8;
  f.friction = 0.4;
  f.restitution = 0.6;

  body->CreateFixture(&f);
}

void createSomeWorld() {
  b2Vec2 gravity(0, 10);
  myWorld = new b2World(gravity);

  b2Draw *draw = new TFTDebugDraw();
  draw->SetFlags(1);
  myWorld->SetAllowSleeping(true);
  myWorld->SetDebugDraw(draw);

  b2BodyDef groundBodyDef;
  b2Body *groundBody = myWorld->CreateBody(&groundBodyDef);
  b2PolygonShape groundBox;

  groundBox.SetAsBox(16, .05, b2Vec2(16, 0), 0);
  groundBody->CreateFixture(&groundBox, 0.0f);

  groundBox.SetAsBox(16, .05, b2Vec2(16, 24), 0);
  groundBody->CreateFixture(&groundBox, 0.0f);

  groundBox.SetAsBox(.05, 12, b2Vec2(0, 12), 0);
  groundBody->CreateFixture(&groundBox, 0.0f);

  groundBox.SetAsBox(.05, 12, b2Vec2(32, 12), 0);
  groundBody->CreateFixture(&groundBox, 0.0f);

  for (int i = 0; i < 16; i++) {
    createSomeBox();
  }
  for (int i = 0; i < 24; i++) {
    createSomeBall();
  }
}

void setup() {
  disable_speaker();
  Serial.begin(115200);
  lcd.init();
  lcd.setBrightness(20);
  createSomeWorld();
  delay(200);
  Wire.begin(21, 22, 2000);
  imu.begin();
  sprite.setColorDepth(8);
  sprite.createSprite(320, 240);
}

void loop() {
  myWorld->Step(0.1, 6, 2);
  sprite.clear();
  myWorld->DebugDraw();
  sprite.pushSprite(0, 0);
  if (millis() - lastMs >= 1000) {
    lastMs = millis();
    float gx, gy, gz;
    imu.getAccel(&gx, &gy, &gz);
    myWorld->SetGravity(b2Vec2(-gy * 10, -gx * 10));
  }
}
