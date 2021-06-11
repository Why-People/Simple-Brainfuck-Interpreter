#pragma once

#include <stdint.h>

#ifndef __MAP_H__
#define __MAP_H__

/* A simple HashMap */

typedef struct map_t map_t;

map_t *create_map(void);

void delete_map(map_t **m);

void insert_map_value(map_t *m, uint32_t key, uint32_t val);

uint32_t get_map_value(map_t *m, uint32_t key); 

#endif
