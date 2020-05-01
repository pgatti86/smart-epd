

#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_log.h"
#include "esp_spiffs.h"

#include "storage_manager.h"

static const char *TAG = "STORAGE MANAGER";

static const char* ENROLLMENTSTATUS_KEY =  "enr-status"; // max 15 char

static const char* WEATHER_ZIP_KEY =  "zip-code"; // max 15 char

static const char *SPIFFS_DEVICE_PARTITION = "device";

static const char *SPIFFS_SECURITY_PARTITION = "security";

static nvs_handle nvs_memory_handle = -1;

static char* storage_manager_read_file_in_buffer(FILE* f) {

	/* quit if the file does not exist */
	if (f == NULL) {
		return NULL;
	}

	char  *buffer;
	long  numbytes;

	/* Get the number of bytes */
	fseek(f, 0L, SEEK_END);
	numbytes = ftell(f);

	/* reset the file position indicator to
	the beginning of the file */
	fseek(f, 0L, SEEK_SET);

	/* grab sufficient memory for the
	buffer to hold the text */
	buffer = (char*)calloc(numbytes, sizeof(char));

	/* memory error */
	if(buffer == NULL) {
		free(buffer);
		return NULL;
	}

	/* copy all the text into the buffer */
	fread(buffer, sizeof(char), numbytes, f);
	buffer[numbytes -1] = '\0';
	return buffer;
}

static char* storage_manager_read_file_from_spiffs(char *file_path) {

	FILE* f = fopen(file_path, "r");
	if (f == NULL) {
		ESP_LOGE(TAG, "Failed to open file for reading");
		return NULL;
	}

	char *buffer = storage_manager_read_file_in_buffer(f);
	fclose(f);

	return buffer;
}

static void storage_manager_init_spiffs(esp_vfs_spiffs_conf_t conf) {

	esp_err_t ret = esp_vfs_spiffs_register(&conf);

	if(ret != ESP_OK) {
		ESP_LOGI(TAG, "error while mounting spiffs %s", conf.base_path);
		return;
	}

	size_t total = 0, used = 0;
	ret = esp_spiffs_info(SPIFFS_DEVICE_PARTITION, &total, &used);
	if (ret != ESP_OK) {
		ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
	} else {
		ESP_LOGI(TAG, "Partition size %s: total: %d, used: %d", conf.base_path, total, used);
	}
}

static void storage_manager_device_config_spiffs_init() {

	esp_vfs_spiffs_conf_t conf = {
	  .base_path = "/device",
	  .partition_label = SPIFFS_DEVICE_PARTITION,
	  .max_files = 5,
	  .format_if_mount_failed = false
	};

	storage_manager_init_spiffs(conf);
}

static void storage_manager_security_spiffs_init() {

	esp_vfs_spiffs_conf_t conf = {
	  .base_path = "/security",
	  .partition_label = SPIFFS_SECURITY_PARTITION,
	  .max_files = 5,
	  .format_if_mount_failed = false
	};

	storage_manager_init_spiffs(conf);
}

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

    ESP_LOGI(TAG, "init nvs storage...");
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        err = nvs_flash_init();
    }

    err = nvs_open("storage", NVS_READWRITE, &nvs_memory_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!", esp_err_to_name(err));
    }

    ESP_LOGI(TAG, "init spiffs storage...");
    storage_manager_device_config_spiffs_init();
    storage_manager_security_spiffs_init();
}

int storage_manager_get_weather_zip_code() {
	
	ESP_LOGI(TAG, "getting enrollment status...");
	int zip_code = 25100; // default to BS

	if (nvs_memory_handle < 0)
        return zip_code;

	esp_err_t err = nvs_get_i32(nvs_memory_handle, WEATHER_ZIP_KEY, &zip_code);
	return zip_code;
}

void storage_manager_set_weather_zip_code(int zip_code) {
	
	if (nvs_memory_handle > 0) {
        nvs_set_i32(nvs_memory_handle, WEATHER_ZIP_KEY, zip_code);
        storage_manager_commit();
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

char* storage_manager_get_device_config_from_spiffs() {
	return storage_manager_read_file_from_spiffs("/device/config.json");
}

char* storage_manager_get_ca_cert_from_spiffs() {
	return storage_manager_read_file_from_spiffs("/security/cacert.pem");
}

char* storage_manager_get_device_cert_from_spiffs() {
	return storage_manager_read_file_from_spiffs("/security/devcert.pem");
}

char* storage_manager_get_device_key_from_spiffs() {
	return storage_manager_read_file_from_spiffs("/security/devkey.pem");
}

char* storage_manager_get_ota_cert_from_spiffs() {
	return storage_manager_read_file_from_spiffs("/security/ota_cert.pem");
}