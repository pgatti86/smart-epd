/*
 * epd_manager.cpp
 *
 *  Created on: Sep 22, 2019
 *      Author: paolo
 */

#include "epd2in9.h"
#include "epdpaint.h"
#include "esp_log.h"
#include "epd_images.h"

#include "epd_manager.h"

static const char *TAG = "EPD MANAGER";

static const int TOP_LINE_Y = 30;
static const int BOTTOM_LINE_Y = EPD_WIDTH -30;

unsigned char image[EPD_WIDTH*EPD_HEIGHT/8];
Paint paint(image, 0, 0);    // width should be the multiple of 8
Epd epd;

static void epd_manager_clear() {

  ESP_LOGI(TAG, "Clearing display...");
  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();

  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();
}

static void epd_manager_apply() {

  epd.SetFrameMemory(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());
  epd.DisplayFrame();
}

static void epd_manager_draw_grid() {

  paint.DrawHorizontalLine(0, TOP_LINE_Y, EPD_HEIGHT, COLORED);

  paint.DrawHorizontalLine(0, BOTTOM_LINE_Y, EPD_HEIGHT, COLORED);
}

static void epd_manager_draw_time(char *time) {

  paint.DrawStringAt(0, 50, time, &Font40, COLORED);
}

static void epd_manager_draw_date(char *date) {

  int y = (TOP_LINE_Y - 16) / 2;
  paint.DrawStringAt(180, y, date, &Font16, COLORED);

  //epd.SetFrameMemory(calendar_image, 10, 10, 25, 25);
}

void epd_manager_init() {

  if (epd.Init(lut_partial_update) != 0) {
    ESP_LOGE(TAG, "e-Paper init failed");
    return;
  }

  ESP_LOGI(TAG, "e-Paper initialized");

  epd_manager_clear();

  paint.SetRotate(ROTATE_90);
  paint.SetWidth(EPD_WIDTH);
  paint.SetHeight(EPD_HEIGHT);
}

void epd_manager_update(char *time, char *date) {

  paint.Clear(UNCOLORED);
  
  epd_manager_draw_grid();
  epd_manager_draw_date(date);
  epd_manager_draw_time(time);
  
  epd_manager_apply();
}
