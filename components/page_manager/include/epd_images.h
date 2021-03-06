
#ifndef __EPD_IMAGES_H__
#define __EPD_IMAGES_H__

//note: const data are keep in FLASH memory

//'ic_drop' 24x24
const unsigned char DROP_IMAGE_DATA[] = {
/* 0X00,0X01,0X80,0X00,0X28,0X01, */
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x0f, 0xff, 0xe0, 
0x07, 0xff, 0xc0, 0x01, 0xff, 0xc3, 0x80, 0x7f, 0x8e, 0x00, 0x3f, 0x8c, 0x00, 0x0f, 0x18, 0x00, 
0x01, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x0f, 0x80, 0x00, 0x3f, 
0xc0, 0x00, 0x7f, 0xc0, 0x01, 0xff, 0xe0, 0x07, 0xff, 0xf8, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
};

// 'ic_termometer', 24x24px
const unsigned char TEMP_IMAGE_DATA[] = {
/* 0X00,0X01,0X80,0X00,0X28,0X01, */
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xf0, 0xff, 0xff, 0xc0, 0x3f, 0xff, 0x86, 0x00, 0x03, 0x99, 0x80, 0x01, 0x10, 0x7f, 
0xf8, 0x20, 0x00, 0x3c, 0x20, 0x00, 0x3c, 0x10, 0x7f, 0xf8, 0x99, 0x80, 0x01, 0x86, 0x00, 0x03, 
0xc0, 0x3f, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

// 'ic_wifi', 16x16px
const unsigned char WIFI_IMAGE_DATA[] = {
/* 0X00,0X01,0X80,0X00,0X28,0X01, */
0xff, 0xdf, 0xff, 0x9f, 0xff, 0xcf, 0xfe, 0xc7, 0xfc, 0xe7, 0xfe, 0x67, 0xf6, 0x67, 0xe6, 0x73, 
0xe6, 0x73, 0xf6, 0x67, 0xfe, 0x67, 0xfc, 0xe7, 0xfe, 0xc7, 0xff, 0xcf, 0xff, 0x9f, 0xff, 0xdf
};

// 'ic_no_wifi', 16x16px
const unsigned char NO_WIFI_IMAGE_DATA[] = {
/* 0X00,0X01,0X80,0X00,0X28,0X01, */
0xff, 0xbf, 0xff, 0x3b, 0xff, 0x93, 0xfd, 0x87, 0xf8, 0xcf, 0xfc, 0x9f, 0xec, 0x37, 0xcc, 0x67, 
0xcc, 0xc7, 0xe9, 0xc7, 0xf3, 0xcf, 0xe6, 0xcf, 0xcd, 0x8f, 0xdf, 0x9f, 0xff, 0x1f, 0xff, 0xbf 
};

// 'sun-cloud', 48x48px
const unsigned char WEATHER_FEW_CLOUD_D_IMAGE_DATA[] = {
0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0x8f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f, 
0xff, 0xff, 0xff, 0xff, 0xff, 0x07, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x07, 0xff, 0xff, 0xff, 0xfc, 
0x3e, 0x03, 0xe3, 0xff, 0xff, 0xfc, 0x04, 0x03, 0x03, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x03, 0xff, 
0xff, 0xfe, 0x00, 0x20, 0x03, 0xff, 0xff, 0xfe, 0x01, 0xfc, 0x07, 0xff, 0xff, 0xff, 0x07, 0xff, 
0x07, 0xff, 0xff, 0xff, 0x0e, 0x03, 0x87, 0xff, 0xff, 0xff, 0x9c, 0x01, 0x87, 0xff, 0xff, 0xc3, 
0xf8, 0x00, 0xc3, 0xff, 0xff, 0x00, 0xf8, 0x00, 0xc0, 0xff, 0xfe, 0x00, 0x70, 0x00, 0xe0, 0x3f, 
0xfc, 0x00, 0x38, 0x00, 0x60, 0x1f, 0xf8, 0x00, 0x18, 0x00, 0x60, 0x1f, 0xf8, 0x00, 0x1f, 0x00, 
0xc0, 0x7f, 0xf8, 0x00, 0x1f, 0xc0, 0xc1, 0xff, 0xf8, 0x00, 0x01, 0xe1, 0xc7, 0xff, 0xf8, 0x00, 
0x00, 0x73, 0x87, 0xff, 0xf8, 0x00, 0x00, 0x3f, 0x07, 0xff, 0xf8, 0x00, 0x00, 0x1e, 0x07, 0xff, 
0xf8, 0x00, 0x00, 0x1c, 0x03, 0xff, 0xf8, 0x00, 0x00, 0x0c, 0x03, 0xff, 0xf8, 0x00, 0x00, 0x0c, 
0x03, 0xff, 0xf8, 0x00, 0x00, 0x0f, 0x83, 0xff, 0xf8, 0x00, 0x00, 0x0f, 0xf3, 0xff, 0xf8, 0x00, 
0x00, 0x0f, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x1f, 0xff, 0xff, 
0xf8, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x7f, 
0xff, 0xff, 0xf8, 0x00, 0x00, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x01, 0xff, 0xff, 0xff, 0xf8, 0x00, 
0x00, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0xff, 0xff, 0xff, 
0xf8, 0x00, 0x01, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x01, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x03, 0xff, 
0xff, 0xff, 0xf8, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xfe, 0x00, 
0x7f, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc3, 0xff, 0xff, 0xff, 0xff
};

// 'rain', 48x48px
const unsigned char WEATHER_SHOWER_IMAGE_DATA[] = {
0x9f, 0xff, 0xff, 0x03, 0xff, 0xff, 0x07, 0xff, 0xfc, 0x00, 0xff, 0xff, 0x83, 0xff, 0xf8, 0x00, 
0x7f, 0xff, 0xc0, 0xff, 0xf0, 0x00, 0x3f, 0xff, 0xf0, 0x3f, 0xe0, 0x00, 0x1f, 0xff, 0xf8, 0x0f, 
0xe0, 0x00, 0x1f, 0xff, 0xfe, 0x07, 0xe0, 0x00, 0x00, 0xff, 0xff, 0x87, 0xc0, 0x00, 0x00, 0x1f, 
0xff, 0xe7, 0xc0, 0x00, 0x00, 0x0f, 0x9f, 0xff, 0xc0, 0x00, 0x00, 0x07, 0x07, 0xff, 0xc0, 0x00, 
0x00, 0x03, 0x83, 0xff, 0xc0, 0x00, 0x00, 0x01, 0xc0, 0xff, 0xc0, 0x00, 0x00, 0x01, 0xf0, 0x3f, 
0xc0, 0x00, 0x00, 0x01, 0xf8, 0x0f, 0xc0, 0x00, 0x00, 0x00, 0xfe, 0x07, 0xc0, 0x00, 0x00, 0x00, 
0xff, 0x87, 0xc0, 0x00, 0x00, 0x00, 0xff, 0xe7, 0xc0, 0x00, 0x00, 0x00, 0x9f, 0xff, 0xc0, 0x00, 
0x00, 0x00, 0x07, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x83, 0xff, 0xc0, 0x00, 0x00, 0x01, 0xc0, 0xff, 
0xc0, 0x00, 0x00, 0x01, 0xf0, 0x3f, 0xc0, 0x00, 0x00, 0x03, 0xf8, 0x0f, 0xc0, 0x00, 0x00, 0x07, 
0xfe, 0x07, 0xc0, 0x00, 0x00, 0x0f, 0xff, 0x87, 0xc0, 0x00, 0x00, 0x1f, 0xff, 0xe7, 0xc0, 0x00, 
0x00, 0x1f, 0x9f, 0xff, 0xc0, 0x00, 0x00, 0x0f, 0x07, 0xff, 0xc0, 0x00, 0x00, 0x0f, 0x83, 0xff, 
0xc0, 0x00, 0x00, 0x07, 0xc0, 0xff, 0xc0, 0x00, 0x00, 0x07, 0xf0, 0x3f, 0xc0, 0x00, 0x00, 0x07, 
0xf8, 0x0f, 0xc0, 0x00, 0x00, 0x07, 0xfe, 0x07, 0xc0, 0x00, 0x00, 0x07, 0xff, 0x87, 0xc0, 0x00, 
0x00, 0x0f, 0xff, 0xe7, 0xc0, 0x00, 0x00, 0x0f, 0x9f, 0xff, 0xc0, 0x00, 0x00, 0x1f, 0x07, 0xff, 
0xc0, 0x00, 0x00, 0x3f, 0x83, 0xff, 0xc0, 0x00, 0x00, 0xff, 0xc0, 0xff, 0xc0, 0x00, 0x0f, 0xff, 
0xf0, 0x3f, 0xc0, 0x00, 0x0f, 0xff, 0xf8, 0x0f, 0xe0, 0x00, 0x0f, 0xff, 0xfe, 0x07, 0xe0, 0x00, 
0x1f, 0xff, 0xff, 0x87, 0xe0, 0x00, 0x1f, 0xff, 0xff, 0xe7, 0xf0, 0x00, 0x3f, 0xff, 0xff, 0xff, 
0xf8, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xfc, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x87, 0xff, 0xff
};

// 'thunderstorm', 48x48px
const unsigned char WEATHER_THUNDERSTORM_IMAGE_DATA[] = {
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 
0x1f, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x01, 0xff, 0xff, 
0xf8, 0x00, 0x00, 0x00, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 
0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x87, 0xff, 0xf8, 0x00, 
0x00, 0x00, 0x00, 0x7f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x07, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x78, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 
0x60, 0x00, 0xff, 0xc0, 0x00, 0x00, 0x7c, 0x00, 0xff, 0xf0, 0x00, 0x00, 0x7f, 0x80, 0xff, 0xfc, 
0x00, 0x00, 0x7f, 0xf1, 0xff, 0xfe, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0x80, 0x00, 0x7f, 0xff, 
0xff, 0xff, 0xe0, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xfc, 0x00, 
0x7f, 0xff, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x7f, 0xff, 0xff, 0xff, 
0xff, 0xf0, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};


// 'cloud-moon', 48x48px
const unsigned char WEATHER_FEW_CLOUDS_N_IMAGE_DATA[] = {
0xff, 0x07, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x01, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0xff, 0xff, 
0xff, 0xff, 0xf0, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xe0, 0x00, 
0x0f, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x01, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0xff, 0xff, 0xff, 
0xe0, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x3f, 
0xff, 0xff, 0xe0, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xe0, 0x00, 
0x00, 0x1f, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x3f, 0xff, 0xff, 
0xe0, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x7f, 
0xff, 0xff, 0xe0, 0x00, 0x00, 0xf3, 0xff, 0xff, 0xe0, 0x00, 0x01, 0xc0, 0x1f, 0xff, 0xe0, 0x00, 
0x01, 0xc0, 0x07, 0xff, 0xe0, 0x00, 0x00, 0xc0, 0x01, 0xff, 0xe0, 0x00, 0x00, 0xe0, 0x00, 0xff, 
0xe0, 0x00, 0x00, 0xe0, 0x00, 0x7f, 0xe0, 0x00, 0x00, 0xc0, 0x00, 0x3f, 0xe0, 0x00, 0x01, 0xc0, 
0x00, 0x1f, 0xe0, 0x00, 0x01, 0xc0, 0x00, 0x1f, 0xe0, 0x00, 0x07, 0x80, 0x00, 0x0f, 0xe0, 0x00, 
0xff, 0x00, 0x00, 0x0f, 0xe0, 0x01, 0xfe, 0x00, 0x00, 0x0f, 0xf0, 0x01, 0xf8, 0x00, 0x00, 0x07, 
0xf8, 0x03, 0x80, 0x03, 0xe0, 0x07, 0xfc, 0x07, 0x80, 0x1f, 0xfc, 0x07, 0xff, 0xbf, 0x00, 0x3f, 
0xff, 0x07, 0xff, 0xfe, 0x00, 0xff, 0xff, 0x87, 0xff, 0xfc, 0x01, 0xff, 0xff, 0xc7, 0xff, 0xfc, 
0x01, 0xff, 0xff, 0xe7, 0xff, 0xfc, 0x03, 0xff, 0xff, 0xf7, 0xff, 0xfc, 0x07, 0xff, 0xff, 0xff, 
0xff, 0xfc, 0x07, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x07, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x07, 0xff, 
0xff, 0xff, 0xff, 0xff, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 
0x8f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xef, 0xff, 0xff, 0xff
};

// 'rain', 48x48px
const unsigned char WEATHER_RAIN_IMAGE_DATA[] = {
0xff, 0xff, 0xff, 0x03, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 
0x7f, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x1f, 0xff, 0xc0, 0xff, 
0xe0, 0x00, 0x1f, 0xff, 0x80, 0x7f, 0xe0, 0x00, 0x00, 0xff, 0x00, 0x1f, 0xc0, 0x00, 0x00, 0x1f, 
0x00, 0x07, 0xc0, 0x00, 0x00, 0x0f, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x07, 0x00, 0x1f, 0xc0, 0x00, 
0x00, 0x03, 0x80, 0x7f, 0xc0, 0x00, 0x00, 0x01, 0xc0, 0xff, 0xc0, 0x00, 0x00, 0x01, 0xff, 0xff, 
0xc0, 0x00, 0x00, 0x01, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 
0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0xff, 0xff, 0xc0, 0x00, 
0x00, 0x00, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0xc0, 0xff, 0xc0, 0x00, 0x00, 0x01, 0x80, 0x7f, 
0xc0, 0x00, 0x00, 0x01, 0x00, 0x1f, 0xc0, 0x00, 0x00, 0x03, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x07, 
0x00, 0x07, 0xc0, 0x00, 0x00, 0x0f, 0x00, 0x1f, 0xc0, 0x00, 0x00, 0x1f, 0x80, 0x7f, 0xc0, 0x00, 
0x00, 0x1f, 0xc0, 0xff, 0xc0, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x0f, 0xff, 0xff, 
0xc0, 0x00, 0x00, 0x07, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x07, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x07, 
0xff, 0xff, 0xc0, 0x00, 0x00, 0x07, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x07, 0xff, 0xff, 0xc0, 0x00, 
0x00, 0x0f, 0xc0, 0xff, 0xc0, 0x00, 0x00, 0x0f, 0x80, 0x7f, 0xc0, 0x00, 0x00, 0x1f, 0x00, 0x1f, 
0xc0, 0x00, 0x00, 0x3f, 0x00, 0x07, 0xc0, 0x00, 0x00, 0xff, 0x00, 0x07, 0xc0, 0x00, 0x0f, 0xff, 
0x00, 0x1f, 0xc0, 0x00, 0x0f, 0xff, 0x80, 0x7f, 0xe0, 0x00, 0x0f, 0xff, 0xc0, 0xff, 0xe0, 0x00, 
0x1f, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x3f, 0xff, 0xff, 0xff, 
0xf8, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xfc, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x87, 0xff, 0xff
};

// 'clouds', 48x48px
const unsigned char WEATHER_CLOUDS_IMAGE_DATA[] = {
0xff, 0xff, 0x07, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x01, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x7f, 
0xff, 0xff, 0xff, 0xc0, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x1f, 0xff, 0xff, 0xff, 0x80, 
0x00, 0x1f, 0xff, 0xff, 0xff, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0x00, 0x00, 0x03, 0xff, 0xff, 
0xff, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xfe, 0x00, 0x00, 0x00, 
0x07, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x03, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x03, 0xff, 0xfe, 0x00, 
0x00, 0x00, 0x01, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0xff, 
0xfe, 0x00, 0x00, 0x00, 0x00, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0xff, 0xfe, 0x00, 0x00, 0x00, 
0x00, 0x7f, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xfe, 0x00, 
0x00, 0x00, 0x00, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0xff, 
0xfe, 0x00, 0x00, 0x00, 0x01, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x01, 0xff, 0xfe, 0x00, 0x00, 0x00, 
0x03, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x07, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xfe, 0x00, 
0x00, 0x00, 0x1f, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x0f, 0xff, 
0xfe, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xfe, 0x00, 0x00, 0x00, 
0x0f, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xfe, 0x00, 
0x00, 0x00, 0x1f, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x7f, 0xff, 
0xff, 0x00, 0x00, 0x23, 0xff, 0xff, 0xff, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0x00, 0x00, 0x7f, 
0xff, 0xff, 0xff, 0x80, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xc0, 0x00, 0xff, 0xff, 0xff, 0xff, 0xe0, 
0x01, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x03, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x1f, 0xff, 0xff, 0xff
};

// 'clear night', 48x48px
const unsigned char WEATHER_CLEAR_N_IMAGE_DATA[] = {
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xc0, 0x03, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xf8, 
0x00, 0x00, 0x1f, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x07, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x03, 0xff, 
0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xfc, 0x00, 0x00, 0x00, 
0x00, 0x3f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xf0, 0x00, 
0x00, 0x00, 0x00, 0x0f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x07, 
0xc0, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0x00, 
0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 
0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x07, 0xff, 0xf8, 0x00, 
0x00, 0x00, 0x0f, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0x00, 0x00, 0x00, 0x7f, 0xff, 
0xff, 0x80, 0x00, 0x00, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x01, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x03, 
0xff, 0xff, 0xff, 0xf0, 0x00, 0x07, 0xff, 0xff, 0xff, 0xf8, 0x80, 0x07, 0xff, 0xff, 0xff, 0xfd, 
0x80, 0x0f, 0xff, 0xff, 0xff, 0xff, 0x80, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x1f, 0xff, 0xff, 
0xff, 0xff, 0xc0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x1f, 
0xff, 0xff, 0xff, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x3f, 0xff, 0xff, 0xff, 0xff, 
0xf8, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x1f, 0xff, 0xff, 
0xff, 0xff, 0xfe, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

// 'fog', 48x48px
const unsigned char WEATHER_FOG_IMAGE_DATA[] = {
0xf8, 0xff, 0xff, 0xfc, 0x1f, 0xff, 0xf8, 0xff, 0xff, 0xe0, 0x03, 0xff, 0xf8, 0xff, 0xff, 0xc0, 
0x01, 0xff, 0xf8, 0xff, 0xff, 0x80, 0x00, 0xff, 0xf8, 0xff, 0xff, 0x00, 0x00, 0x7f, 0xf8, 0xf1, 
0xfe, 0x00, 0x00, 0x3f, 0xf8, 0xf0, 0xfe, 0x00, 0x00, 0x3f, 0xf8, 0xf0, 0xfc, 0x00, 0x00, 0x1f, 
0xf8, 0xf0, 0xfc, 0x00, 0x00, 0x1f, 0xf8, 0xf0, 0xfc, 0x00, 0x00, 0x1f, 0xf8, 0xf0, 0xfc, 0x00, 
0x00, 0x1f, 0xf8, 0xf0, 0xfc, 0x00, 0x00, 0x1f, 0xff, 0xf0, 0xfc, 0x00, 0x00, 0x1f, 0xff, 0xf0, 
0xfc, 0x00, 0x00, 0x1f, 0xff, 0xf0, 0xfc, 0x00, 0x00, 0x3f, 0xff, 0xf0, 0xfc, 0x00, 0x00, 0x3f, 
0xf8, 0xf0, 0xfc, 0x00, 0x00, 0x7f, 0xf8, 0xf0, 0xfc, 0x00, 0x00, 0x7f, 0xf8, 0xf0, 0xfc, 0x00, 
0x00, 0xff, 0xf8, 0xf0, 0xfc, 0x00, 0x00, 0xff, 0xf8, 0xf0, 0xfc, 0x00, 0x00, 0x7f, 0xf8, 0xf0, 
0xfc, 0x00, 0x00, 0x3f, 0xf8, 0xf0, 0xfc, 0x00, 0x00, 0x3f, 0xf8, 0xf0, 0xf8, 0x00, 0x00, 0x1f, 
0xf8, 0xf0, 0xf0, 0x00, 0x00, 0x1f, 0xf8, 0xf0, 0xf0, 0x00, 0x00, 0x1f, 0xf8, 0xf0, 0xf0, 0x00, 
0x00, 0x1f, 0xf8, 0xf0, 0xe0, 0x00, 0x00, 0x1f, 0xf8, 0xf0, 0xe0, 0x00, 0x00, 0x1f, 0xf8, 0xf0, 
0xe0, 0x00, 0x00, 0x1f, 0xf8, 0xf0, 0xf0, 0x00, 0x00, 0x3f, 0xf8, 0xf0, 0xf0, 0x00, 0x00, 0x3f, 
0xf8, 0xf0, 0xf0, 0x00, 0x00, 0x7f, 0xf8, 0xf0, 0xf8, 0x00, 0x00, 0xff, 0xf8, 0xf0, 0xf8, 0x00, 
0x01, 0xff, 0xf8, 0xf0, 0xfc, 0x00, 0x03, 0xff, 0xf8, 0xf0, 0xfc, 0x00, 0x03, 0xff, 0xf8, 0xf0, 
0xfc, 0x00, 0x03, 0xff, 0xf8, 0xf0, 0xfc, 0x00, 0x03, 0xff, 0xf8, 0xf0, 0xfc, 0x00, 0x03, 0xff, 
0xf8, 0xf0, 0xfc, 0x00, 0x03, 0xff, 0xf8, 0xf0, 0xfc, 0x00, 0x03, 0xff, 0xf8, 0xf0, 0xfc, 0x00, 
0x03, 0xff, 0xff, 0xf0, 0xfe, 0x00, 0x07, 0xff, 0xff, 0xf0, 0xfe, 0x00, 0x07, 0xff, 0xff, 0xf0, 
0xff, 0x00, 0x0f, 0xff, 0xff, 0xf0, 0xff, 0x80, 0x1f, 0xff, 0xff, 0xf1, 0xff, 0xf0, 0xff, 0xff
};

// 'snow', 48x48px
const unsigned char WEATHER_SNOW_IMAGE_DATA[] = {
0xff, 0xff, 0xfc, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x3f, 
0xff, 0xff, 0xff, 0xff, 0x9c, 0x39, 0xff, 0xff, 0xff, 0xff, 0x04, 0x30, 0xff, 0xff, 0xff, 0xff, 
0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x80, 0x01, 0xff, 0xff, 
0xff, 0x87, 0xc0, 0x03, 0xe1, 0xff, 0xff, 0x87, 0xe0, 0x07, 0xe1, 0xff, 0xff, 0x83, 0xf0, 0x0f, 
0xc1, 0xff, 0xf1, 0x83, 0xf8, 0x1f, 0xc1, 0x8f, 0xe0, 0x03, 0xfc, 0x3f, 0xc0, 0x07, 0xe0, 0x03, 
0xfc, 0x3f, 0xc0, 0x07, 0xf0, 0x01, 0xfc, 0x3f, 0x80, 0x0f, 0xf8, 0x01, 0xfc, 0x3f, 0x80, 0x1f, 
0xfe, 0x00, 0xfc, 0x3f, 0x00, 0x7f, 0xf8, 0x00, 0x7c, 0x3e, 0x00, 0x1f, 0xe0, 0x00, 0x1c, 0x38, 
0x00, 0x07, 0xe0, 0x00, 0x04, 0x20, 0x00, 0x07, 0xe0, 0x1c, 0x00, 0x00, 0x38, 0x07, 0xf0, 0xfe, 
0x00, 0x00, 0x7f, 0x0f, 0xff, 0xff, 0x80, 0x01, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 
0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xff, 0xff, 0x80, 0x01, 0xff, 0xff, 0xf0, 0xfe, 0x00, 0x00, 
0x7f, 0x0f, 0xe0, 0x1c, 0x00, 0x00, 0x38, 0x07, 0xe0, 0x00, 0x04, 0x20, 0x00, 0x07, 0xe0, 0x00, 
0x1c, 0x38, 0x00, 0x07, 0xf8, 0x00, 0x7c, 0x3e, 0x00, 0x1f, 0xfe, 0x00, 0xfc, 0x3f, 0x00, 0x7f, 
0xf8, 0x01, 0xfc, 0x3f, 0x80, 0x1f, 0xf0, 0x01, 0xfc, 0x3f, 0x80, 0x0f, 0xe0, 0x03, 0xfc, 0x3f, 
0xc0, 0x07, 0xe0, 0x03, 0xfc, 0x3f, 0xc0, 0x07, 0xf1, 0x83, 0xf8, 0x1f, 0xc1, 0x8f, 0xff, 0x83, 
0xf0, 0x0f, 0xc1, 0xff, 0xff, 0x87, 0xe0, 0x07, 0xe1, 0xff, 0xff, 0x87, 0xc0, 0x03, 0xe1, 0xff, 
0xff, 0xff, 0x80, 0x01, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 
0xff, 0xff, 0xff, 0xff, 0x04, 0x20, 0xff, 0xff, 0xff, 0xff, 0x9c, 0x39, 0xff, 0xff, 0xff, 0xff, 
0xfc, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x3f, 0xff, 0xff
};

// 'clear day', 48x48px
const unsigned char WEATHER_CLEAR_D_IMAGE_DATA[] = {
0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x3f, 
0xff, 0xff, 0xff, 0xff, 0xf8, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x1f, 0xff, 0xff, 0xff, 0xff, 
0xf0, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x0f, 0xff, 0xff, 0xfe, 0x1f, 0xe0, 0x07, 0xf8, 0x7f, 
0xfe, 0x03, 0xe0, 0x07, 0xc0, 0x7f, 0xfe, 0x00, 0x40, 0x02, 0x00, 0x7f, 0xfe, 0x00, 0x00, 0x00, 
0x00, 0x7f, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x07, 0xe0, 0x00, 0xff, 0xff, 0x00, 
0x1f, 0xf8, 0x00, 0xff, 0xff, 0x80, 0x7f, 0xfe, 0x01, 0xff, 0xff, 0x80, 0xfc, 0x3f, 0x01, 0xff, 
0xff, 0x81, 0xe0, 0x07, 0x81, 0xff, 0xff, 0xc3, 0xc0, 0x03, 0xc3, 0xff, 0xff, 0x83, 0x80, 0x01, 
0xc1, 0xff, 0xfe, 0x07, 0x00, 0x00, 0xe0, 0x7f, 0xf8, 0x07, 0x00, 0x00, 0xe0, 0x1f, 0xe0, 0x0f, 
0x00, 0x00, 0xf0, 0x07, 0x80, 0x0e, 0x00, 0x00, 0x70, 0x01, 0x00, 0x0e, 0x00, 0x00, 0x70, 0x00, 
0x00, 0x0e, 0x00, 0x00, 0x70, 0x00, 0x80, 0x0e, 0x00, 0x00, 0x70, 0x01, 0xe0, 0x0f, 0x00, 0x00, 
0x70, 0x07, 0xf8, 0x07, 0x00, 0x00, 0xe0, 0x1f, 0xfe, 0x07, 0x00, 0x00, 0xe0, 0x7f, 0xff, 0x83, 
0x80, 0x01, 0xc1, 0xff, 0xff, 0xc3, 0xc0, 0x03, 0xc3, 0xff, 0xff, 0x81, 0xe0, 0x07, 0x81, 0xff, 
0xff, 0x80, 0xfc, 0x3f, 0x01, 0xff, 0xff, 0x80, 0x7f, 0xfe, 0x01, 0xff, 0xff, 0x00, 0x1f, 0xf8, 
0x00, 0xff, 0xff, 0x00, 0x07, 0xe0, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xfe, 0x00, 
0x00, 0x00, 0x00, 0x7f, 0xfe, 0x00, 0x40, 0x02, 0x00, 0x7f, 0xfe, 0x03, 0xe0, 0x07, 0xc0, 0x7f, 
0xfe, 0x1f, 0xe0, 0x07, 0xf8, 0x7f, 0xff, 0xff, 0xf0, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x0f, 
0xff, 0xff, 0xff, 0xff, 0xf8, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x1f, 0xff, 0xff, 0xff, 0xff, 
0xfc, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff
};

#endif // __EPD_IMAGES_H__