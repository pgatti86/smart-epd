#ifndef COMPONENTS_EPD_MANAGER_INCLUDE_CLOCK_DATA_BUFFER_H_
#define COMPONENTS_EPD_MANAGER_INCLUDE_CLOCK_DATA_BUFFER_H_

#include "time.h"
#include "time_info.h"
#include "strings.h"
#include "weather_icons.h"

class ClockDataBuffer {

    public:

        int minutes = -1;
        
        int day = -1;
        
        bool is_connected = false;
        
        float temperature = -100;
        float humidity = -100;

        char weather_description [20];
        enum weather_icons weather_icon = UNKNOWN;
};

#endif