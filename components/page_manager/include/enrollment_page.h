#ifndef COMPONENTS_ENROLLMENT_PAGE_INCLUDE_ENROLLMENT_PAGE_H_
#define COMPONENTS_ENROLLMENT_PAGE_INCLUDE_ENROLLMENT_PAGE_H_

#include "eink_page.h"

extern "C" {
    #include "enrollment_manager.h"
}

class EnrollmentPage: public EInkPage {

    public:

        EnrollmentPage(Epd *epd, Paint *paint) : EInkPage { epd, paint} {}
        ~EnrollmentPage() {}

        bool draw() {

            if (!has_drew_page) {
                has_drew_page= true;
                epd_manager_show_enrollment();
                return true;
            }    

            return false;
        }

    private:
    
        static constexpr char *TAG = "Enrollment Page";
        
        bool has_drew_page = false;
        
        void epd_manager_show_enrollment() {

            eink_page_set_paint(SCREEN_WIDTH, SCREEN_HEIGHT, UNCOLORED);

            Epd *epd = getEpd();
            Paint *paint = getPaint();

            char *ssid =  enrollment_manager_get_ssid();
            ESP_LOGI(TAG, "enrollment ssid: %s", ssid);
            char *pwd = enrollment_manager_get_password();
            ESP_LOGI(TAG, "enrollment password: %s", pwd);
            int token = enrollment_manager_get_verification_code();
            ESP_LOGI(TAG, "enrollment token: %d", token);

            paint->DrawStringAt(10, 10, "ENROLLMENT MODE:", &Font24, COLORED);

            char ssid_data[30];
            sprintf(ssid_data, "ssid: %s", ssid);
            paint->DrawStringAt(10, 40, ssid_data, &Font16, COLORED);

            char pwd_data[30];
            sprintf(pwd_data, "pwd: %s", pwd);
            paint->DrawStringAt(10, 65, pwd_data, &Font16, COLORED);

            char pop_data[30];
            sprintf(pop_data, "code: %d", token);
            paint->DrawStringAt(10, 90, pop_data, &Font16, COLORED);

            eink_page_draw_paint(0, 0);
            epd->DisplayFrame();
        }
};


#endif