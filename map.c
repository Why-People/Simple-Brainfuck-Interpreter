#include "ll.h"
#include "map.h"

#include <stdlib.h>

#define INITIAL_CAPACITY 32

#define LOAD_THRESHOLD 0.75 // Rehash when load factor hits this threshold

struct map_t {
  uint32_t size, capacity;
  ll_t *keys, *vals;
  ll_t **key_buckets, **val_buckets;
};

map_t *create_map(void) {
  map_t *m = (map_t *) malloc(sizeof(struct map_t));

  if(!m) return NULL;

  m->size = 0;
  m->capacity = INITIAL_CAPACITY;
  m->keys = create_list();
  m->vals = create_list();

  if(!m->keys || !m->vals) {
    free(m);
    return NULL;
  }

  m->key_buckets = (ll_t **) calloc(INITIAL_CAPACITY, sizeof(struct ll_t *));
  m->val_buckets = (ll_t **) calloc(INITIAL_CAPACITY, sizeof(struct ll_t *));

  if(!m->key_buckets || !m->val_buckets) {
    delete_list(&m->keys);
    delete_list(&m->vals);
    free(m);
    return NULL;
  }

  return m;
}

void delete_map(map_t **m) {
  if(*m) {
    delete_list(&(*m)->keys);
    delete_list(&(*m)->vals);

    for(uint32_t i = 0; i < (*m)->capacity; i++) {
      delete_list(&(*m)->key_buckets[i]);
      delete_list(&(*m)->val_buckets[i]);
    }
    
    free((*m)->key_buckets);
    free((*m)->val_buckets);
    free(*m);
    *m = NULL;
  }
}

// Jenkins 32-bit integer hash function
static uint32_t _hash(uint32_t k) {
  k = (k + 0x7ED55D16) + (k << 12);
  k = (k ^ 0xC761C23C) + (k >> 19);
  k = (k + 0x165667B1) + (k << 5);
  k = (k + 0xD3A2646C) + (k << 9);
  k = (k + 0xFD7046C5) + (k << 3);
  k = (k ^ 0xB55A4F09) + (k >> 16);
  return k;
}

static void _rehash(map_t *m) {
  // Delete everything currently in the buckets
  for(uint32_t i = 0; i < m->capacity; i++) {
    delete_list(&m->key_buckets[i]);
    delete_list(&m->val_buckets[i]);
  }

  free(m->key_buckets);
  free(m->val_buckets);
  m->key_buckets = NULL;
  m->val_buckets = NULL;

  // Expand Buckets
  m->capacity *= 2;
  ll_t **temp_keys = (ll_t **) calloc(m->capacity, sizeof(struct ll_t *));
  ll_t **temp_vals = (ll_t **) calloc(m->capacity, sizeof(struct ll_t *));

  if(!temp_keys || !temp_vals) {
    free(temp_keys);
    free(temp_vals);
    return;
  }

  m->key_buckets = temp_keys;
  m->val_buckets = temp_vals;

  uint32_t temp = m->size;
  m->size = 0; // will be restored to its original value after the full rehash

  // Rehash every value
  for(uint32_t i = 0; i < temp; i++) {
    uint32_t key = remove_first(m->keys), val = remove_first(m->vals);
    insert_map_value(m, key, val); // size, key, and value lists will be rebuilt on insertion
  }
}

static inline float _load_factor(map_t *m) {
  return (float) m->size / m->capacity;
}

void insert_map_value(map_t *m, uint32_t key, uint32_t val) {
  if(_load_factor(m) >= LOAD_THRESHOLD) _rehash(m); // check if rehash is needed
  if(!m || !m->key_buckets || !m->val_buckets || !m->capacity) {
    return;
  }

  uint32_t idx = _hash(key) % m->capacity;

  // If there's no collisison
  if(!m->key_buckets[idx]) {
    m->key_buckets[idx] = create_list();
    m->val_buckets[idx] = create_list();
  }

  // Add key value pair
  append(m->keys, key);
  append(m->vals, val);
  append(m->key_buckets[idx], key);
  append(m->val_buckets[idx], val);
  m->size++;
}
  
uint32_t get_map_value(map_t *m, uint32_t key) {
  if(!m || !m->key_buckets || !m->val_buckets || !m->capacity) {
    return (uint32_t) NULL;
  }
  
  uint32_t idx = _hash(key) % m->capacity;

  // Grab the key index, and use it to lookup the value
  int64_t val_index = get_list_index(m->key_buckets[idx], key);
  return val_index != -1 ?
    get_list_value(m->val_buckets[idx], val_index) : (uint32_t) NULL;
}
