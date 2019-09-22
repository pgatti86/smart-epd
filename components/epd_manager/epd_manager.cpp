/*
 * epd_manager.cpp
 *
 *  Created on: Sep 22, 2019
 *      Author: paolo
 */

#include "epd2in9.h"
#include "epdpaint.h"
#include "esp_log.h"

#include "epd_manager.h"

static const char *TAG = "EPD MANAGER";

unsigned char image[EPD_WIDTH*EPD_HEIGHT/8];
Paint paint(image, 0, 0);    // width should be the multiple of 8
Epd epd;

void epd_manager_init() {

  if (epd.Init(lut_partial_update) != 0) {
    ESP_LOGE(TAG, "e-Paper init failed");
    return;
  }

  ESP_LOGE(TAG, "e-Paper initialized");

  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();
  paint.SetRotate(ROTATE_90);
  paint.SetWidth(EPD_WIDTH);
  paint.SetHeight(EPD_HEIGHT);
  paint.Clear(UNCOLORED);
}

void epd_manager_write_on_lcd(char *str) {
  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 0, str, &Font20, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());
  epd.DisplayFrame();
}
