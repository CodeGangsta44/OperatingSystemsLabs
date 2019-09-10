#include <stdlib.h>
#include <stdio.h>
#include "allocator.c"

int main() {
  printf("%s\n", "Hello, world!");
  init(4000);
  display();

  void* test = alloc(6);
  test = mem_realloc(test, 12);
  test = mem_realloc(test, 45);
  mem_free(test);
  
  return 0;
}
