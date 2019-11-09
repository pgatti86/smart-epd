/*
 * enrollment_manager.h
 *
 *  Created on: Nov 09, 2019
 *      Author: matteo
 */

#ifndef COMPONENTS_ENROLLMENT_MANAGER_INCLUDE_ENROLLMENT_MANAGER_H_
#define COMPONENTS_ENROLLMENT_MANAGER_INCLUDE_ENROLLMENT_MANAGER_H_

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef void (*wifi_event_callback_t)(int event_id);

void enrollment_manager_start(wifi_event_callback_t wifi_callback);

#endif /* COMPONENTS_ENROLLMENT_MANAGER_INCLUDE_ENROLLMENT_MANAGER_H_ */