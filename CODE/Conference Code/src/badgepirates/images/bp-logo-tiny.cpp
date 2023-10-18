#include "badgepirates/images/bp-logo-tiny.h"

const unsigned char epd_bitmap_bp_tiny_pixel[] PROGMEM = {
    0x3c, 0x7f, 0xff, 0xd9, 0xdf, 0x7e, 0x7e, 0x3c};

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 32)
const unsigned char *epd_bitmap_allArray_tiny[1] = {
    epd_bitmap_bp_tiny_pixel};
