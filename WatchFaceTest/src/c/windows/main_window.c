#include "main_windows.h"

static Window *s_window;
static TextLayer *s_time_layer;
static GFont s_time_font;
static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;
static TextLayer *s_weather_layer;
static GFont s_weather_font;
static Layer *s_battery_layer;

static void battery_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);

  // Find the width of the bar (total width = 114px)
  int width = (battery_get_level() * 114) / 100;

  // Draw the background
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, bounds, 0, GCornerNone);

  // Draw the bar
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_rect(ctx, GRect(0, 0, width, bounds.size.h), 0, GCornerNone);
}

static void main_window_load(Window *window) {
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);

  // Create GBitmap
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);

  // Create BitmapLayer to display the GBitmap
  s_background_layer = bitmap_layer_create(bounds);

  // Set the bitmap onto the layer and add to the window
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_background_layer));

  // Create the TextLayer with specific bounds
  s_time_layer = text_layer_create(
    GRect(4, 52, bounds.size.w - 2, 50)
  );
  s_weather_layer = text_layer_create(
    GRect(0, 120, bounds.size.w, 25)
  );

  // Create GFont
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PERFECT_DOS_48));

  // Improve layout to be more like a watchface
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
//   text_layer_set_text(s_time_layer, "10:00");
//   text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  // Add it as a child to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));

  // Create second font, apply it and add to Window
  s_weather_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PERFECT_DOS_20));
  text_layer_set_font(s_weather_layer, s_weather_font);
  text_layer_set_background_color(s_weather_layer, GColorClear);
  text_layer_set_text_color(s_weather_layer, GColorWhite);
  text_layer_set_text_alignment(s_weather_layer, GTextAlignmentCenter);
  text_layer_set_text(s_weather_layer, "Loading...");
  layer_add_child(window_layer, text_layer_get_layer(s_weather_layer));

  // Create battery meter Layer
  s_battery_layer = layer_create(GRect(14, 53, 115, 2));
  layer_set_update_proc(s_battery_layer, battery_update_proc);

  // Add to Window
  layer_add_child(window_layer, s_battery_layer);
}

static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_weather_layer);
  // Unload GFont
  fonts_unload_custom_font(s_time_font);
  fonts_unload_custom_font(s_weather_font);
  // Destroy GBitmap
  gbitmap_destroy(s_background_bitmap);
  // Destroy BitmapLayer
  bitmap_layer_destroy(s_background_layer);
  // Destroy BatteryLayer
  layer_destroy(s_battery_layer);
}

void main_window_push() {
    if (!s_window) {
        s_window = window_create();

        // Set handlers
        window_set_window_handlers(s_window, (WindowHandlers) {
            .load = main_window_load,
            .unload = main_window_unload
        });

        // Set Window Background
        window_set_background_color(s_window, GColorBlack);
    }

    // Show window on the watch
    window_stack_push(s_window, true);
}

void main_window_destroy() {
  // Destroy Window
  window_destroy(s_window);
}

void main_window_set_time_text(struct tm *time) {
    if (!s_window) {
        return;
    }
    if (!s_time_layer) {
        return;
    }

    // Write the current hours and minutes into a buffer
    static char s_buffer[8];
    strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ? "%H:%M" : "%I:%M", time);

    // Display this time on the TextLayer
    text_layer_set_text(s_time_layer, s_buffer);
}

void main_window_request_update_battery() {
    if (!s_window) {
        return;
    }
    if (!s_battery_layer) {
        return;
    }

    layer_mark_dirty(s_battery_layer);
}

void main_window_set_weather_text(const char *str) {
  text_layer_set_text(s_weather_layer, str);
}