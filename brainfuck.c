#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_MEMORY 16

int MEMORY[MAX_MEMORY];
int ptr = 0;

bool skip_block = false;

int CALL_STACK[MAX_MEMORY];
int stack_ptr = -1;

int* LOOP_MEMORY[MAX_MEMORY];
int loop_ptr = 0;

char BRACE_STACK[MAX_MEMORY];
int brace_ptr = -1;

void print_memory() {
  for(int i = 0; i < MAX_MEMORY; i++) {
    printf("%d, ", MEMORY[i]);
  }

  printf("\n");
}

void add_loop_index(int idx){
  LOOP_MEMORY[loop_ptr] = &MEMORY[ptr];

  if(*LOOP_MEMORY[loop_ptr++] != 0) {
    CALL_STACK[++stack_ptr] = idx;
  } else {
    skip_block = true;
  }
}

void increment_ptr() {
  if(ptr == MAX_MEMORY - 1) ptr = 0;
  else ptr++;
}

void decrement_ptr() {
  if(ptr == 0) ptr = MAX_MEMORY - 1;
  else ptr--;
}

void interpret_block(char* contents) {
  for(int i = 0; contents[i] != '\0'; i++){
    
    if(!skip_block) {
      // printf("%d | %c\n", i, contents[i]);
      switch(contents[i]){
        case '+':
	  MEMORY[ptr]++;
	  break;
        case '-':
	  MEMORY[ptr]--;
	  break;
        case '.':
	  putchar(MEMORY[ptr]);
	  break;
        case '>':
	  increment_ptr();
	  break;
        case '<':
	  decrement_ptr();
	  break;
        case ',':
	  MEMORY[ptr] = getchar();
	  break;
        case '[':
	  add_loop_index(i); 
	  break;
        case ']':
	  i = CALL_STACK[stack_ptr--] - 1;
 	  loop_ptr--;
	  break;
        default:
	  continue;
	  break;	 
      }
    }

    if(skip_block && contents[i] == '[') {
      BRACE_STACK[++brace_ptr] = '[';
    }

    if(skip_block && contents[i] == ']') {
      BRACE_STACK[brace_ptr--] = '\0';
      
      if(brace_ptr < 0) { 
	skip_block = false;
      }
    }

  }

  printf("\n");
}

char* get_code(char* buffer, FILE* file) {
  int c;
  size_t n = 0;

  while((c = fgetc(file)) != EOF) {
    char ch = (char) c;

    if(ch == '+' || ch == '-' || ch == '>' ||
       ch == '<' || ch == '.' || ch == ',' ||
       ch == '[' || ch == ']') {
      buffer[n++] = ch;
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
  interpret_block(code);
  
  fclose(file);
}

int main(int argc, char** argv) {
  interpret(argv[1]);
  print_memory();
  return 0;
}
