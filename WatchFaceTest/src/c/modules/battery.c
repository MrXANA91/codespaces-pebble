#include "battery.h"

static int s_battery_level = 0;

static void battery_callback(BatteryChargeState state) {
  s_battery_level = state.charge_percent;

  // Update meter
  main_window_request_update_battery();
}

void battery_init() {
  // Register for battery level updates
  battery_state_service_subscribe(battery_callback);

  // Ensure battery level is displayed from the start
  battery_callback(battery_state_service_peek());
}

int battery_get_level() {
    return s_battery_level;
}