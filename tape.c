#include "tape.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define BASE_UNLIMITED 1024

struct tape_t {
  uint32_t ptr, max_size;
  bool has_max, wrap; // wrap only needed if has_max is false
  uint8_t *blocks;
};

tape_t *create_tape(uint32_t size) {
  tape_t *t = (tape_t *) malloc(sizeof(struct tape_t));

  if(!t) return NULL;

  t->has_max = size != UNLIMITED_MEMORY;
  t->wrap = false;
  t->blocks = (uint8_t *) calloc(t->has_max ? size : BASE_UNLIMITED, sizeof(uint8_t));

  if(!t->blocks) {
    free(t);
    return NULL;
  }

  t->max_size = t->has_max ? size : BASE_UNLIMITED;
  t->ptr = 0;
  return t;
} 

void delete_tape(tape_t **t) {
  if(*t) {
    free((*t)->blocks);
    free(*t);
    *t = NULL;
  }
}

void increment_ptr(tape_t *t) {
  if(t->has_max || (!t->has_max && t->wrap)) {
    t->ptr = (t->ptr + 1) % t->max_size;
    if(t->ptr == 0) t->wrap = false;
    return;
  }

  // Expand tape if !t->has_max
  if(++t->ptr == t->max_size) {
    t->max_size *= 2;
    uint8_t *temp = (uint8_t *) realloc(t->blocks, t->max_size * sizeof(uint8_t));

    if(!temp) {
      t->ptr = (t->ptr + 1) % t->max_size; // wrap if realloc fails
      return;
    }

    t->blocks = temp;

    // all new blocks must be initialized to 0
    memset(t->blocks + t->ptr, 0, (t->max_size - t->ptr) * sizeof(uint8_t));
  }
}

void decrement_ptr(tape_t *t) {
  t->ptr = (t->ptr + t->max_size - 1) % t->max_size;

  // Set wrap to true, makes sure that the tape won't
  // expand if the ptr is incremented after this fucntion
  // sets the pointer to the last index of the current tape size
  if(!t->has_max && t->ptr == t->max_size - 1) t->wrap = true;
}

void increment_val(tape_t *t) {
  t->blocks[t->ptr]++;
}

void decrement_val(tape_t *t) {
  t->blocks[t->ptr]--;
}

void set_val(tape_t *t, uint8_t val) {
  t->blocks[t->ptr] = val;
}

uint8_t get_val(tape_t *t) {
  return t->blocks[t->ptr];
}
