//
// Created by: Markus T Peterson
// Date: 1/28/2020
// Course: CSCI-C 335
// 

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

typedef struct CELL *LIST;

struct CELL{
  int val;
  LIST next;
};

extern LIST stack;

void push(int);
int pop(void);
void run_op(char);
void print(void);
void full_print(void);
void clear(void);
void unary_minus(void);
void duplicate(void);
void absolute(void);

LIST stack;
int length;

int main(void){
  stack = NULL;
  stack = (LIST) malloc(sizeof(struct CELL));
  
  char * in;
  in = NULL;
  in = malloc(100);

  length = 0;
  int option = 0;
  while(1){
    int val;
    char c;
    scanf("%s", in);

    c = *in;
    if(isdigit(c)){
      val = atoi(in);
      push(val);
    } else if (c == '*' || c =='+' || c =='-' || c =='/') {
      run_op(c);
    } else if (c == 'p') {
      print();
    } else if (c == 'f') {
      full_print();
    } else if (c == 'c') {
      clear();
    } else if (c == 'a') {
      absolute();
    } else if (c == 'm') {
      unary_minus();
    } else if (c == 'd') {
      duplicate();
    } else if (c == 'q') {
      free(in);
      free(stack);
      exit(1);
    } else if (c == EOF){
      free(in);
      free(stack);
      exit(1);
    }
  }
  free(in);
  return 0;
}

void absolute(void){
  if(length > 0){
    int val = abs(stack->val);
    stack->val = val;
  }else{
    printf("Stack is empty\n");
  }
}

void duplicate(void){
  if(length > 0){
    push(stack->val);
  }else{
    printf("Stack is empty\n");
  }
}

void unary_minus(void){
  if(length > 0){
    int val = -stack->val;
    stack->val = val;
  }else{
    printf("Stack is empty\n");
  }
}

void clear(void){
  if(length > 0){
    free(stack);
    stack = NULL;
    stack = (LIST) malloc(sizeof(struct CELL));
    length = 0;
  }else{
    printf("Stack is already empty\n");
  }
}

void print(void){
  if(length > 0){
    printf("%d\n", stack->val);
  }else{
    printf("Stack is empty\n");
  }
}

void full_print(void){
  if(length > 0){
    LIST curr;
    curr = stack;
    int i;
    for(i = 0; i < length; i++){
      printf("%d\n", curr->val);
      curr = curr->next;
    }
  }else{
    printf("Stack is empty\n");
  }
}

void push(int val) {
  LIST c = (LIST) malloc(sizeof(struct CELL));
  if (c) {
    length++;
    c->val = val;
    c->next = stack;
    stack = c;
  } else {
    printf("Out of Memory\n");
    exit(1);
  }
}

int pop(void){
  int out = -1;
  LIST c = NULL;

  length--;

  c = stack->next;
  out = stack->val;
  free(stack);
  stack = c;
  
  return out;
}

void run_op(char op){
  if(length > 1){
    if(op == '+'){
      push(pop() + pop());
    }else if(op == '-'){
      push(pop() - pop());
    }else if(op == '*'){
      push(pop() * pop());
    }else if(op == '/'){
      int a = pop();
      int b = pop();
      if(b != 0){
	push(a / b);
      }else{
	printf("Can't divide by 0\n");
      }
    }
  }else{
    printf("Input more numbers\n");
  }
}
