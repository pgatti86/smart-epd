#ifndef COMPONENTS_CLOCK_PAGE_INCLUDE_CLOCK_PAGE_H_
#define COMPONENTS_CLOCK_PAGE_INCLUDE_CLOCK_PAGE_H_

#include "eink_page.h"
#include "clock_data_buffer.h"
#include "strings.h"
#include "epd_images.h"

extern "C" {
    #include "dht_manager.h"
    #include "time_manager.h"
    #include "time_formatter.h"   
    #include "weather_manager.h"
    #include "wifi_manager.h"
}

//TODO change methods name
class ClockPage: public EInkPage {
    
    public:

        ClockPage(Epd *epd, Paint *paint) : EInkPage {epd, paint} {}
        ~ClockPage() {}

        bool draw() {
            return clock_page_update();
        }

    private:

        static constexpr char *TAG = "Clock Page";

        unsigned long update_count = 0;

        ClockDataBuffer buffer1, buffer2;
        ClockDataBuffer *currentBuffer = &buffer1;

        void clock_page_draw_grid() {
        
            Paint *paint = getPaint();

            int grid_height = 40;
            eink_page_set_paint(SCREEN_WIDTH, SCREEN_HEIGHT, UNCOLORED);
            paint->DrawHorizontalLine(0, SCREEN_HEIGHT - grid_height, SCREEN_WIDTH, COLORED);
            paint->DrawVerticalLine(98, SCREEN_HEIGHT - grid_height, grid_height, COLORED);
            paint->DrawVerticalLine(196, SCREEN_HEIGHT - grid_height, grid_height, COLORED);

            eink_page_draw_paint(0, 0);
        }

        void clock_page_draw_status_bar(bool is_connected) {
            
            Epd *epd = getEpd();

            if (!is_connected) {
                epd->SetFrameMemory(NO_WIFI_IMAGE_DATA, SCREEN_HEIGHT - 16, SCREEN_WIDTH - 24, 16, 16);
            } else {
                eink_page_set_paint(16, 16, UNCOLORED);
                eink_page_draw_paint(SCREEN_WIDTH - 24, 0);
            }
        }

        void clock_page_draw_date(time_info_t *dst) {

            Paint *paint = getPaint();
  
            char day_buffer [10];
            time_formatter_format_current_day(dst, day_buffer);

            char date_buffer[7];
            time_formatter_format_current_date(dst, date_buffer);

            int paint_width = 96;
            int paint_height = 40;
            eink_page_set_paint(paint_width, paint_height, UNCOLORED);
            paint->DrawStringAt(8, 8, day_buffer, &Font12, COLORED);
            paint->DrawStringAt(8, 24, date_buffer, &Font16, COLORED);
            eink_page_draw_paint(0, SCREEN_HEIGHT - paint_height);
        }

        void clock_page_draw_weather(enum weather_icons weather_icon) {
  
            const unsigned char *icon;

            switch (weather_icon) {
                case CLEAR_SKY_DAY:
                icon = WEATHER_CLEAR_D_IMAGE_DATA;
                break;
                case CLEAR_SKY_NIGHT:
                icon = WEATHER_CLEAR_N_IMAGE_DATA;
                break;
                case FEW_CLOUDS_DAY:
                icon = WEATHER_FEW_CLOUD_D_IMAGE_DATA;
                break;
                case FEW_CLOUDS_NIGHT:
                icon = WEATHER_FEW_CLOUDS_N_IMAGE_DATA;
                break;
                case CLOUDS:
                icon = WEATHER_CLOUDS_IMAGE_DATA;
                break;
                case SHOWER_RAIN:
                icon = WEATHER_SHOWER_IMAGE_DATA;
                break;
                case RAIN:
                icon = WEATHER_RAIN_IMAGE_DATA;
                break;
                case THUNDERSTORM:
                icon = WEATHER_THUNDERSTORM_IMAGE_DATA;
                break;
                case SNOW:
                icon = WEATHER_SNOW_IMAGE_DATA;
                break;
                case FOG:
                icon = WEATHER_FOG_IMAGE_DATA;
                break;
                default:
                icon = WEATHER_FEW_CLOUD_D_IMAGE_DATA;
                break;
            }

            Epd *epd = getEpd();
            epd->SetFrameMemory(icon, 56, 32, 48, 48);
        }

        void clock_page_draw_time(time_info_t *dst) {
            
            Paint *paint = getPaint();

            char time_buffer [20];
            time_formatter_format_current_time(dst, time_buffer);
            eink_page_set_paint(175, 40, UNCOLORED);
            paint->DrawStringAt(0, 0, time_buffer, &Font40, COLORED);
            eink_page_draw_paint(104, 24);
        }

