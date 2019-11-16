/*
 * wifi_manager.c
 *
 *  Created on: Sep 15, 2018
 *      Author: paolo
 */
#include "wifi_manager.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_event_loop.h"
#include "esp_wifi.h"
#include "esp_log.h"

#define CONN_RETRY_STEP 5000

#define MAX_CONN_RETRY 60000

static const char *TAG = "wifi_manager";

const int WIFI_CONN_RETRY_BIT = BIT0;

static int conn_retry_timeout = CONN_RETRY_STEP;

static wifi_event_callback_t event_callback;

static TaskHandle_t conn_retry_task_handler;

static EventGroupHandle_t retry_event_group;

static bool retry_connection;

static bool is_sta_connected;

void wifi_conn_retry_task(void *pvParameter) {

	while(1) {

		xEventGroupWaitBits(retry_event_group, WIFI_CONN_RETRY_BIT, false, true, portMAX_DELAY);

		vTaskDelay(conn_retry_timeout / portTICK_RATE_MS);

		conn_retry_timeout = conn_retry_timeout < MAX_CONN_RETRY ?
				conn_retry_timeout + CONN_RETRY_STEP : MAX_CONN_RETRY;

		ESP_LOGI(TAG,"retry to connect to the AP");
		esp_wifi_connect();
	}
}

static void wifi_manager_print_connection_info() {
	if(is_sta_connected){
		tcpip_adapter_ip_info_t ip_info;
		tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &ip_info);
		ESP_LOGI(TAG, "Connected with following params:");
		ESP_LOGI(TAG, "IP Address:  %s\n", ip4addr_ntoa(&ip_info.ip));
		ESP_LOGI(TAG, "Subnet mask: %s\n", ip4addr_ntoa(&ip_info.netmask));
		ESP_LOGI(TAG, "Gateway:     %s\n", ip4addr_ntoa(&ip_info.gw));
		return;
	}

	ESP_LOGI(TAG, "Cannot resolve connection params. Module disconected");
}

static void wifi_manager_reset_retry_counters() {

	ESP_LOGI(TAG, "Resetting WiFi counters...");

	xEventGroupClearBits(retry_event_group, WIFI_CONN_RETRY_BIT);

	if (conn_retry_task_handler) {
		vTaskDelete(conn_retry_task_handler);
		conn_retry_task_handler = NULL;
		conn_retry_timeout = CONN_RETRY_STEP;
	}
}

static esp_err_t event_handler(void *ctx, system_event_t *event) {

	switch (event-> event_id) {

    		case SYSTEM_EVENT_STA_START:
    			esp_wifi_connect();
    			break;
    		case SYSTEM_EVENT_STA_GOT_IP:
    			is_sta_connected = true;
    			wifi_manager_reset_retry_counters();
    			wifi_manager_print_connection_info();
        		break;
    		case SYSTEM_EVENT_STA_LOST_IP:
    		case SYSTEM_EVENT_STA_DISCONNECTED: {
    			ESP_LOGI(TAG,"connection to the AP failed");
    			is_sta_connected = false;

    			if(!conn_retry_task_handler && retry_connection) {
    				xTaskCreate(&wifi_conn_retry_task, "retry_task", 2048, NULL, 5, &conn_retry_task_handler);
    				xEventGroupSetBits(retry_event_group, WIFI_CONN_RETRY_BIT);
    			}
    			break;
        	}
    		default:
    			break;
    }

	if(event_callback) {
		event_callback(event-> event_id);
	}

    return ESP_OK;
}

void wifi_manager_sta_init() {

	ESP_LOGI(TAG, "configuring wifi manager...");

	retry_event_group = xEventGroupCreate();

	tcpip_adapter_init();
	esp_event_loop_init(event_handler, NULL);

	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	esp_wifi_init(&cfg);
	esp_wifi_set_mode(WIFI_MODE_STA);
}

void wifi_manager_set_callback(wifi_event_callback_t wifi_callback) {
	event_callback = wifi_callback;
}

void wifi_manager_connect(bool retry_on_failure) {
	ESP_LOGI(TAG, "connecting...");
	retry_connection = retry_on_failure;
	esp_wifi_start();
}

void wifi_manager_disconnect() {

	ESP_LOGI(TAG, "disconnecting from wifi...");
	retry_connection = false;
	wifi_manager_reset_retry_counters();
	if (is_sta_connected) {
		esp_wifi_disconnect();
	}
}

bool wifi_manager_is_sta_connected() {
	return is_sta_connected;
}

void wifi_manager_sta_deinit() {

	if (is_sta_connected) {
		ESP_LOGI(TAG, "must disconnect before deinit");
		return;
	}

	ESP_LOGI(TAG, "deinit wifi...");
	esp_wifi_deinit();
}



