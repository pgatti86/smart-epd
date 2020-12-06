#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_http_client.h"
#include "cJSON.h"
#include "storage_manager.h"
#include "weather_manager.h"

#define MAX_HTTP_RECV_BUFFER 512

#define WEATHER_TASK_DELAY 10 * 60 * 1000 

static const char *TAG = "Weather-M";

static int weather_data_index = 0;
static char weather_data[MAX_HTTP_RECV_BUFFER];

static struct weather_data w_data;

static TaskHandle_t weather_task_handler = NULL;

static void weather_manager_parse_data();

static esp_err_t http_event_handler(esp_http_client_event_t *evt) {
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

        cJSON *coordinates = cJSON_GetObjectItemCaseSensitive(json, "coord");
        if (coordinates != NULL) {
            cJSON *lat = cJSON_GetObjectItemCaseSensitive(coordinates, "lat");
            cJSON *lon = cJSON_GetObjectItemCaseSensitive(coordinates, "lon");
            if (lat != NULL && lon != NULL) {
                w_data.latitude = lat->valuedouble;
                w_data.longitude = lon->valuedouble;
            }
        }

        cJSON *main_data = cJSON_GetObjectItemCaseSensitive(json, "main");
        if (main_data != NULL) {
            cJSON *temp = cJSON_GetObjectItemCaseSensitive(main_data, "temp");
            cJSON *feels_like = cJSON_GetObjectItemCaseSensitive(main_data, "feels_like");
            cJSON *temp_min = cJSON_GetObjectItemCaseSensitive(main_data, "temp_min");
            cJSON *temp_max = cJSON_GetObjectItemCaseSensitive(main_data, "temp_max");
            cJSON *pressure = cJSON_GetObjectItemCaseSensitive(main_data, "pressure");
            cJSON *humidity = cJSON_GetObjectItemCaseSensitive(main_data, "humidity");

            if (temp != NULL) {
                w_data.temp = temp->valuedouble;
            }

            if (temp_min != NULL) {
                w_data.temp_min = temp_min->valuedouble;
            }

            if (temp_max != NULL) {
                w_data.temp_max = temp_max->valuedouble;
            }

            if (feels_like != NULL) {
                w_data.perc_temp = feels_like->valuedouble;
            }

            if (pressure != NULL) {
                w_data.pressure = pressure->valueint;
            }

             if (humidity != NULL) {
                w_data.humidity = humidity->valueint;
            }
        }

        cJSON *wind_data = cJSON_GetObjectItemCaseSensitive(json, "wind");
        if (wind_data != NULL) {
            cJSON *speed = cJSON_GetObjectItemCaseSensitive(wind_data, "speed");
            if (speed != NULL) {
                w_data.wind_speed = speed->valuedouble;
            }
        }

        cJSON *weatherArray = cJSON_GetObjectItemCaseSensitive(json, "weather");
        if ( weatherArray != NULL && cJSON_GetArraySize(weatherArray) > 0) {
            
            cJSON *weather = cJSON_GetArrayItem(weatherArray, 0);

            cJSON *weatherDescription = cJSON_GetObjectItemCaseSensitive(weather, "description");
            if (weatherDescription != NULL && weatherDescription->valuestring != NULL) {
                strcpy(w_data.weather_description, weatherDescription->valuestring);
            }
            
            w_data.weather_icon = UNKNOWN;
            cJSON *weatherIcon = cJSON_GetObjectItemCaseSensitive(weather, "icon");
            if (weatherIcon != NULL && weatherIcon->valuestring != NULL) {
                if (strcmp(weatherIcon->valuestring, "01d") == 0) {
                    w_data.weather_icon = CLEAR_SKY_DAY;
                } else if (strcmp(weatherIcon->valuestring, "01n") == 0) {
                    w_data.weather_icon = CLEAR_SKY_NIGHT;
                } else if (strcmp(weatherIcon->valuestring, "02d") == 0) {
                    w_data.weather_icon = FEW_CLOUDS_DAY;
                } else if (strcmp(weatherIcon->valuestring, "02n") == 0) {
                    w_data.weather_icon = FEW_CLOUDS_NIGHT;
                } else if (strcmp(weatherIcon->valuestring, "03d") == 0 || strcmp(weatherIcon->valuestring, "03n") == 0 || 
                        strcmp(weatherIcon->valuestring, "04d") == 0 || strcmp(weatherIcon->valuestring, "04n") == 0) {
                    w_data.weather_icon = CLOUDS;
                } else if (strcmp(weatherIcon->valuestring, "09d") == 0 || strcmp(weatherIcon->valuestring, "09n") == 0) {
                    w_data.weather_icon = SHOWER_RAIN;
                } else if (strcmp(weatherIcon->valuestring, "10d") == 0 || strcmp(weatherIcon->valuestring, "10n") == 0) {
                    w_data.weather_icon = RAIN;
                } else if (strcmp(weatherIcon->valuestring, "11d") == 0 || strcmp(weatherIcon->valuestring, "11n") == 0) {
                    w_data.weather_icon = THUNDERSTORM;
                } else if (strcmp(weatherIcon->valuestring, "13d") == 0 || strcmp(weatherIcon->valuestring, "13n") == 0) {
                    w_data.weather_icon = SNOW;
                } else if (strcmp(weatherIcon->valuestring, "50d") == 0 || strcmp(weatherIcon->valuestring, "50n") == 0) {
                    w_data.weather_icon = FOG;
                }
            }
        }
    }

    cJSON_Delete(json);
}

static void weather_manager_perform_request(char *request_url) {

    weather_data_index = 0;

    esp_http_client_config_t config = {
        .url = request_url,
        .event_handler = http_event_handler
    };

    esp_http_client_handle_t client; 

    client = esp_http_client_init(&config);
    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "HTTP GET Status = %d, content_length = %d",
                esp_http_client_get_status_code(client),
                esp_http_client_get_content_length(client));
    } else {
        ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
    }

    esp_http_client_cleanup(client);
}

static void update_weather_task(void *pvParameters) {
    
    int request_buffer_size = 200;
    char request[request_buffer_size];
    int zip_code = storage_manager_get_weather_zip_code();
    char *api_key = storage_manager_get_weather_api_key();  

    snprintf(request, request_buffer_size, 
        "https://api.openweathermap.org/data/2.5/weather?zip=%d,it&appid=%s&units=metric", 
        zip_code, api_key);

    free(api_key); 

    while (1) {
        weather_manager_perform_request(request);
        vTaskDelay(WEATHER_TASK_DELAY / portTICK_RATE_MS);
    } 

   vTaskDelete(NULL);
}

void weather_manager_init() {
    xTaskCreate(&update_weather_task, "update_weather_task", 4096, NULL, 5, &weather_task_handler);
}

void weather_manager_deinit() {
    if (weather_task_handler) {
		vTaskDelete(weather_task_handler);
		weather_task_handler = NULL;
    }
}

struct weather_data* weather_manager_get_weather_data() {
    return &w_data;
}