        void clock_page_draw_timeline(time_info_t *dst) {
            
            Paint *paint = getPaint();

            int timeline_height = 8;
            int timeline_width = (SCREEN_WIDTH / 4) * ((dst->tm_sec / 15) + 1);
            eink_page_set_paint(SCREEN_WIDTH, timeline_height, UNCOLORED);
            paint->DrawHorizontalLine(timeline_width, timeline_height / 2, SCREEN_WIDTH, COLORED);
            paint->DrawFilledRectangle(0, 0, timeline_width, timeline_height / 2, COLORED);
            eink_page_draw_paint(0, 0);
        }

        void clock_page_draw_dht_data(float t, float h) {

            Paint *paint = getPaint();

            char temp_buffer [5];
            sprintf(temp_buffer, "%.1fC", t); 
            eink_page_set_paint(64, 24, UNCOLORED);
            paint->DrawStringAt(0, 0, temp_buffer, &Font16, COLORED);
            eink_page_draw_paint(132, SCREEN_HEIGHT - paint->GetWidth());

            char hum_buffer [5];
            sprintf(hum_buffer, "%.1f%%", h); 
            eink_page_set_paint(64, 24, UNCOLORED);
            paint->DrawStringAt(0, 0, hum_buffer, &Font16, COLORED);
            eink_page_draw_paint(232, SCREEN_HEIGHT - paint->GetWidth());
        }

        void clock_page_draw_static_images() {
            
            Epd *epd = getEpd();
            epd->SetFrameMemory(TEMP_IMAGE_DATA, 8, 104, 24, 24);
            epd->SetFrameMemory(DROP_IMAGE_DATA, 8, 204, 24, 24);
        }

        bool clock_page_update() {

            time_info_t dst;
            time_manager_get_current_date_time(&dst);

            float t, h;
            dht_manager_get_last_red_values(&t, &h);

            struct weather_data *w_data = weather_manager_get_weather_data();
            bool is_connected = wifi_manager_is_sta_connected();

            bool force_update = update_count < 2;
            bool update_timeline = (dst.tm_sec % 15) == 0;
            bool need_update = currentBuffer->is_connected != is_connected 
                                || currentBuffer->day != dst.tm_yday
                                || currentBuffer->minutes != dst.tm_min
                                || update_timeline
                                || (currentBuffer->temperature != t || currentBuffer->humidity != h)
                                || (currentBuffer->weather_icon != w_data->weather_icon);
            
            ClockDataBuffer *previousBuffer = currentBuffer == &buffer1 ? &buffer2 : &buffer1;
            
            if (force_update) {
                ESP_LOGI(TAG, "Drawing grid and static images");
                clock_page_draw_grid();
                clock_page_draw_static_images();
            }

            if (force_update || (need_update && previousBuffer->is_connected != is_connected)) {
                ESP_LOGI(TAG, "Drawing status bar");
                clock_page_draw_status_bar(is_connected);
            }

            if (force_update || (need_update && previousBuffer->weather_icon != w_data->weather_icon)) {
                ESP_LOGI(TAG, "Drawing weather");
                clock_page_draw_weather(w_data->weather_icon);
            } 

            if (force_update || (need_update && previousBuffer->day != dst.tm_yday)) {
                ESP_LOGI(TAG, "Drawing date");
                clock_page_draw_date(&dst);
            }

            if (force_update || (need_update && previousBuffer->minutes != dst.tm_min)) {
                ESP_LOGI(TAG, "Drawing time");
                clock_page_draw_time(&dst); 
            }

            if (force_update || need_update) {
                ESP_LOGI(TAG, "Drawing timeline");
                clock_page_draw_timeline(&dst);
            }
            
            if (force_update || (need_update && (previousBuffer->temperature != t || previousBuffer->humidity != h))) {
                ESP_LOGI(TAG, "Drawing dht");
                clock_page_draw_dht_data(t, h);
            } 

            bool must_update = force_update || need_update;

            if (must_update) {
                
                update_count ++;

                Epd *epd = getEpd();
                epd->DisplayFrame();

                previousBuffer->is_connected = is_connected;
                previousBuffer->day = dst.tm_yday;
                previousBuffer->minutes = dst.tm_min;
                previousBuffer->temperature = t;
                previousBuffer->humidity = h;
                strcpy(previousBuffer->weather_description, w_data->weather_description);
                previousBuffer->weather_icon = w_data->weather_icon;
                
                currentBuffer = previousBuffer;  
            }

            return must_update;
        }
};

#endif