#ifndef COMPONENTS_WEATHER_MANAGER_INCLUDE_WEATHER_MANAGER_H_
#define COMPONENTS_WEATHER_MANAGER_INCLUDE_WEATHER_MANAGER_H_

#include "weather_icons.h"

void weather_manager_init();

void weather_manager_deinit();

char* weather_manager_get_weather_description();

enum weather_icons weather_manager_get_weather_icon();

#endif 