#ifndef COMPONENTS_EPD_MANAGER_INCLUDE_CLOCK_DATA_H_
#define COMPONENTS_EPD_MANAGER_INCLUDE_CLOCK_DATA_H_

#include "time.h"
#include "time_info.h"

class ClockData {
    
    private:

        int time_update_count = 0;    
        int minutes = 0;
        
        int date_update_count = 0;
        int day = 0;
        
        int connection_status_update_count = 0;
        bool is_connected = false;
        
        int dht_data_update_count = 0;
        float temperature = 0;
        float humidity = 0;

    public:
        ClockData(void) {}
        ~ClockData(void) {}
    
        bool has_data_changed(time_info_t *time, float temperature, float humidity, bool is_connected) {
            
            return this->minutes != time->tm_min ||
                this->day != time->tm_yday ||
                this->is_connected != is_connected ||
                this->temperature != temperature ||
                this->humidity != humidity;
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

        void update_data(time_info_t *time, float temperature, float humidity, bool is_connected) {
            
            this->minutes = time->tm_min;
            this->day = time->tm_yday;
            this->is_connected = is_connected;
            this->temperature = temperature;
            this->humidity = humidity;
        }
};

#endif