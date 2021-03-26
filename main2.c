//Group 8 - Jackson Blair, Mark McCaffetary, Scott Love, Suleman Ahkter, Nuoxu Li

//Implement functions for input and output
#include <stdio.h>
//Implement functions involving strings 
#include <string.h>
//Implement functions
#include <unistd.h>
//Implement functions
#include <limits.h>
//Implement functions
#include <stdlib.h>
//Implement functions
#include <signal.h>
//Implement functions
#include <sys/types.h>
//Implement functions
#include <sys/wait.h>

//Create a structure storing a history 
struct history {
	//this struct stores the string command
	char command[512];
    //this struct stores an integer for easy printing
	int commandNo;
};

//Create a structure that holds the contents of one alias
struct alias {
    //this struct stores the name of the alias
	char* name;
    //this struct stores the command returned by the alias
	char* command;
};

//Store the current number of history commands
int historyCount = 0;
//Store a set of history commands
struct history historyArray[20];
//Store the current number of aliases
int aliasCount = 0;
//Store a set of aliases
struct alias aliases[10];
//Store the number of executions
int numberOfExecutions = 0;

//define the return type and names of methods before implementing them so further code is aware of what the function does
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

//Main method - executed as soon as the program is run
int main(void) {

	//saving the path into a variable
	char savedPath[512];
    //Copy the current path into the variable so it is used later
	strcpy(savedPath,getenv("PATH"));
    //Variable that holds the string input by the user
	char userInput[512];
    //Variable that holds the indivdual words set as the command from the user input delimited by spaces
	char* token[50];
	
	//set current directory to home
	chdir(getenv("HOME"));

	//prompt init message
	printf("Welcome to CS210 Group Shell Project \n");

    	//tries to load the saved aliases into array aliases
        loadAlias();
	
	//prompt loop while true until exit exit(0)
	while (1) {
	
		//takes input, making reference to where the user saved the path	
		input(userInput, savedPath);

		//won't do anything if the user only inputs <enter>
		if (strcmp(userInput, "\n") != 0) {
			
			//tokenize the input
			tokenize(userInput, token);
            //reset the number of executions by the user
			numberOfExecutions = 0;
			
            //Add white space between this code and later code
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
    
    //Set the initial directory to the root directory of the user
	char* user = getenv("USER");
	
    //Store the current working directory
	char cwd[PATH_MAX];
	
	//Get the working directory
	getcwd(cwd,sizeof(cwd));
	
	//print to prompt the user's root name and the working directory
	printf("\n%s: %s ", user,cwd);
}

//Handle the user commands and decide the process to execute
void cmdHandle (char* tokens[50], char* path) {
	
    //The command is set to the first token by the user
	char* cmd = tokens[0];
    //The integer holding the type of command to execute
	int cmdID = 0;
    //The parameters are set to the 2nd token onwards
	char* para = tokens[1];
    //Declare the list of commands
	char* cmdList[11];
	
	//list of possible commands defined by input
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
        //If this command is equal to the command from the list, set id to execute this relevant command and stop looping.
		if (strcmp(cmd, cmdList[i]) == 0) {
			cmdID = i + 1;
			break;
		}
	}

    //Execute a process depending on what the identified is from user input
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
            //Exits the program
			exit(0);
        //exit the swtich statement
		break;
		
		//executes the specified previous history command i.e. ! 2
		case 2:;
			
            //Hold the positive position of the tokens
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
                //Increment this command to the list of history commands
				historyHandle(posPosition, path);
			}
		//exit the swtich statement
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
                    //Increment this command to the list of history commands
					historyHandle(negPosition, path); 
		    	}
			}
		//exit the swtich statement
		break;
		
		//executes the previous history command (!!)
		case 4:
			//prints an error if there's an argument
			if(para != NULL) {
				printf("-> error! this command cannot take any arguments");
			} else {
			    //execute the previous history argument   
				prevHistoryHandle(path);
				
				
			}
		//exit the swtich statement
		break;

		//cd change directory
		case 5:

			//if there's no parameter it goes back to the home directory
			if (para == NULL) {
			
				printf("-> back to home directory");
				//cd back to users home dir
				chdir(getenv("HOME"));

			//run chdir function and check for return. If it does not exist then show error message.
			} else if(tokens[2] != 0) {

				printf("-> you can only enter one path");
            //If the directory returns 0, there is an error
			} else if(chdir(para) != 0){

				//failed - print error statement
				fprintf(stderr,"-> failed to cd, an error occured with [%s]: ", para); 
                //Display error of contents, though no such error files exist
				perror("");

			} else {

				//prints the current working directory
				currentDir();
			}

		//exit the swtich statement
		break;

		//show a list of commands executable by the user.
		case 6:
            //Print history list involving the following contents:
			printf("cd - change working directory\n");
            printf("getpath - print system path\n");
            printf("setPath - set system path\n");
            printf("history - print history contents (numbered list of commands in history including their parameters in ascending order from least to most recent\n");
            printf("!! – invoke the last command from history (e.g. if your last command was ‘ls –lF’ then ‘!!’ should execute ‘ls –lF’)\n");
            printf("!<no> - invoke command with number <no> from history (e.g. !5 will execute the command with number 5 from the history)\n"):
            printf("!-<no> - invoke the command with number the number of the current command minus <no> (e.g. !-3 if the current command number is 5 will execute the command with number 2, or !-1 will execute again the last command)\n");
            printf("alias – print all set aliases (alias plus aliased command)\n");
            printf("alias <name> <command> - alias name to be the command. Note that the command may also include any number of parameters, while (any number of) command parameters should work correctly with aliasing (e.g. if I alias la to be ls –la then when I type la . the shell should execute ls –la .). Note also that aliasing should also work correctly with history (e.g. !5 will execute the command with number 5 from the history if this command is an alias like the la above then !5 will execute ls –la). In the enhanced form of the alias, it should be possible to alias history invocations (e.g. if I alias ‘five’ to be !5 then when I type ‘five’ the shell should execute the command with number 5 from the history). It should also be possible to alias aliases (e.g. If I alias l to be ls and then I alias la to be l –a, then when I type la the shell should execute ls –a).\n");
            printf("unalias <command> - remove any associated alias\n");
		//exit the swtich statement
		break;

		//prints the current path
		case 7:

			//prints an error if there's a parameter
			if (para != NULL) {
				printf("-> you don't need to enter a path");
            //Otherwise, find the current path
			} else {
				currentPath();
			}

		//exit the swtich statement
		break;

		//sets the shell's path
		case 8:

            //Set the path based on the existing tokens
			setPath(tokens);

		//exit the swtich statement
		break;

		//prints the history
		case 9:

			//prints an error if there are any arguments
			if (para != NULL) {
				printf("-> error! this command cannot take any arguments");
			} else {
				printHistory();
			}

		//exit the swtich statement
		break;

		//alias (either prints all of the aliases, or adds a new one)
		case 10:;

			//sets a temporary integer for later printing
			int oldCount =  aliasCount;
			//if there are no parameters then the aliases are printed
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

		//exit the swtich statement
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

		//exit the swtich statement
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

