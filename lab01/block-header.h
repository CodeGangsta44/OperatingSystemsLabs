typedef struct BlockHeader BlockHeader;

struct BlockHeader {
  bool is_free;
  size_t current_block_size;
  size_t previous_block_size;
};

void write_block_header(void* pointer,
                        BlockHeader* block_header,
                        size_t block_size,
                        int max_size_length);


BlockHeader read_block_header(void* pointer,  int max_size_length);
