#ifndef COMPONENTS_WEATHER_PAGE_INCLUDE_WEATHER_PAGE_H_
#define COMPONENTS_WEATHER_PAGE_INCLUDE_WEATHER_PAGE_H_

#include "eink_page.h"
#include "weather_manager.h"

class WeatherPage: public EInkPage {
    
    public:

        WeatherPage(Epd *epd, Paint *paint) : EInkPage {epd, paint} {}
        ~WeatherPage() {}

        bool draw() {
            return weather_page_update();
        }

    private:

        void weather_page_draw_weather_icon(enum weather_icons weather_icon) {
  
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
            epd->SetFrameMemory(icon, 72, 8, 48, 48);
        }

        bool weather_page_update() {

            struct weather_data *w_data = weather_manager_get_weather_data();
            weather_page_draw_weather_icon(w_data->weather_icon);

            Epd *epd = getEpd();
            epd->DisplayFrame();

            return true;
        }
};        

#endif