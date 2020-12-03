#include "epd2in9.h"
#include "epdpaint.h"
#include "esp_log.h"
#include "enrollment_page.h"

extern "C" {
    #include "storage_manager.h"
}

#include "page_manager.h"

static const char *TAG = "PAGE MANAGER";

static int display_refresh_count = 0;

unsigned char image[EPD_WIDTH*EPD_HEIGHT/8];
Paint paint(image, 0, 0);    // width should be the multiple of 8
Epd epd;

EInkPage *currentPage;

static void page_manager_clear() {

  ESP_LOGI(TAG, "Clearing display...");
  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();

  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();
}

static void page_manager_full_clear() {
  
  epd.Init(lut_full_update);
  page_manager_clear(); 
  epd.Init(lut_partial_update);
}

static void page_manager_update_page() {

    if (display_refresh_count == 0) {
        page_manager_full_clear();
    }

    if (currentPage->draw()) {
      ESP_LOGI(TAG, "Page draw successfully");
      display_refresh_count = display_refresh_count == CONFIG_MAX_REWRITE_COUNT ? 
      0 : display_refresh_count + 1;
    }
}

static void update_pages_task(void *args) {

  if (!storage_manager_has_enrollment_done()) {
    
  } 
  
  currentPage = new EnrollmentPage(&epd, &paint);

  while (1) {
    page_manager_update_page();
    vTaskDelay(1000 / portTICK_RATE_MS);
  }
}

void page_manager_init() {

  if (epd.Init(lut_partial_update) != 0) {
    ESP_LOGE(TAG, "e-Paper init failed");
    return;
  }

  ESP_LOGI(TAG, "e-Paper initialized");
  paint.SetWidth(EPD_WIDTH);
  paint.SetHeight(EPD_HEIGHT);
  paint.SetRotate(ROTATE_90);
  
  xTaskCreate(&update_pages_task, "update_pages_task", 4096, NULL, 5, NULL); 

  //TODO listen for gesture sensor
}