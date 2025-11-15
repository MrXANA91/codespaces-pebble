#pragma once

#include <pebble.h>

#include "../windows/main_windows.h"

void weather_init();

void weather_tick(struct tm *tick_time);