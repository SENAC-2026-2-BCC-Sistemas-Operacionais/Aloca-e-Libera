#include "aloca-e-libera.h"
#include <stdint.h>

#define MEMORY_SIZE 16384 // 16 KB
#define MEMORY_HEAD_SIZE sizeof(uint16_t)
#define MEMORY_MAX_AVAILABLE_SIZE MEMORY_SIZE - MEMORY_HEAD_SIZE
#define END_OF_MEMORY_FLAG 0
#define METADATA_SIZE sizeof(Metadata)

// 1st offset is reserved for the memory head
// HEAD = 0  → empty memory
// HEAD != 0 → points to first block of occupied memory
static unsigned char MEM[MEMORY_SIZE] = {0};

// metadata always starts at the beginning of a block
// (offset + METADATA_SIZE) is the start of the data
// allocated_size always have data (allocated_size > 0)
// next_offset always points to next metadata
// next_offset > current offset when a next block exists
// next_offset == END_OF_MEMORY_FLAG means that current block is the last one
typedef struct Metadata {
  uint16_t allocated_size;
  uint16_t next_offset;
} Metadata;

static uint16_t get_head();
static void set_head(uint16_t offset);
static int is_mem_empty();
static Metadata *get_metadata(uint16_t offset);
static void new_metadata(uint16_t offset, uint16_t size, uint16_t next_offset);
static uint16_t get_block_end(uint16_t offset);
static void *get_block_pointer(uint16_t offset);
static uint16_t calculate_free_space_after_block(uint16_t offset);

void *aloca(size_t size) {
  // check max size before storing value inside uint16_t var to avoid overflow
  if (size == 0 || size > MEMORY_MAX_AVAILABLE_SIZE - METADATA_SIZE)
    return NULL;

  uint16_t needed_space = size + METADATA_SIZE;
  uint16_t curr_offset = get_head();

  int is_empty = is_mem_empty();
  int has_space_after_head =
      is_empty || curr_offset - MEMORY_HEAD_SIZE >= needed_space;

  if (has_space_after_head) {
    new_metadata(MEMORY_HEAD_SIZE, size,
                 is_empty ? END_OF_MEMORY_FLAG : curr_offset);
    set_head(MEMORY_HEAD_SIZE);
    return get_block_pointer(MEMORY_HEAD_SIZE);
  }

  Metadata *metadata;
  do {
    metadata = get_metadata(curr_offset);

    if (calculate_free_space_after_block(curr_offset) >= needed_space) {
      uint16_t new_metadata_offset = get_block_end(curr_offset);
      new_metadata(new_metadata_offset, size, metadata->next_offset);
      metadata->next_offset = new_metadata_offset;
      return get_block_pointer(new_metadata_offset);
    }

    curr_offset = metadata->next_offset;
  } while (metadata->next_offset != END_OF_MEMORY_FLAG);

  return NULL;
}

void libera(void *ptr) {
  if (ptr == NULL || is_mem_empty())
    return;

  uint16_t curr_offset = get_head();
  Metadata *prev_metadata = NULL, *curr_metadata;
  do {
    curr_metadata = get_metadata(curr_offset);

    if (get_block_pointer(curr_offset) == ptr) {
      if (prev_metadata == NULL)
        set_head(curr_metadata->next_offset);
      else
        prev_metadata->next_offset = curr_metadata->next_offset;
      return;
    }

    prev_metadata = curr_metadata;
    curr_offset = curr_metadata->next_offset;
  } while (curr_metadata->next_offset != END_OF_MEMORY_FLAG);
}

uint16_t get_head() { return *(uint16_t *)&MEM[0]; }

void set_head(uint16_t offset) { *(uint16_t *)&MEM[0] = offset; }

int is_mem_empty() { return get_head() == 0; }

Metadata *get_metadata(uint16_t offset) { return (Metadata *)&MEM[offset]; }

void new_metadata(uint16_t offset, uint16_t size, uint16_t next_offset) {
  Metadata *metadata = (Metadata *)&MEM[offset];
  metadata->allocated_size = size;
  metadata->next_offset = next_offset;
}

uint16_t get_block_end(uint16_t offset) {
  return offset + METADATA_SIZE + get_metadata(offset)->allocated_size;
}

void *get_block_pointer(uint16_t offset) {
  return (void *)&MEM[offset + METADATA_SIZE];
}

uint16_t calculate_free_space_after_block(uint16_t offset) {
  uint16_t next_offset = get_metadata(offset)->next_offset;
  return next_offset == END_OF_MEMORY_FLAG
             ? MEMORY_SIZE - get_block_end(offset)
             : next_offset - get_block_end(offset);
}
