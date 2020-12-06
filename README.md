# Smart e-ink paper display

Smart-epd is a study project for the esp32 IDF framework.

The final scope is to build an alarm clock with some added features like temperature and humidity monitoring, local weather infos, and OTA updates.

[comment]:[smart-epd.jpg](https://i.postimg.cc/7hjTc85C/smart-epd.jpg)](https://postimg.cc/30CR4qC7)
[![smart-epd.jpg](https://i.postimg.cc/W4xnvr2g/20201206-211905-COLLAGE.jpg)](https://postimg.cc/Z9F3PWv5)

As the project evolved, I wanted to add some optional features, such as the ability to use gestures to change the page displayed on the clock.
In the 3d-print folder you will find both versions of clock body, with and withoud the gesture module housing.

 
## BOM

- Any [esp32](https://live.staticflickr.com/4764/40089095211_ec1fee0087_b.jpg) breakout board (with at least 4MB flash size)
- [Waveshare 2.9inch E-paper Module b/w](https://www.waveshare.com/wiki/2.9inch_e-Paper_Module) (296x128 px)
- [DHT22 temperature and humidity module](https://imgaz.staticbg.com/thumb/large/2014/xiemeijuan/07/SKU146979/SKU146979a.jpg)
- [APDS-9960 module](https://learn.sparkfun.com/tutorials/apds-9960-rgb-and-gesture-sensor-hookup-guide/all) (optional)
- USB-A to micro USB-B cable

Depending on the store from which you buy the material, the price can vary between 40€ and 50€

DHT22 requires a pullup resistor. I recommend buying a module that integrates resistor and sensor like the one visible in the link above.

## Getting Started

This project is based on Espressif IDF v4.1 (stable branch at writing time).
Follow this link to configure your environment: [esp idf v4.1](https://docs.espressif.com/projects/esp-idf/en/v4.1/get-started/index.html)

To clone this project use **git clone --recursive git@gitlab.com:paolo.gatti/smart-epd.git** 

This repo depends on a couple of mine libraries as git submodules:
- epd [library](https://github.com/pgatti86/epd) (handles Eink screen)
- apds9960 [library](https://github.com/pgatti86/apds9960-idf) (handles gesture module)

Project Makefile has the following configuration to include the submodule library in the build process:

**EXTRA_COMPONENT_DIRS += $(PROJECT_PATH)/epd/components/ $(PROJECT_PATH)/apds9960-idf/components/**

## Configurations

Before flashing the app you will need to configure the device with **make menuconfig** command or **idf.py menuconfig**

### App settings

Enter SMART-EPD config menu to configure the application (some defaults values applies)

- SNTP_SERVER: defaults to "pool.ntp.org"
- OTA_SERVER: https server that hosts your generated bin file. 
- DHT_GPIO: reads temperature and humidity from DHT22 on specified GPIO. Defauts to GPIO 4
- BUTTON_GPIO: wipes the device memory when holded longer then 3s. Defaults to 0 (builtin button on my board)
- MAX_REWRITE_COUNT: number of screen rewrite before eink full clean apply. Defualts to 3600
- SCL: i2c scl pin. Defaults to 22
- SDA: i2c sda pin. Defaults to 21
- APDS_INT: Apds9960 interrupt GPIO. Defaults to 19

Save and return to the main menu.

### Partition Tables

The app uses a custom partition table defined in partitions.csv file:

In "Partition table" menù select "Partition Table" sub-menù.
Check the "custom partition table CSV" option.

Back in main menu you also need to change the embedded flash size:
In "Serial flasher" menù enter "Flash size" sub-menù and select the memory size that match your module.
The minimum size required is 4MB.

Save and exit.

Back in CLI run **make** or **idf.py build** command. It will take a while, have a coffee ;)
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

Look for spiffs folder in project, it contains two sub folders:

- device: contains the device configurations in a json file.
Generate your new device [uuid](https://it.wikipedia.org/wiki/Universally_unique_identifier) on this [page](https://www.uuidgenerator.net/) 

```json
{
"deviceId":"1d3053b1-4c31-40da-9533-2eacca528add"
}
```

This uuid will be returned by the enrollment process.

#### NB: an empty line at the end of the config file is mandatory otherwise the json parsing library (cJSON) will fail deserialization.
#### NB: don't change the file name.

- security: This folder must contain your server public certificate in PEM format.

This is required by the OTA process to verify the server before the firmware download can take place.
The existing certificate is a sample, leave it as is if all this sounds like new to you. Simply the OTA will not work.

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

The response to this POST request will be the previously configured UUID.

## Reset

You can reset the device pressing and holding the right module button (builtin) for at least 3 seconds.
After reset the device will return in enrollment mode.

## Defaults connections

### eink display

| Signal | GPIO |
| --- | --- |
| DIN / MOSI  | GPIO_NUM_23  |
| CLK | GPIO_NUM_18  |
| CS | GPIO_NUM_5   |
| DC | GPIO_NUM_26  |
| RST | GPIO_NUM_27  |
| BUSY | GPIO_NUM_32  |

### DHT

| Signal | GPIO |
| --- | --- |
| VCC | 3.3V |
| GND | GND |
| DATA | GPIO_NUM_4 |

### APDS-9960 (optional)

| Signal | GPIO | 
|---|---|
| VCC | 3.3V |
| GND | GND |
| SDA | GPIO_NUM_21 |
| SCL | GPIO_NUM_22 |
| INT | GPIO_NUM_19 |

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