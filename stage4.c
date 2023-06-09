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
void currentPath();
void setPath();
	
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

        // take input and handle the cmd
        cmdHandle(part1(savedPath),savedPath);
                                                                                                                                                                                                                         
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
    char* cmdList[6];

    //list of possible input, subject to change
    cmdList[0] = "exit";
    cmdList[1] = "cd";
    cmdList[2] = "help";
    cmdList[3] = "getpath";
    cmdList[4] = "setpath";

    //for loop to check for which command
    for (int i = 0;  i < 6; i++){
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
        
        // cd change directry 
        case 2 :

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
        case 3:

            printf("-> SOS");
        
        break;

        case 4:

            if(para!= NULL){
                printf("-> you don't need to enter a path");
            }else{
                currentPath();
            }

            

        break;

        case 5:

            setPath(tokens);
        
        break;

        //if none of these cmd has been entered then print error statement
        default:

            //printf("->this is ls cmd");
            process(tokens);
            //printf("[Sorry this command can not be recognised.] \n[Please check your input.]\n");


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


	
char** part1(char* path){

    char input[512];
    char** arrToken = malloc(50 * sizeof(char*));
    int i = 0;
	
	
    currentDir();
    printf("+ ");
			
	//Control D
	fgets(input, 512, stdin); 

	if(feof(stdin)){

        // restore the original path before closing   
        setenv("PATH", path, 1);
        currentPath();
        printf("\n-> exit shell \n");
        exit(0);
			
	//Will break when u type "exit"
    }else{

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
