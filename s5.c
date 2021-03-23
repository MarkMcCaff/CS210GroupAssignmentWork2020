//
//  main.c
//  test
//
//  Created by SULEMAN AKHTER on 25/02/2021.
//

#include "header.h"

char* history [20];

//C function interface
void currentDir();
void cmdHandle();
char* input();
void tokenize();
void process();
void extProcess();
void currentPath();
void setPath();
void cmdHistory();
void historyHandle();

    
int main(void){


    char* arrToken [50];
    //char* tokenizedInput [50];
    
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
        tokenize(userInput, arrToken);
        // DO NOT EDIT OR DELETE THIS LINE FOR NOW
        //printf("%s\n", tokenizedInput[0]);
       

        //handling the tokenized input
        cmdHandle(arrToken,savedPath);
                                                                                                                                                                                                                         
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
void cmdHandle (char* tokens[], char* path){

    char* cmd =  tokens[0];
    int cmdID = 0;
    char* para = tokens[1];
    char* cmdList[6];
    
    
    
    

    //list of possible input, subject to change
   
    cmdList[0] = "exit";
    cmdList[1] = "!";
    cmdList[2] = "cd";
    cmdList[3] = "help";
    cmdList[4] = "getpath";
    cmdList[5] = "setpath";
    
    
    //for loop to check for which command
    for (int i = 0;  i < 7; i++){
        if (strcmp(cmd, cmdList[i])==0){
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
            
            
            
        // cd change directry
        case 3 :

            

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

        //help just for testing, can be change to something more useful
        case 4:

            

            printf("-> SOS");
        
        break;

        //getpath
        case 5:

            if(para!= NULL){
                printf("-> you don't need to enter a path");
            }else{
                currentPath();
            }

        break;

        //setpath
        case 6:

            

            setPath(tokens);
        
        break;
            
    

        //if none of these cmd has been entered then print error statement
        default:
            process(tokens);
            //printf("[Sorry this command can not be recognised.] \n[Please check your input.]\n");


    }
}

void cmdHistory(char* input){

        for(int i = 0; i<20 ; i++){
        
            if (history[i] == NULL){
                history[i] = input;
                break;
            }
            
            
        }
        
        

}

void historyHandle(char* para, char* path){

    printf("%s\n",para);

    int  i = 0;
    i = atoi(para);
    
    if(i >0 && i < 21){
       
        if(history[i - 1] != NULL){

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
void process(char * token[]){

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


    
char[] input(char* path){

    char input[512];
    //input = (char*)malloc(512);
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
        cmdHistory(input);
    }

    return input;
    
}

void  tokenize(char input[], char* arrToken[]){

    int i = 0;
    //char* arrToken [50];

    strtok(input, "\n");
    
        char* token = strtok (input," \t,|><&;");

        while(token != NULL){
                //printf ("%s\n",token);Just used for testing purposes,
                
                arrToken[i] = malloc(strlen(token));

                token = strtok (NULL, " \t,|><&;");

                i++;
        }

    

}

void currentPath() {

    char* savedPath = getenv("PATH");
    printf("%s\n", savedPath);

}

void setPath(char* token[]){

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