void mem_dump();

void init(size_t size);

void* mem_alloc(size_t size);

void mem_free(void* pointer);

void* mem_realloc(void* pointer, size_t size);
