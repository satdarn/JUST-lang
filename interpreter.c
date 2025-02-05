#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STACK_SIZE 1024
#define MAX_JUMP_SIZE 1024
#define MAX_VAR_SIZE 100

struct Stack {
  int top;
  int data[MAX_STACK_SIZE];
};

struct JumpLabel {
  char name[30];
  long pos;
};

struct LabelSpace {
  struct JumpLabel labels[MAX_JUMP_SIZE];
  int count;
};

struct Var {
  char name[30];
  int value;
};

struct VarSpace {
  struct Var vars[MAX_VAR_SIZE];
  int count;
};

void createStack(struct Stack *stack) { stack->top = -1; }
void createVarSpace(struct VarSpace *var_space) { var_space->count = 0;};
void createVar(struct Var *var, char* name, int value) { 
  var->value = value; 
  strncpy(var->name, name, sizeof(var->name) - 1);
  var->name[sizeof(var->name) - 1] = '\0'; // Ensure null termination
}

void createLabelSpace(struct LabelSpace *label_space) { label_space->count = 0;};


void makeLabel(struct LabelSpace *label_space, FILE* file, char *name) {
  if (label_space->count < MAX_JUMP_SIZE) {
    label_space->labels[label_space->count].pos = ftell(file);
    strcpy(label_space->labels[label_space->count].name, name);
    label_space->count++;
  }
}
// Stack Operations
void push(struct Stack *stack, int data);
int pop(struct Stack *stack);
int peak(struct Stack *stack);
void swap(struct Stack *stack);
void duplicate(struct Stack *stack);

// Output
void print(struct Stack *stack, struct VarSpace *var_space, char* name);
void printc(struct Stack *stack, struct VarSpace *var_space, char* name);
void out(struct Stack *stack);
void outc(struct Stack *stack);

// Artimatic
void add(struct Stack *stack);
void subtract(struct Stack *stack);
void multiply(struct Stack *stack);
void divide(struct Stack *stack);
void modulo(struct Stack *stack);
void inc(struct Stack *stack);
void dec(struct Stack *stack);

// Variable
void def(struct Stack *stack, struct VarSpace *var_space, char*name, int value);
void set(struct Stack *stack, struct VarSpace *var_space, char *name, char *value);
void get(struct Stack *stack, struct VarSpace *var_space, char *name);
void increment(struct Stack *stack, struct VarSpace *var_space, char *name);
void decrement(struct Stack *stack, struct VarSpace *var_space, char *name);
int get_index(struct Stack *stack, struct VarSpace *var_space, char *name);
void remove_newlines(char *str);

// Loops
void jump(struct Stack *stack, FILE *file, struct LabelSpace* labels, char *label);

void error(int error, int line);

