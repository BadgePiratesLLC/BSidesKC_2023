#include "badgepirates/images/bp-logo.h"

// 'bp_small_pixel', 16x16px
const unsigned char epd_bitmap_bp_small_pixel[] PROGMEM = {
    0x03, 0xe0, 0x1b, 0xf8, 0x3d, 0xfc, 0x7d, 0xfe, 0xfe, 0xff, 0xe3, 0x47, 0xe1, 0x87, 0x61, 0xc3,
    0x63, 0xc2, 0x77, 0x6c, 0x3f, 0x7c, 0x1f, 0xf8, 0x1f, 0xf8, 0x1b, 0xdc, 0x1a, 0xf0, 0x03, 0xc0};

const unsigned char *epd_bitmap_allArray[1] = {
    epd_bitmap_bp_small_pixel};