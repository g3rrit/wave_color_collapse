#include "std.h"
#include "img.h"
#include "wave.h"

int main() {
  init_rand();

  img_t *img = img_load("in.bmp");

  wave_color(img);

  img_save(img, 96, "./test.bmp");

  img_delete(img);

  return 0;
}
