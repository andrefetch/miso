#include "mpc.h"
#include <stdio.h>
#include <stdlib.h>

// IF we are using a win32 sys
#ifdef _WIN32
#include <string.h>

static char buffer[2048]; // buffer for user input

/* fake readline func */
char* readline(char* prompt) {
  fputs(prompt, stdout);
  fgets(buffer, 2048, stdin);
  char* cpy = malloc(strlen(buffer)+1); // + 1 because we include the null terminator 
  strcpy(cpy, buffer);
  cpy[strlen(cpy)-1] = '\0'; //removes null terminator from cpy 
  return cpy;
}

/* fake add_history func */
void add_history(char* unused) {}

#else 
#include <editline/readline.h>
// #include <editline/history.h>
#endif


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

  char *name = "Miso";
  char *version = "0.0.1";
  char *credits = "contribute @ github.com/andrefetch/miso";

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
      /*on success*/
      mpc_ast_print(r.output);
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
