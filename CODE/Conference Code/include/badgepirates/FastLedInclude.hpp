#include <FastLED.h>
#define NUM_LEDS 1

#if CONFIG_IDF_TARGET_ESP32S3
#define DATA_PIN 18
#else
// Sean's dev board
#define DATA_PIN 32
#endif

extern struct CRGB leds[];
