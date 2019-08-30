#ifndef COLOR_H
#define COLOR_H

typedef struct col_t {
  unsigned char r;
  unsigned char g;
  unsigned char b;
} col_t;

void color_print(col_t col);

#endif
