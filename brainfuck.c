#include "tape.h"
#include "map.h"
#include "stack.h"
#include "instruction_buffer.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <getopt.h>

#define OPTIONS "hi:s:"

static bool build_instruction_buffer_and_jump_map(FILE *in, ib_t *buf, map_t *jump_map) {
  map_t *line_map = create_map();
  stack_t *temp_stack = create_stack();
  uint32_t current_line = 1;
  bool build_success = true;

  // Read file and parse instructions to buf
  int c;
  while((c = fgetc(in)) != EOF) {
    // Valid Instructions: + - , . [ ] < >
    bool is_instruction = (c >= '+' && c <= '.') || c == '<' || c == '>' || c == '[' || c == ']';
    
    if(is_instruction) push_instruction(buf, c);
    if(c == '\n') current_line++;

    // Build jump map using a temp_stack to track the instruction indexes
    if(c == '[') {
      uint32_t top_idx = get_top_instruction_index(buf);
      push(temp_stack, top_idx);
      insert_map_value(line_map, top_idx, current_line); 
    }

    // Insert jump map indexes
    if(c == ']') {
      uint32_t top_val = pop(temp_stack) + 1, top_idx = get_top_instruction_index(buf);
      insert_map_value(jump_map, top_val, top_idx);
      insert_map_value(jump_map, top_idx, top_val);
    }
      
  }

  // Handle Syntax Errors (Unmatched Brackets) (Print the most recent one)
  if(!stack_empty(temp_stack)) {
    fprintf(stderr, "Syntax Error: Unmatched bracket(s) '[' on line: %u\n",
	    get_map_value(line_map, pop(temp_stack)));
    build_success = false;
  }

  delete_map(&line_map);
  delete_stack(&temp_stack);
  return build_success;
}
  
// The actual interpreter
// "Language Spec" (all handled through the tape struct):
// '+' -> increment value at ptr
// '-' -> decrement value at ptr
// '.' -> print the current char at the ptr to stdout
// ',' -> read input from stdin
// '>' -> increment the ptr to the next block
// '<' -> decrement the ptr to the previous block
// '[' -> begin loop (if the current value at ptr is greater than 0)
// ']' -> end loop
static void interpret(tape_t *tape, ib_t *buf, map_t *jump_map) {
  uint8_t instruction;
  while((instruction = next_instruction(buf)) != EOBUF) {
    switch(instruction) {
    case '+': increment_val(tape); break; 
    case '-': decrement_val(tape); break;
    case '.': putchar(get_val(tape)); break;
    case ',': set_val(tape, (uint8_t) getchar()); break;
    case '>': increment_ptr(tape); break;
    case '<': decrement_ptr(tape); break;
    case '[': {
      if(!get_val(tape)) {
	// Perform loop jump
	uint32_t closing_bracket_idx = get_current_instruction_index(buf) + 1;
        jump_to(buf, get_map_value(jump_map, closing_bracket_idx));
      }
      break;
    }
    case ']': {
      if(get_val(tape)) {
	// Perform loop jump
	uint32_t opening_bracket_idx = get_current_instruction_index(buf);
	jump_to(buf, get_map_value(jump_map, opening_bracket_idx));
      }
      break;
    }
    default: break; // should never be invoked
    }
  }
}

static void print_helper_msg(void) {
    printf("SYNOPSIS\n"
           "\tA A simple brainfuck interpreter.\n\n"
           "USAGE\n"
           "\t./brainfuck [-h] [-s *value*] [-i *infile*]\n\n"
           "OPTIONS\n"
           "\t-h          (Prints this message)\n"
	   "\t-i *infile* (File to interpret)\n"
           "\t-s *value*  (Set a hard limit on interpreter memory [Default: Unlimited])\n");
}

int main(int argc, char **argv) {
  FILE *in = stdin; // default to stdin if no infile specified
  uint32_t tape_size = UNLIMITED_MEMORY;

  // Parse options
  int opt;
  while((opt = getopt(argc, argv, OPTIONS)) != -1) {
    switch(opt) {
    case 's': tape_size = strtoul(optarg, NULL, 10); break;
    case 'i': {
      in = fopen(optarg, "rb");

      // IO Error
      if(!in) {
	perror(optarg);
	return EXIT_FAILURE;
      }
      
      break;
    }
    case '?': print_helper_msg(); return EXIT_FAILURE;
    case 'h':
    default: print_helper_msg(); return EXIT_SUCCESS;
    }
  }

  tape_t *tape = create_tape(tape_size);
  ib_t *instructions = create_instruction_buffer();
  map_t *jump_map = create_map();

  // Parse input file
  bool build_success = build_instruction_buffer_and_jump_map(in, instructions, jump_map);
  if(build_success) interpret(tape, instructions, jump_map);
  
  delete_instruction_buffer(&instructions);
  delete_map(&jump_map);
  delete_tape(&tape);
  fclose(in);
  return EXIT_SUCCESS;
}
