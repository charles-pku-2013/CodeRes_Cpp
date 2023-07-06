#include <stdio.h>

extern char **environ;

int main() {
  char **s = environ;

  for (; *s; s++) {
    printf("%s\n", *s);
  }

  return 0;
}
