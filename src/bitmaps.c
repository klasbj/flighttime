#include "bitmaps.h"
#include <pebble.h>

static GBitmap* nums_bmps[NUMS_LAST][10] = { {NULL,}, };

static GBitmap* texts_bmps[TEXTS_LAST] = { NULL,};


/*
 * Size settings
 */
#define LARGE_HEIGHT (30)
#define LARGE_WIDTH  (24)
#define SMALL_HEIGHT (14)
#define SMALL_WIDTH  (12)

#define THREE_TEXT_HEIGHT (14)
#define THREE_TEXT_WIDTH  (SMALL_WIDTH*3 + 2)


const GSize text_sizes[TEXTS_LAST] = {
    [T]     = { .w = SMALL_WIDTH, .h = SMALL_HEIGHT }
  , [LCL]   = { .w = THREE_TEXT_WIDTH, .h = SMALL_HEIGHT }
  , [UTC]   = { .w = THREE_TEXT_WIDTH, .h = SMALL_HEIGHT }
  , [FLT]   = { .w = THREE_TEXT_WIDTH, .h = SMALL_HEIGHT }
};
const GSize num_sizes[NUMS_LAST] = {
    [SMALL] = { .w = SMALL_WIDTH, .h = SMALL_HEIGHT }
  , [LARGE] = { .w = LARGE_WIDTH, .h = LARGE_HEIGHT }
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

static void load_and_draw(GContext *ctx, GBitmap* *bmp, ResourceId id, GPoint p) {
  if (*bmp == NULL) {
    *bmp = gbitmap_create_with_resource(id);
  }

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
