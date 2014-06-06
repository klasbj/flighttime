#include <pebble.h>
#include "fullclock.h"
#include "bitmaps.h"

static Window *window;
static Layer *clock_layers[5];

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void tick(struct tm *tick_time, TimeUnits changed) {
  for (int i = 0; i < 5; ++i) {
    full_clock_set_time(clock_layers[i], tick_time);
  }
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);


  window_set_background_color(window, GColorBlack);

  time_t ts = time(NULL);
  struct tm *t = localtime(&ts);

  Text clocks[5] = { LCL, UTC, FLT, 123, 124 };
  for (int i = 0; i < 5; ++i) {
    if (i < 2)
      clock_layers[i] = full_clock_create(clocks[i], (GPoint){0, 10+40*i});
    else
      clock_layers[i] = full_clock_create(clocks[i], (GPoint){0, 10+40*2 + 20*(i-2)});
    full_clock_set_time(clock_layers[i], t);
    layer_add_child(window_layer, clock_layers[i]);
  }


  tick_timer_service_subscribe(SECOND_UNIT, &tick);
}

static void window_unload(Window *window) {
  tick_timer_service_unsubscribe();
  for (int i = 0; i < 5; ++i) {
    full_clock_destroy(clock_layers[i]);
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
