#pragma once

#include <pebble.h>

#include "../windows/main_windows.h"

#define VALID_API_SETTING    1

void weather_init();

void weather_tick(struct tm *tick_time);