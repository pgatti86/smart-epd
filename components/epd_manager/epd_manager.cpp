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
#include "clock_data.h"
#include "strings.h"

#include "epd_manager.h"

static const char *TAG = "EPD MANAGER";

static const int SCREEN_WIDTH = EPD_HEIGHT;
static const int SCREEN_HEIGHT = EPD_WIDTH;

static int display_refresh_count = 0;

static ClockData clockData;

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
  int grid_height = 40;
  epd_manager_set_paint(SCREEN_WIDTH, SCREEN_HEIGHT, UNCOLORED);
  paint.DrawHorizontalLine(0, SCREEN_HEIGHT - grid_height, SCREEN_WIDTH, COLORED);
  paint.DrawVerticalLine(98, SCREEN_HEIGHT - grid_height, grid_height, COLORED);
  paint.DrawVerticalLine(196, SCREEN_HEIGHT - grid_height, grid_height, COLORED);
  epd_manager_draw_paint(0, 0);
}

static void epd_manager_draw_status_bar(bool is_connected) {

  if(!is_connected) {
    epd.SetFrameMemory(NO_WIFI_IMAGE_DATA, SCREEN_HEIGHT - 16, SCREEN_WIDTH - 24, 16, 16);
  } else {
      epd_manager_set_paint(16, 16, UNCOLORED);
      epd_manager_draw_paint(SCREEN_WIDTH - 24, 0);
  }
}

static void epd_manager_draw_date(time_info_t *dst) {
  
  char day_buffer [10];
  time_formatter_format_current_day(dst, day_buffer);

  char date_buffer[7];
  time_formatter_format_current_date(dst, date_buffer);

  int paint_width = 96;
  int paint_height = 40;
  epd_manager_set_paint(paint_width, paint_height, UNCOLORED);
  paint.DrawStringAt(8, 8, day_buffer, &Font12, COLORED);
  paint.DrawStringAt(8, 24, date_buffer, &Font16, COLORED);
  epd_manager_draw_paint(0, SCREEN_HEIGHT - paint_height);
}

static void epd_manager_draw_weather(enum weather_icons weather_icon, char *description) {
  
  const unsigned char *icon;

  switch (weather_icon) {
    case CLEAR_SKY_DAY:
      icon = WEATHER_CLEAR_D_IMAGE_DATA;
      break;
    case CLEAR_SKY_NIGHT:
      icon = WEATHER_CLEAR_N_IMAGE_DATA;
      break;
    case FEW_CLOUDS_DAY:
      icon = WEATHER_FEW_CLOUD_D_IMAGE_DATA;
      break;
    case FEW_CLOUDS_NIGHT:
      icon = WEATHER_FEW_CLOUDS_N_IMAGE_DATA;
      break;
    case CLOUDS:
      icon = WEATHER_CLOUDS_IMAGE_DATA;
      break;
    case SHOWER_RAIN:
      icon = WEATHER_SHOWER_IMAGE_DATA;
      break;
    case RAIN:
      icon = WEATHER_RAIN_IMAGE_DATA;
      break;
    case THUNDERSTORM:
      icon = WEATHER_THUNDERSTORM_IMAGE_DATA;
      break;
    case SNOW:
      icon = WEATHER_SNOW_IMAGE_DATA;
      break;
    case FOG:
      icon = WEATHER_FOG_IMAGE_DATA;
      break;
    default:
      icon = WEATHER_FEW_CLOUD_D_IMAGE_DATA;
      break;
  }

  epd.SetFrameMemory(icon, 72, 24, 48, 48);

  int description_length = strlen(description);
  int description_text_width = 96;
  int description_padding = (((description_text_width / 7) - description_length) / 2) * 7;
  epd_manager_set_paint(description_text_width, 16, UNCOLORED);
  paint.DrawStringAt(description_padding, 0, description, &Font12, COLORED);
  epd_manager_draw_paint(8, 64);
}

static void epd_manager_draw_time(time_info_t *dst) {

  char time_buffer [20];
  time_formatter_format_current_time(dst, time_buffer);
    
  epd_manager_set_paint(175, 40, UNCOLORED);
  paint.DrawStringAt(0, 0, time_buffer, &Font40, COLORED);
  epd_manager_draw_paint(104, 24);
}

