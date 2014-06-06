#ifndef BITMAPS_H
#define BITMAPS_H

#include <pebble.h>

typedef enum {
  T           = 0,
  LCL,
  UTC,
  FLT,
  TEXT_LAST
} Text;

typedef enum {
  NUMBER_T      = 0,
  SMALL,
  LARGE,
  NUMBER_LAST,
  NONE        = -1
} NumberStyle;

typedef enum {
  STYLE_LARGE,
  STYLE_LARGE_MINUTE,
  STYLE_SMALL,
  STYLE_SMALL_HM,
  STYLE_SMALL_MS,
  STYLE_LAST
} ClockStyle;

extern GSize bitmaps_get_size_number(NumberStyle n);
extern GSize bitmaps_get_size_text(Text t);
extern GSize bitmaps_get_size_clock(ClockStyle s);

extern void bitmaps_draw_text(GContext *ctx, Text t, GPoint p);
extern void bitmaps_draw_number(GContext *ctx, NumberStyle n, int i, GPoint p);
extern void bitmaps_draw_clock(GContext *ctx, ClockStyle style, GPoint tl, struct tm *t);

extern void bitmaps_init();
extern void bitmaps_deinit();

#endif /* BITMAPS_H */
