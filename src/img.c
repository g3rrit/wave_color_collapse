#include "std.h"
#include "img.h"

img_t *img_new(uint width, uint height, col_t sc) {
  struct img_t *res = salloc(sizeof(img_t), "allocating image");

  res->width = width;
  res->height = height;
  res->col_buf = salloc(sizeof(col_t) * width * height, "allocating collor buffer");

  for(uint i = 0; i < width * height; i++) {
    col_t *col = &res->col_buf[i];
    col->r = sc.r;
    col->g = sc.g;
    col->b = sc.b;
  }

  return res;
}

void img_delete(img_t *this) {
  if(!this) return;
  free(this->col_buf);
  free(this);
}

col_t img_at(img_t *this, uint x, uint y) {
  if(y >= this->height || x >= this->width) {
    error("pixel reading out of bounds (%d, %d) - x: %d y: %d\n", this->width, this->height, x, y);
  }
  return this->col_buf[y * this->width + x];
}

void img_set(img_t *this, uint x, uint y, col_t col) {
  if(y >= this->height || x >= this->width) {
    error("pixel setting out of bounds (%d, %d) - x: %d y: %d\n", this->width, this->height, x, y);
  }
  this->col_buf[y * this->width + x] = col;
}

void img_map(img_t *this, void (*fn)(img_info_t *info)) {
  img_info_t info;
  info.img = this;

  for(uint x = 0; x < this->width; x++) {
    for(uint y = 0; y < this->height; y++) {
      info.x = x;
      info.y = y;
      info.col = img_at(this, x, y);
      fn(&info);
    }
  }
}

int img_save(img_t *this, uint dpi, char *path) {
  if(!this) return 0;

  FILE *file = fopen(path, "wb");
  if(!file) {
    error("unable to open output file");
  }

  int image_size = this->width * this->height;
  int file_size = 54 + 4 * image_size;
  int ppm = dpi * 39.375;

  struct {
    unsigned char bitmap_type[2];
    int file_size;
    short r_1;
    short r_2;
    unsigned int offset_bits;
  } bfh;

  struct {
    unsigned int size_header;
    unsigned int width;
    unsigned int height;
    short int planes;
    short int bit_count;
    unsigned int compression;
    unsigned int image_size;
    unsigned int ppm_x;
    unsigned int ppm_y;
    unsigned int clr_used;
    unsigned int clr_important;
  } bih;

  memcpy(&bfh.bitmap_type, "BM", 2);
  bfh.file_size = file_size;
  bfh.r_1 = 0;
  bfh.r_2 = 0;
  bfh.offset_bits = 0;

  bih.size_header = sizeof(bih);
  bih.width = this->width;
  bih.height = this->height;
  bih.planes = 1;
  bih.bit_count = 24;
  bih.compression = 0;
  bih.image_size = file_size;
  bih.ppm_x = ppm;
  bih.ppm_y = ppm;
  bih.clr_used = 0;
  bih.clr_important = 0;

  fwrite(&bfh, 1, 14, file);
  fwrite(&bih, 1, sizeof(bih), file);

  for(int i = 0; i < image_size; i++) {
    unsigned char color[3] = { this->col_buf[i].b, this->col_buf[i].g, this->col_buf[i].r };

    fwrite(color, 1, sizeof(color), file);
  }

  fclose(file);

  return 1;
}