static void epd_manager_draw_dht_data(float t, float h) {

  char temp_buffer [5];
  sprintf(temp_buffer, "%.1fC", t); 
  epd_manager_set_paint(64, 24, UNCOLORED);
  paint.DrawStringAt(0, 0, temp_buffer, &Font16, COLORED);
  epd_manager_draw_paint(132, SCREEN_HEIGHT - paint.GetWidth());

  char hum_buffer [5];
  sprintf(hum_buffer, "%.1f%%", h); 
  epd_manager_set_paint(64, 24, UNCOLORED);
  paint.DrawStringAt(0, 0, hum_buffer, &Font16, COLORED);
  epd_manager_draw_paint(232, SCREEN_HEIGHT - paint.GetWidth());
}

static void epd_manager_draw_static_images() {
  epd.SetFrameMemory(TEMP_IMAGE_DATA, 8, 104, 24, 24);
  epd.SetFrameMemory(DROP_IMAGE_DATA, 8, 204, 24, 24);
}

void epd_manager_init() {

  if (epd.Init(lut_partial_update) != 0) {
    ESP_LOGE(TAG, "e-Paper init failed");
    return;
  }

  ESP_LOGI(TAG, "e-Paper initialized");

  epd_manager_clear();
  paint.SetWidth(EPD_WIDTH);
  paint.SetHeight(EPD_HEIGHT);
  paint.SetRotate(ROTATE_90);
}

void epd_manager_update(time_info_t *dst, float temperature, float humidity, 
    bool is_connected, enum weather_icons weather_icon, char* weather_description) {

  if (display_refresh_count == 0) {
    epd_manager_full_clear();
  }

  bool force_update = display_refresh_count < 2;
  bool need_update = clockData.has_data_changed(dst, temperature, humidity, is_connected, weather_icon, weather_description);

  if (force_update) {
    epd_manager_draw_grid();
    epd_manager_draw_static_images();
    ESP_LOGD(TAG, "Drawing grid and static images");
  }

  if (force_update || (need_update && clockData.has_status_bar_changed(is_connected))) {
    ESP_LOGD(TAG, "Drawing status bar");
    epd_manager_draw_status_bar(is_connected);
  }

  if (force_update || (need_update && clockData.has_date_changed(dst))) {
    ESP_LOGD(TAG, "Drawing date");
    epd_manager_draw_date(dst);
  }
  
  if (force_update || (need_update && clockData.has_time_changed(dst))) {
    ESP_LOGD(TAG, "Drawing time");
    epd_manager_draw_time(dst); 
  }

  if (force_update || (need_update && clockData.has_dht_data_changed(temperature, humidity))) {
    ESP_LOGD(TAG, "Drawing dht");
    epd_manager_draw_dht_data(temperature, humidity);
  } 

  if (force_update || (need_update && clockData.has_weather_data_changed(weather_icon, weather_description))) {
    ESP_LOGI(TAG, "Drawing weather");
    epd_manager_draw_weather(weather_icon, weather_description);
  } 

  if (force_update || need_update) {
    epd.DisplayFrame();

    display_refresh_count = display_refresh_count == CONFIG_MAX_REWRITE_COUNT ? 
      0 : display_refresh_count + 1;
  }

  if (!force_update) {
    clockData.update_data(dst, temperature, humidity, is_connected, weather_icon, weather_description);   
  }
}

void epd_manager_show_enrollment(char *ssid, char *pwd, int code) {
    
    epd_manager_full_clear();

    epd_manager_set_paint(SCREEN_WIDTH, SCREEN_HEIGHT, UNCOLORED);
    
    paint.DrawStringAt(10, 10, "ENROLLMENT MODE:", &Font24, COLORED);
    
    char ssid_data[30];
    sprintf(ssid_data, "ssid: %s", ssid); 
    paint.DrawStringAt(10, 40, ssid_data, &Font16, COLORED);
    
    char pwd_data[30];
    sprintf(pwd_data, "pwd: %s", pwd);
    paint.DrawStringAt(10, 65, pwd_data, &Font16, COLORED);

    char pop_data[30];
    sprintf(pop_data, "code: %d", code);
    paint.DrawStringAt(10, 90, pop_data, &Font16, COLORED);

    epd_manager_draw_paint(0, 0);
    epd.DisplayFrame();
}

