#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STACK_SIZE 100
#define MAX_JUMP_SIZE 100

struct Stack {
  int top;
  int data[MAX_STACK_SIZE];
};

struct JumpLabel {
  char name[30];
  long pos;
};

void createStack(struct Stack *stack) { stack->top = -1; }

// Stack Operations
void push(struct Stack *stack, int data);
int pop(struct Stack *stack);
int peak(struct Stack *stack);
void swap(struct Stack *stack);
void duplicate(struct Stack *stack);

// Output
void print(struct Stack *stack, int *var_space, int pos);
void out(struct Stack *stack);

// Artimatic
void add(struct Stack *stack);
void subtract(struct Stack *stack);
void multiply(struct Stack *stack);
void divide(struct Stack *stack);
void modulo(struct Stack *stack);
void inc(struct Stack *stack);
void dec(struct Stack *stack);

// Variable
void set(struct Stack *stack, int *var_space, char index, char *value);
void get(struct Stack *stack, int *var_space, char index);
void increment(struct Stack *stack, int *var_space, char index);
void decrement(struct Stack *stack, int *var_space, char index);
int get_index(struct Stack *stack, char index);
void remove_newlines(char *str);
void jump(struct Stack *stack, FILE *file, struct JumpLabel *labels,
          int label_count, char *label);

void error(int error, int line);

/*
Stack Instruction set
        SET
        GET
        PUSH
        POP
        PRINT
        SWAP
        DUP
        OUT
        ADD
        SUB
        MUL
        DIV
        MOD
        INC
        DEC
        INCV
        DECV
        LABEL
        JMP
        JMPZ
 */

int main(int argc, char **argv) {
  struct Stack stack;
  createStack(&stack);

  int var_space[26];

  if (argc < 2) {
    printf("Usage: %s <filename>\n", argv[0]);
    return 1;
  }
  FILE *file = fopen(argv[1], "r");
  if (file == NULL) {
    error(100, 0);
    return 1;
  }

  char buffer[1024];
  int line_counter = 1;

  struct JumpLabel labels[MAX_JUMP_SIZE];
  int label_count = 0;
  while (fgets(buffer, sizeof(buffer), file)) {
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n' && buffer[len - 2] == '\r') {
      buffer[len - 2] = '\0';
    }
    char *token = strtok(buffer, " ");
    if (strcmp(token, "LABEL") == 0) {
      token = strtok(NULL, " ");
      remove_newlines(token);
      if (label_count < MAX_JUMP_SIZE) {
        labels[label_count].pos = ftell(file);
        strcpy(labels[label_count].name, token);
        label_count++;
      }
    }
    line_counter++;
  }
  rewind(file);
  line_counter = 1;
  // Read the file line by line
  while (fgets(buffer, sizeof(buffer), file)) {
    // Remove the newline character at the end of the line (if present)
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n' && buffer[len - 2] == '\r') {
      buffer[len - 2] = '\0'; // Replace newline with null terminator
    }

    // Parse the instruction from the buffer
    if (buffer == NULL) {
      continue;
    }
    char *token = strtok(buffer, " ");
    remove_newlines(token);
    if (token == NULL) {
      continue;
    }
    if (strcmp(token, "PUSH") == 0) {
      token = strtok(NULL, " ");
      int operand = atoi(token);
      push(&stack, operand);
    } 
    else if (strcmp(token, "POP") == 0) {
      pop(&stack);
    } 
    else if (strcmp(token, "PRINT") == 0) {
      token = strtok(NULL, " ");
      int address = get_index(&stack, token[0]);
      print(&stack, var_space, address);
    } 
    else if (strcmp(token, "SWP") == 0) {
      swap(&stack);
    } 
    else if (strcmp(token, "DUP") == 0) {
      duplicate(&stack);
    } 
    else if (strcmp(token, "OUT") == 0) {
      out(&stack);
    } 
    else if (strcmp(token, "ADD") == 0) {
      add(&stack);
    } 
    else if (strcmp(token, "SUB") == 0) {
      subtract(&stack);
    } 
    else if (strcmp(token, "MUL") == 0) {
      multiply(&stack);
    } 
    else if (strcmp(token, "DIV") == 0) {
      divide(&stack);
    } 
    else if (strcmp(token, "MOD") == 0) {
      modulo(&stack);
    } 
    else if (strcmp(token, "INC") == 0) {
      inc(&stack);
    } 
    else if (strcmp(token, "DEC") == 0) {
      dec(&stack);
    } 
    else if (strcmp(token, "SET") == 0) {
      token = strtok(NULL, " ");
      char address = token[0];
      token = strtok(NULL, " ");
      set(&stack, var_space, address, token);
    } 
    else if (strcmp(token, "GET") == 0) {
      token = strtok(NULL, " ");
      char address = token[0];
      get(&stack, var_space, address);
    } 
    else if (strcmp(token, "INCV") == 0) {
      token = strtok(NULL, " ");
      char address = token[0];
      increment(&stack, var_space, address);
    } 
    else if (strcmp(token, "DECV") == 0) {
      token = strtok(NULL, " ");
      char address = token[0];
      decrement(&stack, var_space, address);
    } 
    else if (strcmp(token, "JMP") == 0) {
      token = strtok(NULL, " ");
      int condition;
      if (token[0] == '$') {
        condition = peak(&stack);
      } 
      else if (token[0] >= 'A' && token[0] <= 'Z') {
        condition = var_space[token[0] - 'A'];
      }
      else {
        condition = atoi(token);
      }
      token = strtok(NULL, " "); 
      if (condition != 0) {
        remove_newlines(token); 
        jump(&stack, file, labels, label_count, token);
      }
    } 
    else if (strcmp(token, "JMPZ") == 0) {
      token = strtok(NULL, " ");
      int condition;
      if (token[0] == '$') {
        condition = peak(&stack);
      } 
      else if (token[0] >= 'A' && token[0] <= 'Z') {
        condition = var_space[token[0] - 'A'];
      }
      else {
        condition = atoi(token);
      }
      token = strtok(NULL, " "); 
      if (condition == 0) {
        remove_newlines(token);
        jump(&stack, file, labels, label_count, token);
      }
    }

    line_counter++;
  }
  fclose(file);
  return 0;
}

