/*
 * storage_manager.h
 *
 *  Created on: May 28, 2019
 *      Author: paolo
 */

#ifndef COMPONENTS_STORAGE_MANAGER_INCLUDE_STORAGE_MANAGER_H_
#define COMPONENTS_STORAGE_MANAGER_INCLUDE_STORAGE_MANAGER_H_

#define ENROLLMENT_NOT_COMPLETED 0
#define ENROLLMENT_COMPLETED 1

void storage_manager_init();

int storage_manager_get_enrollment_status();

bool storage_manager_has_enrollment_done();

void storage_manager_set_enrollment_status(int status);

void storage_manager_close_nvs();

void storage_manager_close_nvs();

void storage_manager_format_nvs();

#endif /* COMPONENTS_TIME_MANAGER_INCLUDE_TIME_MANAGER_H_ */