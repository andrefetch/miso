#include <stdio.h>

static char input[2048]; // buffer for user input
char *name = "miso";
char *version = "0.0.1";

int main(int argc, char** argv) {
  
  /* Prints information to the terminal, version + exit information*/
  printf("%s version %s\n", name, version);
  puts("Ctrl+C to Exit\n");

  while (1) {

    fputs("miso >> ", stdout); //outputs prompt

    fgets(input, 2048, stdin); // reads a line from the user input, max size 2048                                 bytes

    printf("No you are a %s", input);

  }

  return 0;

}
