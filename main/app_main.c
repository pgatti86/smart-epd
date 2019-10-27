#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_log.h"

#include "wifi_manager.h"
#include "time_manager.h"
#include "epd_manager.h"
#include "dht_manager.h"
#include "gpio_manager.h"

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

  ESP_LOGI(TAG, "init gpio_manager");
  gpio_manger_init();

  ESP_LOGI(TAG, "init epd_manager");
  epd_manager_init();

  ESP_LOGI(TAG, "init dht_manager");
  dht_manager_startReading();

  ESP_LOGI(TAG, "init wifi_manager");
  wifi_manager_sta_init();
  wifi_manager_set_callback(wifi_manager_callback);
  wifi_manager_connect(true);

  ESP_LOGI(TAG, "init time_manager");
  time_manager_init();

  time_info_t dst;
  while(1) {
    
    time_manager_get_current_date_time(&dst);

    float t = dht_manager_getTemperature();
    float h = dht_manager_getHumidity();  

    bool is_connected = wifi_manager_is_sta_connected();
    epd_manager_update(&dst, t, h, is_connected);
    
    vTaskDelay(1000 / portTICK_RATE_MS);
  }
}

void app_main() {

  ESP_LOGI(TAG, "[APP] Startup..");
  ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
  ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());

  esp_log_level_set("*", ESP_LOG_INFO);

  ESP_ERROR_CHECK(nvs_flash_init());

  xTaskCreate(&app_task, "app_task", 4096, NULL, 5, NULL);
}

