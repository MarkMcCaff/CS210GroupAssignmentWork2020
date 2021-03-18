//
// main.c
// test 
// 
// Created by SULEMAN AKHTER on 25/01/2021.
// Modified by Jackson Blair
// Error exists - everything
//

#include "header.h"

//C function interface
void currentDir();
void cmdHandle();
char* input();
char** part1();
void process();
void extProcess();
void currentPath();
void setPath();
void cmdHistory();
void historyHandle();
void addAlias();
void removeAlias();
void printAlias();


int main(void) {

	//Construct a 2D array of strings to hold alias names and commands
	char* alias[10][2];
	/*char* alias[10][2] = malloc(sizeof(char*) * 10);
	for (int i = 0; i < 10; i++) {
		alias[i] = malloc(sizeof(char*) * 2);
	}*/
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
	
		// take input
		
		char *userInput = input(savedPath);
		//printf("%s\n", userInput);
		
		//tokenize the input
		char** tokenizedInput = tokenize(userInput);
		// DO NOT EDIT OR DELETE THIS LINE FOR NOW
		printf("%s\n", tokenizedInput[0]);
	
		// take input and handle the cmd
		cmdHandle(part1(savedPath),savedPath, alias, aliasCount);
		
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
void cmdHandle (char** tokens, char* path, char* alias[10][2], int aliasCount) {

	char* cmd = *tokens;
	int cmdID = 0;
	char* para = tokens[1];
	char* cmdList[8];
	
	//Allow aliases to alias other aliases, loop until we know the command is not an alias:
	int notAlias = 0;
	while (notAlias = 0) {
		notAlias = 1;
		for (int i = 0; i < 10; i++) {
			if (*tokens == alias[i][0]) {
				tokens = alias[i][1];
				notAlias = 0;
			}	
		}
	}
	
	// should be holding the history commands
	
	
	
	//list of possible input, subject to change
	
	cmdList[0] = "exit";
	cmdList[1] = "!";
	cmdList[2] = "cd";
	cmdList[3] = "help";
	cmdList[4] = "getpath";
	cmdList[5] = "setpath";
	cmdList[6] = "alias";
	cmdList[7] = "unalias";
	
	
	//for loop to check which command
	for (int i = 0; i < 9; i++) {
		if (strcmp(cmd, cmdList[i])==0) {
			cmdID = i + 1;
			break;
		}
	}
	
	//Variable unused in current program. Purpose unknown
	//int intP;
	
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
		
			//History involved functions go here
				
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
		
		//getpath
		case 5:
		
			if(para!=NULL){
				printf("-> you don't need to enter a path");
			}else{
				currentPath();
			}
			
		break;
		
		//setPath(tokens)
		case 6:
		
			setPath(tokens);
			
		break;
		
		//alias
		case 7:
		
			if (para == NULL) {
				printAlias(alias, aliasCount);
			} else if (tokens[2]!= NULL) {
				addAlias(tokens[0], **tokens + 1, alias, aliasCount);
			} else {
				printf("-> to use alias function, specify a name and command, or only use word \"alias\"\n");
			}
		
		break;
		
		//unalias
		case 8:
		
			if (para == NULL) {
				printf("-> Please specify an existing alias name to remove it\n");
			} else if (tokens[1] != NULL && tokens[2] == NULL) {
				removeAlias(tokens[1], alias, aliasCount);
			} else if (tokens[2] != NULL) {
				printf("-> You should only enter one alias name");
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

/*
//process function take in tokenised input the create process, execute the return the process
void process(char** token) {

	//declare pid and create child process
	pid)t pid = fork();
	
	//if we can't create child process failed
	if(pid < 0){
		//print error msg
		perror("-> failed to create process");
		return;
		
	//if we can create a child process
	}else if (pid == 0) {
		// execute the cmd with it's arg, then check for errors
		if(execvp(token[0],token)<0){
		
		
			//error occured print errMsg
			fprintf(stderr,"-> [%s]: ", token[0]);
			perror ("");
			
			
			exit(1);
		}
		//kill the process
		//exit(EXIT_FAILURE);
	}else{
		
		//no error occured but child process to wait (that is the end of that child process)
		wait(NULL);
		return;
	}
}
*/

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
}
	
	//function to add a new alias
	void addAlias(char* name, char** command, char* alias[10][2], int aliasCount){
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
	void removeAlias(char* name, char* alias[10][2], int aliasCount){
		if (name == NULL) {
			printf("-> input the name of the alias you are removing");
		} else {
			int removed = 0;
			for (int i = 0; i < 10 && removed == 0; i++) {
				alias [i][0] = NULL;
				alias [i][1] = NULL;
				removed = 1;
				aliasCount -= 1;
			}
			if (removed == 0) {
				printf("Alias %s does not exist.", name);
			}
		}
	}
	
	//Function to print all currently set aliases to the screen
	void printAlias(char* alias[10][2], int aliasCount){
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
	}
