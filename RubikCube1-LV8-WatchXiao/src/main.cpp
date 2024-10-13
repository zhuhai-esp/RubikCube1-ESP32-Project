#include "ui.h"
#include "wt32_lv.h"
#include "wt32_wifi.h"
#include <Arduino.h>

long check1s = 0;
long check300ms = 0;
long check100ms = 0;
long check5ms = 0;

static lv_obj_t *anchors[8];
const char *weekday_str[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

lv_obj_t *label_date;
lv_obj_t *label_ip;
lv_obj_t *label_time;

uint8_t archIdx = 0;

void ui_text_init() {
  auto *parent = lv_scr_act();

  label_date = lv_label_create(parent);
  lv_obj_align(label_date, LV_ALIGN_TOP_RIGHT, -4, 4);
  lv_label_set_text(label_date, "2000-01-01");

  label_ip = lv_label_create(parent);
  lv_obj_align(label_ip, LV_ALIGN_BOTTOM_RIGHT, -4, -4);
  lv_obj_set_style_text_font(label_ip, &lv_font_montserrat_12, LV_PART_MAIN);
  lv_label_set_text(label_ip, "IP: 0.0.0.0");

  label_time = lv_label_create(parent);
  lv_obj_align(label_time, LV_ALIGN_BOTTOM_LEFT, 4, -4);
  lv_label_set_text(label_time, "12:33:45");

  anchors[0] = ui_Image1;
  anchors[1] = ui_Image2;
  anchors[2] = ui_Image3;
  anchors[3] = ui_Image4;
  anchors[4] = ui_Image5;
  anchors[5] = ui_Image6;
  anchors[6] = ui_Image7;
  anchors[7] = ui_Image8;
  for (auto *ac : anchors) {
    lv_obj_add_flag(ac, LV_OBJ_FLAG_HIDDEN);
  }
}

inline void showClientIP() {
  sprintf(buf, "IP: %s", WiFi.localIP().toString().c_str());
  lv_label_set_text(label_ip, buf);
}

inline void showCurrentTime() {
  struct tm info;
  getLocalTime(&info);
  strftime(buf, 32, "%F", &info);
  lv_label_set_text(label_date, buf);
  strftime(buf, 32, "%T", &info);
  lv_label_set_text(label_time, buf);

  lv_img_set_angle(ui_ImageArmSecond, info.tm_sec * 6 * 10);
  lv_img_set_angle(ui_ImageArmMinute, info.tm_min * 6 * 10);
  lv_img_set_angle(ui_ImageArmHour,
                   info.tm_hour * 300 + info.tm_min / 12 % 12 * 60);

  sprintf(buf, "%s %d", weekday_str[info.tm_wday], info.tm_mday);
  lv_label_set_text(ui_LabelDate, buf);
}

void inline startConfigTime() {
  const int timeZone = 8 * 3600;
  configTime(timeZone, 0, "ntp6.aliyun.com", "cn.ntp.org.cn", "ntp.ntsc.ac.cn");
  while (time(nullptr) < 8 * 3600 * 2) {
    delay(500);
  }
}

void inline showNextArch() {
  lv_obj_add_flag(anchors[archIdx], LV_OBJ_FLAG_HIDDEN);
  archIdx++;
  if (archIdx >= 8) {
    archIdx = 0;
  }
  lv_obj_clear_flag(anchors[archIdx], LV_OBJ_FLAG_HIDDEN);
}

void setup() {
  disable_speaker();
  Serial.begin(115200);
  init_wt32_lvgl();
  ui_init();
  ui_text_init();
  autoConfigWifi();
  setupOTAConfig();
  startConfigTime();
  showClientIP();
}

void loop() {
  auto ms = millis();
  if (ms - check1s > 1000) {
    check1s = ms;
    ArduinoOTA.handle();
  }
  if (ms - check300ms > 300) {
    check300ms = ms;
    showCurrentTime();
  }
  if (ms - check100ms > 100) {
    check100ms = ms;
    showNextArch();
  }
  if (ms - check5ms > 5) {
    check5ms = ms;
    lv_timer_handler();
  }
}