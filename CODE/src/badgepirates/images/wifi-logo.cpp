#include "badgepirates/images/wifi-logo.h"

const unsigned char epd_bitmap_wifi_logo[] PROGMEM = {
    0x00, 0x7e, 0x81, 0x00, 0x3c, 0x42, 0x00, 0x18};
// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 1312)
const unsigned char *epd_bitmap_wifi_Array[1] = {epd_bitmap_wifi_logo};
