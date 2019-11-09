#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_log.h"

#include "wifi_manager.h"
#include "enrollment_manager.h"
#include "time_manager.h"
#include "epd_manager.h"
#include "dht_manager.h"
#include "gpio_manager.h"
#include "storage_manager.h"

static const char *TAG = "MAIN";

void gpio_event_handler(void *handler_arg, esp_event_base_t base, int32_t id, void *event_data)
{
  if (id == GPIO_CLICK_EVENT)
  {
    printf("this is a click\n\r");
  }
  else
  {
    printf("this is a reset\n\r");
  }
}

static void wifi_manager_callback(int event_id)
{
  switch (event_id)
  {
  case SYSTEM_EVENT_STA_GOT_IP:
    time_manager_sync_time(true);
    break;
  case SYSTEM_EVENT_STA_LOST_IP:
  case SYSTEM_EVENT_STA_DISCONNECTED:
    //TODO handle event
    break;
  }
}

static void enrollment_manager_callback(int event_id)
{
  switch (event_id)
  {
  case SYSTEM_EVENT_AP_STACONNECTED:
	ESP_LOGI(TAG, "SYSTEM_EVENT_AP_STACONNECTED...");
    //TODO: Change value here
    esp_restart();
    break;
  case SYSTEM_EVENT_AP_START:
  case SYSTEM_EVENT_AP_STADISCONNECTED:
    //TODO handle event
    break;
  }
}

void app_task(void *pvParameter)
{

  esp_event_loop_create_default();
  esp_event_handler_register(GPIO_EVENTS, ESP_EVENT_ANY_ID, gpio_event_handler, NULL);

  ESP_LOGI(TAG, "init gpio_manager");
  gpio_manger_init();

  ESP_LOGI(TAG, "init epd_manager");
  epd_manager_init();

  ESP_LOGI(TAG, "init dht_manager");
  dht_manager_startReading();

  if (true) {
    ESP_LOGI(TAG, "init enrollment_manager");
    enrollment_manager_start(enrollment_manager_callback);
  }
  else {
    ESP_LOGI(TAG, "init wifi_manager");
    wifi_manager_sta_init();
    wifi_manager_set_callback(wifi_manager_callback);
    wifi_manager_connect(true);
  }

  ESP_LOGI(TAG, "init time_manager");
  time_manager_init();

  time_info_t dst;

  while (1)
  {

    time_manager_get_current_date_time(&dst);

    float t = dht_manager_getTemperature();
    float h = dht_manager_getHumidity();

    bool is_connected = wifi_manager_is_sta_connected();

    epd_manager_update(&dst, t, h, is_connected);

    vTaskDelay(250 / portTICK_RATE_MS);
  }
}

void app_main()
{

  ESP_LOGI(TAG, "[APP] Startup..");
  ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
  ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());

  esp_log_level_set("*", ESP_LOG_INFO);

  storage_manager_init();

  xTaskCreate(&app_task, "app_task", 4096, NULL, 5, NULL);
}
