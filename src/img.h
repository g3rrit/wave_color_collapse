#ifndef IMG_H
#define IMG_H

#include "std.h"
#include "color.h"

typedef struct img_t {
  uint width;
  uint height;
  col_t *col_buf;
} img_t;

img_t *img_new(uint width, uint height, col_t sc);

void img_delete(img_t *this);

col_t img_at(img_t *this, uint x, uint y);

void img_set(img_t *this, uint x, uint y, col_t col);

typedef struct img_info_t {
  img_t *img;
  uint x;
  uint y;
  col_t col;
} img_info_t;

void img_map(img_t *this, void (*fn)(img_info_t *info));

img_t *img_load(char *path);

int img_save(img_t *this, uint dpi, char *path);

#endif
