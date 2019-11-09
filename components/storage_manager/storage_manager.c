

#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_log.h"

#include "storage_manager.h"

#define ENROLLMENTSTATUS_KEY "enr-status" // max 15 char

static const char *TAG = "STORAGE MANAGER";

static nvs_handle nvs_memory_handle = -1;

static void storage_manager_commit() {

    if (nvs_memory_handle < 0) {
        ESP_LOGE(TAG, "Save error");
        return;
    }
    
    esp_err_t err = nvs_commit(nvs_memory_handle);
    if(err == ESP_OK) {
        ESP_LOGI(TAG, "Data saved"); 
    } else {
        ESP_LOGE(TAG, "Commit error"); 
    }
}

void storage_manager_init() {

    ESP_LOGI(TAG, "init storage manager...");
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        err = nvs_flash_init();
    }

    err = nvs_open("storage", NVS_READWRITE, &nvs_memory_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!", esp_err_to_name(err));
    }
}

int storage_manager_get_enrollment_status() {

    ESP_LOGI(TAG, "getting enrollment status...");
    int enrollment_status = ENROLLMENT_NOT_COMPLETED;

    if (nvs_memory_handle < 0)
        return enrollment_status;

   esp_err_t err = nvs_get_i32(nvs_memory_handle, ENROLLMENTSTATUS_KEY, &enrollment_status);
   if(err != ESP_OK) {
       return ENROLLMENT_NOT_COMPLETED;
   }

   return enrollment_status;
}

bool storage_manager_has_enrollment_done() {

    int status = storage_manager_get_enrollment_status();
    return status == ENROLLMENT_COMPLETED;
}

void storage_manager_set_enrollment_status(int status) {

    if (nvs_memory_handle > 0) {
        nvs_set_i32(nvs_memory_handle, ENROLLMENTSTATUS_KEY, status);
        storage_manager_commit();
    }
}

void storage_manager_close_nvs() {

    if (nvs_memory_handle > 0 ){
        nvs_close(nvs_memory_handle);
    }
}

void storage_manager_format_nvs() {
    nvs_flash_erase();
    storage_manager_close_nvs();
    storage_manager_init();
}