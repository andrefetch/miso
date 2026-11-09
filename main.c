#include "mpc.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// ints unsigned
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

// ints signed
typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

// ptrs (unsigned & signed)
typedef uintptr_t uptr;
typedef intptr_t  iptr;

// size_t unsigned + signed
// (usize and isize are now defined in mpc.h)

// floats & doubles
typedef float  f32;
typedef double f64;

// bool
typedef bool b8;

// IF we are using a win32 sys
#ifdef _WIN32
#include <string.h>

static char buffer[2048]; // buffer for user input

/* fake readline func */
char* readline(char* prompt) {
  fputs(prompt, stdout);
  fgets(buffer, 2048, stdin);
  usize len = strlen(buffer);
  char* cpy = malloc(len+1); // + 1 because we include the null terminator 
  strcpy(cpy, buffer);
  cpy[len-1] = '\0'; //removes null terminator from cpy 
  return cpy;
}

/* fake add_history func */
void add_history(char* unused) {}

#else 
#include <editline/readline.h>
// #include <editline/history.h>
#endif

i64 eval_op(i64 x, char* op, i64 y);

i64 eval(mpc_ast_t* t) {
  /* if tagged as num return */
  if (strstr(t->tag, "number")) {
    return (i64)atol(t->contents);
  }

  /* operator gives second child */ 
  char* op = t->children[1]->contents;

  i64 x = eval(t->children[2]);

  usize i = 3;
  while (i < (usize)t->children_num && strstr(t->children[i]->tag, "expr")) {
    x = eval_op(x, op, eval(t->children[i]));
    i++;
  }

  return x;
}

i64 eval_op(i64 x, char* op, i64 y) {
  if (strcmp(op, "+") == 0) {return x + y;}
  if (strcmp(op, "-") == 0) {return x - y;}
  if (strcmp(op, "*") == 0) {return x * y;}
  if (strcmp(op, "/") == 0) {return x / y;}
  return 0;
}


int main(int argc, char** argv) {

  /* Parsers */
  mpc_parser_t* Number = mpc_new("number");
  mpc_parser_t* Operator = mpc_new("operator");
  mpc_parser_t* Expr = mpc_new("expr");
  mpc_parser_t* Miso = mpc_new("miso");

  /* Defined with the following regular expressions language */
  mpca_lang(MPCA_LANG_DEFAULT,
  "                                                     \
    number   : /-?[0-9]+/ ;                             \
    operator : '+' | '-' | '*' | '/' ;                  \
    expr     : <number> | '(' <operator> <expr>+ ')' ;  \
    miso     : /^/ <operator> <expr>+ /$/ ;             \
  ",
  Number, Operator, Expr, Miso);

  const char *name = "Miso";
  const char *version = "0.0.1";
  const char *credits = "contribute @ github.com/andrefetch/miso";

  printf("%s Version %s\n", name, version);
  puts("Press Ctrl+c to Exit");
  printf("%s\n\n", credits);

  while (1) {

    char* input = readline("miso >> "); //outputs prompt
    if (input == NULL) {
      break;
    }
    
    add_history(input); 

    // printf("Your input was.. %s\n", input);
    mpc_result_t r;
    if(mpc_parse("<stdin>", input, Miso, &r)) {
      i64 result = eval(r.output);
      printf("%lli\n", result);
      mpc_ast_delete(r.output);
    } else {
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }
    
    free(input);

  }

  /* Before return, undefine and del parsers */
  mpc_cleanup(4, Number, Operator, Expr, Miso);

  return 0;

}
