#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

char history[20][512];
int counter = 0;

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
int main(void){

    // saving the path into a variable
    char savedPath[512];
    
    strcpy(savedPath,getenv("PATH"));

    // set current directory to home
    chdir(getenv("HOME"));

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

        //exit shell terminates
        case 1 :
            // restore the original path before closing
            setenv("PATH", path, 1);
            currentPath();
            printf("-> exit shell \n");
            exit(0);
        break;
        
        case 2:
        
            historyHandle(para,path); 
            
        break; 
        
        case 3:
         
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

     	for (int i = 0; i < 20; i++) {
     		printf("element %d: %s", i, history[i]);
     	}
        
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

void pushHistory(char* input){

        strcpy(history[counter], input);
	counter = (counter + 1) % 20;
        //printf("-> current cmd is %s \n", history[i]);
        //printf("-> current cmd pos is %d \n", i);
               
}

void prevHistoryHandle(char* path) {

    // If the counter is 0 then the previous command must be stored at index 19 as it recently reset
    if (counter == 0) {
         // if the chosen history command is empty then it needs to print an error message 
         if (histEmpty(19) == 1) {
             // tokenises the 19th command and then executes it
             char** tInput = tokenize(history[19]);
             printf("%s\n", tInput[0]);
             cmdHandle(tInput,path);
         } else {
            // prints a number alongside the command and inserts a '/0' for formatting
            printf("Error! There is no command stored at this element in history.");
        }      
             
    }
    // If it's not 0 then the index will be counter - 1 as counter always increments after pushing
    else {
         // if the chosen history command is empty then it needs to print an error message 
         if (histEmpty(counter - 1) == 1) {
         // tokenises the specific command at counter - 1 (because of previous increments) and executes
         char** tInput = tokenize(history[counter - 1]);
         printf("%s\n", tInput[0]);
         cmdHandle(tInput,path);
         } else {
            // prints a number alongside the command and inserts a '/0' for formatting
            printf("Error! There is no command stored at this element in history.");
        }

    }
}

int histEmpty (int index) {
    if (history[index][0] != '\0') {
        // returns 1 if the element is NOT empty
        return 1;
    }
    // otherwise returns 0
    return 0;
}

void printHistory() {
    // prints the current list of commands stores in history 
    for (int i = 0; i < 20; i++) {
        // if the only character present is a null terminator then it won't print anything
        if (histEmpty(i) == 1) {
            // prints a number alongside the command and inserts a '/0' for formatting
            printf("%d %s%c", (i + 1), history[i], '\0');
        }
    }
}

void historyHandle(char* para, char* path){

    int i = 0;
    i = atoi(para);
    
    if(i > 0 && i < 21){
       
        if(histEmpty(i -1) == 1){

            char** tInput = tokenize(history[i - 1]);

            //DO NOT DELETE THIS LINE CODE IS BASED ON THIS TO RUN
            printf("%s\n", tInput[0]);

            //handling the tokenized input
            cmdHandle(tInput,path);

        }else{
            printf("-> sorry, command not found ");
        }
                
    }else{
        printf("-> Sorry history only stores last 20 command");
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
        printf("\n-> exit shell \n");
        exit(0);
            
    //Will break when u type "exit"
    }

    if(strchr(input, '!') == NULL){
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

void setPath(char** token){

    if (token[1] == NULL) {
        printf("-> path could not be set: no address");
    }else if (token[2] != NULL){
        printf("-> you can only enter one path");
    }else if (setenv("PATH", token[1], 1) != 0) {
        printf("-> path could not be set: bad address");
    }else{
        currentPath();
    }
}
