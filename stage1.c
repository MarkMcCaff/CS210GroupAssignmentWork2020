#include <stdio.h>
#include <string.h>

int main(void) {
	// set up delimiters for the program to look out for 
	char delimiters[] =  " \t|><&;";
	// input variable
	char *input; 
	// looping determinant
	int loop = 1;
   
	// loops until the user has requested that the shell close
	while (loop == 1) { 
		// sets up the shell environment to get user's input
		printf("$ ");
		gets(input);
		// Extracts the first token
		char *token = strtok(input, delimiters);
		// stops the program from continuing past this loop if the user wants to exit // 
		if (strcmp(token, "exit") == 0 || strcmp(token, "EXIT") == 0) {
			loop = 0;
		} 
		// otheriwse, prints all of the tokens present
		else {
			while (token != NULL) {
				printf("'%s\' ", token);
				token = strtok(NULL, delimiters);
			}
		}
		// sets up a new line for the next input
		printf("\n");
	}
	// notifies the user that the shell is closing
	printf("Terminating shell...\n");
	return 0;
}