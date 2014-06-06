/* Copyright (c) 2014, Klas Björkqvist, see COPYING */

#include "PDutils.h"
#include <pebble.h>
#include "elapsed.h"
#include "bitmaps.h"

typedef struct {
  time_t time;    /* current time */
  bool valid;     /* current time valid */
  time_t since;   /* started time */
  bool running;   /* is started */
  int ident;      /* timer identifier */
} ElapsedData;

#define WIDTH (144)
#define HEIGHT (14)

static void elapsed_update(Layer *l, GContext *ctx);

Layer *elapsed_create(const GPoint r, int ident) {
  GSize s = bitmaps_get_size_clock(CLOCK_SMALL_MS);
  Layer *l = layer_create_with_data((GRect){ .origin = r, .size = { .w = WIDTH, .h = s.h } },
                                    sizeof(ElapsedData));
  layer_set_update_proc(l, elapsed_update);

  ElapsedData *d = (ElapsedData*) layer_get_data(l);
  d->time = 0;
  d->since = 0;
  d->valid = false;
  d->ident = ident;

  return l;
}

void elapsed_destroy(Layer *l) {
  layer_destroy(l);
}

void elapsed_set_time(Layer *l, struct tm *time) {
  ElapsedData *d = (ElapsedData*) layer_get_data(l);
  time_t diff = d->time - d->since;

  d->time = p_mktime(time);
  d->valid = true;
  if (!d->running) {
    d->since = d->time - diff;
  }

  layer_mark_dirty(l);
}

void elapsed_start_stop(Layer *l) {
  ElapsedData *d = (ElapsedData*) layer_get_data(l);
  if (!d->valid) return; /* invalid -> do nothing */

  d->running = !d->running;

  layer_mark_dirty(l);
}
void elapsed_start(Layer *l) {
  ElapsedData *d = (ElapsedData*) layer_get_data(l);
  if (!d->valid) return; /* invalid -> do nothing */

  d->running = true;

  layer_mark_dirty(l);
}

void elapsed_stop(Layer *l) {
  ElapsedData *d = (ElapsedData*) layer_get_data(l);
  if (!d->valid || !d->running) return; /* invalid, or not running -> do nothing */

  d->running = false;

  layer_mark_dirty(l);
}

void elapsed_reset(Layer *l) {
  ElapsedData *d = (ElapsedData*) layer_get_data(l);
  if (!d->valid) return; /* invalid -> do nothing */

  d->since = d->time;

  layer_mark_dirty(l);
}

static void elapsed_update(Layer *l, GContext *ctx) {
  ElapsedData *d = (ElapsedData*) layer_get_data(l);
  if (!d->valid) return; /* invalid -> do nothing */

  GSize clock_size = bitmaps_get_size_clock(CLOCK_SMALL_MS);
  GSize ident_t_size = bitmaps_get_size_text(T);
#define IDENT_LABEL_DIST (6)
  GSize ident_n_size = bitmaps_get_size_number(NUMBER_T);

  /* calculate time diff and display the time */
  time_t diff = d->time - d->since;
  struct tm tm_diff = {
    .tm_hour  = (diff / 3600) % 24,
    .tm_min   = (diff / 60) % 60,
    .tm_sec   = diff % 60
  };

  int width = ident_t_size.w + 1 +
              ident_n_size.w + IDENT_LABEL_DIST +
              clock_size.w;
  int x = WIDTH/2 - width/2;

  bitmaps_draw_text(ctx, T, (GPoint){.x = x, .y = 0});
  x += ident_t_size.w + 1;
  bitmaps_draw_number(ctx, NUMBER_T, d->ident % 10, (GPoint){.x = x, .y = 0});
  x += ident_n_size.w + IDENT_LABEL_DIST;
  bitmaps_draw_clock(ctx, tm_diff.tm_hour > 0 ? CLOCK_SMALL_HM : CLOCK_SMALL_MS, (GPoint){.x = x, .y = 0}, &tm_diff);
}
