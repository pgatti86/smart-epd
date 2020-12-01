#ifndef COMPONENTS_APDS_MANAGER_INCLUDE_APDS_MANAGER_H_
#define COMPONENTS_APDS_MANAGER_INCLUDE_APDS_MANAGER_H_

#include "esp_event.h"

#ifdef __cplusplus

extern "C" {
#endif

	ESP_EVENT_DECLARE_BASE(GESTURE_EVENTS);

	enum {                                     
    	GESTURE_EVENT_UP,
    	GESTURE_EVENT_DOWN,
    	GESTURE_EVENT_LEFT,
		GESTURE_EVENT_RIGHT,
		GESTURE_EVENT_NEAR,
		GESTURE_EVENT_FAR,
		GESTURE_EVENT_NONE
	};

	void apds_manager_init();

#ifdef __cplusplus
}
#endif

#endif