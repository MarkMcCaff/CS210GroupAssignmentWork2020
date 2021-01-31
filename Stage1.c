#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <signal.h>

//initalise shell we don't really need it 
void init (){
    printf("SHELL STAGE 1");
}

//get current dir may come handy later 
void currentDir(){

    char cwd[PATH_MAX];
    //getcwd function get current working dir
    getcwd(cwd,sizeof(cwd));
    //print to prompt 
    printf("\nDir: %s", cwd);
}

// read user input and phase user input then return an array of token to be handled (may break it into two function later)
char** getInput(){

    //Set delimiters as string of delimiter characters
    char delimiters[] = " \t|><&;";

    //Declare userInput
    char* input;
    //Scan input from user as userInput
    get(input);

    //If CTRL+D detected during input
			//Set loopCondition to false
			//Exit loop
    
    //Set token as delimited string userInput with delimiter characters
    char** token = strtok(input, delimiters);

    // return the array of tokens to be handled 
    return token;

}


//handle cmd base on tokens 
int cmdHandle (char** token){

    
    char* cmd = token[0];

    //If first token is 'exit' or 'EXIT' 
    if (strcmp(cmd, 'exit')==0 || strcmp(cmd, 'EXIT'==0)){

        //Set loopCondition to false
        return 0;

    }else{
        //While token is not empty
        while (token != Null){

            //Output next token content on new line
            printf("'%s\' ", token);
            //Set token to next character in delimited 
			

        }
    }

}



int main (void){

    init();


    while (1){

        currentDir();
        
        printf("$ ");
        char * token = getInput();

        if (cmdHandle(token)==0){
            printf("exit shell\n");
            exit(0);
        }

    }
    return 0;
    

}