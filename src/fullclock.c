/* Copyright (c) 2014, Klas Björkqvist, see COPYING */
#include "fullclock.h"

#include <pebble.h>
#include <pebble_fonts.h>

#include "bitmaps.h"

#define WIDTH (144)
#define HEIGHT (30)

#define UTC_HOUR_DELTA (-2)

typedef struct {
  struct tm time;
  bool valid;
  bool utc;
} FullClockData;

static void full_clock_update(Layer *l, GContext *ctx);

Layer *full_clock_create(const GPoint r, bool utc) {
  Layer *l = layer_create_with_data((GRect){.origin = r, .size = { WIDTH, HEIGHT }},
                                    sizeof(FullClockData));
  layer_set_update_proc(l, full_clock_update);

  FullClockData *d = (FullClockData*) layer_get_data(l);
  d->valid = false;
  d->utc = utc;

  return l;
}

void full_clock_destroy(Layer *l) {
  layer_destroy(l);
}

void full_clock_set_time(Layer *l, struct tm *time) {
  FullClockData *d = (FullClockData*) layer_get_data(l);
  d->time = *time;
  d->valid = true;

  if (d->utc) {
    d->time.tm_hour = (d->time.tm_hour + 24 + UTC_HOUR_DELTA) % 24;
  }

  layer_mark_dirty(l);
}

static void full_clock_update(Layer *l, GContext *ctx) {
#define CLOCK_LABEL_DIST (2)
  FullClockData *d = (FullClockData*) layer_get_data(l);
  if (!d->valid) return;

  ClockStyle s = d->utc ? CLOCK_SMALL_HM : CLOCK_LARGE;
  GSize z_size = bitmaps_get_size_text(Z);
  GSize clock_size = bitmaps_get_size_clock(s);
  int width = clock_size.w;
  if (d->utc) {
    width += CLOCK_LABEL_DIST + z_size.w;
  }

  int begin_x = WIDTH/2 - width/2;
  bitmaps_draw_clock(ctx, s, (GPoint){.x = begin_x, .y = 0}, &d->time);

  if (d->utc) {
    bitmaps_draw_text(ctx, Z, (GPoint){ .x = begin_x + clock_size.w + CLOCK_LABEL_DIST,
                                        .y = clock_size.h - z_size.h });
  }
}
