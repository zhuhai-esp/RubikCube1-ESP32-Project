#ifndef __WT32_LV_H__
#define __WT32_LV_H__

#define LGFX_USE_V1

#include "lgfx.h"
#include <Arduino.h>
#include <lvgl.h>

LGFX tft;

#define PIN_SPEAKER 25

void disable_speaker() {
  pinMode(PIN_SPEAKER, OUTPUT);
  digitalWrite(PIN_SPEAKER, LOW);
}

#define LV_DISP_HOR_RES 320
#define LV_DISP_VER_RES 240

static const uint32_t buf_size = LV_DISP_HOR_RES * 10;
static lv_color_t dis_buf1[buf_size];
// static lv_color_t dis_buf2[buf_size];

void inline lv_touch_init() {
  auto *indev_touchpad = lv_indev_create();
  lv_indev_set_type(indev_touchpad, LV_INDEV_TYPE_POINTER);
  auto f_touch = [](lv_indev_t *indev_drv, lv_indev_data_t *data) {
    uint16_t touchX, touchY;
    data->state = LV_INDEV_STATE_RELEASED;
    if (tft.getTouch(&touchX, &touchY)) {
      data->state = LV_INDEV_STATE_PRESSED;
      data->point.x = touchX;
      data->point.y = touchY;
    }
  };
  lv_indev_set_read_cb(indev_touchpad, f_touch);
}

void inline lv_disp_init() {
  auto *disp = lv_display_create(LV_DISP_HOR_RES, LV_DISP_VER_RES);
  auto f_disp = [](lv_display_t *disp, const lv_area_t *area, uint8_t *px_map) {
    if (tft.getStartCount() == 0) {
      tft.endWrite();
    }
    tft.pushImageDMA(area->x1, area->y1, area->x2 - area->x1 + 1,
                     area->y2 - area->y1 + 1, (lgfx::rgb565_t *)px_map);
    lv_display_flush_ready(disp);
  };
  lv_display_set_flush_cb(disp, f_disp);
  auto mode = LV_DISPLAY_RENDER_MODE_PARTIAL;
  lv_display_set_buffers(disp, dis_buf1, nullptr, buf_size, mode);
}

inline void init_wt32_lvgl() {
  tft.init();
  tft.setBrightness(20);
  lv_init();
  lv_tick_set_cb((lv_tick_get_cb_t)millis);
  lv_disp_init();
  lv_touch_init();
}

inline void show_center_msg(const char *msg) {
  tft.drawCenterString(msg, screenWidth / 2, screenHeight / 2,
                       &fonts::AsciiFont8x16);
}

#endif