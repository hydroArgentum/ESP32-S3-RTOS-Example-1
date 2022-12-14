// NeoPixel.
#include <Adafruit_NeoPixel.h>

#define LED_COUNT 1
#define BRIGHTNESS 25

// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// Pins
static const int led_pin = LED_BUILTIN;

// NeoPixel
Adafruit_NeoPixel strip(LED_COUNT, PIN_NEOPIXEL, NEO_KHZ800 + NEO_GRB);

// Our Task: Blinky
void toggleLED(void *parameter) {
  while (true) {
    digitalWrite(led_pin, HIGH);
    vTaskDelay(500 / portTICK_PERIOD_MS); // Use vTaskDelay instead of delay
    digitalWrite(led_pin, LOW); // Because vTaskDelay is guaranteed to be non-blocking.
    vTaskDelay(500 / portTICK_PERIOD_MS); // Since the timer is based on a tick
    // timer. portTICK_PERIOD_MS is 1 tick/x MS. In FreeRTOS, the default x = 1ms.
    // vTaskDelay expects the number of ticks, not time.
  }
}

void neopixelRainbow(void *parameter) {
  while (true) {
      rainbow(10);             // Flowing rainbow cycle along the whole strip
  }
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    // strip.rainbow() can take a single argument (first pixel hue) or
    // optionally a few extras: number of rainbow repetitions (default 1),
    // saturation and value (brightness) (both 0-255, similar to the
    // ColorHSV() function, default 255), and a true/false flag for whether
    // to apply gamma correction to provide 'truer' colors (default true).
    strip.rainbow(firstPixelHue);
    // Above line is equivalent to:
    // strip.rainbow(firstPixelHue, 1, 255, 255, true);
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}

void setup() {
  // Configure pin as push-pull.
  pinMode(led_pin, OUTPUT);

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(BRIGHTNESS); // Set BRIGHTNESS to about 1/5 (max = 255)

  // Task to run forever.
  xTaskCreatePinnedToCore ( // Use xTaskCreate() in vanilla FreeRTOS.
    toggleLED,              // Function to be called.
    "Toggle LED",           // Name of task.
    1024,                   // Stack size (bytes in ESP32, words in FreeRTOS)
    NULL,                   // Parameter to pass to function.
    1,                      // Task priority (0 to configMAX_PRIORITIES  - 1)
    NULL,                   // Task handle
    app_cpu);               // This is exclusive to xTaskCreatePinnedToCore and defines
                            // a particular core that we want to run the task. (ESP32
                            // only.)
  // If this was vanilla FreeRTOS, you'd want to call vTaskStartScheduler() in main
  // after setting up your tasks.

  // Try a second task.
  xTaskCreatePinnedToCore (
    neopixelRainbow,
    "NeoPixel Rainbow",
    2048,
    NULL,
    1,
    NULL,
    app_cpu);
    
}

void loop() {

}
