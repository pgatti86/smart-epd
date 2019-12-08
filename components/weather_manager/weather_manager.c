#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_http_client.h"
#include "cJSON.h"
#include "weather_manager.h"

#define MAX_HTTP_RECV_BUFFER 512

static const char *TAG = "Weather-M";

static int weather_data_index = 0;
static char weather_data[MAX_HTTP_RECV_BUFFER];

static char weather_description[20];
static enum weather_icons weather_icon = UNKNOWN; 

static void weather_manager_parse_data();

static esp_err_t _http_event_handler(esp_http_client_event_t *evt) {
    switch(evt->event_id) {
        case HTTP_EVENT_ERROR:
            ESP_LOGI(TAG, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_CONNECTED");
            break;
        case HTTP_EVENT_HEADER_SENT:
            ESP_LOGI(TAG, "HTTP_EVENT_HEADER_SENT");
            break;
        case HTTP_EVENT_ON_HEADER:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
            break;
        case HTTP_EVENT_ON_DATA:

            ESP_LOGI(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
            if (weather_data_index + evt->data_len < MAX_HTTP_RECV_BUFFER) {
                memcpy(weather_data + weather_data_index, (char*)evt->data, evt->data_len);
                weather_data_index += evt->data_len;
            }
        
            break;
        case HTTP_EVENT_ON_FINISH:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_FINISH");
            weather_manager_parse_data();
            break;
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
            printf("%.*s", MAX_HTTP_RECV_BUFFER, weather_data);
            break;
    }
    return ESP_OK;
}

static void weather_manager_parse_data() {
    
    cJSON *json = cJSON_Parse(weather_data);
    if (json != NULL) {
        cJSON *weatherArray = cJSON_GetObjectItemCaseSensitive(json, "weather");
        if ( weatherArray != NULL && cJSON_GetArraySize(weatherArray) > 0) {
            
            cJSON *weather = cJSON_GetArrayItem(weatherArray, 0);

            cJSON *weatherDescription = cJSON_GetObjectItemCaseSensitive(weather, "description");
            if (weatherDescription != NULL && weatherDescription->valuestring != NULL) {
                strcpy(weather_description, weatherDescription->valuestring);
            }
            
            cJSON *weatherIcon = cJSON_GetObjectItemCaseSensitive(weather, "icon");
            if (weatherIcon != NULL && weatherIcon->valuestring != NULL) {
                if (strcmp(weatherIcon->valuestring, "01d") == 0) {
                    weather_icon = CLEAR_SKY_DAY;
                } else if (strcmp(weatherIcon->valuestring, "01n") == 0) {
                    weather_icon = CLEAR_SKY_NIGHT;
                } else if (strcmp(weatherIcon->valuestring, "02d") == 0) {
                    weather_icon = FEW_CLOUDS_DAY;
                } else if (strcmp(weatherIcon->valuestring, "02n") == 0) {
                    weather_icon = FEW_CLOUDS_NIGHT;
                } else if (strcmp(weatherIcon->valuestring, "03d") == 0 || strcmp(weatherIcon->valuestring, "03n") == 0 || 
                        strcmp(weatherIcon->valuestring, "04d") == 0 || strcmp(weatherIcon->valuestring, "04n") == 0) {
                    weather_icon = CLOUDS;
                } else if (strcmp(weatherIcon->valuestring, "09d") == 0 || strcmp(weatherIcon->valuestring, "09n") == 0) {
                    weather_icon = SHOWER_RAIN;
                } else if (strcmp(weatherIcon->valuestring, "10d") == 0 || strcmp(weatherIcon->valuestring, "10n") == 0) {
                    weather_icon = RAIN;
                } else if (strcmp(weatherIcon->valuestring, "11d") == 0 || strcmp(weatherIcon->valuestring, "11n") == 0) {
                    weather_icon = THUNDERSTORM;
                } else if (strcmp(weatherIcon->valuestring, "13d") == 0 || strcmp(weatherIcon->valuestring, "13n") == 0) {
                    weather_icon = SNOW;
                } else if (strcmp(weatherIcon->valuestring, "50d") == 0 || strcmp(weatherIcon->valuestring, "50n") == 0) {
                    weather_icon = FOG;
                }
            }
        }
    }

    cJSON_Delete(json);
}

static void update_weather_task(void *pvParameters) {
   
    while(1) {
        
        weather_data_index = 0;
        char request[200];
        snprintf(request, 200, "https://api.openweathermap.org/data/2.5/weather?zip=%d,it&appid=%s&units=metric", 
            25100, CONFIG_WEATHER_API_KEY);

        esp_http_client_config_t config = {
            .url = request,
            .event_handler = _http_event_handler
        };

        esp_http_client_handle_t client = esp_http_client_init(&config);

        // GET
        esp_err_t err = esp_http_client_perform(client);
        if (err == ESP_OK) {
            ESP_LOGI(TAG, "HTTP GET Status = %d, content_length = %d",
                    esp_http_client_get_status_code(client),
                    esp_http_client_get_content_length(client));
        } else {
            ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
        }

        esp_http_client_cleanup(client);

        vTaskDelay(1000 * 60 * 15 / portTICK_RATE_MS);
   } 

   vTaskDelete(NULL);
}

void weather_manager_init() {
    xTaskCreate(&update_weather_task, "update_weather_task", 4096, NULL, 5, NULL);
}

char* weather_manager_get_weather_description() {
    return weather_description;
}

enum weather_icons weather_manager_get_weather_icon() {
    return weather_icon;
}