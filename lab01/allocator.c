#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "block-header.h"
#include "allocator.h"

size_t make_multiple_of_four(size_t size) {
  return ((size + 3) / 4) * 4;
}

size_t make_multiple_of_eight(size_t size) {
  return ((size + 7) / 8) * 8;
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

int max_size_length = 0;
size_t block_header_size;

void mem_dump() {

  void* current_block_pointer = start_pointer + divided_lists_size;
  BlockHeader current_block = read_block_header(current_block_pointer, max_size_length);

  while(current_block.current_block_size > 0) {
    current_block = read_block_header(current_block_pointer, max_size_length);

    printf("\n=== HEADER ===\n");
    printf("ADDRESS: %p\n", current_block_pointer);
    printf("IS FREE: %s\n", current_block.is_free ? "true" : "false");
    printf("CURRENT BLOCK SIZE: %d\n", (int)current_block.current_block_size);
    printf("PREVIOUS BLOCK SIZE: %d\n", (int)current_block.previous_block_size);

    current_block_pointer += block_header_size + current_block.current_block_size;
  }

  printf("\n\n");
}

void init(size_t size) {
  size = make_multiple_of_four(size);
  total_size = size;
  start_pointer = malloc(total_size);
  memset(start_pointer, 0, total_size);

  max_block_size_for_lists =
    get_divided_lists_max_block_size(min_block_size_for_lists,
      list_length,
      total_size / 2);

  divided_lists_size =
    get_divided_lists_size(min_block_size_for_lists,
      list_length,
      max_block_size_for_lists);

  additional_size = total_size - divided_lists_size;

  row_size = divided_lists_size / list_length;

  for (int i = additional_size; i > 0; i /= 2, max_size_length++);

  block_header_size = make_multiple_of_eight((max_size_length * 2) + 1) / 8;

  BlockHeader start_block_header;
  start_block_header.is_free = true;
  start_block_header.current_block_size = additional_size - block_header_size * 2;
  start_block_header.previous_block_size = 0;

  BlockHeader end_block_header;
  end_block_header.is_free = false;
  end_block_header.current_block_size = 0;
  end_block_header.previous_block_size = additional_size - block_header_size * 2;

  write_block_header(start_pointer + divided_lists_size, &start_block_header, block_header_size, max_size_length);
  write_block_header(start_pointer + divided_lists_size + additional_size + block_header_size, &start_block_header, block_header_size, max_size_length);
}

void* alloc_in_lists(size_t size) {
  size = size == min_block_size_for_lists ? 0 : size >> 1;
  for (int i = 0; i < list_length; i++) {
    void* current_pointer = start_pointer + size + row_size * i;
    if (!(*(bool*)current_pointer)) {
      *(bool*)current_pointer = true;
      return current_pointer + 1;
    }
  }
}

void* alloc_in_additional(size_t size) {
  void* current_block_pointer = start_pointer + divided_lists_size;
  void* best_block_pointer = NULL;
  BlockHeader current_block = read_block_header(current_block_pointer, max_size_length);
  BlockHeader best_block;

  while (current_block.current_block_size > 0) {

    if (current_block.is_free && (current_block.current_block_size == size
                                  ||
                                  current_block.current_block_size - size <= block_header_size)) {

      current_block.is_free = false;
      write_block_header(current_block_pointer, &current_block, block_header_size, max_size_length);
      return current_block_pointer + block_header_size;

    } else if (best_block_pointer == NULL && current_block.is_free && current_block.current_block_size >= size + block_header_size) {
      best_block_pointer = current_block_pointer;
      best_block = current_block;

    } else if (current_block.is_free
      && current_block.current_block_size >= (size + block_header_size)
      && current_block.current_block_size - (size + block_header_size)
      < best_block.current_block_size - (size + block_header_size)) {

        best_block_pointer = current_block_pointer;
        best_block = current_block;
    }

    current_block_pointer += block_header_size + current_block.current_block_size;
    current_block = read_block_header(current_block_pointer, max_size_length);
  }

  if (best_block_pointer == NULL) return NULL;

  void* new_block_pointer = best_block_pointer  + (best_block.current_block_size - size);
  void* next_block_pointer = best_block_pointer + best_block.current_block_size + block_header_size;

  BlockHeader next_block_header = read_block_header(next_block_pointer, max_size_length);
  next_block_header.previous_block_size = size;


  BlockHeader new_block_header;
  new_block_header.is_free = false;
  new_block_header.current_block_size = size;
  new_block_header.previous_block_size = best_block.current_block_size - block_header_size - size;

  best_block.current_block_size = new_block_header.previous_block_size;

  write_block_header(next_block_pointer, &next_block_header, block_header_size, max_size_length);
  write_block_header(best_block_pointer, &best_block, block_header_size, max_size_length);
  write_block_header(new_block_pointer, &new_block_header, block_header_size, max_size_length);

  return (new_block_pointer + block_header_size);
}

void* mem_alloc(size_t size) {
  void* result = NULL;

  size_t needed_size = get_rounded_size(size, min_block_size_for_lists, max_block_size_for_lists);

  if ((size > max_block_size_for_lists)
      || (needed_size - size + 1 > block_header_size)) {

    result = alloc_in_additional(size);
  }

  if (result != NULL) return result;

  return alloc_in_lists(needed_size);
}


void join_blocks(void* pointer) {
  void* block_header_pointer = pointer;
  BlockHeader block_header = read_block_header(block_header_pointer, max_size_length);

  void* next_block_header_pointer = pointer + block_header.current_block_size + block_header_size;
  BlockHeader next_block_header = read_block_header(next_block_header_pointer, max_size_length);

  while (next_block_header.is_free) {
    block_header.current_block_size += next_block_header.current_block_size + block_header_size;
    next_block_header_pointer += next_block_header.current_block_size + block_header_size;
    next_block_header = read_block_header(next_block_header_pointer, max_size_length);
  }

  next_block_header.previous_block_size = block_header.current_block_size;

  write_block_header(block_header_pointer, &block_header, block_header_size, max_size_length);
  write_block_header(next_block_header_pointer, &next_block_header, block_header_size, max_size_length);
}

void free_in_lists(void* pointer) {
  *((bool*)(--pointer)) = false;
}

void free_in_additional(void* pointer) {
  void* block_header_pointer = pointer - block_header_size;
  BlockHeader block_header = read_block_header(block_header_pointer, max_size_length);

  void* prev_block_header_pointer = block_header_pointer - block_header.previous_block_size - block_header_size;
  void* next_block_header_pointer = block_header_pointer + block_header.previous_block_size + block_header_size;
  BlockHeader prev_block_header = read_block_header(prev_block_header_pointer, max_size_length);
  BlockHeader next_block_header = read_block_header(next_block_header_pointer, max_size_length);

  block_header.is_free = true;
  write_block_header(block_header_pointer, &block_header, block_header_size, max_size_length);

  join_blocks(prev_block_header.is_free ? prev_block_header_pointer : block_header_pointer);
}

void mem_free(void* pointer) {
  if (pointer > start_pointer &&
    pointer <= start_pointer + list_length * row_size) {
      free_in_lists(pointer);

  } else if (pointer > start_pointer + divided_lists_size &&
    pointer < start_pointer + total_size) {
      free_in_additional(pointer);
  }
}

size_t get_size_by_address(void* pointer) {
  int diff = pointer - start_pointer;
  return (diff % row_size) + 8;
}

void* mem_realloc_in_lists(void* pointer, size_t size) {
  size_t old_size = get_size_by_address(pointer);

  if (old_size >= size) {
    return pointer;
  }

  void* new_pointer = mem_alloc(size);

  if (new_pointer == NULL) return NULL;

  memcpy(new_pointer, pointer, old_size < size ? old_size : size);
  mem_free(pointer);
  return new_pointer;
}

void* mem_realloc_in_additional(void* pointer, size_t size) {
  void* new_block_header_pointer = NULL;
  void* block_header_pointer = pointer - block_header_size;
  BlockHeader block_header = read_block_header(block_header_pointer, max_size_length);
  int difference = size - block_header.current_block_size;

  void* prev_block_header_pointer = block_header_pointer - block_header.previous_block_size - block_header_size;
  void* next_block_header_pointer = block_header_pointer + block_header.current_block_size + block_header_size;
  BlockHeader prev_block_header = read_block_header(prev_block_header_pointer, max_size_length);
  BlockHeader next_block_header = read_block_header(next_block_header_pointer, max_size_length);

  void* buffer = (void*)malloc(block_header.current_block_size);
  memcpy(buffer, pointer, block_header.current_block_size);

  if (block_header.current_block_size > size) {

    if (-difference > block_header_size) {
      block_header.current_block_size = size;

      BlockHeader new_block_header;
      new_block_header.current_block_size = -difference - block_header_size;
      new_block_header.previous_block_size = size;
      new_block_header.is_free = true;

      next_block_header.previous_block_size = -difference - block_header_size;

      write_block_header(pointer + size, &new_block_header, block_header_size, max_size_length);

      new_block_header_pointer = block_header_pointer;
    } else {
      return pointer;
    }
  }

  else if (block_header.current_block_size + prev_block_header.current_block_size > size && prev_block_header.is_free) {
    new_block_header_pointer = block_header_pointer - difference;

    prev_block_header.current_block_size -= difference;
    next_block_header.previous_block_size = size;
    block_header.current_block_size = size;
    block_header.previous_block_size = prev_block_header.current_block_size;

  } else if (block_header.current_block_size + next_block_header.current_block_size > size && next_block_header.is_free) {

    next_block_header_pointer += difference;

    next_block_header.current_block_size -= difference;
    next_block_header.previous_block_size = size;
    block_header.current_block_size = size;
    new_block_header_pointer = block_header_pointer;

  } else if (block_header.current_block_size
    + prev_block_header.current_block_size
    + next_block_header.current_block_size >= size
    && prev_block_header.is_free
    && next_block_header.is_free) {
      printf("Join with both\n");
      new_block_header_pointer = prev_block_header_pointer;
      next_block_header_pointer += size + block_header_size;

      next_block_header.current_block_size += block_header_size
            - (size - block_header.current_block_size - prev_block_header.current_block_size);

      prev_block_header.current_block_size = size;
      next_block_header.previous_block_size = size;
      block_header.current_block_size = size;
      block_header.previous_block_size = prev_block_header.previous_block_size;

    } else {
      new_block_header_pointer = mem_alloc(size);
      if (new_block_header_pointer != NULL) {
        block_header.is_free = true;
        write_block_header(pointer - block_header_size, &block_header, block_header_size, max_size_length);
        join_blocks(prev_block_header.is_free ? prev_block_header_pointer : block_header_pointer);
        memcpy(new_block_header_pointer + block_header_size, buffer, difference > 0 ? block_header.current_block_size : size);
      }
      return new_block_header_pointer;
    }

  memcpy(new_block_header_pointer + block_header_size, buffer, difference > 0 ? block_header.current_block_size : size);

  write_block_header(prev_block_header_pointer, &prev_block_header, block_header_size, max_size_length);
  write_block_header(next_block_header_pointer, &next_block_header, block_header_size, max_size_length);
  write_block_header(new_block_header_pointer, &block_header, block_header_size, max_size_length);

  return (new_block_header_pointer + block_header_size);
}

void* mem_realloc(void* pointer, size_t size) {

  if (pointer == NULL) {
    return mem_alloc(size);
  }

  void* result = NULL;
  size_t old_size;
  size_t needed_size = get_rounded_size(size, min_block_size_for_lists, max_block_size_for_lists);
  bool is_in_lists = false;

  if (pointer > start_pointer &&
    pointer <= start_pointer + divided_lists_size) {
      is_in_lists = true;
      old_size = get_size_by_address(pointer);

  } else if (pointer > start_pointer + divided_lists_size &&
    pointer < start_pointer + total_size) {
      old_size = read_block_header(pointer - block_header_size, max_size_length).current_block_size;
  }

  if ((size > max_block_size_for_lists)
       || (needed_size - size + 1 > block_header_size)) {

       if (is_in_lists) result = alloc_in_additional(size);
       else return mem_realloc_in_additional(pointer, size);

  } else {
    if (is_in_lists) return mem_realloc_in_lists(pointer, size);
    else result = alloc_in_lists(size);
  }

  if (result != NULL) {
    memcpy(result, pointer, old_size < size ? old_size : size);
    is_in_lists ? free_in_lists(pointer) : free_in_additional(pointer);
  }


  return result;
}
