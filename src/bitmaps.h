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

extern const GSize text_sizes[TEXTS_LAST];
extern const GSize num_sizes[NUMS_LAST];

void bitmaps_draw_text(GContext *ctx, Texts t, GPoint p);
void bitmaps_draw_num(GContext *ctx, Nums n, int i, GPoint p);

void bitmaps_init();
void bitmaps_deinit();

#endif /* BITMAPS_H */
