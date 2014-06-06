#include "bitmaps.h"
#include <pebble.h>

static GBitmap* nums_bmps[NUMS_LAST][10] = { {NULL,}, };

static GBitmap* texts_bmps[TEXTS_LAST] = { NULL,};


/*
 * Style settings
 */

int number_styles[STYLE_LAST][6] = {
  [STYLE_LARGE] = { LARGE, LARGE, LARGE, LARGE, SMALL, SMALL },
  [STYLE_LARGE_MINUTE] = { SMALL, SMALL, LARGE, LARGE, SMALL, SMALL },
  [STYLE_SMALL] = { SMALL, SMALL, SMALL, SMALL, SMALL, SMALL },
  [STYLE_SMALL_HM] = { SMALL, SMALL, SMALL, SMALL, NONE, NONE },
  [STYLE_SMALL_MS] = { NONE, NONE, SMALL, SMALL, SMALL, SMALL },
};

/*
 * Resource ID mappings
 */
static const ResourceId resid_texts[TEXTS_LAST] = {
    [T]     = RESOURCE_ID_IMAGE_T
  , [LCL]   = RESOURCE_ID_IMAGE_LCL
  , [UTC]   = RESOURCE_ID_IMAGE_UTC
  , [FLT]   = RESOURCE_ID_IMAGE_FLT

};

static const ResourceId resid_nums[NUMS_LAST][10] = {
  [NUMS_T] = {
        [0] = RESOURCE_ID_IMAGE_S0
      , [1] = RESOURCE_ID_IMAGE_S1
      , [2] = RESOURCE_ID_IMAGE_S2
      , [3] = RESOURCE_ID_IMAGE_S3
      , [4] = RESOURCE_ID_IMAGE_S4
      , [5] = RESOURCE_ID_IMAGE_S5
      , [6] = RESOURCE_ID_IMAGE_S6
      , [7] = RESOURCE_ID_IMAGE_S7
      , [8] = RESOURCE_ID_IMAGE_S8
      , [9] = RESOURCE_ID_IMAGE_S9
  },
  [SMALL] = {
        [0] = RESOURCE_ID_IMAGE_S0
      , [1] = RESOURCE_ID_IMAGE_S1
      , [2] = RESOURCE_ID_IMAGE_S2
      , [3] = RESOURCE_ID_IMAGE_S3
      , [4] = RESOURCE_ID_IMAGE_S4
      , [5] = RESOURCE_ID_IMAGE_S5
      , [6] = RESOURCE_ID_IMAGE_S6
      , [7] = RESOURCE_ID_IMAGE_S7
      , [8] = RESOURCE_ID_IMAGE_S8
      , [9] = RESOURCE_ID_IMAGE_S9
  },
  [LARGE] = {
        [0] = RESOURCE_ID_IMAGE_L0
      , [1] = RESOURCE_ID_IMAGE_L1
      , [2] = RESOURCE_ID_IMAGE_L2
      , [3] = RESOURCE_ID_IMAGE_L3
      , [4] = RESOURCE_ID_IMAGE_L4
      , [5] = RESOURCE_ID_IMAGE_L5
      , [6] = RESOURCE_ID_IMAGE_L6
      , [7] = RESOURCE_ID_IMAGE_L7
      , [8] = RESOURCE_ID_IMAGE_L8
      , [9] = RESOURCE_ID_IMAGE_L9
  }
};

static GBitmap* load(GBitmap* *bmp, ResourceId id) {
  if (*bmp == NULL) {
    *bmp = gbitmap_create_with_resource(id);
  }
  return *bmp;
}

GSize bitmaps_get_size_num(Nums n) {
  /* TODO: Check return value */
  GBitmap* b = load(&nums_bmps[n][0], resid_nums[n][0]);
  return b->bounds.size;
}

GSize bitmaps_get_size_text(Texts t) {
  /* TODO: Check return value */
  GBitmap* b = load(&texts_bmps[t], resid_texts[t]);
  return b->bounds.size;
}

#define MAX(a,b) ((a) > (b) ? (a) : (b))

GSize bitmaps_get_size_clock(Style style) {
  GSize total_size = {0,0};
  GSize current_size = {0,0};

  for (int i = 0; i < 6; ++i) {
    if (number_styles[style][i] == NONE) continue;
    current_size = bitmaps_get_size_num(number_styles[style][i]);
    total_size.h = MAX(total_size.h, current_size.h);
    if (total_size.w > 0) {
      total_size.w += 1 + ((i+1) % 2);
    }
    total_size.w += current_size.w;
  }

  return total_size;
}

static void load_and_draw(GContext *ctx, GBitmap* *bmp, ResourceId id, GPoint p) {
  /* TODO: Check return value */
  load(bmp, id);

  GRect bounds = (*bmp)->bounds;

  graphics_draw_bitmap_in_rect(ctx, *bmp, (GRect) { .origin = p, .size = bounds.size });
}

void bitmaps_init() {
  /* deinit nums */
  for (int i = 0; i < NUMS_LAST; ++i)
    for (int j = 0; j < 10; ++j)
      if (nums_bmps[i][j] != NULL)
        nums_bmps[i][j] = NULL; //gbitmap_create_with_resource(resid_nums[i][j]);

  /* deinit texts */
  for (int i = 0; i < TEXTS_LAST; ++i)
    if (texts_bmps[i] != NULL)
      texts_bmps[i] = NULL;
}

void bitmaps_deinit() {
  /* deinit nums */
  for (int i = 0; i < NUMS_LAST; ++i)
    for (int j = 0; j < 10; ++j)
      if (nums_bmps[i][j] != NULL) {
        gbitmap_destroy(nums_bmps[i][j]);
        nums_bmps[i][j] = NULL;
      }

  /* deinit texts */
  for (int i = 0; i < TEXTS_LAST; ++i)
    if (texts_bmps[i] != NULL) {
      gbitmap_destroy(texts_bmps[i]);
      texts_bmps[i] = NULL;
    }
}

void bitmaps_draw_text(GContext *ctx, Texts t, GPoint p) {
  load_and_draw(ctx, &texts_bmps[t], resid_texts[t], p);
}

void bitmaps_draw_num(GContext *ctx, Nums n, int i, GPoint p) {
  load_and_draw(ctx, &nums_bmps[n][i], resid_nums[n][i], p);
}

static void set_two_digit_string(int *str, int num) {
  if (num > 100) {
    num %= 100;
  }

  str[0] = num/10;
  str[1] = num%10;
}

void bitmaps_draw_clock(GContext *ctx, Style style, GPoint tl, struct tm *t) {
  int nums[6];
  int height[STYLE_LAST] = {
    [STYLE_LARGE] = 30,
    [STYLE_LARGE_MINUTE] = 30,
    [STYLE_SMALL] = 14,
    [STYLE_SMALL_HM] = 14,
    [STYLE_SMALL_MS] = 14
  };

  set_two_digit_string(&nums[0], t->tm_hour);
  set_two_digit_string(&nums[2], t->tm_min);
  set_two_digit_string(&nums[4], t->tm_sec);

  int x = tl.x;
  for (int i = 0; i < 6; ++i) {
    if (number_styles[style][i] == NONE) continue;
    if (x > tl.x) {
      x += 1 + ((i+1) % 2);
    }
    bitmaps_draw_num(ctx, number_styles[style][i], nums[i], (GPoint){ .x = x, .y = tl.y + height[style] - num_sizes[number_styles[style][i]].h });
    x += num_sizes[number_styles[style][i]].w;
  }

}