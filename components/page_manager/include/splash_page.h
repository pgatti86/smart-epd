#ifndef COMPONENTS_SPLASH_PAGE_INCLUDE_SPLASH_PAGE_H_
#define COMPONENTS_SPLASH_PAGE_INCLUDE_SPLASH_PAGE_H_

#include "eink_page.h"

class SplashPage: public EInkPage {

    public:

        SplashPage(Epd *epd, Paint *paint) : EInkPage {epd, paint} {}
        ~SplashPage() {}

        bool draw() {

            if (!has_drew_page) {
                has_drew_page= true;
                splash_page_show_logo();
                return true;
            }    

            return false;
        }

    private:
        
        bool has_drew_page = false;

        void splash_page_show_logo() {

            Epd *epd = getEpd();
            Paint *paint = getPaint();

            eink_page_set_paint(SCREEN_WIDTH, SCREEN_HEIGHT, UNCOLORED);
            paint->DrawStringAt(8, 24, "SMART", &Font40, COLORED);
            paint->DrawStringAt(176, 72, "EPD", &Font40, COLORED);
            eink_page_draw_paint(0, 0);
            epd->DisplayFrame();
        }   
};


#endif