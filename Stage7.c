//
// main.c
// test 
// 
// Created by SULEMAN AKHTER on 25/01/2021.
//

#include <header.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

//C function interface
void currentDir();
void cmdHandle();
char** part1();
void process();
void extProcess();
void currentPath();
void setPath();
void cmdHistory();
void addAlias();
void removeAlias();
void printAlias();


int main(void) {

	//Construct a 2D array of strings to hold alias names and commands
	char* alias[10][2];
	//Hold the number of aliases currently held in the shell
	int aliasCount = 0;

	// saving the path into a variable
	char savedPath[512];
	strcpy(savedPath,getenv("PATH"));
	
	// set current directory to home
	chdir(getenv("HOME"));
	
	// prompt init message
	printf("Welcome to CS210 Group Shell Project \n");
	
	// pronpt loop while true until exit exit(0)
	while (1){
	
		// take input and handle the cmd
		cmdHandle(part1(savedPath),savedPath);
		
	}
	
	//return main
	return 0;
	
}

//print the work directory
void currentDir(){

	char* user = getenv("USER");
	
	char cwd[PATH_MAX];
	//getcwd function get current working dir
	getcwd(cwd,sizeof(cwd));
	//print to prompt
	printf("\n%s: %s ", user,cwd);
}

//the tokenised input
void cmdHandle (char** tokens, char* path) {

	char* cmd = *tokens;
	int cmdID = 0;
	char* para = tokens[1];
	char* cmdList[6];
	int empty;
	char** history = malloc(20 * sizeof(char*));
	char** tempHistory = malloc(20 * sizeof(char*));
	tempHistory[0] = "I hate Mac";
	tempHistory[1] = "I hate Pointers";
	
    //Check aliasing: if text is equal to alias, set it.
	for (int i = 0; i < 10; i++) {
		if (*tokens == alias[i][0]) {
			tokens = alias[i][1];
			break;
		}	
	}
	
	
	for (int i = 0; i < 2; i++) {
	
	
		//printf(*tempHistory);
		
	}
	
	// Checks for an empty index
	for(int i = 0; i<20; i++) {
	
		if (history[i] == NULL) {
		
			empty = i;
			break;
			
		}
		
	}
	if(strcmp(tokens[0],"!")){
		history[empty] = *tokens;
	}
	
	// should be holding the history commands
	
	
	
	//list of possible input, subject to change
	
	cmdList[0] = "exit";
	cmdList[1] = "!";
	cmdList[2] = "cd";
	cmdList[3] = "help";
	cmdList[4] = "getpath";
	cmdList[5] = "setpath";
	cmdList[6] = "alias"
	cmdList[7] = "unalias";
	
	
	//for loop to check which command
	for (int i = 0; i < 8; i++) {
		if (strcmp(cmd, cmdList[i])==0) {
			cmdID = i + 1;
			break;
		}
	}
	
	int intP;
	
	
	switch(cmdID) {
	
		//exit shell terminates
		case 1 :
			// restore the original path before closing
			setenv("PATH", path, 1);
			currentPath();
			printf("-> exit shell \n");
			exit(0);
		break;
		
		case 2:
			//[["!b'],[abc]]
			
			
			intP = atoi(para);
			if(para != NULL && intP > 0 && intP < 21) {
				printf("%s \n" ,para);
				printf("%s \n" ,*history[intP - 1]);
			}else{
				printf("-< Sorry history only stores last 20 command");
			}
			
			
			
			break;
			
			
			
		// cd change directory
		case 3:
		
			if (para==NULL) {
			
				printf("-> back to home directory");
				//cd back to users home dir
				chdir(getenv("HOME"));
				
			//run chdir function and check for return
			}else if(tokens[2]!=0) {
			
				printf("-> you can only enter one path");
			
			}else if(chdir(para)!=0){
			
				//failed - print error statement
				fprintf(stderr,"-> failed to cd, an error occured with [%s]: ", para);
				perror ("");
				
			
			
			
			}else{
			
				// worked print the current working directory
				currentDir();
			}
			
		break;
		
		//help just for testing, can be changed to something more useful
		case 4:
			printf("-> SOS");
			
		break;
		
		case 5:
		
			if(para!=NULL){
				printf("-> you don't need to enter a path");
			}else{
				currentPath();
			}
			
			
			
			
		break;
		
		case 6:
		
			setPath(tokens);
			
		break;
		
        case 7:

            if (para != NULL) {
                printAlias();
            } else if (tokens[2] != NULL) {
                addAlias(token[1], **token[2]);
            } else {
                printf("-> to use alias function, specify a name and command, or only use word \"alias\"");
            }

        break;

        case 8:

                if (tokens[1] != NULL && tokens[2] == NULL) {
                    removeAlias(tokens[1]);
                } else {
                    printf("-> to use unAlias function, specify the name of an existing alias");
                }

        break;
		
		//if none of these cmd has been entered then print error statement
		default:
		
			//printf("->this is ls cmd");
			process(tokens);
			//printf("[Sorry this command can not be recognised.] \n[Please check your input.]\n");
			
			
	}
}

