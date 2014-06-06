#ifndef BITMAPS_H
#define BITMAPS_H

#include <pebble.h>

typedef enum {
  T           = 0,
  LCL,
  UTC,
  FLT,
  TEXTS_LAST
} Texts;

typedef enum {
  NUMS_T      = 0,
  SMALL,
  LARGE,
  NUMS_LAST,
  NONE        = -1
} Nums;

typedef enum {
  STYLE_LARGE,
  STYLE_LARGE_MINUTE,
  STYLE_SMALL,
  STYLE_SMALL_HM,
  STYLE_SMALL_MS,
  STYLE_LAST
} Style;

extern GSize bitmaps_get_size_num(Nums n);
extern GSize bitmaps_get_size_text(Texts t);
extern GSize bitmaps_get_size_clock(Style s);

extern void bitmaps_draw_text(GContext *ctx, Texts t, GPoint p);
extern void bitmaps_draw_num(GContext *ctx, Nums n, int i, GPoint p);
extern void bitmaps_draw_clock(GContext *ctx, Style style, GPoint tl, struct tm *t);

extern void bitmaps_init();
extern void bitmaps_deinit();

#endif /* BITMAPS_H */