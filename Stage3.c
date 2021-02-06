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

    //Save original directory
    char* originalPath = getenv("PATH");

    //Set current directory to HOME
    chdir(getenv("HOME"));

    // propomt init message 
    printf("Welcome to CS210 Group Shell Project \n");

    // propomt loop while true until exit exit(0)
    while (1){

        // take input and handle the cmd
        cmdHandle(part1());
        


    }
        //Restore original directory
        setenv("PATH", originalPath, 1);

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
void cmdHandle (char** tokens){

    char* cmd =  *tokens;
    int cmdID = 0;
    char* para = tokens[1];
    char* cmdList[7];

    //list of possible input, subject to change
    cmdList[0] = "exit";
    cmdList[1] = "ls";
    cmdList[2] = "cd";
    cmdList[3] = "help";
    cmdList[4] = "pwd";
    cmdList[5] = "getpath";
    cmdList[6] = "setpath";

    //for loop to check for which command
    for (int i = 0;  i < 7; i++){
        if (strcmp(cmd, cmdList[i])==0||strstr(cmd, "./")!=NULL){
            cmdID = i + 1;
            break;
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

            
            //run chdir function and check for return  
            if(chdir(para)!=0){

                //failed - print error statement
                perror ("-> cd cmd has failed");
            }else{

                // worked print the currnt working directory
                currentDir();
            }
        
        break;

        //help just for testing, can be change to something more useful
        case 4 :

            printf("-> SOS");
        
        break;

        case 5:

            process(tokens);

        break;

        //if none of these cmd has been entered then print error statement
        default:
            printf("[Sorry this command can not be recognised.] \n[Please check your input.]\n");


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

// ./stage1 sule jack mic scot mark \0


void extProcess(const char **tokens){

    

    pid_t pid = fork();

    if (pid < 0){
        perror("failed to create processssssssssss");
    }else if(pid == 0){

        if(execve(tokens[0],(char**)tokens, NULL)<0){

            perror("-> failed to excute");
        }
    }else{

        wait(NULL);
        return;
    }

}
	
	
	
	
char** part1(){

    char input[512];
    char** arrToken = malloc(50 * sizeof(char*));
    int i = 0;
	
	
    currentDir();
    printf("+ ");
			
	//Control D
	fgets(input, 512, stdin); 

	if(feof(stdin)){

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

char* getPath() {

}

void setPath() {

}