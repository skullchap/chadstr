#include "chadstr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

str random_string(size_t length) {
  chadstr pool = str("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
  size_t randindex;
  size_t i;

  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);

  /* using nano-seconds instead of seconds */
  srand((time_t)ts.tv_nsec);

  char buffer[length+1];

  for (i = 0; i < length; ++i) {
    randindex = rand() % pool->len;
    buffer[i] = pool->data[randindex] ^ ((rand() % 2) ? 0 : 0x20);
  }

  buffer[i] = '\0';

  return str(buffer);
}

int main(void) {
  for (int i = 0; i < 1000; ++i) {
    chadstr s = random_string(120);
    puts(str(*s));
  }
  return 0;
}
