/* Copyright (c) 2014, Klas Björkqvist, see COPYING */

#include "PDutils.h"
#include <pebble.h>
#include "flighttimer.h"
#include "bitmaps.h"

typedef enum {
  PREFLIGHT,
  FLYING,
  LANDED,
  STATE_LAST
} State;

typedef struct {
  time_t  time;     /* current time */
  bool    valid;    /* current time valid */
  time_t  takeoff;  /* takeoff time */
  time_t  landing;  /* landing time */
  State   state;    /* flight state */
} FlighttimerData;

#define WIDTH (144)

static void flighttimer_update(Layer *l, GContext *ctx);

Layer *flighttimer_create(const GPoint r) {
  GSize s = bitmaps_get_size_clock(CLOCK_SMALL_MS);
  Layer *l = layer_create_with_data((GRect){ .origin = r, .size = { .w = WIDTH, .h = s.h*2+5 } },
                                    sizeof(FlighttimerData));
  layer_set_update_proc(l, flighttimer_update);

  FlighttimerData *d = (FlighttimerData*) layer_get_data(l);
  d->time = 0;
  d->valid = false;
  d->takeoff = 0;
  d->landing = 0;
  d->state = PREFLIGHT;

  return l;
}

void flighttimer_destroy(Layer *l) {
  layer_destroy(l);
}

void flighttimer_set_time(Layer *l, struct tm *time) {
  FlighttimerData *d = (FlighttimerData*) layer_get_data(l);

  d->time = p_mktime(time);
  d->valid = true;

  layer_mark_dirty(l);
}

void flighttimer_start_stop(Layer *l) {
  FlighttimerData *d = (FlighttimerData*) layer_get_data(l);
  if (!d->valid) return; /* invalid -> do nothing */

  switch (d->state) {
    case PREFLIGHT:
      d->takeoff = d->time;
      d->state = FLYING;
      break;
    case FLYING:
      d->landing = d->time;
      d->state = LANDED;
      break;
    case LANDED:
      flighttimer_reset(l);
      break;
    default:
      return; /* Don't do anything */
  }

  layer_mark_dirty(l);
}
void flighttimer_start(Layer *l) {
  FlighttimerData *d = (FlighttimerData*) layer_get_data(l);
  if (!d->valid
      || d->takeoff != 0
      || d->state != PREFLIGHT) return; /* invalid -> do nothing */

  d->takeoff = d->time;
  d->state = FLYING;

  layer_mark_dirty(l);
}

void flighttimer_stop(Layer *l) {
  FlighttimerData *d = (FlighttimerData*) layer_get_data(l);
  if (!d->valid
      || d->takeoff == 0
      || d->state != FLYING) return; /* invalid, or not flying -> do nothing */

  d->landing = d->time;
  d->state = LANDED;

  layer_mark_dirty(l);
}

void flighttimer_reset(Layer *l) {
  FlighttimerData *d = (FlighttimerData*) layer_get_data(l);

  d->state = PREFLIGHT;
  d->takeoff = 0;
  d->landing = 0;

  layer_mark_dirty(l);
}

static void draw_time(GContext *ctx, time_t t, GPoint p) {
  struct tm *tt = localtime(&t);
  bitmaps_draw_clock(ctx, tt->tm_hour > 0 ? CLOCK_SMALL_HM : CLOCK_SMALL_MS,
                     p, tt);
}

static void flighttimer_update(Layer *l, GContext *ctx) {
  FlighttimerData *d = (FlighttimerData*) layer_get_data(l);
  if (!d->valid) return; /* invalid -> do nothing */

  GSize clock_size = bitmaps_get_size_clock(CLOCK_SMALL_MS);
  GSize flt_size = bitmaps_get_size_text(FLT);
#define LABEL_DIST (6)

  /* display takeoff time */
  if (d->takeoff != 0) {
    draw_time(ctx, d->takeoff, (GPoint){.x = 6, .y = 0 });
  }
  /* display landing time */
  if (d->landing != 0) {
    draw_time(ctx, d->landing,
              (GPoint){.x = WIDTH - LABEL_DIST - bitmaps_get_size_clock(CLOCK_SMALL_MS).w, .y = 0 });
  }

  graphics_context_set_stroke_color(ctx, GColorWhite);
  GPoint p0 = { .x = clock_size.w/2, .y = clock_size.h + 2 };
  GPoint p1 = { .x = clock_size.w + LABEL_DIST, .y = p0.y };
  graphics_draw_line(ctx, p0, p1);
  p0 = p1;
  p1.x += clock_size.h/2;
  p1.y = clock_size.h/2;
  graphics_draw_line(ctx, p0, p1);
  p0 = p1;
  p1.x = WIDTH - LABEL_DIST - clock_size.w - clock_size.h/2;
  //graphics_draw_line(ctx, p0, p1);
  p0 = p1;
  p1.x += clock_size.h/2;
  p1.y = clock_size.h + 2;
  graphics_draw_line(ctx, p0, p1);
  p0 = p1;
  p1.x = WIDTH - clock_size.w/2;
  graphics_draw_line(ctx, p0, p1);

  /* calculate time diff and display the time */
  time_t diff = 0;
  switch (d->state) {
    case FLYING:
      diff = d->time - d->takeoff;
      break;
    case LANDED:
      diff = d->landing - d->takeoff;
      break;
    default:
      break;
  }
  struct tm tm_diff = {
    .tm_hour  = (diff / 3600) % 24,
    .tm_min   = (diff / 60) % 60,
    .tm_sec   = diff % 60
  };

  int width = flt_size.w + LABEL_DIST +
              clock_size.w;
  GPoint p = {.x = WIDTH/2 - width/2, .y = clock_size.h + 5 };

  bitmaps_draw_text(ctx, FLT, p);
  p.x += flt_size.w + LABEL_DIST;
  bitmaps_draw_clock(ctx, tm_diff.tm_hour > 0 ? CLOCK_SMALL_HM : CLOCK_SMALL_MS, p, &tm_diff);
}
