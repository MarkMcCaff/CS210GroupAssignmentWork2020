//
// main.c
// test 
// 
// Created by SULEMAN AKHTER on 25/01/2021.
// Modified by Jackson Blair
// Error exists - everything
//

#include "header.h"

struct history {
	//this struct stores the string command and also an integer for easy printing
	char command[512];
	int commandNo;
};

struct alias {
	char* name;
	char* command; //How do I format an array of these
};

//commandNo = count + 1
int counter = 0;
struct history historyArray[20];
int aliasCount = 0;
struct alias aliases[10];

//C function interface
void currentDir();
void cmdHandle();
void input();
void tokenize();
void process();
void extProcess();
void currentPath();
void setPath();
void cmdHistory();
void historyHandle();
void printHistory();
void prevHistoryHandle();
int histEmpty();
void addAlias();
void removeAlias();
void printAlias();


int main(void) {

	// saving the path into a variable
	char savedPath[512];
	strcpy(savedPath,getenv("PATH"));
	char userInput[512];
	char* token[50];
	
	// set current directory to home
	chdir(getenv("HOME"));
	
	// prompt init message
	printf("Welcome to CS210 Group Shell Project \n");
	
	// pronpt loop while true until exit exit(0)
	while (1){
	
		// take input
		
		input(userInput, savedPath);
		
		//tokenize the input
		tokenize(userInput, token);
		// DO NOT EDIT OR DELETE THIS LINE FOR NOW
		printf("\n");
	
		// take input and handle the cmd
		cmdHandle(token, savedPath);
		
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
void cmdHandle (char* tokens[50], char* path) {

	char* cmd = tokens[0];
	int cmdID = 0;
	char* para = tokens[1];
	char* cmdList[10];
	int i = 0;
	
	while (tokens[i] != NULL) {
		for (int j = 0; j < 10; j++) {
			if (aliases[j].name == NULL) {
				break;
			}
			if (strcmp(tokens[i], aliases[j].name) == 0) {
				tokens[i] = aliases[j].command;
				break;
			}	
		}
		i++;
	}
	
	// should be holding the history commands
	
	
	
	//list of possible input, subject to change
	
	cmdList[0] = "exit";
	cmdList[1] = "!";
	cmdList[2] = "!!";
	cmdList[3] = "cd";
	cmdList[4] = "help";
	cmdList[5] = "getpath";
	cmdList[6] = "setpath";
	cmdList[7] = "history";
	cmdList[8] = "alias";
	cmdList[9] = "unalias";
	
	
	//for loop to check which command
	for (int i = 0; i < 10; i++) {
		if (strcmp(cmd, cmdList[i])==0) {
			cmdID = i + 1;
			break;
		}
	}

	switch(cmdID) {
	
		//exit shell terminates
		case 1 :
			// restore the original path before closing
			setenv("PATH", path, 1);
			currentPath();
			printf("-> exit shell \n");
			exit(0);
		break;
		
		//executes the specified previous history command i.e. ! 2
		case 2:
			//prints an error if there's more than one parameter
			if (tokens[2] != NULL) {
				printf("-> you can only enter one argument");
			} else {
				historyHandle(para, path);
			}
		break;
		
		//executes the previous history command (!!)
		case 3:
			//prints an error if there's an argument
			if(para != NULL) {
				printf("-> error! this command cannot take any arguments");
			} else {
				prevHistoryHandle(path);
			}
		break;
		
		// cd change directory
		case 4:
		
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
		case 5:
			
			printf("-> SOS");
			
		break;
		
		//getpath
		case 6:
		
			if(para!=NULL){
				printf("-> you don't need to enter a path");
			}else{
				currentPath();
			}
			
		break;
		
		//setPath(tokens)
		case 7:
		
			setPath(tokens);
			
		break;
		
		//print history
		case 8:
		
			if(para != NULL) {
				printf("-> error! this command cannot take any arguments");
			} else {
				printHistory();
			}
			
		break;
		
		//alias (either show all aliases, or add a new alias
		case 9:
		
			if (para == NULL) {
				printAlias();
			} else if (tokens[2]!= NULL) {
				addAlias(tokens[0], **tokens + 1);
			} else {
				printf("-> to use alias function, specify a name and command, or only use word \"alias\"\n");
			}
		
		break;
		
		//unalias
		case 10:
		
			if (para == NULL) {
				printf("-> Please specify an existing alias name to remove it\n");
			} else if (tokens[1] != NULL && tokens[2] == NULL) {
				removeAlias();
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

// adds the command to the history array
void pushHistory(char* input) {
	// if the array is full then everything shifts to the left, deleting the earliest command
	if (counter == 20) {
		for (int i = 0; i < 19; i++) {
			// copies the elements from the next command and updates the number 
			historyArray[i] = historyArray[i + 1];
			historyArray[i].commandNo = i + 1;
		}
		// add the new command to the last entry (19)
		historyArray[19].commandNo = counter;
		strcpy(historyArray[19].command, input);
	} else {
		// if it's not full it can just add the new command and increment counter
		historyArray[counter].commandNo = counter + 1;
		strcpy(historyArray[counter].command, input);
		counter++;
	}
}

//handles the !! command
void prevHistoryHandle(char* path) {
	
	char* tInput[50];
	// If the counter is 0 then there is no previous command so it prints an error 
	if (counter != 0) {
		//because we're using pointers we need to store the command and parameters in a seperate variable so they don't get deleted
		char inputCopy[512];
		strcpy(inputCopy, historyArray[(counter - 1)].command);
		
		//tokenises and executes the command
		tokenize(tInput, historyArray[counter - 1].command);
		printf("%s\n", tInput[0]);
		cmdHandle(tInput,path);
		
		//makes sure the original command doesn't change in history and pushes the invoked command
		strcpy(historyArray[(counter - 1)].command, inputCopy);
	} else {
		printf("Error! There is no command stored at this element in history.");
	}
}

//this was used mostly for a circular array (cheks if the element is empty)
int histEmpty (int index) {
	if (historyArray[index].command[0] != '\0') {
		// returns 1 if the element is NOT empty
		return 1;
	}
	// otherwise returns 0
	return 0;
}

//prints the history and parameters alongside its command number
void printHistory() {
	if (counter != 0) {
		//prints the current list of list of commands stored in history
		for (int i = 0; i < 20; i++) {
			// if the only character present is a null terminator then it won't print anything
			if (histEmpty(i) == 1) {
				// prints a number alongside the command and inserts a '/0' for formatting
				printf("%d %s%c", historyArray[i].commandNo, historyArray[i].command, '\0');
			}
		}
	} else {
		printf("Error! There were no previous commands to print");
	}
}

void historyHandle(char* para, char* path) {
	
	int i = 0;
	i = atoi(para);
	
	// if a negative number is given then it tries to convert it into a valid command 
	if (i < 0) {
	// uses counter + 1 so that the last command can be an option
		i = ((counter + 1) - abs(i));
	}
	
	if(i > 0 && i < 21) {
		
		if(histEmpty(i - 1) == 1) {
		
		// temporary variable used to store the original command so that the parameters are not lost when tokenised
			char inputCopy[512];
		strcpy(inputCopy, historyArray[(i - 1)].command);
		
		char* tInput[50];
		tokenize(tInput, historyArray[(i - 1)].command);
		
		//DO NOT DELETE THIS LINE CODE IS BASED ON THIS TO RUN
		printf("%s\n", tInput[0]);
		
		//handle the tokenized input
		cmdHandle(tInput,path);
		
		//handle the tokenized input
		cmdHandle(tInput,path);
		
		//makes sure the original command doesn't change in history and pushes the invoked command
		strcpy(historyArray[(i - 1)].command, inputCopy);
		} else {
			printf("-> sorry, command not found ");
		}
	} else {
		printf("-> sorry, command not found. Use a parameter wihin current range");
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

void input(char input[512], char* path){

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
	}
	
	// doesn't add to the history if it's either an invocation, calling history, or just 'enter' is pressed
	if((strchr(input, '!') == NULL) && (strcmp(input, "history\n") != 0) && (strcmp(input, "\n") != 0)) {
		pushHistory(input);
	}	
	
}

void tokenize(char* input, char* arrToken[50]) {

	int i = 0;
	
	arrToken[0] = strtok(input, " \t,><&;\n");
	
		while(arrToken[i] != NULL){
			i++;
			arrToken[i] = strtok(NULL, " \t,><&;");
		}
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
				if (aliases[i].name == name) {
					placePosition = i;
					printf("Alias command has been overriden");
					break;
				} else if (aliases[i].name == NULL && i < placePosition) {
					placePosition = i;
				}
			}
			if (placePosition > -1) {
				aliases[placePosition].name = malloc(strlen(name) + 1);
				strcpy(aliases[placePosition].name, name);
				aliases[placePosition].command = malloc(strlen(*command) + 1);
				strcpy(aliases[placePosition].command, *command);
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
				aliases[i].name = NULL;
				aliases[i].command = NULL;
				removed = 1;
				aliasCount -= 1;
			}
			if (removed == 0) {
				printf("Alias %s does not exist.", name);
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
				if (aliases[i].name != NULL) {
					printf("%s\n", aliases[i].name);
				}
			}
		}
	}
