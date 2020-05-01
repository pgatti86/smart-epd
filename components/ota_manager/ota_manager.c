#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_https_ota.h"
#include "esp_ota_ops.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "storage_manager.h"

#include "ota_manager.h"

static const char *TAG = "ota manager";

static const int UPDATE_CHECK_TTL = 1000 * 60 * 60;

static void ota_manager_check_fw_update();

static esp_err_t ota_manager_validate_image_header(esp_app_desc_t *new_app_info);

void ota_manager_ota_task(void *pvParameter) {
    
     while (1) {
        ota_manager_check_fw_update(); 
        vTaskDelay(UPDATE_CHECK_TTL / portTICK_RATE_MS);
    }
}

void ota_manager_init() {
    xTaskCreate(&ota_manager_ota_task, "ota_task", 1024*8, NULL, 5, NULL);
}

static esp_err_t ota_manager_validate_image_header(esp_app_desc_t *new_app_info) {
    
    if (new_app_info == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    const esp_partition_t *running = esp_ota_get_running_partition();
    esp_app_desc_t running_app_info;
    if (esp_ota_get_partition_description(running, &running_app_info) == ESP_OK) {
        ESP_LOGI(TAG, "Running firmware version: %s", running_app_info.version);
    }

    if (memcmp(new_app_info->version, running_app_info.version, sizeof(new_app_info->version)) == 0) {
        ESP_LOGW(TAG, "Current running version is the same as a new. We will not continue the update.");
        return ESP_FAIL;
    }

    return ESP_OK;
}

static void ota_manager_check_fw_update() {

    ESP_LOGI(TAG, "Starting OTA");  

    esp_http_client_config_t config = {
        .url = CONFIG_OTA_SERVER,
        .cert_pem = storage_manager_get_ota_cert_from_spiffs(),
    };

    config.skip_cert_common_name_check = true;

    esp_https_ota_config_t ota_config = {
        .http_config = &config,
    };

    esp_https_ota_handle_t https_ota_handle = NULL;
    esp_err_t err = esp_https_ota_begin(&ota_config, &https_ota_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "ESP HTTPS OTA Begin failed");
        return;
    }

    esp_app_desc_t app_desc;
    err = esp_https_ota_get_img_desc(https_ota_handle, &app_desc);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_https_ota_read_img_desc failed");
        return;
    }
    err = ota_manager_validate_image_header(&app_desc);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "image header verification failed");
        return;
    }

    while (1) {
        err = esp_https_ota_perform(https_ota_handle);
        if (err != ESP_ERR_HTTPS_OTA_IN_PROGRESS) {
            break;
        }
       
        ESP_LOGD(TAG, "Image bytes read: %d", esp_https_ota_get_image_len_read(https_ota_handle));
    }

    if (esp_https_ota_is_complete_data_received(https_ota_handle) != true) {
        ESP_LOGE(TAG, "Complete data was not received.");
    }

    esp_err_t ota_finish_err = esp_https_ota_finish(https_ota_handle);

    if ((err == ESP_OK) && (ota_finish_err == ESP_OK)) {
        ESP_LOGI(TAG, "ESP_HTTPS_OTA upgrade successful. Rebooting ...");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        esp_restart();
    } else {
        if (ota_finish_err == ESP_ERR_OTA_VALIDATE_FAILED) {
            ESP_LOGE(TAG, "Image validation failed, image is corrupted");
        }
        ESP_LOGE(TAG, "ESP_HTTPS_OTA upgrade failed %d", ota_finish_err);
    }
}