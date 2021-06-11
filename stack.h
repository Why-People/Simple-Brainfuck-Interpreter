#pragma once

#ifndef __STACK_H__
#define __STACK_H__

#include <stdint.h>
#include <stdbool.h>

/* A Simple Stack */

typedef struct stack_t stack_t;

stack_t *create_stack(void);

void delete_stack(stack_t **s);

bool stack_empty(stack_t *s);

uint32_t stack_size(stack_t *s);

uint32_t peek(stack_t *s);

void push(stack_t *s, uint32_t val);

uint32_t pop(stack_t *s);

#endif
