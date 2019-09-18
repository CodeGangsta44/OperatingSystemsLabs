#include <stdlib.h>
#include <stdio.h>
#include "allocator.h"
// #include "block-header.c"

int main() {
  init(4000);
  mem_dump();

  void* test = mem_alloc(100);
  mem_dump();

  void* test2 = mem_alloc(100);
  mem_dump();

  void* test3 = mem_alloc(100);
  mem_dump();

  void* test4 = mem_alloc(100);
  mem_dump();

  test2 = mem_realloc(test2, 32);
  mem_dump();

  return 0;
}
