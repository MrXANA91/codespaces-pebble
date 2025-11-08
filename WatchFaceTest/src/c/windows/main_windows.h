#pragma once

#include <pebble.h>

#include "../modules/battery.h"

void main_window_push();
void main_window_destroy();

void main_window_set_time_text(struct tm *time);
void main_window_request_update_battery();
