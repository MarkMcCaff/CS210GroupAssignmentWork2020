//
//  main.c
//  test
//
//  Created by SULEMAN AKHTER on 25/02/2021.
//  Modified by Michael Li 
//  error exists - temp bandaid is do not delete or modifie line 51 ans 233  
//

#include "header.h"

char* history [20];

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
void loadHistory();
void saveHistory();

    
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
        // DO NOT EDIT OR DELETE THIS LINE FOR NOW
        printf("%s\n", tokenizedInput[0]);
       

        //handling the tokenized input
        cmdHandle(tokenizedInput,savedPath);
                                                                                                                                                                                                                         
    }
    
    //return main
    return 0;
        
}

//print the work directory
void currentDir(){

    char* user = getenv("USER");

    char cwd[512];
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
    cmdList[2] = "cd";
    cmdList[3] = "help";
    cmdList[4] = "getpath";
    cmdList[5] = "setpath";
    cmdList[6] = "save";
    cmdList[7] = "load";
    
    
    //for loop to check for which command
    for (int i = 0;  i < 8; i++){
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

        //saveHistroy
        case 7:

            saveHistory();
        break;
        //loadHistory
        case 8:

            loadHistory();
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
                //printf("-> current cmd is %s \n", history[i]);
                //printf("-> current cmd pos is %d \n", i);
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
        cmdHistory(input);
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

void saveHistory(){
    FILE *fPtr;
    fPtr = fopen("//home//tmb19188//Documents//Project//CMDHistory.txt", "w+");
    if(fPtr == NULL)
    {
        //File does not exist
        printf("Unable to create requested file.\n");
        fflush(stdout);
        exit(1);
    }
        for(int i =0; i < 20 ;i++){
            fprintf(fPtr, "%s \n" ,history[i]);
        }
        
        printf("Command History Saved.\n");
        fflush(stdout);
    fclose(fPtr);

}

void loadHistory(){
    FILE *fPtr;
    fPtr = fopen("//home//tmb19188//Documents//Project//CMDHistory.txt", "r");
    if(fPtr == NULL)
    {
        //File does not exist
        printf("Unable to create requested file.\n");
        fflush(stdout);
        exit(1);
    }
    for(int i =0; i < 20 ;i++){
            fscanf(fPtr, "%s" ,history[i]);
        fflush(stdout);
        }

        printf("Command History Read.\n");
        fflush(stdout);
    fclose(fPtr);
}