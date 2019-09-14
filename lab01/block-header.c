#include <stddef.h>
#include <stdbool.h>

typedef struct BlockHeader BlockHeader;

struct BlockHeader {
  bool is_free;
  size_t current_block_size;
  size_t previous_block_size;
};

void write_block_header(void* pointer,
                        BlockHeader* block_header,
                        size_t block_size,
                        int max_size_length) {

  unsigned char* char_pointer = (unsigned char*)pointer;
  unsigned char chunk = 0;
  int current_bit = 0;
  chunk |= block_header -> is_free ? 1 : 0;

  size_t current_number = block_header -> current_block_size;

  for (int i = 0; i < (max_size_length * 2); i++) {

    if (i == max_size_length) {
      current_number = block_header -> previous_block_size;
    }

    current_bit++;

    if (current_bit == 8) {
      *char_pointer = chunk;
      char_pointer++;
      current_bit = 0;
      chunk = 0;
    }

    chunk = chunk << 1;
    chunk |= current_number % 2;
    current_number = current_number >> 1;
  }

  for (int i = 0; i < (block_size * 8 - (max_size_length * 2) - 1); i++) {
    chunk = chunk << 1;
  }

  *char_pointer = chunk;
}


BlockHeader read_block_header(void* pointer,  int max_size_length) {
  bool is_free = false;
  size_t current_block_size = 0;
  size_t previous_block_size = 0;

  unsigned char* char_pointer = (unsigned char*)pointer;
  unsigned char chunk = *char_pointer;

  is_free = (1 << 7) & chunk;
  chunk = chunk << 1;
  int current_bit = 0;

  unsigned int current_number = 0;
  for (int i = 0; i < (max_size_length * 2); i++) {

    if (i == max_size_length) {
      current_block_size = current_number >> (4 * 8 - max_size_length);
      current_number = 0;
    }

    current_bit++;

    if (current_bit == 8) {
      chunk = *(++char_pointer);
      current_bit = 0;
    }

    current_number = current_number >> 1;

    if (chunk & 128) {
      current_number = current_number | 2147483648;
    }
    chunk = chunk << 1;
  }

  previous_block_size = current_number >> 4 * 8 - max_size_length;

  BlockHeader block_header;
  block_header.is_free = is_free;
  block_header.current_block_size = current_block_size;
  block_header.previous_block_size = previous_block_size;
  return block_header;
}
