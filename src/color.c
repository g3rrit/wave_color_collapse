#include "std.h"
#include "color.h"

void col_print(col_t col) {
  printf("color (%d, %d, %d)\n", col.r, col.g, col.b);
}

col_t col_rand() {
  return (col_t) {
    .r = rri(0, 255),
    .g = rri(0, 255),
    .b = rri(0, 255)
  };
}
