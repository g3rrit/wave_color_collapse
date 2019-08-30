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

  pos_t pos_arr[w * h];
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

    printf("pos: (%d, %d)\n", x, y);
    col_t col = wave_color_pos(img, x, y);
    color_print(col);
    img_set(img, x, y, col);

  }

}

double wave_color_prob(img_t *img, uint x, uint y, col_t col) {
  if(x == 0 || y == 0 || x == img->width - 1|| y == img->height - 1) {
    return 0;
  }
  //printf("x: %d, y: %d\n", x, y);

  col_t ca[8] = { 0 };
  ca[0] = img_at(img, x - 1, y);
  ca[1] = img_at(img, x - 1, y - 1);
  ca[2] = img_at(img, x, y - 1);
  ca[3] = img_at(img, x + 1, y - 1);
  ca[4] = img_at(img, x + 1, y);
  ca[5] = img_at(img, x + 1, y + 1);
  ca[6] = img_at(img, x, y + 1);
  ca[7] = img_at(img, x - 1, y + 1);

  uint r = 0;
  uint g = 0;
  uint b = 0;
  uint c = 0;
  for(uint i = 0; i < 8; i++) {
    if(ca[i].r == 0 && ca[i].g == 0 && ca[i].b == 0) {
      continue;
    }
    c++;
    r = ca[i].r;
    g = ca[i].g;
    b = ca[i].b;
  }
  if(c == 0) return 1.0;

  col_t avg = {
    .r = r / c,
    .g = g / c,
    .b = b / c
  };

  uint dist = abs(col.r - r) + abs(col.g - g) + abs(col.b - b);
  if(dist == 0) {
    return 0;
  }

  return (4.0 / dist);

}

#define r_range 25
#define g_range 25
#define b_range 25
#define col_range r_range * g_range * b_range

col_t wave_color_pos(img_t *img, uint x, uint y) {
  uint w = img->width;
  uint h = img->height;
  static double col_dens[col_range] = { 0 };

  for(unsigned char r = 0; r < r_range; r++) {
    for(unsigned char g = 0; g < g_range; g++) {
      for(unsigned char b = 0; b < b_range; b++) {
        uint i = (r * b_range * g_range) + (g * r_range) + b;
        col_t col =  {
          .r = (r / r_range) * 255,
          .g = (g / g_range) * 255,
          .b = (b / b_range) * 255
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
    .r = (((cp / (g_range * b_range)) % r_range) / r_range) * 255,
    .g = (((cp / b_range) % g_range) / g_range) * 255,
    .b = ((cp % b_range) / b_range) * 255
  };
}
