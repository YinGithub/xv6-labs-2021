#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int g(int x) {
  return x+3;
}

int f(int x) {
  return g(x);
}

void main(void) {
  char a[] = "Which registers contain arguments to functions? For example, which register holds 13 in main's call to printf?";
  printf("x=%d y=%d", 3);
  printf("%s.\n",a);

  exit(0);
}