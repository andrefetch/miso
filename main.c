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
  cpy[strlen(cpy)-1] = '\0'; //remoces null terminator from cpy 
  return cpy;
}

/* fake add_history func */
void add_history(char* unused) {}

#else 
#include <editline/readline.h>
// #include <editline/history.h>
#endif


int main(int argc, char** argv) {

  char *name = "Miso";
  char *version = "0.0.1";
  char *credits = "@andrefetch on GitHub || andresilva.tech";

  printf("%s Version %s\n", name, version);
  puts("Press Ctrl+c to Exit");
  printf("%s\n\n", credits);

  while (1) {

    char* input = readline("miso >> "); //outputs prompt
    
    add_history(input); 

    printf("Your input was.. %s\n", input);
    free(input);

  }

  return 0;

}
