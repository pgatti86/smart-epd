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

static const int SCREEN_WIDTH = EPD_HEIGHT;
static const int SCREEN_HEIGHT = EPD_WIDTH;

static int display_refresh_count = 0;

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

static void epd_manager_full_clear() {
  
  epd.Init(lut_full_update);
  epd_manager_clear(); 
  epd.Init(lut_partial_update);
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
  int top_line_height = 20;
  epd_manager_set_paint(SCREEN_WIDTH, SCREEN_HEIGHT, UNCOLORED);
  paint.DrawHorizontalLine(0, top_line_height, SCREEN_WIDTH, COLORED);
  paint.DrawVerticalLine(SCREEN_WIDTH - 132, 0, top_line_height, COLORED);
  epd_manager_draw_paint(0, 0);
}

static void epd_manager_draw_status_bar(bool is_connected) {
  const unsigned char *icon = is_connected ? WIFI_IMAGE_DATA : NO_WIFI_IMAGE_DATA;
  epd.SetFrameMemory(icon, SCREEN_HEIGHT - 16, 8, 16, 16);
}

static void epd_manager_draw_time(time_info_t *dst) {

  char time_buffer [20];
  time_formatter_format_current_time(dst, time_buffer);
    
  epd_manager_set_paint(SCREEN_WIDTH, 40, UNCOLORED);
  paint.DrawStringAt(0, 0, time_buffer, &Font40, COLORED);
  epd_manager_draw_paint(0, 32);
}

static void epd_manager_draw_date(time_info_t *dst) {
  
  char date_buffer [20];
  time_formatter_format_current_date(dst, date_buffer);

  int paint_width = 120;
  int paint_height = 16;
  epd_manager_set_paint(paint_width, paint_height, UNCOLORED);
  paint.DrawStringAt(0, 4, date_buffer, &Font16, COLORED);
  epd_manager_draw_paint(SCREEN_WIDTH - paint_width, 0);
}

static void epd_manager_draw_dht_data(float t, float h) {

  char temp_buffer [5];
  epd.SetFrameMemory(TEMP_IMAGE_DATA, 8, 10, 24, 24);
  sprintf(temp_buffer, "%.1fC", t); 
  epd_manager_set_paint(64, 24, UNCOLORED);
  paint.DrawStringAt(0, 0, temp_buffer, &Font16, COLORED);
  epd_manager_draw_paint(40, SCREEN_HEIGHT - paint.GetWidth());

  char hum_buffer [5];
  epd.SetFrameMemory(DROP_IMAGE_DATA, 8, 120, 24, 24);
  sprintf(hum_buffer, "%.1f%%", h); 
  epd_manager_set_paint(64, 24, UNCOLORED);
  paint.DrawStringAt(0, 0, hum_buffer, &Font16, COLORED);
  epd_manager_draw_paint(152, SCREEN_HEIGHT - paint.GetWidth());
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

void epd_manager_update(time_info_t *dst, float temperature, float humidity, bool is_connected) {

  if (display_refresh_count == 0) {
    epd_manager_full_clear();
  }

  epd_manager_draw_grid();
  epd_manager_draw_status_bar(is_connected);
  epd_manager_draw_date(dst);
  epd_manager_draw_time(dst);
  epd_manager_draw_dht_data(temperature, humidity);

  epd.DisplayFrame();

  display_refresh_count = display_refresh_count == CONFIG_MAX_REWRITE_COUNT ? 
      0 : display_refresh_count + 1;
}
