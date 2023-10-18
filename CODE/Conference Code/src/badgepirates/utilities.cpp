#include "Arduino.h"
// Handy things Sean uses. Should normally get excluded because they're not in use. Left here for preservation or future debugging.

static void printmem()
{
    log_d("   Heap: ");
    log_d("      Total: %d", ESP.getHeapSize());
    log_d("      Used: %d", ESP.getHeapSize() - ESP.getFreeHeap());
    log_d("      Free: %d", ESP.getFreeHeap());
    log_d("   PSRAM: ");
    log_d("      Total: %d", ESP.getPsramSize());
    log_d("      Used: %d", ESP.getPsramSize() - ESP.getFreePsram());
    log_d("      Free: %d", ESP.getFreePsram());
}