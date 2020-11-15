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
#include "clock_data_buffer.h"
#include "strings.h"

#include "epd_manager.h"

static const char *TAG = "EPD MANAGER";

static const int SCREEN_WIDTH = EPD_HEIGHT;
static const int SCREEN_HEIGHT = EPD_WIDTH;

static int display_refresh_count = 0;

static ClockDataBuffer buffer1, buffer2;
static ClockDataBuffer *currentBuffer = &buffer1;

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

  if (!is_connected) {
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

  epd.SetFrameMemory(icon, 56, 32, 48, 48);
}

static void epd_manager_draw_time(time_info_t *dst) {

  char time_buffer [20];
  time_formatter_format_current_time(dst, time_buffer);
  epd_manager_set_paint(175, 40, UNCOLORED);
  paint.DrawStringAt(0, 0, time_buffer, &Font40, COLORED);
  epd_manager_draw_paint(104, 24);
}

static void epd_manager_draw_timeline(time_info_t *dst) {

  int timeline_height = 8;
  int timeline_width = (SCREEN_WIDTH / 4) * ((dst->tm_sec / 15) + 1);
  epd_manager_set_paint(SCREEN_WIDTH, timeline_height, UNCOLORED);
  paint.DrawHorizontalLine(timeline_width, timeline_height / 2, SCREEN_WIDTH, COLORED);
  paint.DrawFilledRectangle(0, 0, timeline_width, timeline_height / 2, COLORED);
  epd_manager_draw_paint(0, 0);
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
  bool update_timeline = (dst->tm_sec % 15) == 0;
  bool need_update = currentBuffer->is_connected != is_connected 
                      || currentBuffer->day != dst->tm_yday
                      || currentBuffer->minutes != dst->tm_min
                      || update_timeline
                      || (currentBuffer->temperature != temperature || currentBuffer->humidity != humidity)
                      || (currentBuffer->weather_icon != weather_icon);
  
  ClockDataBuffer *previousBuffer = currentBuffer == &buffer1 ? &buffer2 : &buffer1;
  
  if (force_update) {
    ESP_LOGI(TAG, "Drawing grid and static images");
    epd_manager_draw_grid();
    epd_manager_draw_static_images();
  }

  if (force_update || (need_update && previousBuffer->is_connected != is_connected)) {
    ESP_LOGI(TAG, "Drawing status bar");
    epd_manager_draw_status_bar(is_connected);
  }

  if (force_update || (need_update && previousBuffer->weather_icon != weather_icon)) {
    ESP_LOGI(TAG, "Drawing weather");
    epd_manager_draw_weather(weather_icon, weather_description);
  } 

  if (force_update || (need_update && previousBuffer->day != dst->tm_yday)) {
    ESP_LOGI(TAG, "Drawing date");
    epd_manager_draw_date(dst);
  }

  if (force_update || (need_update && previousBuffer->minutes != dst->tm_min)) {
    ESP_LOGI(TAG, "Drawing time");
    epd_manager_draw_time(dst); 
  }

  if (force_update || need_update) {
    ESP_LOGI(TAG, "Drawing timeline");
    epd_manager_draw_timeline(dst);
  }
  
  if (force_update || (need_update && (previousBuffer->temperature != temperature || previousBuffer->humidity != humidity))) {
    ESP_LOGI(TAG, "Drawing dht");
    epd_manager_draw_dht_data(temperature, humidity);
  } 

  if (force_update || need_update) {
    epd.DisplayFrame();

    display_refresh_count = display_refresh_count == CONFIG_MAX_REWRITE_COUNT ? 
      0 : display_refresh_count + 1;

    
    previousBuffer->is_connected = is_connected;
    previousBuffer->day = dst->tm_yday;
    previousBuffer->minutes = dst->tm_min;
    previousBuffer->temperature = temperature;
    previousBuffer->humidity = humidity;
    strcpy(previousBuffer->weather_description, weather_description);
    previousBuffer->weather_icon = weather_icon;
    
    currentBuffer = previousBuffer;  
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

