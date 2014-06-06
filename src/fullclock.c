#include "fullclock.h"

#include <pebble.h>
#include <pebble_fonts.h>

#include "bitmaps.h"

#define WIDTH (144)
#define HEIGHT (30)

typedef struct {
  struct tm time;
  Texts title;
  bool valid;
} FullClockData;

static void full_clock_update(Layer *l, GContext *ctx);

Layer *full_clock_create(const Texts title, const GPoint r) {
  Layer *l = layer_create_with_data((GRect){.origin = r, .size = { WIDTH, HEIGHT }},
                                    sizeof(FullClockData));
  layer_set_update_proc(l, full_clock_update);

  FullClockData *d = (FullClockData*) layer_get_data(l);
  d->valid = false;
  d->title = title;

  return l;
}

void full_clock_destroy(Layer *l) {
  layer_destroy(l);
}

void full_clock_set_time(Layer *l, const struct tm *time) {
  FullClockData *d = (FullClockData*) layer_get_data(l);
  d->time = *time;
  d->valid = true;

  layer_mark_dirty(l);
}

static void full_clock_update(Layer *l, GContext *ctx) {
  /*int text_x[STYLE_LAST] = {
    [STYLE_LARGE] = 6 + 4*(num_sizes[LARGE].w + 1) + 6 + 5,
    [STYLE_LARGE_MINUTE] = 6 + 4*(num_sizes[LARGE].w + 1) + 6 + 5,
    [STYLE_SMALL] = 6 + 4*(num_sizes[LARGE].w + 1) + 6 + 5,
    [STYLE_SMALL_HM] = 6 + 4*(num_sizes[LARGE].w + 1) + 6 + 5,
    [STYLE_SMALL_MS] = 6 + 4*(num_sizes[LARGE].w + 1) + 6 + 5
  };*/
  /*
  int begin_x[STYLE_LAST] = {
    [STYLE_LARGE] = WIDTH/2 - 3*num_sizes[LARGE].w - 3 - 1,
    [STYLE_LARGE_MINUTE] = WIDTH/2 - 2*num_sizes[SMALL].w - num_sizes[LARGE].w - 4,
    [STYLE_SMALL] = WIDTH - 9*num_sizes[SMALL].w - 5 - 2,
    [STYLE_SMALL_HM] = WIDTH - 7*num_sizes[SMALL].w - 5 - 2,
    [STYLE_SMALL_MS] = WIDTH - 7*num_sizes[SMALL].w - 5 - 2
  };*/

  FullClockData *d = (FullClockData*) layer_get_data(l);

  if (!d->valid) return;

  Style s = STYLE_LARGE;
  switch ((int)d->title) {
    case LCL:
      s = STYLE_LARGE;
      break;
    case UTC:
      s = STYLE_LARGE_MINUTE;
      break;
    case FLT:
      s = STYLE_SMALL;
      break;
    case 123:
      s = STYLE_SMALL_HM;
      break;
    case 124:
      s = STYLE_SMALL_MS;
      break;
    default:
      return;
  }

  GSize size = bitmaps_get_size_clock(s);
  int begin_x = WIDTH/2 - size.w/2;
  bitmaps_draw_clock(ctx, s, (GPoint){.x = begin_x, .y = 0}, &d->time);

  //bitmaps_draw_text(ctx, d->title, (GPoint){ .x = text_x[style], .y = 0 });
}