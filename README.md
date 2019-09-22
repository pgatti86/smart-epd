# Smart edp

An IoT projec build by an awesome team 

## Getting Started

This repo depends on epd library as a git submodule.

To clone the project use **git clone --recursive <project url>** 

Project Makefile has the following configuration for include the submodule library in the build process:

**EXTRA_COMPONENT_DIRS += $(PROJECT_PATH)/epd/components/**

build sistem [reference](https://docs.espressif.com/projects/esp-idf/en/v3.3/api-guides/build-system.html)

## Configure WiFi credentials

Before flashing the app configure your WiFi credentials using **make menuconfig** command.
Look for "SMART-EPD config" menù for project settings.

## How connect eink display

| Signal | GPIO |
| --- | --- |
| DIN / MOSI  | GPIO_NUM_23  |
| CLK | GPIO_NUM_18  |
| CS | GPIO_NUM_5   |
| DC | GPIO_NUM_26  |
| RST | GPIO_NUM_27  |
| BUSY | GPIO_NUM_32  |


