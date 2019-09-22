#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_log.h"

#include "wifi_manager.h"
#include "time_manager.h"
#include "epd_manager.h"

static const char *TAG = "MAIN";

static void wifi_manager_callback(int event_id) {
  switch(event_id) {
  	  case SYSTEM_EVENT_STA_GOT_IP:
  		  time_manager_sync_time(true);
		  break;
  	  case SYSTEM_EVENT_STA_LOST_IP:
  	  case SYSTEM_EVENT_STA_DISCONNECTED:
  		  //TODO handle event
  		  break;
	}
}

void app_task(void *pvParameter) {

  ESP_LOGI(TAG, "init epd_manager");
  epd_manager_init();

  ESP_LOGI(TAG, "init wifi_manager");
  wifi_manager_sta_init();
  wifi_manager_set_callback(wifi_manager_callback);
  wifi_manager_connect(true);

  ESP_LOGI(TAG, "init time_manager");
  time_manager_init();

  while(1) {
	vTaskDelay(200 / portTICK_RATE_MS);
	char timestamp_buffer [20];
	time_manager_format_current_date(timestamp_buffer);
	epd_manager_write_on_lcd(timestamp_buffer);
  }
}

void app_main() {

  ESP_LOGI(TAG, "[APP] Startup..");
  ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
  ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());

  esp_log_level_set("*", ESP_LOG_INFO);

  ESP_ERROR_CHECK(nvs_flash_init());

  xTaskCreate(&app_task, "app_task", 2048, NULL, 5, NULL);
}

