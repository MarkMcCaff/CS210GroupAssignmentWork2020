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
	
int main(void){

    // saving the path into a variable
    char* savedPath = getenv("PATH");

    // set current directory to home
    chdir(getenv("HOME"));

    // propomt init message 
    printf("Welcome to CS210 Group Shell Project \n");

    //Hold data of previous 20 commands from user
    char** commandHistory = malloc(20 * sizeof(char*));

    // propomt loop while true until exit exit(0)
    while (1){

        // take input and handle the cmd, handling the current commandHistory
        cmdHandle(part1(), commandHistory);

    }

    // restore the original path before closing    
    setenv("PATH", savedPath, 1);
		
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
void cmdHandle (char** tokens, char** commandHistory){

    char* cmd =  *tokens;
    int cmdID = 0;
    char* para = tokens[1];
    char* cmdList[8];

    //list of possible input, subject to change
    cmdList[0] = "exit";
    cmdList[1] = "ls";
    cmdList[2] = "cd";
    cmdList[3] = "help";
    cmdList[4] = "pwd";
    cmdList[5] = "getpath";
    cmdList[6] = "setpath";
    cmdList[7] = "history";

    //for loop to check for which command
    for (int i = 0;  i < 8 i++){
        if (strcmp(cmd, cmdList[i])==0){
            cmdID = i + 1;
            break;
        }else if(strstr(cmd, "./")!=NULL){

            cmdID = 9;
            break;

        } else if (strstr(cmd, "!")!=NULL) {
            cmdID = 10;
        }
    }

    switch(cmdID){

        //exit shell terminates 
        case 1 :
            printf("-> exit shell \n");
            exit(0);
        break;
        
        //ls prcoss cmd
        case 2 :

            //printf("->this is ls cmd");
            process(tokens);
        
        break;

        // cd change directry 
        case 3 :
            if (para==NULL){

                printf("-> back to home dirctory");
                //cd back to users home dir
                chdir(getenv("HOME"));
            
            //run chdir function and check for return  
            }else if(chdir(para)!=0){

                //failed - print error statement
                perror ("-> failed change directory");

            }else{

                // worked print the current working directory
                currentDir();
            }

        
        break;

        //help just for testing, can be change to something more useful
        case 4 :

            printf("-> SOS");
        
        break;

        case 5:

            currentDir();
            //process(tokens);

        break;

        case 6:

            currentPath();

        break;

        case 7:

            getCommandHistory(commandHistory);

        break;

        case 8:

            if (para == NULL) {
                printf("-> path could not be set: no address");            
            }
            else if (setenv("PATH", para, 1) != 0) {
                printf("-> path could not be set: bad address");
            }
        
        break;

        case 9:
        
            extProcess(tokens);
            //printf("external process");
        
        break;

        case 10:

            
        
        break;

        //if none of these cmd has been entered then print error statement
        default:
            printf("[Sorry this command can not be recognised.] \n[Please check your input.]\n");

        //If command not previous command, add this command to our command history.
        if (cmdID != 10)
            addCommandHistory(tokens, commandHistory);
    }
}

//process function take in tokenised input the create process, excute the return the process
void process(char ** token){

    //declare pid and create child process
    pid_t pid = fork();

    //if we can't create child process failed 
    if(pid < 0){
        //print error msg
        perror("-> failed to create process");
        return;

    //if we can and created a child process 
    }else if (pid == 0){
        // excute the cmd with it's arug, then check for errors
        if(execvp(token[0],token)<0){

            //error occured print errMsg
            perror("-> failed to excute");
        }
        //kill the process
        exit(0);
    }else{
        
        //no error occured put child process to wait (that is the end od that child process)
        wait(NULL);
        //printf("child done ");
        return;
    }

}


void extProcess(const char **tokens){

    pid_t pid = fork();

    if (pid < 0){
        perror("failed to create processssssssssss");
    }else if(pid == 0){

        if(execve(tokens[0],(char**)tokens, NULL)<0){

            perror("-> failed to excute");
        }
        exit(0);
    }else{

        wait(NULL);
        return;
    }

}
	
	
char** part1(){

    char input[512];
    char** arrToken = malloc(50 * sizeof(char*));
    int i = 0;
    char** commandHistory = malloc(20 * sizeof(char*));
	
	
    currentDir();
    printf("+ ");
			
	//Control D
	fgets(input, 512, stdin); 

	if(feof(stdin)){

        printf("\n-> exit shell \n");
        exit(0);
    //If something else; execute command typed
    } else {

		strtok(input, "\n");
	
		char* token = strtok (input," \t,|><&;");

        while(token != NULL){
				//printf ("%s\n",token);Just used for testing purposes,
				
                arrToken[i] = token;

                token = strtok (NULL, " \t,|><&;");

                i++;
		}
	}

    return arrToken;	 	
	
}

void currentPath() {

    char* savedPath = getenv("PATH");
    printf("%s\n", savedPath);

}

//Add the user's new command to the command history
char** addCommandHistory(char** commandHistory, char** newLine) {
    //For items 2 to 20, move each item back by 1.
    for (int i = 19; i > 0 i--) {
        commandHistory[i] = commandHistory[i - 1];
    }
    //Set next item to newly added command
    commandHistory[0] = newLine;
}

//Print previous 20 commands from history
void getCommandHistory(char** commandHistory) {
    for (int i = 0; i < 20; i++) {
        printf("%d %s\n",i + 1, commandHistory[i]);
    }
}