#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "string.h"
#include <stdlib.h>
#include "utils.h"

static char *esp_ssid_prefix =  "ESP-";

const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";

static void utils_generate_random_string(char *dst, int size, int offset) {

    dst[size-1] = '\0';

    for(int i = offset; i < size -1; i++) {
        int key = esp_random() % (int) ((sizeof(charset)/sizeof(charset[0])) -1);
        dst[i] = charset[key];
    }
}

void utils_generate_random_pwd(char *dst, int size) {

    utils_generate_random_string(dst, size, 0);
}

void utils_generate_random_ssid(char *dst, int size) {

    memcpy(dst, esp_ssid_prefix, 4);
    utils_generate_random_string(dst, size, 4);
}

int utils_generate_random_token() {

    return esp_random() % 1000000;
}
