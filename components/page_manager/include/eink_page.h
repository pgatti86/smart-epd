#ifndef COMPONENTS_EINK_PAGE_INCLUDE_EINK_PAGE_H_
#define COMPONENTS_EINK_PAGE_INCLUDE_EINK_PAGE_H_

#include "esp_log.h"
#include "epd2in9.h"
#include "epdpaint.h"

class EInkPage {
    public:
        static const int SCREEN_WIDTH = EPD_HEIGHT;
        static const int SCREEN_HEIGHT = EPD_WIDTH;
  
        EInkPage(Epd *epd, Paint *paint) {
            r_epd = epd;
            r_paint = paint;
        }

        virtual ~EInkPage() {}
    
        virtual bool draw() = 0;

        void eink_page_set_paint(int width, int height, int clear_color) { 
            r_paint->Clear(clear_color);
            r_paint->SetWidth(height);
            r_paint->SetHeight(width);
        }

        void eink_page_draw_paint(int x, int y) {
            int x_abs_coord = EPD_WIDTH - r_paint->GetWidth() - y;
            int y_abs_coord = x;
            r_epd->SetFrameMemory(r_paint->GetImage(), x_abs_coord , y_abs_coord, r_paint->GetWidth(), r_paint->GetHeight());
        }
        
        Epd* getEpd() { return r_epd; };
        Paint* getPaint() { return r_paint; };

    private:
        Epd *r_epd;
        Paint *r_paint;

};

#endif