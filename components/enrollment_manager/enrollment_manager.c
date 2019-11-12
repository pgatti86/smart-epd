/*
 * enrollment_manager.c
 *
 *  Created on: Nov 09, 2019
 *      Author: matteo
 */
#include "enrollment_manager.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_event_loop.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include <string.h>
#include <esp_http_server.h>
#include <sys/param.h>
#include "cJSON.h"
#include "strings.h"

static const char *TAG = "enrollment_manager";

#define SSID "PLUTO"
#define PWD "PLUTO1234567"
#define CODE 12345

static wifi_event_callback_t event_callback;

static httpd_handle_t server = NULL;

static void stop_webserver();

static esp_err_t event_handler(void *ctx, system_event_t *event) {

    switch (event-> event_id) {
        case SYSTEM_EVENT_STA_GOT_IP:
            stop_webserver();
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            break;    
        default:
            break;    
    }
	
    if (event_callback) {
		event_callback(event-> event_id);
	}

    return ESP_OK;
}

static esp_err_t credentials_post_handler(httpd_req_t *req) {
    
    char buf[100];
    int ret = 0, remaining = req->content_len;

    while (remaining > 0) {
        /* Read the data for the request */
        ret =  httpd_req_recv(req, buf, MIN(remaining, sizeof(buf)));
        if (ret <= 0) { /* 0 return value indicates connection closed */
            if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
                /* Retry receiving if timeout occurred */
                continue;
            }
            return ESP_FAIL; // close the socket
        }

        remaining -= ret;
    }

    /* Log data received */
    ESP_LOGI(TAG, "=========== RECEIVED DATA ==========");
    ESP_LOGI(TAG, "%.*s", ret, buf);
    ESP_LOGI(TAG, "====================================");

    cJSON *root = cJSON_Parse(buf);

    if (root == NULL) {
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, NULL);
        return ESP_FAIL;
    }

    cJSON *ssidJson = cJSON_GetObjectItemCaseSensitive(root, "ssid");
    cJSON *pwdJson = cJSON_GetObjectItemCaseSensitive(root, "password");
    cJSON *codeJson = cJSON_GetObjectItemCaseSensitive(root, "code");
    
    if ((!cJSON_IsString(ssidJson) || (ssidJson->valuestring == NULL)) ||
        (!cJSON_IsString(pwdJson) || (pwdJson->valuestring == NULL)) ||
        (!cJSON_IsNumber(codeJson)) || (codeJson->valueint != CODE)) {            
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, NULL);
        return ESP_FAIL;
    }

    wifi_config_t wifi_config =  {};
    strcpy((char *)wifi_config.sta.ssid,(char *)ssidJson->valuestring);
    strcpy((char *)wifi_config.sta.password,(char *)pwdJson->valuestring);

    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);
    cJSON_Delete(root);
    
    //httpd_resp_send_err(credentials_request, HTTPD_500_INTERNAL_SERVER_ERROR, NULL);

    esp_wifi_connect();

    httpd_resp_send(req, NULL, 0);
    
    return ESP_OK;
}

static const httpd_uri_t set_credentials = {
    .uri       = "/setCredentials",
    .method    = HTTP_POST,
    .handler   = credentials_post_handler,
    .user_ctx  = NULL
};

static httpd_handle_t start_webserver() {

    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_register_uri_handler(server, &set_credentials);
        return server;
    }

    ESP_LOGI(TAG, "Error starting server!");
    return NULL;
}

static void stop_webserver() {

    if(server != NULL) {
        httpd_stop(server);
        server = NULL;
    }
}

void enrollment_manager_init(wifi_event_callback_t wifi_callback) {

	event_callback = wifi_callback;
	ESP_LOGI(TAG, "configuring enrollment manager start...");
    
    tcpip_adapter_init();

    tcpip_adapter_dhcps_stop(TCPIP_ADAPTER_IF_AP);
	tcpip_adapter_ip_info_t info;
    memset(&info, 0, sizeof(info));
    IP4_ADDR(&info.ip, 192, 168, 1, 1);
    IP4_ADDR(&info.gw, 192, 168, 1, 1);
    IP4_ADDR(&info.netmask, 255, 255, 255, 0);
    tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_AP, &info); 
    tcpip_adapter_dhcps_start(TCPIP_ADAPTER_IF_AP);

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

    esp_wifi_set_mode(WIFI_MODE_APSTA);
    esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config);
    esp_wifi_start();

	server = start_webserver();
}