/*
 * wifi_manager.h
 *
 *  Created on: Sep 15, 2018
 *      Author: paolo
 */

#ifndef COMPONENTS_WIFI_MANAGER_INCLUDE_WIFI_MANAGER_H_
#define COMPONENTS_WIFI_MANAGER_INCLUDE_WIFI_MANAGER_H_

#include <stdbool.h>

typedef void (*wifi_event_callback_t)(int event_id);

void wifi_manager_sta_init();

void wifi_manager_set_callback(wifi_event_callback_t wifi_callback);

void wifi_manager_connect(bool retry_on_failure);

void wifi_manager_disconnect();

bool wifi_manager_is_sta_connected();

void wifi_manager_sta_deinit();

#endif /* COMPONENTS_WIFI_MANAGER_INCLUDE_WIFI_MANAGER_H_ */
