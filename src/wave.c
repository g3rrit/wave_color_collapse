#include "std.h"
#include "wave.h"


void wave_color(img_t *img) {
  typedef struct pos_t {
    int v; //pos is valid
    uint x;
    uint y;
  } pos_t;

  uint w = img->width;
  uint h = img->height;

  pos_t *pos_arr = salloc(sizeof(pos_t) * w * h, "unable to alloc pos array");
  for(uint x = 0; x < w; x++) {
    for(uint y = 0; y < h; y++) {
      pos_t *p = &pos_arr[y * w + x];
      p->x = x;
      p->y = y;
      p->v = 1;
    }
  }

  for(uint pcount = w * h; pcount; pcount--) {
    uint r = rri(1, pcount);
    uint p = 0;
    for(;;) {
      if(pos_arr[p].v) { r--; }
      if(r <= 0) break;
      p++;
    }
    uint x = pos_arr[p].x;
    uint y = pos_arr[p].y;
    pos_arr[p].v = 0;

    printf("\rprog: %d/%d", w * h, pcount);
    col_t col = wave_color_pos(img, x, y);
    img_set(img, x, y, col);

  }
  printf("\n");

  free(pos_arr);
}

double wave_color_prob(img_t *img, uint x, uint y, col_t col) {
  if(x == 0 || y == 0 || x == img->width - 1|| y == img->height - 1) {
    return 0;
  }
  //printf("x: %d, y: %d\n", x, y);

  uint ix = 3;
  uint iy = 3;
  col_t ca[4 * ix * iy];
  for(uint dix = 0; dix < 2 * ix; dix++) {
    for(uint diy = 0; diy < 2 * iy; diy++) {
      ca[diy * 2 * ix + dix] = img_at(img, x + dix - ix, y + diy - iy);
    }
  }
  uint nc = 0;
  uint d = 0;
  do {
    d = rri(0, 7);
    nc++;
  } while(ca[d].r == 0 && ca[d].g == 0 && ca[d].b == 0 && nc <= 7);
  if(nc == ix * iy * 4) return 1.0;

  uint dist = abs(col.r - ca[d].r) + abs(col.g - ca[d].g) + abs(col.b - ca[d].b);
  if(dist == 0) {
    return 400;
  }
  //printf("dist: %d\n", dist);

  return (100.0 / dist);
}

#define r_range 25
#define g_range 25
#define b_range 25
#define col_range r_range * g_range * b_range

col_t wave_color_pos(img_t *img, uint x, uint y) {
  uint w = img->width;
  uint h = img->height;

  /*
  static double col_dens[col_range] = { 0 };

  for(unsigned char r = 0; r < r_range; r++) {
    for(unsigned char g = 0; g < g_range; g++) {
      for(unsigned char b = 0; b < b_range; b++) {
        uint i = (r * b_range * g_range) + (g * r_range) + b;
        col_t col =  {
          .r = ((float) r / r_range) * 255,
          .g = ((float) g / g_range) * 255,
          .b = ((float) b / b_range) * 255
        };

        col_dens[i] = wave_color_prob(img, x, y, col);
      }
    }
  }

  double dens_sum = 0;
  for(uint i = 0; i < col_range; i++) {
    dens_sum += col_dens[i];
  }
  printf("dens_sum: %lf\n", dens_sum);
  for(uint i = 0; i < col_range; i++) {
    col_dens[i] /= dens_sum;
  }

  uint r = rand();
  uint cp = 0;
  for(; cp < col_range; cp++) {
    uint dr = RAND_MAX * col_dens[cp];
    if(dr >= r) break;
    r -= dr;
  }

  return (col_t) {
    .r = ((double) ((int) ((double) cp / (g_range * b_range)) % r_range) / r_range) * 255,
    .g = ((double) ((int) ((double) cp / b_range) % g_range) / g_range) * 255,
    .b = ((double) (cp % b_range) / b_range) * 255
  };
  */


  uint ix = 1;
  uint iy = 1;

  if(x <= ix || y <= iy || x >= img->width - ix|| y >= img->height - iy) {
    return img_at(img, x, y);
  }

  col_t ca[4 * ix * iy];
  for(uint dix = 0; dix < 2 * ix; dix++) {
    for(uint diy = 0; diy < 2 * iy; diy++) {
      ca[diy * 2 * ix + dix] = img_at(img, x + dix - ix, y + diy - iy);
    }
  }

  uint nc = 0;
  uint d = 0;
  do {
    d = rri(0, 4 * ix * iy);
    nc++;
  } while(ca[d].r == 0 && ca[d].g == 0 && ca[d].b == 0 && nc <= 4 * ix * iy);
  if(nc >= 4 * ix * iy) return img_at(img, x, y);

  return ca[d];
}
