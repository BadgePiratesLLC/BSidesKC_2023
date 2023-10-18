#pragma once
/*
 * BadgePirates configuration
 * Add project-specific configurations to constants.h
 * This file should be used to enable and configure common features shared between badges.
 *
 * For reference, use #define for things constant and used seldomly(because we can save memory with pointers). Hence it should be used for things like
 * Version numbers or OTA WiFi credentials.
 */

#define BADGE_VERSION 2
#define DEBUG // Comment this out to disable debugging mode
#define BLE_MODE 0
#define FACTORY_MODE // Comment this out to disable factory mode, which unless you removed the calls in main.cpp, it simply runs that when setup and loop are called.

// OTA bits

// This network is included as a fallback in WifiManager now
//  #define OTA_WIFI_SSID "badgepirates"
//  #define OTA_WIFI_PASSWORD "APirateIsFr33"
#define OTA_SERVER_PATH "https://api.badgepirates.com/api/firmware/"
#define OTA_DOWNLOAD_PATH "https://api.badgepirates.com/api/firmware/download"
#define OTA_MANIFEST_NAME "version"

#define OTA_MANIFEST_URL OTA_SERVER_PATH OTA_MANIFEST_NAME
#define OTA_DOWNLOAD_URL OTA_DOWNLOAD_PATH

// Dumb LEDS
extern const int NUM_DUMB_LEDS;
extern const int all_leds[6]; // Declaration without definition.

///////////////////////////////////////////
///////////////////////////////////////////
#define OLED_I2C_ADDRESS 0x3C // Defined OLED I2C Address

/*
 * Define your font from the list.
 * Default font: lcd5x7
 * Comment out the following for using the default font.
 */
#define LARGE_FONT // Verdana12

// Navigate buttons
#define BTN_SEL 36  // Select button
#define BTN_UP 38   // Up Button
#define BTN_DOWN 37 // Down Button
#define BTN_ESC 46  // Exit Button

// buttons Right - IO35
// button Center - IO36
// button Up - IO37
// button Down - IO38
// button Left - IO46
// Comment the following to disable internal pullup for Navigate buttons
#define NAV_BUTTONS_INPUT_PULLUP

#define TOTAL_NAV_BUTTONS 4 // Total Navigation Button used

/*Demonstrate PWM with LED on D11*/
#define LED_PIN 11 // Defined LED Pin to D11
#define LED_BUILTIN 12

#define MAX_DEPTH 2

#ifdef LOC
// #define LARGE_FONT
#define INV
#endif

/*Do not change the values(recomended)*/
// #ifdef LARGE_FONT
// #define menuFont LARGE_FONT
// #define fontW 8
// #define fontH 16
// #else
// #define menuFont System5x7
#define menuFont lcd5x7
#define fontW 5
#define fontH 8
// #endif
