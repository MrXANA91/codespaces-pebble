#include <pebble.h>

#include "modules/battery.h"
#include "modules/watch.h"
#include "modules/weather.h"
#include "windows/main_windows.h"

static void init() {
  weather_init();
  main_window_push();
  
  battery_init();
  watch_init();
}

static void deinit() {
  // Destroy Window
  main_window_destroy();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}