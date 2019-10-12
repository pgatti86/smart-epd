# Smart edp

An IoT project built by an awesome team

## Getting Started

This repo depends on epd [library](https://github.com/pgatti86/epd) as git submodule.

To clone the project use **git clone --recursive <project url>** 

Project Makefile has the following configuration to include the submodule library in the build process:

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

## Custom fonts

To create custom fonts you need to generate a PNG file with all required characters (95).
See this [guide](https://kapusta.cc/2019/02/10/font2bytes/) for reference

Useful reading: [Making graphics and fonts for embedded systems](https://lb9mg.no/2018/02/10/making-graphics-and-fonts-for-embedded-systems/)

Look at fonts folder for examples.

## Images

To create an image visit http://javl.github.io/image2cpp/
Note: Rotate the image before the conversion.