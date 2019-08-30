#include "std.h"
#include "img.h"
#include "wave.h"

int main() {
  init_rand();

  img_t *img = img_new(500, 500, (col_t) { .r = 0, .g = 0, .b = 0 });

  wave_color(img);

  img_save(img, 96, "./test.bmp");

  img_delete(img);

  return 0;
}
