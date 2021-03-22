#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

// structure for the history storage
struct history {
    // this struct stores the string command and also an integer for easy printing 
    char command[512];
    int commandNo;
};

    //commandNo = count + 1
    int counter = 0;
    struct history historyArray[20];

//C function interface
void currentDir();
void cmdHandle();
char* input();
char** tokenize();
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

int main(void){

    // saving the path into a variable
    char savedPath[512];
    
    strcpy(savedPath,getenv("PATH"));

    // set current directory to home
    chdir(getenv("HOME"));

    // tries to load the previously saved history
    loadHistory();

    // prompt init message
    printf("Welcome to CS210 Group Shell Project \n");

    // prompt loop while true until exit exit(0)
    while (1){

        // take input 
        char * userInput = input(savedPath);
        //printf("%s\n", userInput);

        //tokenize the input 
        char** tokenizedInput = tokenize(userInput);
        
        printf("\n");

        //handling the tokenized input
        cmdHandle(tokenizedInput,savedPath);
                                                                                                                                                                                                          
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
void cmdHandle (char** tokens, char* path){

    char* cmd =  *tokens;
    int cmdID = 0;
    char* para = tokens[1];
    char* cmdList[8];
    
    //list of possible input, subject to change
   
    cmdList[0] = "exit";
    cmdList[1] = "!";
    cmdList[2] = "!!";
    cmdList[3] = "cd";
    cmdList[4] = "help";
    cmdList[5] = "getpath";
    cmdList[6] = "setpath";
    cmdList[7] = "history";
    
    //for loop to check for which command
    for (int i = 0;  i < 8; i++){
        if (strcmp(cmd, cmdList[i]) == 0){
            cmdID = i + 1;
            break;
        }
    }

    switch(cmdID){

        // exit shell terminates
        case 1 :
            // restore the original path before closing
            setenv("PATH", path, 1);
            currentPath();
            // save the current history before closing
            printf("-> exit shell \n");
            saveHistory();
            exit(0);
        break;
        
        // executes the specified previous history command i.e ! 2
        case 2:
            // prints an error if there's more than one parameter 
            if (tokens[2] != NULL) {
                 printf("-> you can only enter one argument");
            } else {
                historyHandle(para,path); 
            }
        break; 
        
        // executes the previous history command (!!) 
        case 3:
            // prints an error if there's an arguments  
            if(para != NULL) {
                printf("-> error! this command cannot take any arguments");
            } else {
                prevHistoryHandle(path); 
            }          
        break;
            
        // cd change directry
        case 4 :
             
            if (para==NULL){

                printf("-> back to home dirctory");
                //cd back to users home dir
                chdir(getenv("HOME"));
            
            //run chdir function and check for return
            }else if(tokens[2]!=0){

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

        //help just for testing, can be change to something more useful (given even more testing)
        case 5:

     	    printf("SOS");
        
        break;

        //getpath
        case 6:

            if(para!= NULL){
                printf("-> you don't need to enter a path");
            }else{
                currentPath();
            }

        break;

        //setpath
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

        //if none of these cmd has been entered then print error statement
        default:
            process(tokens);
            //printf("[Sorry this command can not be recognised.] \n[Please check your input.]\n");
    }
}

// adds the command to the history array
void pushHistory(char* input){
    // if the array is full then everything shifts to the left, deleting the earlier command
    if (counter == 20) {
        for (int i = 0; i < 19; i++) {
            // copies the elements from the next command over and updates the number
            historyArray[i] = historyArray[i + 1];
            historyArray[i].commandNo = i + 1; 
        }
        // add the new command to the last entry (19)
        historyArray[19].commandNo = counter; 
        strcpy(historyArray[19].command, input);
     }
     else {
        // if it's not full it can just add the new command and increment counter
        historyArray[counter].commandNo = counter + 1;
        strcpy(historyArray[counter].command, input);
        counter++;
    }
}

// handles the !! command
void prevHistoryHandle(char* path) {
    char** tInput;
    // If the counter is 0 then there is no previous command so it prints an error
    if (counter != 0) {
            //because we're using pointers we need to store the command and parameters in a seperate variable so they dont get deleted 
            char inputCopy[512];
            strcpy(inputCopy, historyArray[(counter - 1)].command);
            // tokenises and executes the command 
            tInput = tokenize(historyArray[counter - 1].command);
            printf("%s\n", tInput[0]);
            cmdHandle(tInput,path);
            
            //makes sure the original command doesnt't change in history and pushes the invoked command
            strcpy(historyArray[(counter - 1)].command, inputCopy);
            
    } else {
            printf("-> Error! There is no command stored at this element in history.");
           }
}

// this was used mostly for a circular array (checks if the element is empty)
int histEmpty (int index) {
    if (historyArray[index].command[0] != '\0') {
        // returns 1 if the element is NOT empty
        return 1;
    }
    // otherwise returns 0
    return 0;
}

// prints the history and parameters alongside its command number 
void printHistory() {
    if (counter != 0) {
        // prints the current list of commands stored in history 
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

void historyHandle(char* para, char* path){

    int i = 0;
    i = atoi(para);
    
    // if a negative number if given then it tries to convert it into a valid command
    if (i < 0) {
    // uses counter + 1 so that the last command can be an option
        i = ((counter + 1) - abs(i));
    }
    
    if(i > 0 && i < 21){
       
        if(histEmpty(i -1) == 1){

            // temporary variable used to store the original command so that the parameters are not lost when tokenised 
	    char inputCopy[512];
            strcpy(inputCopy, historyArray[(i - 1)].command);

            char** tInput = tokenize(historyArray[(i - 1)].command);
            
            //DO NOT DELETE THIS LINE CODE IS BASED ON THIS TO RUN
            printf("%s\n", tInput[0]);

            //handle the tokenized input
            cmdHandle(tInput,path);
            
            //makes sure the original command doesnt't change in history and pushes the invoked command
            strcpy(historyArray[(i - 1)].command, inputCopy);

        }else{
            printf("-> Sorry, no command stored at [%d].",i);
        }
                
    }else{
        printf("-> Sorry, command not found. Use a parameter between -20 and 20 except 0");
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
            fprintf(stderr,"-> [%s]: ", token[0]);
            perror ("");

            
            exit(1);
        }
        //kill the process
        //exit(EXIT_FAILURE);
    }else{
        
        //no error occured put child process to wait (that is the end od that child process)
        wait(NULL);
        return;
    }

}
    
char* input(char* path){

    char* input;
    input = (char*)malloc(512);
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
        saveHistory();
        printf("\n-> exit shell \n");
        exit(0);
            
    //Will break when u type "exit"
    }
    
    // doesn't add to the history if it's either an invocation, calling history, or just 'enter' is pressed
    if((strchr(input, '!') == NULL) && (strcmp(input, "history\n") != 0) && (strcmp(input, "\n") != 0)){
        pushHistory(input);
    }

    return input;
    
}

char** tokenize(char* input){

    int i = 0;
    char** arrToken = malloc(50 * sizeof(char*));

    strtok(input, "\n");
    
        char* token = strtok (input," \t,|><&;");

        while(token != NULL){
                //printf ("%s\n",token);Just used for testing purposes,
                
                arrToken[i] = token;

                token = strtok (NULL, " \t,|><&;");

                i++;
        }
        
    return arrToken;

}

void currentPath() {

    char* savedPath = getenv("PATH");
    printf("%s\n", savedPath);

}

void setPath(char** token) {

    if (token[1] == NULL) {
        printf("-> path could not be set: no address");
    } else if (token[2] != NULL){
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
