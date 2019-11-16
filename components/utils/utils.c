#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "string.h"
#include <stdlib.h>
#include "utils.h"

#define DR_REG_RNG_BASE 0x3ff75144

static char *esp_ssid_prefix =  "ESP-";

const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";

static void utils_generate_random_string(char *dst, int size, int offset) {

    dst[size-1] = '\0';

    for(int i = offset; i < size -1; i++) {
        int key = rand() % (int) ((sizeof(charset)/sizeof(charset[0])) -1);
        dst[i] = charset[key];
    }
}

void utils_generate_random_pwd(char *dst, int size) {
    srand(READ_PERI_REG(DR_REG_RNG_BASE));
    utils_generate_random_string(dst, size, 0);
}

void utils_generate_random_ssid(char *dst, int size) {

    strncpy(dst, esp_ssid_prefix, 4);
    srand(READ_PERI_REG(DR_REG_RNG_BASE));
    utils_generate_random_string(dst, size, 4);
}

int utils_generate_random_token() {

    srand(READ_PERI_REG(DR_REG_RNG_BASE));
    return rand() % 1000000;
}