#include "watch.h"

static struct tm *current_time;

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL);
  current_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  // Display this time on the TextLayer
  main_window_set_time_text(current_time);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

void watch_init() {
  // Register with TickTimeService
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);

  // Make sure time is displayed on start
  update_time();
}