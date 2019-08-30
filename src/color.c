#include "std.h"
#include "color.h"

void color_print(col_t col) {
  printf("color (%d, %d, %d)\n", col.r, col.g, col.b);
}
