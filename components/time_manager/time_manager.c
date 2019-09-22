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
	const int retry_count = MAX_RETRY_COUNT;
	while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET && ++retry < retry_count) {

		if (!wifi_manager_is_sta_connected()) {
			return;
		}

		ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
		vTaskDelay(2000 / portTICK_PERIOD_MS);
	}
}

static void ntp_task(void *pvParameter) {

	sync_time_with_ntp();
	time_t current_time;
	time_info_t timeinfo = { 0 };
	time(&current_time);
	localtime_r(&current_time, &timeinfo);

	char timestamp [20];
	time_manager_format_date(timestamp, timeinfo);

	vTaskDelete(NULL);
}

void time_manager_init() {
	ESP_LOGI(TAG, "Initializing SNTP");
	sntp_setoperatingmode(SNTP_OPMODE_POLL);
	sntp_setservername(0, CONFIG_SNTP_SERVER);
	sntp_init();
}

void time_manager_sync_time(bool async) {
	if (async) {
		xTaskCreate(&ntp_task, "ntp_task", 2048, NULL, 5, NULL);
		return;
	}
	sync_time_with_ntp();
}

bool time_mnager_is_time_synched() {
	return false; //current_time != 0;
}

void time_manager_format_current_date(char *dst) {
	time_t current_time;
	time_info_t timeinfo;
	time(&current_time);
	localtime_r(&current_time, &timeinfo);
	time_manager_format_date(dst, timeinfo);
}

void time_manager_format_date(char *dst, time_info_t timeinfo) {
	strftime(dst, ISO_8601_DATE_LENGTH, "%Y-%m-%dT%H:%M:%SZ", &timeinfo);
	ESP_LOGI(TAG, "Current date is: %s", dst);
}

