#include "instruction_buffer.h"

#include <stdlib.h>

#define BASE_SIZE 1024

struct ib_t {
  uint32_t curr, top, size;
  uint8_t *instructions;
};

ib_t *create_instruction_buffer(void) {
  ib_t *buf = (ib_t *) malloc(sizeof(struct ib_t));

  if(!buf) return NULL;

  buf->curr = 0;
  buf->top = 0;
  buf->size = BASE_SIZE;
  buf->instructions = (uint8_t *) calloc(buf->size, sizeof(uint8_t));

  if(!buf->instructions) {
    free(buf);
    return NULL;
  }

  return buf;
}

void delete_instruction_buffer(ib_t **buf) {
  if(*buf) {
    free((*buf)->instructions);
    free(*buf);
    *buf = NULL;
  }
}

uint8_t next_instruction(ib_t *buf) {
  // End of Buffer when the current instruction is the top instruction
  return buf->curr < buf->top ? buf->instructions[buf->curr++] : EOBUF;
}

uint32_t get_current_instruction_index(ib_t *buf) {
  return buf->curr - 1;
}

uint32_t get_top_instruction_index(ib_t *buf) {
  return buf->top - 1;
}

void push_instruction(ib_t *buf, uint8_t instruction) {
  // Resisze if needed
  if(buf->top == buf->size) {
    buf->size *= 2;
    uint8_t *temp = (uint8_t *) realloc(buf->instructions, buf->size * sizeof(uint8_t));
    
    if(!temp) return;
    
    buf->instructions = temp;
  }
  
  buf->instructions[buf->top++] = instruction;
}

void jump_to(ib_t *buf, uint32_t index) {
  // Make sure the index is within scope of the brainfuck program
  if(index < buf->top) {
    buf->curr = index;
  }
}
