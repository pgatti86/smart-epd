#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#define ESP_INTR_FLAG_DEFAULT 0
#define DEBOUNCE_TIME 300
#define LONG_CLICK_THRESHOLD 3000

static xQueueHandle gpio_evt_queue = NULL;

uint32_t getTimeSinceStart() {
	return (uint32_t) (xTaskGetTickCount() * portTICK_PERIOD_MS);
}

static void IRAM_ATTR gpio_isr_handler(void* arg) {
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

static void gpio_isr_task() {

    uint32_t io_num;
    uint64_t last_btn_pressed_time = 0;
    
    for(;;) {
        if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            
            uint64_t current = getTimeSinceStart();

            if ((current - last_btn_pressed_time) > DEBOUNCE_TIME) {
            
                while(!gpio_get_level(io_num)) {
                    vTaskDelay(200 / portTICK_RATE_MS);
                }

                if(getTimeSinceStart() - current >= LONG_CLICK_THRESHOLD) {
                    printf("long click\n\r");
                } else {
                    printf("short click\n\r");
                }   

                last_btn_pressed_time = getTimeSinceStart();
            }
        }
    }
}

void gpio_manger_init() {

    //create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(5, sizeof(uint32_t));
    //start gpio task
    xTaskCreate(gpio_isr_task, "gpio_isr_task", 2048, NULL, 5, NULL);
    
    // configure button and led pins as GPIO pins
    gpio_pad_select_gpio(CONFIG_BUTTON_GPIO);

    gpio_config_t io_conf;

    //interrupt on falling edge
    io_conf.intr_type = GPIO_PIN_INTR_NEGEDGE;
    //bit mask of the pins
    io_conf.pin_bit_mask = (1ULL<<CONFIG_BUTTON_GPIO);
    //set as input mode    
    io_conf.mode = GPIO_MODE_INPUT;
    //enable pull-up mode
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    //disable pull-down mode
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    
    gpio_config(&io_conf);

    //install gpio isr service
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(CONFIG_BUTTON_GPIO, gpio_isr_handler, (void*) CONFIG_BUTTON_GPIO);
}