/*
 * time_manager.c
 *
 *  Created on: May 28, 2019
 *      Author: paolo
 */
#include "time_manager.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "wifi_manager.h"

#define ISO_8601_DATE_LENGTH 21

#define MAX_RETRY_COUNT 10

static const char *TAG = "time_manager";

static void sync_time_with_ntp() {
	
	int retry = 0;
	
	while (!time_manager_is_time_synched() && retry < MAX_RETRY_COUNT) {

		if (!wifi_manager_is_sta_connected()) {
			return;
		}
		retry++;
		ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry, MAX_RETRY_COUNT);
		vTaskDelay(2000 / portTICK_PERIOD_MS);
	}
}

static void ntp_task(void *pvParameter) {

	sync_time_with_ntp();
	vTaskDelete(NULL);
}

void time_manager_init() {
	
	ESP_LOGI(TAG, "Initializing SNTP");
	sntp_setoperatingmode(SNTP_OPMODE_POLL);
	sntp_setservername(0, CONFIG_SNTP_SERVER);
	setenv("TZ", "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00", 1);
	tzset();
	sntp_init();
}

void time_manager_sync_time(bool async) {
	
	if (async) {
		xTaskCreate(&ntp_task, "ntp_task", 2048, NULL, 5, NULL);
		return;
	}
	sync_time_with_ntp();
}

bool time_manager_is_time_synched() {
	
	time_info_t timeinfo;
	time_manager_get_current_date_time(&timeinfo);
	return timeinfo.tm_year > (2000 - 1970);
}

void time_manager_get_current_date_time(time_info_t *dst) {
	
	time_t current_time;
	time(&current_time);
	localtime_r(&current_time, dst);
}

