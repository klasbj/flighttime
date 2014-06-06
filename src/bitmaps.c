#include "bitmaps.h"
#include <pebble.h>

#define MAX(a,b) ((a) > (b) ? (a) : (b))

/*
 * Memoization structures
 */

typedef struct {
  GBitmap *bmp;
  ResourceId id;
} Bitmap;

#define NUMBER_ASSIGN(Num,Size) \
  [Num] = { .bmp = NULL, .id = RESOURCE_ID_IMAGE_ ## Size ## Num }

static Bitmap numbers[NUMBER_LAST][10] = {
  [NUMBER_T] = {
      NUMBER_ASSIGN(0, S)
    , NUMBER_ASSIGN(1, S)
    , NUMBER_ASSIGN(2, S)
    , NUMBER_ASSIGN(3, S)
    , NUMBER_ASSIGN(4, S)
    , NUMBER_ASSIGN(5, S)
    , NUMBER_ASSIGN(6, S)
    , NUMBER_ASSIGN(7, S)
    , NUMBER_ASSIGN(8, S)
    , NUMBER_ASSIGN(9, S)
  },
  [SMALL] = {
      NUMBER_ASSIGN(0, S)
    , NUMBER_ASSIGN(1, S)
    , NUMBER_ASSIGN(2, S)
    , NUMBER_ASSIGN(3, S)
    , NUMBER_ASSIGN(4, S)
    , NUMBER_ASSIGN(5, S)
    , NUMBER_ASSIGN(6, S)
    , NUMBER_ASSIGN(7, S)
    , NUMBER_ASSIGN(8, S)
    , NUMBER_ASSIGN(9, S)
  },
  [LARGE] = {
      NUMBER_ASSIGN(0, L)
    , NUMBER_ASSIGN(1, L)
    , NUMBER_ASSIGN(2, L)
    , NUMBER_ASSIGN(3, L)
    , NUMBER_ASSIGN(4, L)
    , NUMBER_ASSIGN(5, L)
    , NUMBER_ASSIGN(6, L)
    , NUMBER_ASSIGN(7, L)
    , NUMBER_ASSIGN(8, L)
    , NUMBER_ASSIGN(9, L)
  }
};

#define TEXT_ASSIGN(Text) \
  [Text] = { .bmp = NULL, .id = RESOURCE_ID_IMAGE_ ## Text }

static Bitmap texts[TEXT_LAST] = {
    TEXT_ASSIGN(T)
  , TEXT_ASSIGN(LCL)
  , TEXT_ASSIGN(UTC)
  , TEXT_ASSIGN(FLT)
};

static struct { GSize size; bool valid; } total_sizes[STYLE_LAST] = { {{0},false}, };


/*
 * Style settings
 */

static const int number_styles[STYLE_LAST][6] = {
  [STYLE_LARGE] = { LARGE, LARGE, LARGE, LARGE, SMALL, SMALL },
  [STYLE_LARGE_MINUTE] = { SMALL, SMALL, LARGE, LARGE, SMALL, SMALL },
  [STYLE_SMALL] = { SMALL, SMALL, SMALL, SMALL, SMALL, SMALL },
  [STYLE_SMALL_HM] = { SMALL, SMALL, SMALL, SMALL, NONE, NONE },
  [STYLE_SMALL_MS] = { NONE, NONE, SMALL, SMALL, SMALL, SMALL },
};

/*
 * Helper functions
 */

static GBitmap* bitmaps_create(Bitmap *bmp);
static void bitmaps_destroy(Bitmap *b);
static void bitmaps_draw(GContext *ctx, Bitmap *b, GPoint p);
static void set_two_digit_string(int *str, int num);

/*
 * Externally accessible functions
 */

GSize bitmaps_get_size_num(NumberStyle n) {
  /* TODO: Check return value */
  GBitmap* b = bitmaps_create(&numbers[n][0]);
  return b->bounds.size;
}

GSize bitmaps_get_size_text(Text t) {
  /* TODO: Check return value */
  GBitmap* b = bitmaps_create(&texts[t]);
  return b->bounds.size;
}

GSize bitmaps_get_size_clock(ClockStyle style) {
  if (total_sizes[style].valid) return total_sizes[style].size;

  total_sizes[style].valid = true;
  GSize *total_size = &total_sizes[style].size;
  GSize current_size = {0,0};

  for (int i = 0; i < 6; ++i) {
    if (number_styles[style][i] == NONE) continue;
    current_size = bitmaps_get_size_num(number_styles[style][i]);
    total_size->h = MAX(total_size->h, current_size.h);
    if (total_size->w > 0) {
      total_size->w += 1 + ((i+1) % 2);
    }
    total_size->w += current_size.w;
  }

  return *total_size;
}

void bitmaps_init() {
  /* We don't actually have anything to init, do we? */
}

void bitmaps_deinit() {
  /* deinit nums */
  for (int i = 0; i < NUMBER_LAST; ++i)
    for (int j = 0; j < 10; ++j)
      bitmaps_destroy(&numbers[i][j]);

  /* deinit texts */
  for (int i = 0; i < TEXT_LAST; ++i)
      bitmaps_destroy(&texts[i]);
}

void bitmaps_draw_text(GContext *ctx, Text t, GPoint p) {
  bitmaps_draw(ctx, &texts[t], p);
}

void bitmaps_draw_num(GContext *ctx, NumberStyle n, int i, GPoint p) {
  bitmaps_draw(ctx, &numbers[n][i], p);
}

void bitmaps_draw_clock(GContext *ctx, ClockStyle style, GPoint tl, struct tm *t) {
  int nums[6];
  GSize total_size = bitmaps_get_size_clock(style);

  set_two_digit_string(&nums[0], t->tm_hour);
  set_two_digit_string(&nums[2], t->tm_min);
  set_two_digit_string(&nums[4], t->tm_sec);

  int x = tl.x;
  for (int i = 0; i < 6; ++i) {
    if (number_styles[style][i] == NONE) continue;
    GSize size = bitmaps_get_size_num(number_styles[style][i]);
    if (x > tl.x) {
      x += 1 + ((i+1) % 2);
    }
    bitmaps_draw_num(ctx, number_styles[style][i], nums[i],
                     (GPoint){ .x = x, .y = tl.y + total_size.h - size.h });
    x += size.w;
  }

}


/*
 * Helper function implementations
 */

static GBitmap* bitmaps_create(Bitmap *b) {
  if (b->bmp == NULL) {
    b->bmp = gbitmap_create_with_resource(b->id);
  }
  return b->bmp;
}

static void bitmaps_destroy(Bitmap *b) {
  if (b->bmp != NULL) {
    gbitmap_destroy(b->bmp);
    b->bmp = NULL;
  }
}

static void bitmaps_draw(GContext *ctx, Bitmap *b, GPoint p) {
  /* TODO: Check return value */
  GBitmap* bmp = bitmaps_create(b);

  GRect bounds = bmp->bounds;

  graphics_draw_bitmap_in_rect(ctx, bmp, (GRect) { .origin = p, .size = bounds.size });
}

static void set_two_digit_string(int *str, int num) {
  if (num > 100) {
    num %= 100;
  }

  str[0] = num/10;
  str[1] = num%10;
}

