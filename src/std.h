#ifndef STD_H
#define STD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef unsigned int uint;

#define error(fmt, ...) {     \
  printf("error: ");          \
  printf(fmt, ##__VA_ARGS__); \
  exit(-1);                   \
}


static inline void *salloc(size_t size, char *err) {
  void *res = malloc(size);
  if(!res) {
    error("%s\n", err);
  }
  return res;
}

static inline void init_rand() {
  time_t t;
  srand((unsigned) time(&t));
}

static inline uint rri(uint s, uint e) {
  if(e < s) error("invalid range (%d, %d)\n", s, e);
  if(e == s) return s;
  return rand() % (e - s) + s;
}

static inline double rrf(double s, double e) {
  if(e < s) error("invalid range (%d, %d)\n", s, e);
  if(e == s) return s;
  return (((double)rand())/RAND_MAX) * (e - s) + s;
}

#endif
