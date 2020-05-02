# Smart e-ink paper display

Smart-epd is a study project for the esp32 IDF framework.

The final scope is to build an alarm clock with some added features like temperature and humidity monitoring, local weather infos, and OTA updates.

[![smart-epd.jpg](https://i.postimg.cc/7hjTc85C/smart-epd.jpg)](https://postimg.cc/30CR4qC7)

## BOM

- Any esp32 breakout board (with at least 4MB flash size)
- Waveshare 2.9inch E-paper Module (296x128 px)
- DHT22 temperature and humidity module

## Getting Started

This repo depends on epd [library](https://github.com/pgatti86/epd) as git submodule.

To clone this project use **git clone --recursive <project url>** 

Project Makefile has the following configuration to include the submodule library in the build process:

**EXTRA_COMPONENT_DIRS += $(PROJECT_PATH)/epd/components/**

This project is based on Espressif IDF v4.0 (current stable branch).
Follow this link to configure your environment: [esp idf v4.0](https://docs.espressif.com/projects/esp-idf/en/v4.0/get-started/index.html)

Note: depending on your python virtualenv version you may encounter an error while running ESP install script (install.sh).
To solve simply remove the invalid option (--no-site-packages) from idf_tools.py 

## Configurations

Before flashing the app you will need to configure the device with **make menuconfig** command:

### App settings

Enter SMART-EPD config menu to configure the application (some defaults values applies)

- SNTP_SERVER: defaults to "pool.ntp.org"
- OTA_SERVER: https server that hosts your generated bin file. 
- DHT_GPIO: reads temperature and humidity from DHT22 on specified GPIO. Defauts to GPIO 4
- BUTTON_GPIO: wipes the device memory when holded longer then 3s. Defaults to 0 (builtin button)
- MAX_REWRITE_COUNT: number of screen rewrite before eink full clean apply. Defualts to once in an hour

Save and exit.

### Partition Tables

The app uses a custom partition table defined in partitions.csv file:

In "Partition table" menù select "Partition Table" sub-menù.
Check the "custom partition table" option.

You also need to change the embedded flash size:
In "Serial flasher" menù enter "Flash size" sub-menù and select 4MB.

Save and exit.
Back in CLI run **make** command.
You can check partition table with **make partition_table** command.

### Open weather API key

You need an open api key for weather updates, register your account at https://openweathermap.org.

### Timezone

Actually timezone is hardcoded. You need to manually adjust the TZ environment variable in time_manager.c file.
Look for time_manager_init function and change TZ based on your location.
I will externalize this configuration asap.

The current TZ value is "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00" (Europe, Rome)

Here some detail on [TZ variable](https://www.gnu.org/software/libc/manual/html_node/TZ-Variable.html)
and [abbreviations](https://remotemonitoringsystems.ca/time-zone-abbreviations.php)

## Spiffs partition generation

Look for spiffs folder in project, it contains:

- device subfolder: contains the device configurations:
```json
{
"deviceId":"1d3053b1-4c31-40da-9533-2eacca528add"
}
```
#### NB: an empty line at the end of the config file is mandatory otherwise the json parsing library (cJSON) will fail deserialization.
#### NB: don't change the file name.

- security subfolder: This folder must contain your server public certificate in PEM format.

#### NB: don't change the file name.

- spiffsgen.py: script used to generate *.img partitions

- esptool.py: script to write partitions generated with the above script into target device flash memory

To generate and flash the device configuration image use the following commands

```console
spiffsgen.py 204800 ~/device-folder-path ~/destination-path/device.bin

esptool.py --chip esp32 --port /dev/cu.SLAB_USBtoUART --baud 921600 write_flash 0x311000 ~/img-path/device.bin
```

To generate and flash the device security image use the following commands

```console
spiffsgen.py 204800 ~/security-folder-path/security ~/destination-path/security.bin

esptool.py --chip esp32 --port /dev/cu.SLAB_USBtoUART --baud 921600 write_flash 0x343000 ~/img-path/security.bin
```


## Build

You can build this project using **make** or **idf.py build**

Flash the binaries that you just built onto your ESP32 board by running **make flash** or **idf.py -p PORT [-b BAUD] flash**

## Enrollment

A new device starts in Access Point (AP) mode, it displays WiFi credentials and PoP (proof of possession) token on the screen.
After connecting to the device network you need to send the following payload at the device (192.168.1.1) with a POST request:

```json
{
"ssid":"YOUR_HOME_SSID",
"password":"PASSWORD",
"code":123456,
"zip":25086,
"wApiKey":"asd123098asdasdasdads"
}
```

The provided zip code and weather api key are used for weather forecasting.

cURL example

```console
curl --location --request POST '192.168.1.1/setCredentials' \
--header 'Content-Type: text/plain' \
--data-raw '{
"ssid":"YOUR_HOME_SSID",
"password":"PASSWORD",
"code":123456,
"zip":25086,
"wApiKey":"asd123098asdasdasdads"
}'
```

## Reset

You can reset the device pressing and holding the right module button (builtin) for at least 3 seconds.
After reset the device will return in enrollment mode.

## How connect eink display

| Signal | GPIO |
| --- | --- |
| DIN / MOSI  | GPIO_NUM_23  |
| CLK | GPIO_NUM_18  |
| CS | GPIO_NUM_5   |
| DC | GPIO_NUM_26  |
| RST | GPIO_NUM_27  |
| BUSY | GPIO_NUM_32  |

## OTA

Before releasing new binaries you must update the build version in version.txt file.

## 3d print

3d print folder contains required stl files.

## Custom fonts

To create custom fonts you need to generate a PNG file with all required characters (95).
See this [guide](https://kapusta.cc/2019/02/10/font2bytes/) for reference

Useful reading: [Making graphics and fonts for embedded systems](https://lb9mg.no/2018/02/10/making-graphics-and-fonts-for-embedded-systems/)

Look at fonts folder for examples.

## Images

To create an image visit http://javl.github.io/image2cpp/
Note: Rotate the image before the conversion.