#ifndef COMPONENTS_CLOCK_PAGE_INCLUDE_CLOCK_PAGE_H_
#define COMPONENTS_CLOCK_PAGE_INCLUDE_CLOCK_PAGE_H_

#include "eink_page.h"

class ClockPage(Epd *epd , Paint *paint): public EInkPage(epd, paint) {
    public:
        boolean draw() {
            return true;
        }

    private:
        void epd_manager_draw_grid();
        void epd_manager_draw_status_bar(bool is_connected);
        void epd_manager_draw_date(time_info_t *dst);
        void epd_manager_draw_weather(enum weather_icons weather_icon, char *description);
        void epd_manager_draw_time(time_info_t *dst);
        void epd_manager_draw_timeline(time_info_t *dst);
        void epd_manager_draw_dht_data(float t, float h);
        void epd_manager_draw_static_images();
        void epd_manager_update(time_info_t *dst, float temperature, float humidity, 
            bool is_connected, enum weather_icons weather_icon, char* weather_description);
        void epd_manager_show_enrollment(char *ssid, char *pwd, int code);

};

#endif