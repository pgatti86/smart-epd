/*
 * time_manager.h
 *
 *  Created on: May 28, 2019
 *      Author: paolo
 */

#ifndef COMPONENTS_TIME_MANAGER_INCLUDE_TIME_MANAGER_H_
#define COMPONENTS_TIME_MANAGER_INCLUDE_TIME_MANAGER_H_

#include "esp_sntp.h"
#include <stdbool.h>

typedef struct tm time_info_t;

void time_manager_init();

void time_manager_sync_time(bool async);

bool time_mnager_is_time_synched();

void time_manager_format_current_date(char *dst);

void time_manager_format_current_time(char *dst);

#endif /* COMPONENTS_TIME_MANAGER_INCLUDE_TIME_MANAGER_H_ */