void push(struct Stack *stack, int data) {
  stack->top++;
  if (stack->top >= MAX_STACK_SIZE) {
    error(102, 0);
    exit(1);
  }
  stack->data[stack->top] = data;
}

int pop(struct Stack *stack) {
  if (stack->top < 0) {
    error(103, 0);
    exit(1);
  }
  return stack->data[stack->top--];
}
int peak(struct Stack *stack) {
  return stack->data[stack->top];
}

void print(struct Stack *stack, int *var_space, int pos) { 
  
    printf("%d\n", var_space[pos]); 
    }

void swap(struct Stack *stack) {
  int a = pop(stack);
  int b = pop(stack);
  push(stack, a);
  push(stack, b);
}

void duplicate(struct Stack *stack) {
  int a = peak(stack);
  push(stack, a);
}

void out(struct Stack *stack) { printf("%d\n", peak(stack)); }

void add(struct Stack *stack) {
  int a = pop(stack);
  int b = pop(stack);
  push(stack, a + b);
}

void subtract(struct Stack *stack) {
  int a = pop(stack);
  int b = pop(stack);
  push(stack, a - b);
}

void multiply(struct Stack *stack) {
  int a = pop(stack);
  int b = pop(stack);
  push(stack, a * b);
}

void divide(struct Stack *stack) {
  int a = pop(stack);
  int b = pop(stack);
  if (b == 0) {
    error(104, 0);
    exit(1);
  }
  push(stack, a / b);
}

void modulo(struct Stack *stack) {
  int a = pop(stack);
  int b = pop(stack);
  push(stack, a % b);
}
void inc(struct Stack *stack) {
  int a = pop(stack);
  push(stack, ++a);
}
void dec(struct Stack *stack) {
  if (stack->top < 0) {
    error(103, 0); // Stack underflow error
    return;
  }
  stack->data[stack->top]--;
}

int get_index(struct Stack *stack, char index) {
  int pos;
  if (index < 'A' && index > 'Z') {
    error(103, 0);
    exit(0);
  }

  if (index == '$') {
    pos = pop(stack);
  } 
  else {
    pos = index - 'A';
  }
  return pos;
}

void remove_newlines(char *str) {
  int i, j = 0;
  for (i = 0; str[i] != '\0'; i++) {
    if (str[i] != '\n' && str[i] != '\r') {
      str[j++] = str[i]; // Copy only non-newline characters
    }
  }
  str[j] = '\0'; // Null-terminate the modified string
}

void set(struct Stack *stack, int *var_space, char index, char *value) {
  int pos = get_index(stack, index);
  int data;

  if (value[0] == '$') {
    data = pop(stack);
  } else {
    data = atoi(value);
  }

  var_space[pos] = data;
}
void get(struct Stack *stack, int *var_space, char index) {
  int pos = get_index(stack, index);

  push(stack, var_space[pos]);
}

void increment(struct Stack *stack, int *var_space, char index) {
  int pos = get_index(stack, index);
  var_space[pos]++;
}

void decrement(struct Stack *stack, int *var_space, char index) {
  int pos = get_index(stack, index);
  var_space[pos]--;
}

void jump(struct Stack *stack, FILE *file, struct JumpLabel *labels,
          int label_count, char *name) {
  for (int i = 0; i < label_count; i++) {
    if (strcmp(labels[i].name, name) == 0) {
      fseek(file, labels[i].pos, SEEK_SET);
      return;
    }
  }
  error(106, 0); // Invalid label error
  exit(1);
}

void error(int error, int line) {
  printf("Error %d on line %d: \n \t", error, line);
  switch (error) {
  case 100:
    printf("File not found\n");
    break;
  case 101:
    printf("Error reading file\n");
    break;
  case 102:
    printf("Stack Overflow error");
    break;
  case 103:
    printf("Stack Call out of bounds");
    break;
  case 104:
    printf("Divide by Zero error");
    break;
  case 105:
    printf("Incorrect Variable Index");
    break;
  case 106:
    printf("Invalid Label");
    break;
  default:
    printf("An error happened at line %d\n", line);
    break;
  }
}
