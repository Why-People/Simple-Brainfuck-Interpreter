#include "ll.h"
#include "stack.h"

#include <stdlib.h>

struct stack_t {
  ll_t *list;
};

stack_t *create_stack(void)  {
  stack_t *s = (stack_t *) malloc(sizeof(struct stack_t));

  if(!s) return NULL;

  // A Linked List will carry here
  s->list = create_list();

  if(!s->list) {
    free(s);
    return NULL;
  }

  return s;
}

void delete_stack(stack_t **s) {
  if(*s) {
    delete_list(&(*s)->list);
    free(*s);
    *s = NULL;
  }
}

bool stack_empty(stack_t *s) {
  return list_empty(s->list);
}

uint32_t stack_size(stack_t *s) {
  return list_size(s->list);
}

uint32_t peek(stack_t *s) {
  return get_last(s->list);
}

void push(stack_t *s, uint32_t val) {
  append(s->list, val);
}

uint32_t pop(stack_t *s) {
  return remove_last(s->list);
}
