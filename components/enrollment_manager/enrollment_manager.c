/*
 * enrollment_manager.c
 *
 *  Created on: Nov 09, 2019
 *      Author: matteo
 */
#include "enrollment_manager.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_event_loop.h"
#include "esp_wifi.h"
#include "esp_log.h"

static const char *TAG = "enrollment_manager";

#define SSID "PIPPO"
#define PWD "PLUTO1234567"

static wifi_event_callback_t event_callback;

static esp_err_t event_handler(void *ctx, system_event_t *event) {

	if(event_callback) {
		event_callback(event-> event_id);
	}

    return ESP_OK;
}

void enrollment_manager_start(wifi_event_callback_t wifi_callback) {
	event_callback = wifi_callback;
	ESP_LOGI(TAG, "configuring enrollment manager start...");

	tcpip_adapter_init();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    esp_event_loop_init(event_handler, NULL);

    wifi_config_t wifi_config = {
        .ap = {
            .ssid = SSID,
            .ssid_len = strlen(SSID),
            .password = PWD,
            .max_connection = 1,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK
        },
    };

    esp_wifi_set_mode(WIFI_MODE_AP);
    esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config);
    esp_wifi_start();

	ESP_LOGI(TAG, "configuring enrollment manager end...");
}