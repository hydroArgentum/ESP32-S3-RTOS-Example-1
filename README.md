# ESP32-S3-RTOS-Example-1
A simple example code for ESP-IDF RTOS for ESP32-S3 showcasing two tasks running on a single core using Arduino.
## Hardware Used
ESP32-S3 Feather 4MB Flash/2MB PSRAM
### Untested Compatible Hardware
- ESP32 Feather (most variants, preferably ones that have both a Red LED [LED_BUILTIN] and a NeoPixel [WS2812] onboard)
- ESP32-S3 Feather (most variants, preferably ones that have both a Red LED [LED_BUILTIN] and a NeoPixel [WS2812] onboard)
## Usage
1. Open "IntroductionToRTOSPart2.ino" in Arduino IDE.
2. Connect the ESP32-S3 Feather board via USB.
3. Put the ESP32-S3 Feather board into ROM bootloader mode.
4. Adjust the COM port in Arduino IDE settings (under Tools).
5. Upload sketch.
## Considerations
- Use https://learn.adafruit.com/adafruit-esp32-s3-feather/using-with-arduino-ide to learn how to setup Arduino IDE to use the ESP32-S3. (Current stable library does not yet include support for ESP32-S3 Feather boards.)
- Sometimes, the sketch will fail to upload. In those cases, reset again into ROM bootloader mode and try again.
- To get to the ROM bootloader mode, hold the "boot" button before pressing the "reset" button.
- If you don't have a WS2812 NeoPixel on your board, or you want to connect your own NeoPixel, the third argument of the Adafruit_NeoPixel constructor may be different:
	- Argument 3 - Pixel type flags, add together as needed:
		- NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
		- NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
		- NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
		- NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
		- NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
	- See the strandtest.ino example script from the Adafruit NeoPixel library for more information.
- I guessed that the blinky code and NeoPixel code will take 1KB and 2KB, respectively, by just guessing.
	- However, remember that, for ESP-IDF RTOS, the minimum stack size considering overhead is 768 bytes and this increases if you enable stack options.
		- CONFIG_STACK_CHECK_STRONG adds an additional 256 bytes to the overhead.
		- CONFIG_COMPILER_OPTIMIZATION_NONE adds an additional 320 bytes to the overhead.
		- CONFIG_APPTRACE_ENABLE adds an additional 1280 bytes to the overhead.
		- CONFIG_FREERTOS_WATCHPOINT_END_OF_STACK adds an additional 60 bytes to the overhead.
		- CONFIG_STACK_CHECK_ALL will add all of the above.
		- So remember to add the minimum 768 bytes, the additional stack options (if used), and the size returned from the build (plus a little more if you want to stay on the safe side; round up to the nearest "nice" value) when assigning the stack size.
		- TODO the above.
