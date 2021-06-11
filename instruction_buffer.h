#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifndef __INSTRUCTION_BUFFER_H__
#define __INSTRUCTION_BUFFER_H__

#define EOBUF 0 // End of Buffer

/* 
   A simple struct to make storing instructions easier.
   Useful for performing loop jumps ('[' and ']' instructions)
*/

typedef struct ib_t ib_t;

ib_t *create_instruction_buffer(void);

void delete_instruction_buffer(ib_t **buf);

uint8_t next_instruction(ib_t *buf);

void push_instruction(ib_t *buf, uint8_t instruction);

uint32_t get_current_instruction_index(ib_t *buf);

uint32_t get_top_instruction_index(ib_t *buf);

// Loop jump
void jump_to(ib_t *buf, uint32_t index);

#endif
