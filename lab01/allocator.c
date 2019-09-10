#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

size_t make_multiple_of_four(size_t size) {
  return ((size + 3) / 4) * 4;
}

size_t get_divided_lists_max_block_size(size_t start_size,
                                        int list_length,
                                        size_t max_size) {

  size_t current_size = 0;

  while (true) {
    current_size += (start_size + 1) * list_length;
    if (current_size > max_size) break;
    start_size = start_size << 1;
  }

  return start_size >> 1;
}

size_t get_divided_lists_size(size_t start_size,
                              int list_length,
                              size_t max_size) {

  size_t size = 0;

  while (start_size <= max_size) {
    size += (start_size + 1) * list_length;
    start_size = start_size << 1;
  }

  return size;
}

size_t get_rounded_size(size_t size, size_t min_size, size_t max_size) {
  while (min_size < size && min_size <= max_size) {
    min_size = min_size << 1;
  }

  return min_size;
}

bool is_initialized;
size_t total_size;
size_t divided_lists_size;
size_t additional_size;
void* start_pointer;

int list_length = 10;
size_t min_block_size_for_lists = 8;
size_t max_block_size_for_lists;
size_t row_size;

void init(size_t size) {
  size = make_multiple_of_four(size);
  total_size = size;
  start_pointer = malloc(total_size);

  max_block_size_for_lists =
    get_divided_lists_max_block_size(min_block_size_for_lists,
      list_length,
      total_size / 2);

  divided_lists_size =
    get_divided_lists_size(min_block_size_for_lists,
      list_length,
      max_block_size_for_lists);

  row_size = divided_lists_size / list_length;

  memset(start_pointer, 0, total_size);
}

void* alloc_in_lists(size_t size) {
  for (int i = 0; i < list_length; i++) {
    void* current_pointer = start_pointer + size + row_size * i;
    if (!(*(bool*)current_pointer)) {
      *(bool*)current_pointer = true;
      return current_pointer + 1;
    }
  }
}

void* alloc(size_t size) {
  size = get_rounded_size(size, min_block_size_for_lists, max_block_size_for_lists);
  void* first_result = alloc_in_lists(size == min_block_size_for_lists ? 0 : size >> 1);
}

void free_in_lists(void* pointer) {
  *((bool*)(--pointer)) = false;
}

void mem_free(void* pointer) {
  if (pointer > start_pointer &&
    pointer <= start_pointer + list_length * row_size) {
      free_in_lists(pointer);
  }
}

size_t get_size_by_address(void* pointer) {
  int diff = pointer - start_pointer;
  return (diff % row_size) + 8;
}

void* mem_realloc(void* pointer, size_t size) {
  void* new_pointer = alloc(size);
  size_t old_size = get_size_by_address(pointer);
  memcpy(new_pointer, pointer, old_size);
  mem_free(pointer);
  return new_pointer;
}

void display() {
  printf("Min size: %d\n", (int)min_block_size_for_lists);
  printf("Max size: %d\n", (int)max_block_size_for_lists);
  printf("Total size: %d\n", (int)divided_lists_size);
}
