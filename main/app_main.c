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
#include "weather_manager.h"
#include "ota_manager.h"

static const char *TAG = "MAIN";

void gpio_event_handler(void *handler_arg, esp_event_base_t base, int32_t id, void *event_data) {
  
  if (id == GPIO_CLICK_EVENT) {
    ESP_LOGI(TAG, "This is a click");
  } else {
    ESP_LOGI(TAG, "Reset flash...");
    storage_manager_format_nvs();
    esp_restart();
  }
}

static void wifi_manager_callback(int event_id) {
  switch (event_id) {
    case SYSTEM_EVENT_STA_GOT_IP:
      time_manager_sync_time();
      weather_manager_init();
      ota_manager_init();
      break;
    case SYSTEM_EVENT_STA_LOST_IP:
    case SYSTEM_EVENT_STA_DISCONNECTED:
      weather_manager_deinit();
      ota_manager_deinit();
      break;
  }
}

static void enrollment_manager_callback(int event_id) {
  switch (event_id) {
    case SYSTEM_EVENT_STA_GOT_IP:
	    ESP_LOGI(TAG, "SYSTEM_EVENT_STA_GOT_IP...");
      storage_manager_set_enrollment_status(ENROLLMENT_COMPLETED);
      esp_restart();
    break;
  }
}

void enrollment_task(void *args) {

  enrollment_manager_init(enrollment_manager_callback);

  char *ssid =  enrollment_manager_get_ssid();
  ESP_LOGI(TAG, "enrollment ssid: %s", ssid);
  char *pwd = enrollment_manager_get_password();
  ESP_LOGI(TAG, "enrollment password: %s", pwd);
  int token = enrollment_manager_get_verification_code();
  ESP_LOGI(TAG, "enrollment token: %d", token);
  epd_manager_show_enrollment(ssid, pwd, token);

  while (1) {
    vTaskDelay(500 / portTICK_RATE_MS);
  }
}

void app_task(void *pvParameter) {

  ESP_LOGI(TAG, "init wifi_manager");
  wifi_manager_sta_init();
  wifi_manager_set_callback(wifi_manager_callback);
  wifi_manager_connect(true);

  ESP_LOGI(TAG, "init dht_manager");
  dht_manager_start_update_task();

  ESP_LOGI(TAG, "init time_manager");
  time_manager_init();

  time_info_t dst;
  float t, h;
  char *weather_description;
  enum weather_icons weather_icon;
  bool is_connected;
  //size_t mem;
    
  while (1) {

    //mem = heap_caps_get_free_size(MALLOC_CAP_8BIT); 
    //ESP_LOGE(TAG, "Available mem: %d", mem);

    time_manager_get_current_date_time(&dst);

    dht_manager_get_last_red_values(&t, &h);

    weather_description = weather_manager_get_weather_description();
    weather_icon = weather_manager_get_weather_icon();

    is_connected = wifi_manager_is_sta_connected();

    epd_manager_update(&dst, t, h, is_connected, weather_icon, weather_description);

    vTaskDelay(1000 / portTICK_RATE_MS);
  }
}

void app_main() {

  ESP_LOGI(TAG, "[APP] Startup..");
  ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
  ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());

  esp_log_level_set("*", ESP_LOG_INFO);

  storage_manager_init();

  esp_event_loop_create_default();
  esp_event_handler_register(GPIO_EVENTS, ESP_EVENT_ANY_ID, gpio_event_handler, NULL);

  ESP_LOGI(TAG, "init gpio_manager");
  gpio_manger_init();

  ESP_LOGI(TAG, "init epd_manager");
  epd_manager_init();

  if (!storage_manager_has_enrollment_done()) {
    xTaskCreate(&enrollment_task, "enrollment_task", 4096, NULL, 5, NULL);   
  } else {
    xTaskCreate(&app_task, "app_task", 4096, NULL, 5, NULL);  
  }
}
