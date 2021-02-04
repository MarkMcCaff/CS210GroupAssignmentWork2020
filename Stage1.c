#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <signal.h>


//initalise shell we don't really need it 
void init (){
    printf("------SHELL STAGE 1------");
}

//get current dir may come handy later 
void currentDir(){

    char* user = getenv("USER");

    char cwd[PATH_MAX];
    //getcwd function get current working dir
    getcwd(cwd,sizeof(cwd));
    //print to prompt 
    printf("\n%s : %s $ ", user,cwd);
}

// read user input and return the input
char* getInput(){

    //Declare userInput
    char* input;
    input = (char*)malloc(512);

    //Scan input from user as userInput
    //If CTRL+D detected during input
    if (!fgets(input, 512, stdin)){

        //Exit loop
    
        printf(">>> exit shell \n");
            exit(0);
    }

    return input;
}

//parse the input to an array of strings then return it
char** parseInput(char* input){

    //printf("%s",input);

    //Set delimiters as string of delimiter characters
    char delimiters[] = " \t|><&;";

    //set up the array 
    char ** arrToken = malloc(200*sizeof(char*)); 

    //set up counter
    int i = 0;

    
    //Set token as delimited string userInput with delimiter characters
    char* token = strtok(input, delimiters);

    //place the rest of part onto array 
    while( token!= NULL){
        arrToken[i]= token;

        token = strtok(NULL, delimiters);
        i++;
    }
    

    

    // return the array of tokens to be handled 
    return arrToken;

}





//handle cmd base on tokens 
void cmdHandle (char** token){

    //set string cmd to be the first pointer of the array
    char* cmd = *token;

    //still need to be fixed 
    char* paramter = token[1];

    //If first token is 'exit' or exit with space, then exit the shell
    if (strcmp(cmd, "exit\n")==0||strcmp(cmd,"exit")==0){
        printf(">>> exit shell \n");
        exit(0);

    }else{
        
        //else print the paramter(later to be changed, paramter need to be processed)
        printf(">>> %s ", paramter);
    }

}



int main (void){

    init();

    printf("HEllo world");


    while (1){

        currentDir();
        
        //cmd handle take parse input as input and parse input take get input as input 
        cmdHandle(parseInput(getInput()));

    }
    return 0;
    

}