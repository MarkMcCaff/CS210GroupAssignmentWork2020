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
int numberOfExecutions = 0;


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
void saveAlias();
void loadAlias();


int main(void) {

	//saving the path into a variable
	char savedPath[512];
	strcpy(savedPath,getenv("PATH"));
	char userInput[512];
	char* token[50];
	
	//set current directory to home
	chdir(getenv("HOME"));

	//prompt init message
	printf("Welcome to CS210 Group Shell Project \n");
    	
    	//tries to load the previously saved history
        //loadHistory();

    	//tries to load the saved aliases into array aliases
        loadAlias();
	
	//prompt loop while true until exit exit(0)
	while (1) {
	
		//takes input	
		input(userInput, savedPath);

		//won't do anything if the user only inputs <enter>
		if (strcmp(userInput, "\n") != 0) {
			
			//tokenize the input
			tokenize(userInput, token);
			numberOfExecutions = 0;
			
			printf("\n");
			
			//sends the tokenised input to check if there's any aliases
			aliasHandle(token, savedPath);		
	 	}
	}
	
	//return main
	return 0;
}

//print the work directory
void currentDir() {

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
	
	//list of possible commands, subject to change
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
		
	//for loop to give each command a case
	for (int i = 0; i < 11; i++) {
		if (strcmp(cmd, cmdList[i]) == 0) {
			cmdID = i + 1;
			break;
		}
	}

	switch(cmdID) {

		//exit shell terminates
		case 1 :
		
			//resets the directory back to home to save the history and aliases
			chdir(getenv("HOME"));
			saveHistory();
            		saveAlias();
			//restores the original path before closing
			setenv("PATH", path, 1);
			currentPath();
			printf("-> exit shell \n");

			exit(0);
		break;
		
		//executes the specified previous history command i.e. ! 2
		case 2:;
			
			int posPosition  =  0;
			
			//prints an error if there's more than one parameter
			if (tokens[2] != NULL) {
				printf("-> you can only enter one argument");
			//prints an error if there's no parameters 
			} else if (para == NULL) {
				printf(" -> error! this command needs an argument");
			//otherwise turns the parameter into an integer for the historyHandle method
			} else {
				posPosition = atoi(para);
				historyHandle(posPosition, path);
			}
		break;

		//execute the history command if negative 
		case 3:;

			//prints an error if there's no parameters
			if (para == NULL) {
				printf(" -> error! this command needs an argument");
			} else {
				
				//turns the parameter into an integer	
				int negPosition;
				negPosition = atoi(para);

				//uses counter + 1 so that the last command can be an option
				negPosition = ((counter + 1) - abs(negPosition));

				//prints an error if there's more than one parameter 
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

		//cd change directory
		case 5:

			//if there's no parameter it goes back to the home directory
			if (para == NULL) {
			
				printf("-> back to home directory");
				//cd back to users home dir
				chdir(getenv("HOME"));

			//run chdir function and check for return
			} else if(tokens[2] != 0) {

				printf("-> you can only enter one path");

			} else if(chdir(para) != 0){

				//failed - print error statement
				fprintf(stderr,"-> failed to cd, an error occured with [%s]: ", para);
				perror("");

			} else {

				//prints the current working directory
				currentDir();
			}

		break;

		//help just for testing, can be changed to something more useful
		case 6:

			printf("-> SOS");

		break;

		//prints the current path
		case 7:

			//prints an error if there's a parameter
			if (para != NULL) {
				printf("-> you don't need to enter a path");
			} else {
				currentPath();
			}

		break;

		//sets the shell's path
		case 8:

			setPath(tokens);

		break;

		//prints the history
		case 9:

			//prints an error if there are any arguments
			if (para != NULL) {
				printf("-> error! this command cannot take any arguments");
			} else {
				printHistory();
			}

		break;

		//alias (either prints all of the aliases, or adds a new one)
		case 10:;

			//sets a temporary integer for later printing
			int oldCount =  aliasCount;
			//if there areno parameters the aliases are printed
			if (para == NULL) {
				printAlias();
			//sets a new alias if there are 2 parameters
			} else if (tokens[2] != NULL) {
				//sets up and initialises a new array
				char addArray[512] = {0};
				for (int i = 2; i < 20; i++) {
					//breaks out of the loop as soon as it reaches the end of the string
					if (tokens[i] == NULL) {
						break;
					}
					//adds the tokens to the new array
					strcat(strcat(addArray, " "), tokens[i]);
				}
				//pointer points to the array and removes the first empty space
				char* passArray = addArray;
				passArray++;    
				//passes in the first token and the new string
				addAlias(para, passArray);
				//if a new alias was set then it prints this
				if(aliasCount > oldCount) {
					printf("-> alias set");	
				}                
			//if there's only one parameter an error is printed
			} else {
				printf("-> error! there is only one parameter. use alias <name> <command>\n");
			}

		break;

		//removes an alias
		case 11:

			//prints an error if there's no parameters
			if (para == NULL) {
				printf("-> Please specify an existing alias command to remove it\n");
			//otherwise removes the specified alias
			} else {  
				removeAlias(para);
			}

		break;

		//if none of these are entered, then it processes the tokens 
		default:
			process(tokens);
	}
}

//adds the command to the history array
void pushHistory(char* input) {

	//if the array is full then everything shifts to the left, deleting the earliest command
	if (counter == 20) {
		for (int i = 0; i < 19; i++) {
			// copies the elements from the next command and updates the number 
			historyArray[i] = historyArray[i + 1];
			historyArray[i].commandNo = i + 1;
		}
		//adds the new command to the last entry (19)
		historyArray[19].commandNo = counter;
		strcpy(historyArray[19].command, input);
	} else {
		//if it's not full it can just add the new command and increment counter
		historyArray[counter].commandNo = counter + 1;
		strcpy(historyArray[counter].command, input);
		counter++;
	}
}

//handles the !! command
void prevHistoryHandle(char* path) {

	//temporary array for passing into aliasHandle
	char* tInput[50];
	
	//if the counter is 0 then there is no previous command so it prints an error 
	if (counter != 0) {
		//we need to store the command and parameters in a seperate variable so they don't get deleted
		char inputCopy[512];
		//copies the command into the inputCopy array
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
		//returns 1 if the element is NOT empty
		return 1;
	}
	//otherwise returns 0
	return 0;
}

//prints the history and parameters alongside its command number
void printHistory() {

	if (counter != 0) {
		//prints the current list of list of commands stored in history
		for (int i = 0; i < 20; i++) {
			//if the only character present is a null terminator then it won't print anything
			if (histEmpty(i) == 1) {
				//prints a number alongside the command and inserts a '/0' for formatting
				printf("%d %s%c", historyArray[i].commandNo, historyArray[i].command, '\0');
			}
		}
	//prints an error if there are no commands stored
	} else {
		printf("-> Error! There were no previous commands to print");
	}
}


//executes the specified history command
void historyHandle(int i, char* path) {

	if (i > 0 && i < 21) {
		if(histEmpty(i - 1) == 1) {
			// temporary variable used to store the original command so that the parameters are not lost when tokenised
			char inputCopy[512];
			strcpy(inputCopy, historyArray[(i - 1)].command);
			char* tInput[50];
			//tokenises the command and stores it into tInput
			tokenize(historyArray[i - 1].command,tInput);
			//handles any aliases
			aliasHandle(tInput, path);
			//makes sure the original command doesn't change in history and pushes the invoked command
			strcpy(historyArray[(i - 1)].command, inputCopy);
		//prints an error if there's no command stores at the specified location
		} else {
			printf("-> Sorry, no command stored at [%d].",i);
		}
	//prints an error if the element is outwith the range
	} else {
		printf("-> Sorry, command not found. Use a parameter between -20 and 20 except 0");
	}
}

//process function takes the tokenised input, creates process, and executes it
void process(char ** token) {

	//declare pid and create child process
	pid_t pid = fork();

	//if we can't create child process failed
	if(pid < 0) {
		//prints an error message
		perror("-> failed to create process");
		return;
		//if we can create a child process
	} else if (pid == 0) {
		//execute the cmd with it's arg, then check for errors
		if(execvp(token[0],token) < 0) {		
			//prints a message if an error occured
			fprintf(stderr,"-> [%s]: ", token[0]);
			perror("");
			exit(1);
		}
	} else {	
		//if no error occured, we make the child process to wait (which is the end of that child process)
		wait(NULL);
		return;
	}
}

//deals with the user's input
void input(char input[512], char* path) {

	int c = 0;

	//prints for the interface
	currentDir();
	printf("+ ");
	
	//Control D
	fgets(input, 512, stdin);
	c++;

	if (feof(stdin)) {	
		//sets the directory to home before saving the history and alias
		chdir(getenv("HOME"));
		saveHistory();
            	saveAlias();
            	//restore the original path before closing
		setenv("PATH", path, 1);
		currentPath();
		printf("\n-> exit shell \n");
		exit(0);
	}

	//doesn't add to the history if it's either an invocation, calling history, or just 'enter' is pressed
	
	
	if((strchr(input, '!') == NULL) && (strcmp(input, "history\n") != 0) && (strcmp(input, "\n") != 0)) {
	
	
		
		
		pushHistory(input);
	}	
}

//tokenises the user's input
void tokenize(char* input, char* arrToken[50]) {

	//sets a temporary variable for looping
	int i = 0;

	//tokenises the input based on delimiters
	arrToken[0] = strtok(input, " \t><&;\n|");

	//loops until it reaches the end of the string
	while(arrToken[i] != NULL){
		i++;
		arrToken[i] = strtok(NULL, " \t><&;\n|");
	}
}

//gets and prints the current path
void currentPath() {

	char* savedPath = getenv("PATH");
	printf("%s\n", savedPath);
	
}

//sets a new path based on the user's input
void setPath(char** token) {

	//prints an error if there is no address
	if (token[1] == NULL) {
		printf("-> path could not be set: no address");
	//prints an error if there is more than one path
	} else if (token[2] != NULL) {
		printf("-> you can only enter one path");
	//prints an error if the path couldn't be set
	} else if (setenv("PATH", token[1], 1) != 0) {
		printf("-> path could not be set: bad address");
	} else {
		currentPath();
	}
}

//saves the current history to a .hist_list
void saveHistory() {

	//opens the .hist_file for writing
	FILE *fptr;
	fptr = fopen(".hist_list", "w");
	//doesn't run the rest of the method if the file cannot be found
	if (fptr == NULL) {
		return;
	}
	//loops for the maximum number of elements possible in historyArray
	for (int i = 0; i < 20; i++) {
		// in the first instance it finds an empty command it will stop running the method 
		if (histEmpty(i) == 0) {
			break;
		}
		//puts the command into the next line of fptr
		fputs(historyArray[i].command, fptr);
	}
	//closes the file
	fclose(fptr);
}

void loadHistory() {

	//opens the .hist_file for reading
	FILE *fptr;
	fptr = fopen(".hist_list", "r");
	//temportary variable for reading
	char commandStr[512]; 
	//doesn't run the rest of the method if the file cannot be found
	if (fptr == NULL) {
		return;
	}
	//loops for the maximum number of elements possible in historyArray
	for (int i = 0; i < 20; i++) {
		// pushes the command it read as long as there was something there
		if (fgets(commandStr, 512, fptr) != NULL) {
			pushHistory(commandStr);
		}
	}
	//closes the file
	fclose(fptr);
}

//function to add a new alias
void addAlias(char* newName, char newCommand[]) {

	//sets a temporary variable for positions
	int placePosition = -1;

	//if the alias list is empty, the position can be added the start
	if (aliasCount == 0) {
		placePosition = 0;
	} else {
		//loops for the number of aliases
		for (int i = 0; i < aliasCount; i++) {
			//if we reach the last element of the array and the names aren't equal, an error's printed
			if (i == 9 && strcmp(aliases[i].name, newName) != 0) {
				printf("-> max number of aliases reached - cannot add any more");
				return;
			//if there's identical names, the original is overwritten
			} else if (strcmp(aliases[i].name, newName) == 0) {
				strcpy(aliases[i].command, newCommand);
				printf("-> alias command has been overriden");
				return;
			}
			placePosition = aliasCount;
		}
	}    
	//if there is a location for the new alias, it gets added
	if (placePosition > -1) {
		aliases[placePosition].name = malloc(strlen(newName) + 1);
		strcpy(aliases[placePosition].name, newName);	
		aliases[placePosition].command = malloc(strlen(newCommand) + 1);
		strcpy(aliases[placePosition].command, newCommand);		
		aliasCount++;
	}
}

//funtion to remove an aliased command from the list
void removeAlias(char removeName[]) {

	//temporary variable for printing
	int removed = 0;
	//loops for the number of aliases
	for (int i = 0; i < aliasCount && removed == 0; i++) {
		//if there's only one alias and the names match, it sets the element to null
		if (strcmp(aliases[i].name, removeName) == 0 && aliasCount == 1) {
			aliases[i].name = NULL;
			aliases[i].command = NULL;
			removed = 1;
		//otherwise if the names match then all the elements shift left
		} else if (strcmp(aliases[i].name, removeName) == 0) {		
			for (int j = i; j < aliasCount; j++) {
				//removes the last element of the array
				if (j == aliasCount - 1) {
					aliases[j].name = NULL;
					aliases[j].command = NULL;
				}
				//otherwise copies the elements of the next array
				else {
					strcpy(aliases[j].name, aliases[j+1].name);
					strcpy(aliases[j].command, aliases[j+1].command);					
				}
			}
			removed = 1;
			break;			
		}
	}
	
	//prints an error if the alias didn't exist
	if (removed == 0) {
		printf("-> alias [%s] does not exist.", removeName);
	} else {
        aliasCount--;
		printf("-> alias [%s] was removed.", removeName);
	}
}

//Function to print all currently set aliases to the screen
void printAlias(){
	//If there are no aliases set, display a message
	if (aliasCount == 0) {
		printf("-> There are no aliases set");
	//Otherwise, loop through all the currently set aliases
	} else {
	//While set aliases are detected, print the alias name to the screen.
		for (int i = 0; i < 10; i++) {
			if (aliases[i].name != NULL) {
				printf("-> alias %s = '%s'\n", aliases[i].name, aliases[i].command);
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
	numberOfExecutions++;
	if(numberOfExecutions <=3){
	cmdHandle(tokens, path);
	}
	else{
	printf("%s", "Maximum number of errors");
	numberOfExecutions = 0;
	}
       
}

void saveAlias(){

	//open .aliases to write.
	FILE *fptr;
    	fptr = fopen(".aliases","w");

	//do nothing if the file can't be found
	if(fptr == NULL){
		return;
	}

	for (int i = 0; i < 10; i++) {
		if (aliases[i].name == NULL && aliases[i].command == NULL) {
			break;
		}
		fprintf(fptr,"%s %s\n", aliases[i].name, aliases[i].command);
	}
	fclose(fptr);
}

void loadAlias() {

	//opens the file for reading
	FILE *fptr;
	fptr =  fopen(".aliases","r");  
	//temporary arrays for passing
	char wholeStr[512];
	char* tokens[50];  

	//prints an error if the file can't be opened
	if (fptr == NULL) {
		printf("-> The file does not exist - no aliases.");
		return;
	}

	for (int i = 0; i < 10; i++) {
		//initialising a temporary array
		char aliasStr[512] = {0};
		//tokenises if the line is not null
		if (fgets(wholeStr, 512, fptr) != NULL) {
			tokenize(wholeStr,tokens);
			//concatenates a string until there is nothing left 
			if (tokens[1] != NULL && tokens[0] != NULL) {
				for(int i = 1; i < 20; i++) {
					if(tokens[i] == NULL) {
					
						break;
					}
					//concatenates with an additional space
					strcat(strcat(aliasStr, " "), tokens[i]);
				}
				//eliminates ant empty space and passes it
				char* passArray = aliasStr;
				passArray++;
				addAlias(tokens[0],passArray);
			}
		}
	}
	//closes the file
	fclose(fptr);
}