//process function take in tokenised input the create process, execute the return process
void process(char ** token){

	//declare pid and create child process
	pid_t pid = fork();
	
	//if we can't create child process failed
	if(pid < 0) {
		//print error msg
		perror("-> failed to create process");
		return;
	
	//if we can create a child process
	}else if (pid == 0){
		//execute the cmd with it's arg, then check for errors
		if(execvp(token[0],token)<0){
		
		
			//error occured print errMsg
			fprintf(stderr,"-> [%s]: ", token[0]);
			perror("");
			
			
			exit(1);
		}
		//kill the process
		//exit(EXIT_FAILURE);
	}else{
	
		//no error occured put child process to wait (that is the end of that child process)
		wait(NULL);
		return;
	}
	
}



char** part1(char* path){

	char input[512];
	char** arrToken = malloc(50 * sizeof(char*));
	int i = 0;
	int c = 0;
	
	currentDir();
	printf("+ ");
	
	//Control D
	fgets(input, 512, stdin);
	c++;
	if(feof(stdin)){
	
		// restore the original path before closing
		setenv("PATH", path, 1);
		currentPath();
		printf("\n-> exit shell \n");
		exit(0);
		
	//Will break when u type "exit"
	}else{
	
		strtok(input, "\n");
		
		char* token = strtok(input," \t,|><&;");
		
		while(token != NULL){
			//printf ("%s\n",token);Just used for testing purposes.
			
			arrToken[i] = token;
			
			token = strtok(NULL, " \t,|><&;");
			
			i++;
		}
	}
	
	return arrToken;

}

void currentPath() {

	char* savedPath = getenv("PATH");
	printf("%s\n", savedPath);
	
}

void setPath(char** token) {

	if(token[1] == NULL) {
		printf("-> path could not be set: no address");
	}else if (token[2] != NULL) {
		printf("-> you can only enter one path");
	}else if (setenv("PATH", token[1], 1) != 0) {
		printf("-> path could not be set: bad address");
	}else{
		currentPath();
	}
	
	//function to add a new alias
	void addAlias(char* name, char** command){
		if (name != NULL || command != NULL) {
			printf("-> input the name and command for the new alias");
		} else if (aliasCount == 10) {
			printf("Max number of aliases reached, cannot add anymore");
		} else {
			int placePosition = -1;
			for (int i = 0; i < 10; i++) {
				if (alias[i][0] == name) {
					placePosition = i;
					printf("Alias command has been overriden");
					break;
				} else if (alias[i][0] == NULL && i < placePosition) {
					placePosition = i;
				}
			}
			if (placePosition > -1) {
				alias[placePosition][0] = name;
				alias[placePosition][1] = command;
			}
		}	
	}
	
	//funtion to remove an aliased command from the list
	void removeAlias(char* name){
		if (name == NULL) {
			printf("-> input the name of the alias you are removing");
		} else {
			int removed = 0;
			for (int i = 0; i < 10 && removed == 0; i++) {
				alias [i][0] = NULL;
				alias [i][1] = NULL;
				removed == 1;
				aliasCount -= 1;
			if (swapped == 0) {
				printf("Alias %s does not exist.", *name);
			}
		}
	}
	
	//Function to print all currently set aliases to the screen
	void printAlias(){
		//If there are no aliases set, display a message
		if (aliasCount == 0) {
			printf("There are no aliases set");
		//Otherwise, loop through all the currently set aliases
		} else {
			//While set aliases are detected, print the alias name to the screen.
			for (int i = 0; i < 10; i++) {
				if (alias[i][0] != NULL) {
					printf("%s\n", alias[i][0]);
				}
			}
		}
