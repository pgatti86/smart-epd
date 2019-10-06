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
#include "time_formatter.h"

#include "epd_manager.h"

static const char *TAG = "EPD MANAGER";

static const int TOP_LINE_HEIGHT = 30;
static const int SCREEN_WIDTH = EPD_HEIGHT;
static const int SCREEN_HEIGHT = EPD_WIDTH;

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

static void epd_manager_set_paint(int width, int height, int clear_color) {
  
  paint.Clear(clear_color);
  paint.SetWidth(height);
  paint.SetHeight(width);
}

static void epd_manager_draw_paint(int x, int y) {
  
  int x_abs_coord = EPD_WIDTH - paint.GetWidth() - y;
  int y_abs_coord = x;
  epd.SetFrameMemory(paint.GetImage(), x_abs_coord , y_abs_coord, paint.GetWidth(), paint.GetHeight());
}

static void epd_manager_draw_grid() {

  epd_manager_set_paint(SCREEN_WIDTH, SCREEN_HEIGHT, UNCOLORED);
  paint.DrawHorizontalLine(0, TOP_LINE_HEIGHT, SCREEN_WIDTH, COLORED);
  paint.DrawVerticalLine(SCREEN_WIDTH - 132, 0, TOP_LINE_HEIGHT, COLORED);
  epd_manager_draw_paint(0, 0);
}

static void epd_manager_draw_time(time_info_t *dst) {

  char time_buffer [20];
  time_formatter_format_current_time(dst, time_buffer);
    
  epd_manager_set_paint(SCREEN_WIDTH, 40, UNCOLORED);
  paint.DrawStringAt(0, 0, time_buffer, &Font24, COLORED);
  epd_manager_draw_paint(0, SCREEN_HEIGHT / 2);
}

static void epd_manager_draw_date(time_info_t *dst) {
  
  char date_buffer [20];
  time_formatter_format_current_date(dst, date_buffer);

  int paint_width = 120;
  int paint_height = 24;
  epd_manager_set_paint(paint_width, paint_height, UNCOLORED);
  paint.DrawStringAt(0, 8, date_buffer, &Font16, COLORED);
  epd_manager_draw_paint(SCREEN_WIDTH - paint_width, 0);
}

void epd_manager_init() {

  if (epd.Init(lut_partial_update) != 0) {
    ESP_LOGE(TAG, "e-Paper init failed");
    return;
  }

  ESP_LOGI(TAG, "e-Paper initialized");

  epd_manager_clear();
  paint.Clear(UNCOLORED);
  paint.SetRotate(ROTATE_90);
}

void epd_manager_update(time_info_t *dst, float temperature, float humidity) {

  //epd_manager_draw_grid();
  //epd_manager_draw_date(date);
  epd_manager_draw_time(dst);
  
  epd.DisplayFrame();
}
