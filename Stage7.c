#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

struct history {
	//this struct stores the string command and also an integer for easy printing
	char command[512];
	int commandNo;
};

struct alias {
	char* name;
	char* command;
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
void saveHistory();
void loadHistory();
void addAlias();
void removeAlias();
void printAlias();
void aliasHandle();


int main(void) {

	// saving the path into a variable
	char savedPath[512];
	strcpy(savedPath,getenv("PATH"));
	char userInput[512];
	char* token[50];
	
	// set current directory to home
	chdir(getenv("HOME"));

	// tries to load the previously saved history
        loadHistory();
	
	// prompt init message
	printf("Welcome to CS210 Group Shell Project \n");
	
	// prompt loop while true until exit exit(0)
	while (1){
	
		// take input	
		input(userInput, savedPath);

		if (strcmp(userInput, "\n") != 0) {
			
			//tokenize the input
			tokenize(userInput, token);
			
			// DO NOT EDIT OR DELETE THIS LINE FOR NOW
			printf("\n");
			
			aliasHandle(token, savedPath);
				
	 	}
		
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
	char* cmdList[11];
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
	cmdList[2] = "!-";
	cmdList[3] = "!!";
	cmdList[4] = "cd";
	cmdList[5] = "help";
	cmdList[6] = "getpath";
	cmdList[7] = "setpath";
	cmdList[8] = "history";
	cmdList[9] = "alias";
	cmdList[10] = "unalias";
	
	
	//for loop to check which command
	for (int i = 0; i < 11; i++) {
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
			saveHistory();
			exit(0);
		break;
		
		//executes the specified previous history command i.e. ! 2
		case 2:;
			
			int posPosition  =  0;
			
			//prints an error if there's more than one parameter
			if (tokens[2] != NULL) {
				printf("-> you can only enter one argument");
			} else if (para == NULL) {
				printf(" -> error! this command needs an argument");
			} else {
				posPosition = atoi(para);
				historyHandle(posPosition, path);
			}
		break;

		//excute the history command in negative 
		case 3:;


			if (para == NULL) {
				printf(" -> error! this command needs an argument");
			}
			else {
				
				int negPosition;
				negPosition = atoi(para);

				// uses counter + 1 so that the last command can be an option
				negPosition = ((counter + 1) - abs(negPosition));

				// prints an error if there's more than one parameter 
		    		if (tokens[2] != NULL) {
		         		printf("-> you can only enter one argument");
		    		} else {
		        		historyHandle(negPosition, path); 
		    		}
		    	}
		break;
		
		//executes the previous history command (!!)
		case 4:
			//prints an error if there's an argument
			if(para != NULL) {
				printf("-> error! this command cannot take any arguments");
			} else {
				prevHistoryHandle(path);
			}
		break;
		
		// cd change directory
		case 5:
		
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
		case 6:
			
			printf("-> SOS");
			
		break;
		
		//getpath
		case 7:
		
			if(para!=NULL){
				printf("-> you don't need to enter a path");
			}else{
				currentPath();
			}
			
		break;
		
		//setPath(tokens)
		case 8:
		
			setPath(tokens);
			
		break;
		
		//print history
		case 9:
		
			if(para != NULL) {
				printf("-> error! this command cannot take any arguments");
			} else {
				printHistory();
			}
			
		break;
		
		//alias (either show all aliases, or add a new alias
		case 10:
			if (para == NULL) {
				printAlias();
			} else if (tokens[2] != NULL) {
				char addArray[512] = {0};
				for (int i = 2; i < 20; i++) {
					if (tokens[i] == NULL) {
						break;
					}
				    strcat(strcat(addArray, " "), tokens[i]);
				}
				char* passArray = addArray;
				passArray++;    
				addAlias(para, passArray);
			} else {
				printf("-> to use alias function, specify a name and command, or only use word \"alias\"\n");
			}
		
		break;
		
		//unalias
		case 11:
			if (para == NULL) {
				printf("-> Please specify an existing alias command to remove it\n");
			} else {  
				removeAlias(para);
			}
		break;
		
		//if none of these cmd has been entered then process 
		default:
			process(tokens);
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
		tokenize(historyArray[counter - 1].command,tInput);
		printf("%s\n", tInput[0]);
		aliasHandle(tInput,path);
		
		//makes sure the original command doesn't change in history and pushes the invoked command
		strcpy(historyArray[(counter - 1)].command, inputCopy);
	} else {
		printf("-> Error! There is no command stored at this element in history.");
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
		printf("-> Error! There were no previous commands to print");
	}
}

void historyHandle(int i, char* path) {
	
	if(i > 0 && i < 21) {
		
		if(histEmpty(i - 1) == 1) {


		// temporary variable used to store the original command so that the parameters are not lost when tokenised
		char inputCopy[512];
		strcpy(inputCopy, historyArray[(i - 1)].command);

		
		char* tInput[50];
		tokenize(historyArray[i - 1].command,tInput);
		
		//DO NOT DELETE THIS LINE CODE IS BASED ON THIS TO RUN
		aliasHandle(tInput, path);
		
		//makes sure the original command doesn't change in history and pushes the invoked command
		strcpy(historyArray[(i - 1)].command, inputCopy);
		} else {
			printf("-> Sorry, no command stored at [%d].",i);
		}
	} else {
		printf("-> Sorry, command not found. Use a parameter between -20 and 20 except 0");
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
		saveHistory();
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
			arrToken[i] = strtok(NULL, " \t,><&;\n");
			
		}
}

void currentPath() {

	char* savedPath = getenv("PATH");
	printf("%s\n", savedPath);
	
}

void setPath(char** token) {

	if (token[1] == NULL) {
		printf("-> path could not be set: no address");
	} else if (token[2] != NULL) {
		printf("-> you can only enter one path");
	} else if (setenv("PATH", token[1], 1) != 0) {
		printf("-> path could not be set: bad address");
	} else {
		currentPath();
	}
}

void saveHistory() {
    
    // opens the .hist_file for writing
    FILE *fptr;
    fptr = fopen(".hist_list", "w");
    // doesn't run the rest of the method if the file cannot be found
    if (fptr == NULL) {
        return;
    }
    // loops for the maximum number of elements possible in historyArray
    for (int i = 0; i < 20; i++) {
        // in the first instance it finds an empty command it will stop running the method 
        if (histEmpty(i) == 0) {
            break;
        }
        // puts the command into the next line of fptr
        fputs(historyArray[i].command, fptr);
    }
    // closes the file
    fclose(fptr);
}

void loadHistory() {

    // opens the .hist_file for reading
    FILE *fptr;
    fptr = fopen(".hist_list", "r");
    // temportary variable for reading
    char commandStr[512]; 
    // doesn't run the rest of the method if the file cannot be found
    if (fptr == NULL) {
        return;
    }
    // loops for the maximum number of elements possible in historyArray
    for (int i = 0; i < 20; i++) {
        // pushes the command it read as long as there was something there
        if (fgets(commandStr, 512, fptr) != NULL) {
            pushHistory(commandStr);
        }
    }
    // closes the file
    fclose(fptr);
}
	
//function to add a new alias
void addAlias(char* newName, char newCommand[]) {

    int placePosition = -1;

    if (aliasCount == 0) {
        placePosition = 0;
    } else {
        for (int i = 0; i < aliasCount; i++) {
            if (i == 9 && strcmp(aliases[i].name, newName) != 0) {
            	printf("max number of aliases reached - cannot add any more");
            	return;
            } else if (strcmp(aliases[i].name, newName) == 0) {
                strcpy(aliases[i].command, newCommand);
                printf("alias command has been overriden");
                return;
            }
                placePosition = aliasCount;
        }
    }    
    if (placePosition > -1) {
        aliases[placePosition].name = malloc(strlen(newName) + 1);
        strcpy(aliases[placePosition].name, newName);	
        aliases[placePosition].command = malloc(strlen(newCommand) + 1);
        strcpy(aliases[placePosition].command, newCommand);
        printf("alias set");			
    	aliasCount++;
    }
}
	
//funtion to remove an aliased command from the list
void removeAlias(char removeName[]){

	int removed = 0;
	for (int i = 0; i < aliasCount && removed == 0; i++) {
		if (strcmp(aliases[i].name, removeName) == 0 && aliasCount == 1) {
			aliases[i].name = NULL;
			aliases[i].command = NULL;
			removed = 1;
			aliasCount--;
		} else if (strcmp(aliases[i].name, removeName) == 0) {		
			for (int j = i; j < aliasCount; j++) {
				if (j == aliasCount - 1) {
					aliases[j].name = NULL;
					aliases[j].command = NULL;				
				}
				else {
					strcpy(aliases[j].name, aliases[j+1].name);
					strcpy(aliases[j].command, aliases[j+1].command);					
				}
			}
			removed = 1;
			break;			
		}
	}
	aliasCount--;
	if (removed == 0) {
		printf("alias [%s] does not exist.", removeName);
	} else {
		printf("alias [%s] was removed.", removeName);
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
				printf("alias %s = '%s'\n", aliases[i].name, aliases[i].command);
			}
		}
	}
}

//Function for looping through the tokens to check for instances of aliases
void aliasHandle(char* tokens[50], char* path) {

	char newInput[512] = {0};
	int position = 0;
	
	for (int i = 0; i < aliasCount; i++) {
		if (strcmp(tokens[0], aliases[i].name) == 0) {
			strcpy(newInput, aliases[i].command);
			position = 1;
			break;
		}
	}
	while (tokens[position] != NULL) {
		strcat(strcat(newInput, " "), tokens[position]);
		position++;
	}
	
	tokenize(newInput, tokens);
	
	// take input and handle the cmd
	cmdHandle(tokens, path);
}
