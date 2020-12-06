#ifndef COMPONENTS_WEATHER_MANAGER_INCLUDE_WEATHER_MANAGER_H_
#define COMPONENTS_WEATHER_MANAGER_INCLUDE_WEATHER_MANAGER_H_

#include "weather_icons.h"

struct weather_data {
   double  temp;
   double  perc_temp;
   double  temp_min;
   double  temp_max;
   int  pressure;
   int  humidity;
   double wind_speed;
   char weather_description[20];
   enum weather_icons weather_icon;
   double latitude;
   double longitude;
};

void weather_manager_init();

void weather_manager_deinit();

struct weather_data* weather_manager_get_weather_data();

#endif 