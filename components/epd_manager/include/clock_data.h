#ifndef COMPONENTS_EPD_MANAGER_INCLUDE_CLOCK_DATA_H_
#define COMPONENTS_EPD_MANAGER_INCLUDE_CLOCK_DATA_H_

#include "time.h"
#include "time_info.h"
#include "strings.h"
#include "weather_icons.h"

class ClockData {
    
    private:
        
        int time_update_count = 0;    
        int minutes = -1;
        
        int date_update_count = 0;
        int day = -1;
        
        int connection_status_update_count = 0;
        bool is_connected = false;
        
        int dht_data_update_count = 0;
        float temperature = -100;
        float humidity = -100;

        int weather_data_update_count = 0;
        char weather_description [15];
        enum weather_icons weather_icon = UNKNOWN;

    public:
        ClockData(void) {}
        ~ClockData(void) {}
    
        bool has_data_changed(time_info_t *time, float temperature, float humidity, bool is_connected, 
                enum weather_icons weather_icon, char *weather_description) {
            
            return this->minutes != time->tm_min ||
                this->day != time->tm_yday ||
                this->is_connected != is_connected ||
                this->temperature != temperature ||
                this->humidity != humidity ||
                this->weather_icon != weather_icon || 
                strcmp(this->weather_description, weather_description) != 0;
        }

        bool has_time_changed(time_info_t *t) {

            bool has_time_changed = t->tm_min != minutes;
            if (has_time_changed || time_update_count %2 != 0) {
                time_update_count +=1;
                return true;
            }

            return false;
        }

        bool has_date_changed(time_info_t *t) {
            
            bool has_day_changed = t->tm_yday != day;
            if (has_day_changed || date_update_count %2 != 0) {
                date_update_count +=1;
                return true;
            }

            return false;
        }

        bool has_status_bar_changed(bool is_connected) {

            bool has_status_changed = this->is_connected != is_connected;
            if (has_status_changed || connection_status_update_count %2 != 0) {
                connection_status_update_count +=1;
                return true;
            }

            return false;
        }

        bool has_dht_data_changed(float t, float h) {
            
            bool has_data_changed = temperature != t || humidity != h;
            if (has_data_changed || dht_data_update_count %2 != 0) {
                dht_data_update_count +=1;
                return true;
            } 

            return false;
        }

        bool has_weather_data_changed(enum weather_icons weather_icon, char *weather_description) {
            
            bool has_data_changed = this->weather_icon != weather_icon || 
                strcmp(this->weather_description, weather_description) != 0;

            if (has_data_changed || weather_data_update_count %2 != 0) {
                weather_data_update_count +=1;
                return true;
            } 

            return false;
        }

        void update_data(time_info_t *time, float temperature, float humidity, bool is_connected, 
                enum weather_icons weather_icon, char *weather_description) {
            
            this->minutes = time->tm_min;
            this->day = time->tm_yday;
            this->is_connected = is_connected;
            this->temperature = temperature;
            this->humidity = humidity;
            this->weather_icon = weather_icon;
            strcpy(this->weather_description, weather_description);
        }
};

#endif