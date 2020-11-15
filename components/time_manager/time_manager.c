/*
 * time_manager.c
 *
 *  Created on: May 28, 2019
 *      Author: paolo
 */
#include "esp_sntp.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "time_manager.h"

static const char *TAG = "time_manager";

static TaskHandle_t sync_time_task_handle = NULL;

static void time_manager_print_current_time() {

    char strftime_buf[64];
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);

    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
    ESP_LOGI(TAG, "The current date/time is: %s", strftime_buf);
}

static void sync_time_task(void *args) {

    int retry = 0;
    const int retry_count = 30;
    while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET && ++retry < retry_count) {
        ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
   
    time_manager_print_current_time();

	TaskHandle_t this_task_handle = sync_time_task_handle;
    sync_time_task_handle = NULL;

    if (!time_manager_is_time_set()) {
        time_manager_sync_time();
    }

    vTaskDelete(this_task_handle);
}

void time_sync_notification_cb(struct timeval *tv) {
    ESP_LOGI(TAG, "Notification of a time synchronization event");
}

void time_manager_init() {
    ESP_LOGI(TAG, "Initializing SNTP");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, CONFIG_SNTP_SERVER);
	setenv("TZ", "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00", 1);
	tzset();
    sntp_set_time_sync_notification_cb(time_sync_notification_cb);
    sntp_init();
}

void time_manager_sync_time() {
    
    if (time_manager_is_time_set()) {
        time_manager_print_current_time();
        return;
    }
    
    if (sync_time_task_handle) {
        vTaskDelete(sync_time_task_handle);
    }

    xTaskCreate(&sync_time_task, "sync_time_task", 2048, NULL, 5, &sync_time_task_handle);
}

bool time_manager_is_time_set() {
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    // Is time set? If not, tm_year will be (1970 - 1900).
    return timeinfo.tm_year > (2016 - 1900);
}

void time_manager_format_time(char *destination) {

    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);

    int iso_date_length = 30;
    strftime(destination, iso_date_length, "%Y-%m-%dT%H:%M:%SZ", &timeinfo);
}

void time_manager_get_current_date_time(time_info_t *dst) {
	
	time_t current_time;
	time(&current_time);
	localtime_r(&current_time, dst);
}

