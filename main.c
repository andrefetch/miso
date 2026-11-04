#include <stdio.h>
#include <stdlib.h>
#include <editline/readline.h>

static char input[2048]; // buffer for user input
char *name = "miso";
char *version = "0.0.1";

int main(int argc, char** argv) {
  
  /* Prints information to the terminal, version + exit information*/
  printf("%s version %s\n", name, version);
  puts("Ctrl+C to Exit\n");

  while (1) {

    char* input = readline("miso >> "); //outputs prompt
    
    add_history(input); 

    printf("No you are a %s", input);
    
    free(input)

  }

  return 0;

}
