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
#include "time_info.h"

void time_manager_init();

void time_manager_sync_time(bool async);

bool time_manager_is_time_synched();

void time_manager_format_current_date(char *dst);

void time_manager_format_current_time(char *dst);

void time_manager_get_current_date_time(time_info_t *dst);

#endif /* COMPONENTS_TIME_MANAGER_INCLUDE_TIME_MANAGER_H_ */

