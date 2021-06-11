#pragma once

#include <stdint.h>
#include <stdbool.h>

#define UNLIMITED_MEMORY 0

#ifndef __TAPE_H__
#define __TAPE_H__

/* 
   A simple data structure to handle the ptr and blocks
   in the "Language Spec". Handles instructions + - , . < >
*/

typedef struct tape_t tape_t;

tape_t *create_tape(uint32_t size);

void delete_tape(tape_t **t);

// '>'
void increment_ptr(tape_t *t);

// '<'
void decrement_ptr(tape_t *t);

// '+'
void increment_val(tape_t *t);

// '-'
void decrement_val(tape_t *t);

// ','
void set_val(tape_t *t, uint8_t val);

// '.'
uint8_t get_val(tape_t *t);

#endif
