#include <stdio.h>
#include <string.h>


int main(void) {
    //Set an integer variable that, while set to 1 (while true), allows the user to be continually prompted for input.
	int loopCondition = 1;
    //Set a string variable to hold all characters separating tokens/parameters from user input.
    char delimiters[] =  " \t|><&;";
    //Prompt user input while loopCondition is True.
	while (loopCondition == 1) {
        //Display promt for next user input.
        printf("$ ");
		//Set a string variable to hold the text read from user input.
        char *userInput; 
		//Scan input from the user as userInput.
        fgets(userInput, 512, stdin);
        //If CTRL+D is pressed during input
            //if (fgets('^d')) {
			    //Set loopCondition to 0 (false) so the user is no longer prompted for input.
                //loopCondition = 0;
			    //Exit loop
                //break;
        //}
        //Remove unnecessary enter key input at end.
        userInput = strtok(userInput, "\n");
        //If user input is empty, present user message. Otherwise, continue.
        if (*userInput == '\0') 
            printf("Input is empty\n");
        else {
		    //Set a string variable the user input, split from delimiter characters;
		    char *token = strtok(userInput, delimiters);
            //If first token is 'exit', exit the shell
            if (strcmp(token, "exit") == 0) {
			    //Set loopCondition to 0 (false) so the user is no longer prompted for input.
                loopCondition = 0;
		    //Else
            } else {
			    //While token is not empty
                while (token != NULL) {
                    //Output the next token/parameter to the user.
				    printf("%s\n", token);
				    //Set the current token to the next parameter.
                    token = strtok(NULL, delimiters);
                }
            }
        }
    }
	//Print message to notify user that they have exited the shell.
    printf("Shell exited\n");
}