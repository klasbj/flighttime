/* Copyright (c) 2014, Klas Björkqvist, see COPYING */
#include <pebble.h>
#include "fullclock.h"
#include "elapsed.h"
#include "bitmaps.h"

#define N_CLOCKS (4)

static Window *window;
static Layer *clock_layers[N_CLOCKS];
Text clocks[N_CLOCKS] = { LCL, UTC, T, T };

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
}

static void up_long_click_handler(ClickRecognizerRef recognizer, void *context) {
  elapsed_reset(clock_layers[2]);
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  elapsed_start_stop(clock_layers[2]);
}

static void down_long_click_handler(ClickRecognizerRef recognizer, void *context) {
  elapsed_reset(clock_layers[3]);
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  elapsed_start_stop(clock_layers[3]);
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_long_click_subscribe(BUTTON_ID_UP, 0, up_long_click_handler, NULL);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
  window_long_click_subscribe(BUTTON_ID_DOWN, 0, down_long_click_handler, NULL);
}

static void tick(struct tm *tick_time, TimeUnits changed) {
  for (int i = 0; i < N_CLOCKS; ++i) {
    if (clocks[i] == LCL || clocks[i] == UTC) {
      full_clock_set_time(clock_layers[i], tick_time);
    } else if (clocks[i] == T) {
      elapsed_set_time(clock_layers[i], tick_time);
    }
  }
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);


  window_set_background_color(window, GColorBlack);

  time_t ts = time(NULL);
  struct tm *t = localtime(&ts);

  int elapsed_ident = 0;
  for (int i = 0; i < N_CLOCKS; ++i) {
    if (clocks[i] != T) {
      clock_layers[i] = full_clock_create((GPoint){0, 10+40*i}, clocks[i] == UTC);
      full_clock_set_time(clock_layers[i], t);
    } else {
      clock_layers[i] = elapsed_create((GPoint){0, 10+40*i}, ++elapsed_ident);
      elapsed_set_time(clock_layers[i], t);
    }
    layer_add_child(window_layer, clock_layers[i]);
  }


  tick_timer_service_subscribe(SECOND_UNIT, &tick);
}

static void window_unload(Window *window) {
  tick_timer_service_unsubscribe();
  for (int i = 0; i < N_CLOCKS; ++i) {
    if (clocks[i] == LCL || clocks[i] == UTC) {
      full_clock_destroy(clock_layers[i]);
    } else if (clocks[i] == T) {
      elapsed_destroy(clock_layers[i]);
    }
  }
}

static void init(void) {
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_fullscreen(window, true);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  bitmaps_deinit();
  window_destroy(window);
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
