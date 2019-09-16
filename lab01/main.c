#include <stdlib.h>
#include <stdio.h>
#include "allocator.c"
// #include "block-header.c"

int main() {
  printf("%s\n", "Hello, world!");
  init(4000);
  display();

  void* test = alloc(100);
  printf("%s\n", test != NULL ? "true" : "false");
  // test = mem_realloc(test, 12);
  // test = mem_realloc(test, 45);
  mem_free(test);

  // printf("%d\n", (int)sizeof(char));
  //
  // int my_additional_size = 256;
  // int my_max_size_length = 0;
  //
  // for (int i = my_additional_size; i > 0; i /= 2, my_max_size_length++);
  //
  // int my_block_header_size = make_multiple_of_eight((my_max_size_length * 2) + 1) / 8;
  // printf("Length for number: %d\n", my_max_size_length);
  // printf("Block size: %d\n", my_block_header_size);
  //
  // void* pointer = (void*)malloc(my_block_header_size);
  // BlockHeader block_header;
  // block_header.is_free = true;
  // block_header.current_block_size = 45;
  // block_header.previous_block_size = 65;
  //
  // printf("Is free: %s\n", block_header.is_free ? "true" : "false");
  // printf("Current block size: %d\n", (int)block_header.current_block_size);
  // printf("Previous block size: %d\n", (int)block_header.previous_block_size);
  //
  // printf("Writing:\n");
  // write_block_header(pointer, &block_header, my_block_header_size, my_max_size_length);
  // printf("Reading:\n");
  // block_header = read_block_header(pointer, my_max_size_length);
  //
  // printf("Is free: %s\n", block_header.is_free ? "true" : "false");
  // printf("Current block size: %d\n", (int)block_header.current_block_size);
  // printf("Previous block size: %d\n", (int)block_header.previous_block_size);

  return 0;
}
