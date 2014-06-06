/* Copyright (c) 2014, Klas Björkqvist, see COPYING */
#include <pebble.h>
#include "fullclock.h"
#include "elapsed.h"
#include "flighttimer.h"
#include "bitmaps.h"

#define N_CLOCKS (4)

static Window *window;
static Layer *clock_layers[N_CLOCKS];
Text clocks[N_CLOCKS] = { LCL, UTC, FLT,  T };
typedef void (*ClockTimeSetter)(Layer *l, struct tm *t);
typedef void (*ClockDestroyer)(Layer *l);

ClockTimeSetter setters[N_CLOCKS] = {
  &full_clock_set_time,
  &full_clock_set_time,
  &flighttimer_set_time,
  &elapsed_set_time
};

ClockDestroyer destroyers[N_CLOCKS] = {
  &full_clock_destroy,
  &full_clock_destroy,
  &flighttimer_destroy,
  &elapsed_destroy
};


static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  flighttimer_start_stop(clock_layers[2]);
}

static void up_long_click_handler(ClickRecognizerRef recognizer, void *context) {
  elapsed_reset(clock_layers[3]);
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  elapsed_start_stop(clock_layers[3]);
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
    setters[i](clock_layers[i], tick_time);
  }
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);


  window_set_background_color(window, GColorBlack);

  time_t ts = time(NULL);
  struct tm *t = localtime(&ts);

  int elapsed_ident = 0;
  for (int i = 0; i < N_CLOCKS; ++i) {
    switch (clocks[i]) {
      case LCL:
      case UTC:
        clock_layers[i] = full_clock_create((GPoint){0, 10+40*i}, clocks[i] == UTC);
        full_clock_set_time(clock_layers[i], t);
        break;
      case FLT:
        clock_layers[i] = flighttimer_create((GPoint){0, 10+40*i});
        flighttimer_set_time(clock_layers[i], t);
        break;
      case T:
        clock_layers[i] = elapsed_create((GPoint){0, 10+40*i}, ++elapsed_ident);
        elapsed_set_time(clock_layers[i], t);
        break;
      default:
        continue;
    }
    layer_add_child(window_layer, clock_layers[i]);
  }


  tick_timer_service_subscribe(SECOND_UNIT, &tick);
}

static void window_unload(Window *window) {
  tick_timer_service_unsubscribe();
  for (int i = 0; i < N_CLOCKS; ++i) {
    destroyers[i](clock_layers[i]);
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
