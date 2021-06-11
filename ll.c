#include "ll.h"

#include <stdlib.h>

struct node_t {
  uint32_t val;
  node_t *next, *prev; // doubly linked
};

/* Node Functions */

static node_t *_create_node(uint32_t val) {
  node_t *n = (node_t *) malloc(sizeof(struct node_t));

  if(!n) return NULL;
  
  n->val = val;
  n->next = NULL;
  n->prev = NULL;
  return n;
}

static void _delete_node(node_t **n) {
  if((*n)) {
    free(*n);
    *n = NULL;
  }
}

/* List Functions */

struct ll_t {
  uint32_t size;
  node_t *head;
  node_t *tail;
};

ll_t *create_list(void) {
  ll_t *ll = (ll_t *) malloc(sizeof(struct ll_t));

  if(!ll) return NULL;

  ll->size = 0;
  ll->head = _create_node((uint32_t) NULL);
  ll->tail = _create_node((uint32_t) NULL);

  if(!ll->head || !ll->tail) {
    free(ll);
    return NULL;
  }

  // Connect head and tail
  ll->head->next = ll->tail;
  ll->tail->prev = ll->head;

  return ll;
}

void delete_list(ll_t **ll) {
  if(!(*ll)) return;

  node_t *i = (*ll)->head;
  while(i != NULL) {
    node_t *temp = i->next; // store next node before deletion
    _delete_node(&i);
    i = temp;
  }

  free(*ll);
  *ll = NULL;
}

uint32_t list_size(ll_t *ll) {
  return ll->size;
}

bool list_empty(ll_t *ll) {
  return list_size(ll) == 0;
}

void append(ll_t *ll, uint32_t val) {
  node_t *n = _create_node(val);

  // Insert node at the tail
  n->prev = ll->tail->prev;
  n->next = ll->tail;
  ll->tail->prev->next = n;
  ll->tail->prev = n;
  ll->size++;
}

int64_t get_list_index(ll_t *ll, uint32_t val) {
  node_t *temp = ll->head->next;

  for(uint32_t i = 0; temp != ll->tail; i++) {
    if(temp->val == val) return i;
    temp = temp->next;
  }

  // Value Not Found in list
  return -1;
}

// Returns a node at a specified index in a list
static node_t *_get_node(ll_t *ll, uint32_t index) {
  node_t *temp = ll->head->next;

  for(uint32_t count = 0; count < index && temp != ll->tail; count++) {
    temp = temp->next;
  }

  // Not found if temp reached the tail
  return temp != ll->tail ? temp : NULL;
}

uint32_t get_list_value(ll_t *ll, uint32_t index) {
  return _get_node(ll, index)->val;
}

void set_list_value(ll_t *ll, uint32_t index, uint32_t val) {
  node_t *n = _get_node(ll, index);
  if(n) n->val = val; // Do nothing if list node isn't found
}

uint32_t get_last(ll_t *ll) {
  return ll->tail->prev->val;
}

// Remove a node from the list
static uint32_t _remove(node_t *n) {
  uint32_t val = n->val;
  n->prev->next = n->next;
  n->next->prev = n->prev;
  _delete_node(&n);
  return val;
}

// Remove node at the head
uint32_t remove_first(ll_t *ll) {
  if(list_empty(ll)) return (uint32_t) NULL; // List cannot be empty

  ll->size--;
  return _remove(ll->head->next);
}

// Remove node at the tail
uint32_t remove_last(ll_t *ll) {
  if(list_empty(ll)) return (uint32_t) NULL; // List cannot be empty

  ll->size--;
  return _remove(ll->tail->prev);
}