//check if the element is empty
int histEmpty (int index) {

    //returns 1 if the element is NOT empty
	if (historyArray[index].command[0] != '\0') {
		return 1;
	}
	//otherwise, return 0
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
            //Unsuccessful termination of program
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

    //Start holding command counter
	int c = 0;

	//prints for the interface
	currentDir();
    //Seperate prints via plus symbols
	printf("+ ");
	
	//Take the input from the user in a 
	fgets(input, 512, stdin);
    //Increment command counter
	c++;

    //If t
	if (feof(stdin)) {	
		//sets the directory to home before saving the history and alias
		chdir(getenv("HOME"));
		saveHistory();
            	saveAlias();
            	//restore the original path before closing
		setenv("PATH", path, 1);
		currentPath();
		printf("\n-> exit shell \n");
        //Successful program termination
		exit(0);
	}

	//doesn't add to the history if it's either an invocation, calling history, or just 'enter' is pressed
	if((strchr(input, '!') == NULL) && (strcmp(input, "history\n") != 0) && (strcmp(input, "\n") != 0)) {
        //Increment this command to the list of history commands
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

    //Set the string to the current path name
	char* savedPath = getenv("PATH");
    //Display the path name
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

//load the history commands from a file
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
            //The place position should now be set to the alias count
			placePosition = aliasCount;
		}
	}    
	//if there is a location for the new alias, it gets added, with the alias name and command being inserted to the new location of the array
	if (placePosition > -1) {
		aliases[placePosition].name = malloc(strlen(newName) + 1);
		strcpy(aliases[placePosition].name, newName);
		aliases[placePosition].command = malloc(strlen(newCommand) + 1);
		strcpy(aliases[placePosition].command, newCommand);	
        //As an alias is added, increment the counter	
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
    //Initialise new input and position variables
	char newInput[512] = {0};
	int position = 0;
    
    //For all aliases, check if the new alias matches the alias name, and if it does, insert the new alias name.
	for (int i = 0; i < aliasCount; i++) {
		if (strcmp(tokens[0], aliases[i].name) == 0) {
			strcpy(newInput, aliases[i].command);
			position = 1;
			break;
		}
	}
    //for all remaining tokens, add it to the command of the alias.
	while (tokens[position] != NULL) {
		strcat(strcat(newInput, " "), tokens[position]);
		position++;
	}
	
    //tokenize the input commands
	tokenize(newInput, tokens);
    //Increment the number of executions
	numberOfExecutions++;
    //If there are less than 4 executions, handle the command path
	if(numberOfExecutions <=3){
	    cmdHandle(tokens, path);
	}
	else{
        //Otherwise, print the number of errors and reset the number of executions
	    printf("%s", "Maximum number of errors");
	    numberOfExecutions = 0;
	}
       
}

//save the list of aliases from an external file
void saveAlias(){

	//open .aliases to write.
	FILE *fptr;
    	fptr = fopen(".aliases","w");

	//do nothing if the file can't be found
	if(fptr == NULL){
		return;
	}

    //For all aliases that are not null, write to the file
	for (int i = 0; i < 10; i++) {
		if (aliases[i].name == NULL && aliases[i].command == NULL) {
			break;
		}
		fprintf(fptr,"%s %s\n", aliases[i].name, aliases[i].command);
	}
	fclose(fptr);
}

//load a list of aliases from an external file
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



