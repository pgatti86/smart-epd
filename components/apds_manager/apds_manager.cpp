#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "APDS9960.h"
#include "apds_manager.h"

static const char *TAG = "APDS-9960";

ESP_EVENT_DEFINE_BASE(GESTURE_EVENTS);

static SemaphoreHandle_t xSemaphore = NULL;

SparkFun_APDS9960 apds = SparkFun_APDS9960();

static void IRAM_ATTR apds_gesture_isr_handler(void* arg) {
    xSemaphoreGiveFromISR(xSemaphore, NULL);
}

static void apds_manager_gesture_handler(void *arg) {
    
    int32_t event = GESTURE_EVENT_NONE;
    
    while(1) {
        if (xSemaphoreTake(xSemaphore, portMAX_DELAY)) {
            
            if (apds.isGestureAvailable()) {
                switch (apds.readGesture()) {
                    case DIR_UP:
                        ESP_LOGI(TAG, "UP");
                        event = GESTURE_EVENT_UP;
                        break;
                    case DIR_DOWN:
                        ESP_LOGI(TAG, "DOWN");
                        event = GESTURE_EVENT_DOWN;
                        break;
                    case DIR_LEFT:
                        ESP_LOGI(TAG, "LEFT");
                        event = GESTURE_EVENT_LEFT;
                        break;
                    case DIR_RIGHT:
                        ESP_LOGI(TAG, "RIGHT");
                        event = GESTURE_EVENT_RIGHT;
                        break;
                    case DIR_NEAR:
                        ESP_LOGI(TAG, "NEAR");
                        event = GESTURE_EVENT_NEAR;
                        break;
                    case DIR_FAR:
                        ESP_LOGI(TAG, "FAR");
                        event = GESTURE_EVENT_FAR;
                        break;
                    default:
                        ESP_LOGI(TAG, "NONE");
                        event = GESTURE_EVENT_NONE;
                }

                esp_event_post(GESTURE_EVENTS, event, NULL, 0, portMAX_DELAY);
            }
        }
    }
}

void apds_manager_init() {

    gpio_num_t interrupt_pin = (gpio_num_t)CONFIG_APDS_INT;

    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_NEGEDGE;
    io_conf.pin_bit_mask = (1ULL<<CONFIG_APDS_INT);
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = (gpio_pullup_t)0;
    io_conf.pull_down_en = (gpio_pulldown_t)0;
    gpio_config(&io_conf);
    gpio_isr_handler_add(interrupt_pin, apds_gesture_isr_handler, NULL);  

    xSemaphore = xSemaphoreCreateBinary();
    xTaskCreate(apds_manager_gesture_handler, "apds_gesture_handler", 2048, NULL, 5, NULL);
   
    if (apds.init()) {
        ESP_LOGI(TAG, "APDS-9960 initialization complete");
    } else {
        ESP_LOGI(TAG, "Something went wrong during APDS-9960 init!");
    }

    // Start running the APDS-9960 gesture sensor engine
    if (apds.enableGestureSensor(true)) {
        ESP_LOGI(TAG, "Gesture sensor is now running");
    } else {
        ESP_LOGI(TAG, "Something went wrong during gesture sensor init!");
    }
}