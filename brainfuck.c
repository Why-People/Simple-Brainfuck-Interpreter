#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_MEMORY 30000

unsigned int memory[MAX_MEMORY];
unsigned int* ptr = memory;

void increment_ptr() {
  if(ptr == &memory[MAX_MEMORY - 1]) {
    ptr = memory;
  } else {
    ++ptr;
  }
}

void decrement_ptr() {
  if(ptr == memory) {
    ptr = &memory[MAX_MEMORY - 1];
  } else {
    --ptr;
  }
}

void handle_forward_jump(int* i, char** contents) {
  int loop = 1;
  (*i)++;
  
  while(loop > 0) {
    (*i)++;
    if((*contents)[*i] == '[') loop++;
    if((*contents)[*i] == ']') loop--;
  }
}

void handle_backward_jump(int* i, char** contents) {
  int loop = 1;
  (*i)--;
  
  while(loop > 0) {
    if((*contents)[*i] == ']') loop++;
    if((*contents)[*i] == '[') loop--;
    (*i)--;
  }
}

void handle_instruction(int* i, char** contents) {
  switch((*contents)[*i]) {
    case '+':
      (*ptr)++;
      break;
    case '-':
      (*ptr)--;
      break;
    case '.':
      putchar(*ptr);
      break;
    case '>':
      increment_ptr();
      break;
    case '<':
      decrement_ptr();
      break;
    case ',':
      *ptr = getchar();
      break;
    case '[':
      if(*ptr == 0) {
	handle_forward_jump(i, contents);
      }
      break;
    case ']':
      handle_backward_jump(i, contents);
      break;
    default:
      // The default case should never be invoked
      break;	 
  }
}

void interpret_block(char* contents) {
  for(int i = 0; contents[i] != '\0'; i++){
    handle_instruction(&i, &contents);
  }

  printf("\n");
}

bool is_instruction(int ch) {
  // Ascii Values for Brainfuck Instructions
  return (ch >= 43 && ch <= 46) || ch == 60 || ch == 62 || ch == 91 || ch == 93;
}

char* get_code(char* buffer, FILE* file) {
  int c;
  size_t n = 0;

  while((c = fgetc(file)) != EOF) {
    if(is_instruction(c)) {
      buffer[n++] = (char) c;
    }  
  }

  return buffer;
}

long get_file_size(FILE* file) {
  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  fseek(file, 0, SEEK_SET);
  return size;
}  

void interpret(const char* file_path) {
  FILE* file = fopen(file_path, "r");
  char* code;

  if(file == NULL) {
    printf("Error: System Cannot find the Path Specified.\n");
    return;
  }

  long file_size = get_file_size(file);

  code = get_code(malloc(file_size), file);
  fclose(file);
  
  interpret_block(code);
}

int main(int argc, char** argv) {
  if(argc > 2) {
    printf("Error: Too many program arguments\n");
    return -1;
  }
  
  interpret(argv[1]);
  return 0;
}
