#include "epd2in9.h"
#include "epdpaint.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"

#include "wifi_manager.h"
#include "time_manager.h"

static const char * TAG = "SMART-EPD";

//TODO remove epd login in specific compoment/manager
unsigned char image[EPD_WIDTH*EPD_HEIGHT/8];
Paint paint(image, 0, 0);    // width should be the multiple of 8
Epd epd;


extern "C" void app_task(void *pvParameter) {

  if (epd.Init(lut_partial_update) != 0) {
      ESP_LOGE(TAG, "e-Paper init failed");
      vTaskDelay(2000 / portTICK_RATE_MS);
      return;
  }
  ESP_LOGE(TAG, "e-Paper initialized");

  epd.ClearFrameMemory(0xFF);
  epd.DisplayFrame();
  epd.ClearFrameMemory(0xFF);
  epd.DisplayFrame();

  paint.SetRotate(ROTATE_90);
  paint.SetWidth(EPD_WIDTH);
  paint.SetHeight(EPD_HEIGHT);

  paint.Clear(UNCOLORED);

  paint.DrawStringAt(0, 0, "e-Paper Demo F24", &Font24, COLORED);

  paint.DrawStringAt(0, 28, "e-Paper Demo F20", &Font20, COLORED);

  paint.DrawStringAt(0, 50, "e-Paper Demo F16", &Font16, COLORED);

  paint.DrawStringAt(0, 70, "e-Paper Demo F12", &Font12, COLORED);

  paint.DrawStringAt(0, 90, "e-Paper Demo F8", &Font8, COLORED);

  epd.SetFrameMemory(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());

  epd.DisplayFrame();

  while(1) {
	vTaskDelay(1000 / portTICK_RATE_MS);
  }
}

extern "C" void app_main() {

    ESP_LOGI(TAG, "[APP] Startup..");
    ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());

    esp_log_level_set("*", ESP_LOG_INFO);

    ESP_ERROR_CHECK(nvs_flash_init());

    xTaskCreate(&app_task, "app_task", 2048, NULL, 5, NULL);
}

