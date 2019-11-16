/*
 * enrollment_manager.h
 *
 *  Created on: Nov 09, 2019
 *      Author: matteo
 */

#ifndef COMPONENTS_ENROLLMENT_MANAGER_INCLUDE_ENROLLMENT_MANAGER_H_
#define COMPONENTS_ENROLLMENT_MANAGER_INCLUDE_ENROLLMENT_MANAGER_H_

#include "wifi_callback.h"

void enrollment_manager_init(wifi_event_callback_t wifi_callback);

int enrollment_manager_get_verification_code();

char* enrollment_manager_get_ssid();

char* enrollment_manager_get_password();

#endif /* COMPONENTS_ENROLLMENT_MANAGER_INCLUDE_ENROLLMENT_MANAGER_H_ */