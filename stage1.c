#include <stdio.h>
#include <string.h>

int main(void) {
   char delimiters[] =  " \t|><&;";
   char *input; 
   int loop = 1;
   
   while (loop == 1) { 
	   printf("$ ");
	   gets(input);
	   // Extract the first token
	   char *token = strtok(input, delimiters);
	   // loop through the string to extract all other tokens
	   if (strcmp(token, "exit") == 0 || strcmp(token, "EXIT") == 0) {
			loop = 0;
		} 
		else {
			while (token != NULL) {
				printf("'%s\' ", token);
				token = strtok(NULL, delimiters);
			}
		}
		printf("\n");
   }
   printf("Terminating shell...\n");
   return 0;
}