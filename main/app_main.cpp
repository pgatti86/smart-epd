#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_log.h"

#include "wifi_manager.h"
#include "time_manager.h"
#include "epd2in9.h"
#include "epdpaint.h"

static const char *TAG = "MAIN";

//TODO remove epd login in specific compoment/manager
unsigned char image[EPD_WIDTH*EPD_HEIGHT/8];
Paint paint(image, 0, 0);    // width should be the multiple of 8
Epd epd;


static void wifi_manager_callback(int event_id) {
  switch(event_id) {
  	  case SYSTEM_EVENT_STA_GOT_IP:
  		  time_manager_sync_time(true);
		  break;
  	  case SYSTEM_EVENT_STA_LOST_IP:
  	  case SYSTEM_EVENT_STA_DISCONNECTED:
  		  ESP_LOGI(TAG, "disconnecting from mqtt...");
  		  //TODO handle event
  		  break;
	}
}

void app_task(void *pvParameter) {

  ESP_LOGI(TAG, "init wifi_manager");
  //wifi_manager_sta_init();
  //wifi_manager_set_callback(wifi_manager_callback);
  //wifi_manager_connect(true);

  ESP_LOGI(TAG, "init time_manager");
  //time_manager_init();

  if (epd.Init(lut_full_update) != 0) {
	  ESP_LOGE(TAG, "e-Paper init failed");
      vTaskDelay(2000 / portTICK_RATE_MS);
      return;
  }

  ESP_LOGE(TAG, "e-Paper initialized");

  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();
  paint.SetRotate(ROTATE_90);
  paint.SetWidth(EPD_WIDTH);
  paint.SetHeight(EPD_HEIGHT);
  paint.Clear(UNCOLORED);

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

