#include "weather.h"

static char temperature_buffer[8];
static char conditions_buffer[32];
static char weather_layer_buffer[32];

static bool valid_api_key = false;

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
    // Read tuples for data
    Tuple *config_tuple = dict_find(iterator, MESSAGE_KEY_OWMAPIK);
    Tuple *temp_tuple = dict_find(iterator, MESSAGE_KEY_TEMPERATURE);
    Tuple *conditions_tuple = dict_find(iterator, MESSAGE_KEY_CONDITIONS);

    // If all data is available, use it
    if (config_tuple) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Received api key : '%s'", config_tuple->value->cstring);
        if (strcmp(config_tuple->value->cstring, "Enter here your API key") != 0) {
            // no API key has been entered
            APP_LOG(APP_LOG_LEVEL_DEBUG, "No API key");
            valid_api_key = false;
        } else {
            // an API key has been entered
            APP_LOG(APP_LOG_LEVEL_DEBUG, "API key ok!");
            valid_api_key = true;
        }
    }
    if (temp_tuple && conditions_tuple) {
        snprintf(temperature_buffer, sizeof(temperature_buffer), "%d°C", (int)temp_tuple->value->int32);
        snprintf(conditions_buffer, sizeof(conditions_buffer), "%s", conditions_tuple->value->cstring);

        // Assemble full string and display
        snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "%s, %s", temperature_buffer, conditions_buffer);
        main_window_set_weather_text(weather_layer_buffer);
    }
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

void weather_init() {
    app_message_register_inbox_received(inbox_received_callback);
    app_message_register_inbox_dropped(inbox_dropped_callback);
    app_message_register_outbox_failed(outbox_failed_callback);
    app_message_register_outbox_sent(outbox_sent_callback);

    const int inbox_size = 256;
    const int outbox_size = 128;
    app_message_open(inbox_size, outbox_size);
}

void weather_tick(struct tm *tick_time) {
    if (!valid_api_key) {
        return;
    }

    if (tick_time->tm_min % 30 == 0) {
        // Begin dictionary
        DictionaryIterator *iter;
        app_message_outbox_begin(&iter);

        // Add a key-value pair
        dict_write_uint8(iter, 0, 0);

        // Send the message!
        app_message_outbox_send();
    }
}