#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifndef __LL_H__
#define __LL_H__

/* A Simple Linked List */

typedef struct node_t node_t;

typedef struct ll_t ll_t;

ll_t *create_list(void);

void delete_list(ll_t **ll);

void append(ll_t *ll, uint32_t val);

bool list_empty(ll_t *ll);

uint32_t list_size(ll_t *ll);

int64_t get_list_index(ll_t *ll, uint32_t val);

uint32_t get_list_value(ll_t *ll, uint32_t index);

uint32_t get_last(ll_t *ll);

uint32_t remove_first(ll_t *ll);

uint32_t remove_last(ll_t *ll);

#endif
