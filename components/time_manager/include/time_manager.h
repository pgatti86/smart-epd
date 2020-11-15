/*
 * time_manager.h
 *
 *  Created on: May 28, 2019
 *      Author: paolo
 */

#ifndef COMPONENTS_TIME_MANAGER_INCLUDE_TIME_MANAGER_H_
#define COMPONENTS_TIME_MANAGER_INCLUDE_TIME_MANAGER_H_

#include <stdbool.h>
#include "time_info.h"

void time_manager_init();

void time_manager_sync_time();

bool time_manager_is_time_set();

void time_manager_get_current_date_time(time_info_t *dst);

#endif /* COMPONENTS_TIME_MANAGER_INCLUDE_TIME_MANAGER_H_ */

