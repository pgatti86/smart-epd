#ifndef COMPONENTS_GPIO_MANAGER_INCLUDE_GPIO_MANAGER_H_
#define COMPONENTS_GPIO_MANAGER_INCLUDE_GPIO_MANAGER_H_

#include "esp_event.h"
#include "esp_event_loop.h"

// Declare an event base
ESP_EVENT_DECLARE_BASE(GPIO_EVENTS);        

enum {                                      
    GPIO_CLICK_EVENT,                     
    GPIO_RESET_EVENT
};

void gpio_manger_init();

#endif /* COMPONENTS_GPIO_MANAGER_INCLUDE_GPIO_MANAGER_H_ */