/*
Stack Instruction set
        DEF
        SET
        GET
        INDEXOF
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

  struct VarSpace var_space;
  createVarSpace(&var_space);

  struct LabelSpace label_space;
  createLabelSpace(&label_space);


  if (argc < 2) {
    printf("Usage: %s <filename>\n", argv[0]);
    return 1;
  }
  // Pre loading the stack from the command line
  if (argc >= 3){
    for(int i = 2; i < argc; i++){
      push(&stack, atoi(argv[i]));
    }
  }

  FILE *file = fopen(argv[1], "r");
  if (file == NULL) {
    error(100, 0);
    return 1;
  }

  char buffer[1024];
  int line_counter = 1;
  while (fgets(buffer, sizeof(buffer), file) != NULL) {
    if (strlen(buffer) == 0 || (buffer[0] == '\n' || buffer[0] == '\r')) {
        continue;
    }
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n' && buffer[len - 2] == '\r') {
      buffer[len - 2] = '\0';
    }
    
    char *token = strtok(buffer, " ");
    if (token == NULL) {
      continue; // Skip empty lines or lines without tokens
    }
    if (strcmp(token, "LABEL") == 0) {
      token = strtok(NULL, " ");
      remove_newlines(token);
      makeLabel(&label_space, file, token);
    }
    else if (strcmp(token, "MACRO") == 0) {
      token = strtok(NULL, " ");
      remove_newlines(token);
      makeLabel(&label_space, file, token);
    }
    else if (strcmp(token, "MAIN")==0){
      remove_newlines(token);
      makeLabel(&label_space, file, token);
    }
    line_counter++;
  }
  rewind(file);
  // Read the file line by line
  long call_label;
  jump(&stack, file, &label_space, "MAIN");
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
    if(buffer[0] == '\n' || buffer[0] == '\0'){
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
      print(&stack, &var_space, token);
    }
    else if (strcmp(token, "PRINTC") == 0) {
      token = strtok(NULL, " ");
      printc(&stack, &var_space, token);
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
    else if (strcmp(token, "OUTC") == 0) {
      outc(&stack);
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
    else if (strcmp(token, "ISEQUAL") == 0) {
      int a = pop(&stack);
      int b = pop(&stack);
      if (a == b) {
        push(&stack, 1);
      } else {
        push(&stack, 0);
      }
    }
    else if (strcmp(token, "ISNOTEQUAL") == 0) {
      int a = pop(&stack);
      int b = pop(&stack);
      if (a!= b) {
        push(&stack, 1);
      } else {
        push(&stack, 0);
      }
    }
    else if (strcmp(token, "ISGREATER") == 0) {
      int a = pop(&stack);
      int b = pop(&stack);
      if (a > b) {
        push(&stack, 1);
      } else {
        push(&stack, 0);
      }
    }
    else if (strcmp(token, "ISLESSER") == 0) {
      int a = pop(&stack);
      int b = pop(&stack);
      if (a < b) {
        push(&stack, 1);
      } else {
        push(&stack, 0);
      }
    }
    else if (strcmp(token, "DEF") ==0) {
      char* name = strtok(NULL, " ");
      remove_newlines(token);  
      token = strtok(NULL, " ");
      for (int pos = 0; pos < var_space.count; pos++) {
        if (strcmp(var_space.vars[pos].name, name) == 0) {
          error(200, 0);
        }
      }
      int value = 0;
      if(token != NULL){
        if(token[0] == '$'){
          value = pop(&stack);
        }
        else{
          value = atoi(token);
        }
      }
      def(&stack, &var_space, name, value);
    }
    else if (strcmp(token, "SET") == 0) {
      token = strtok(NULL, " ");
      char* address = token;
      remove_newlines(address); // Remove newline character from token
      token = strtok(NULL, " ");
      remove_newlines(token);
      set(&stack, &var_space, address, token);
    } 
    else if (strcmp(token, "GET") == 0) {
      token = strtok(NULL, " ");
      char* address = token;
      get(&stack, &var_space, address);
    } 
    else if (strcmp(token, "VARINDEX") == 0) {
      token = strtok(NULL, " ");
      remove_newlines(token); // Remove newline character from token
      for (int pos = 0; pos < var_space.count; pos++) {
        if (strcmp(var_space.vars[pos].name, token) == 0) {
          push(&stack, pos);
        }
      }
    }
    else if (strcmp(token, "INCV") == 0) {
      token = strtok(NULL, " ");
      char* address = token;
      increment(&stack, &var_space, address);
    } 
    else if (strcmp(token, "DECV") == 0) {
      token = strtok(NULL, " ");
      char* address = token;
      decrement(&stack, &var_space, address);
    } 
    else if (strcmp(token, "JMP") == 0) {
      token = strtok(NULL, " ");
      int condition;
      if (token[0] == '$') {
        condition = peak(&stack);
      } 
      else{
        condition = var_space.vars[get_index(&stack, &var_space, token)].value;
      }
      token = strtok(NULL, " "); 
      if (condition != 0) {
        remove_newlines(token); 
        jump(&stack, file, &label_space, token);
      }
    } 
    else if (strcmp(token, "JMPZ") == 0) {
      token = strtok(NULL, " ");
      int condition;
      if (token[0] == '$') {
        condition = peak(&stack);
      } 
      else{
        condition = var_space.vars[get_index(&stack, &var_space, token)].value;
      }
      token = strtok(NULL, " "); 
      if (condition == 0) {
        remove_newlines(token);
        jump(&stack, file, &label_space, token);
      }
    }
    else if (strcmp(token, "CALL") == 0) {
      token = strtok(NULL, " ");
      remove_newlines(token);
      call_label = ftell(file);
      jump(&stack, file, &label_space, token);
    }
    else if (strcmp(token, "END") == 0){
      if(call_label != 0) {
        fseek(file, call_label, SEEK_SET);
      }
    }
    else if (strcmp(token, "DONE") == 0 ){
        exit(0);
    }
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

void print(struct Stack *stack, struct VarSpace *var_space, char *name) {
  int pos = get_index(stack, var_space, name);  
  printf("%d\n", var_space->vars[pos].value); 
  }

void printc(struct Stack *stack, struct VarSpace*var_space, char *name){
  int pos = get_index(stack, var_space, name);  
  printf("%c", var_space->vars[pos].value); 
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

void outc(struct Stack *stack) {printf("%c", peak(stack));}

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

int get_index(struct Stack *stack, struct VarSpace *var_space, char *name) {
  int pos;
  if (name[0] == '$') {
    pos = pop(stack);
    return pos;
  } 

  for (pos = 0; pos < var_space->count; pos++) {
    if (strcmp(var_space->vars[pos].name, name) == 0) {
      return pos;
    }
  }
  error(108, 0);
  exit(1);
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

void def(struct Stack *stack, struct VarSpace *var_space, char* name, int value){
  if (var_space->count >= MAX_VAR_SIZE) {
    error(107, 0); // Variable space overflow error
    return;
  }
  
  createVar(&var_space->vars[var_space->count], name, value);
  var_space->count++;
}

void set(struct Stack *stack, struct VarSpace *var_space, char *name, char *value) {
  int pos = get_index(stack, var_space, name);
  int data;
  if (value[0] == '$') {
    data = pop(stack);
  } else {
    data = atoi(value);
  }

  var_space->vars[pos].value = data;
}
void get(struct Stack *stack, struct VarSpace *var_space, char* name) {
  int pos = get_index(stack, var_space, name);
  push(stack, var_space->vars[pos].value);
}

void increment(struct Stack *stack, struct VarSpace *var_space, char* name) {
  int pos = get_index(stack, var_space, name);
  var_space->vars[pos].value++;
}

void decrement(struct Stack *stack, struct VarSpace *var_space, char* name) {
  int pos = get_index(stack, var_space, name);
  var_space->vars[pos].value--;
}

void jump(struct Stack *stack, FILE *file, struct LabelSpace *labels, char *name) {
  for (int i = 0; i < labels->count; i++) {
    if (strcmp(labels->labels[i].name, name) == 0) {
      fseek(file, labels->labels[i].pos, SEEK_SET);
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
  case 106:
    printf("Invalid Label");
    break;
  case 107:
    printf("Variable space overflow");
    break;
  case 108:
    printf("Variable not intialized");
    break;
  case 200:
    printf("Variable already defined");
    break;
  case 201:
    printf("Variable not defined");
    break;
  default:
    printf("An error happened at line %d\n", line);
    break;
  }
}
