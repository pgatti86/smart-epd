#include "epd2in9.h"
#include "epdpaint.h"
#include "esp_log.h"
#include "apds_manager.h"
#include "splash_page.h"
#include "enrollment_page.h"
#include "clock_page.h"

extern "C" {
    #include "storage_manager.h"
}

#include "page_manager.h"

static const char *TAG = "PAGE MANAGER";

static int display_refresh_count = 0;

unsigned char image[EPD_WIDTH*EPD_HEIGHT/8];
Paint paint(image, 0, 0);    // width should be the multiple of 8
Epd epd;

enum EpdPage current_page = PAGE_SPLASH;
EInkPage *einkPageRef;

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

static void page_manager_draw_page() {

  if (display_refresh_count == 0) {
    page_manager_full_clear();
  }

  if (einkPageRef->draw()) {
    ESP_LOGI(TAG, "Page draw successfully");
    display_refresh_count = display_refresh_count == CONFIG_MAX_REWRITE_COUNT ? 
    0 : display_refresh_count + 1;
  }
}

static void page_manager_prepare_page(enum EpdPage page_value) {
  
  current_page = page_value;
  
  EInkPage *page = einkPageRef;

  switch (page_value) {
    case PAGE_SPLASH:
      einkPageRef = new SplashPage(&epd, &paint);
      break;
    case PAGE_ENROLLMENT:
      einkPageRef = new EnrollmentPage(&epd, &paint);
      break;
    
    default:
      einkPageRef = new ClockPage(&epd, &paint);
      current_page = PAGE_CLOCK;
      break;
    }

    delete page;
}

static void update_pages_task(void *args) {

  page_manager_full_clear();
  display_refresh_count += 1;
  page_manager_prepare_page(PAGE_SPLASH);
  page_manager_draw_page();
  vTaskDelay(2500 / portTICK_RATE_MS);

  enum EpdPage next_page = storage_manager_has_enrollment_done() ? PAGE_CLOCK : PAGE_ENROLLMENT;
  page_manager_prepare_page(next_page);

  size_t mem;
  while (1) {
    page_manager_draw_page();
    mem = heap_caps_get_free_size(MALLOC_CAP_8BIT); 
    ESP_LOGE(TAG, "Available mem: %d", mem);
    vTaskDelay(1000 / portTICK_RATE_MS);
  }
}

static void apds_gesture_handler(void* handler_args, esp_event_base_t base, int32_t id, void* event_data) {

  if (!storage_manager_has_enrollment_done()) {
    return;
  }

  int gesture_value = 0;

  switch (id) {

    case GESTURE_EVENT_LEFT:
      gesture_value = -1;
      break;

    case GESTURE_EVENT_RIGHT:
      gesture_value = 1;
      break;  
  
    default:
      gesture_value = 0;
      break;
  }

  if (gesture_value == 0) {
    return;
  }

  int selected_page_index = current_page + gesture_value;
  if (selected_page_index == PAGE_ENROLLMENT && storage_manager_has_enrollment_done()) {
    selected_page_index = current_page + (gesture_value * 2); 
  } 
  
  if (selected_page_index < 0) {
    selected_page_index = epd_page_count -1;
  } else if (selected_page_index >= epd_page_count) {
    selected_page_index = 0;
  }

  page_manager_prepare_page(static_cast<EpdPage>(selected_page_index));
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

  esp_event_handler_register(GESTURE_EVENTS, ESP_EVENT_ANY_ID, apds_gesture_handler, NULL);
}