#ifndef __WT32_LV_H__
#define __WT32_LV_H__

#define LGFX_USE_V1

#include "lgfx.h"
#include <Arduino.h>
#include <lvgl.h>

// Create an instance of the prepared class.
LGFX tft;

static lv_disp_draw_buf_t draw_buf;
static lv_disp_drv_t disp_drv;

static const int buf_size = screenWidth * 10;
static lv_color_t disp_draw_buf[buf_size];
static lv_color_t disp_draw_buf2[buf_size];

#define PIN_SPEAKER 25

void disable_speaker() {
  pinMode(PIN_SPEAKER, OUTPUT);
  digitalWrite(PIN_SPEAKER, LOW);
}

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area,
                   lv_color_t *color_p) {
  if (tft.getStartCount() == 0) {
    tft.endWrite();
  }
  tft.pushImageDMA(area->x1, area->y1, area->x2 - area->x1 + 1,
                   area->y2 - area->y1 + 1, (lgfx::rgb565_t *)&color_p->full);
  lv_disp_flush_ready(disp); /* tell lvgl that flushing is done */
}

/*Read the touchpad*/
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
  uint16_t touchX, touchY;
  bool touched = tft.getTouch(&touchX, &touchY);
  if (!touched) {
    data->state = LV_INDEV_STATE_REL;
  } else {
    data->state = LV_INDEV_STATE_PR;
    /*Set the coordinates*/
    data->point.x = touchX;
    data->point.y = touchY;
  }
}

inline void init_wt32_lvgl() {
  tft.init();
  tft.initDMA();
  tft.startWrite();
  tft.setBrightness(20);

  lv_init();
  lv_disp_draw_buf_init(&draw_buf, disp_draw_buf, disp_draw_buf2, buf_size);
  /* Initialize the display */
  lv_disp_drv_init(&disp_drv);
  /* Change the following line to your display resolution */
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  /* Initialize the input device driver */
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);
}

inline void show_center_msg(const char *msg) {
  tft.drawCenterString(msg, screenWidth / 2, screenHeight / 2,
                       &fonts::AsciiFont8x16);
}

#endif