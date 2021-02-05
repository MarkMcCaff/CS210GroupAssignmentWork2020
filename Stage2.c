#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
	

    void cmdHandle();
	char** part1();
    void process();
	
int main(void){

    printf("Welcome to CS210 Group Shell Project \n");

    while (1){

        cmdHandle(part1());
        


    }
		
		
		
		return 0;
		
}

void cmdHandle (char** tokens){

    char* cmd =  *tokens;
    int cmdID = 0;
    char* para = tokens[1];
    char* cmdList[4];

    cmdList[0] = "exit";
    cmdList[1] = "ls";
    cmdList[2] = "cd";
    cmdList[3] = "help";

    for (int i = 0;  i < 4; i++){
        if (strcmp(cmd, cmdList[i])==0){
            cmdID = i + 1;
            break;
        }
    }


    switch(cmdID){

        case 1 :
            printf(">>> exit shell \n");
            exit(0);
            break;
        
        case 2 :

            //printf(">>>this is ls cmd");
            process(tokens);
        
        break;

        case 3 :

            printf(">>>this is cd cmd");
        
        break;

        case 4 :

            printf(">>>SOS");
        
        break;

        default:
            printf(">>>Sorry this command can not be recognised.<<< \n>>>Please check your input.<<<");


    }
}

void process(char ** token){
    pid_t pid = fork();

    if(pid < 0){
        printf("\nFailed to fork...:(");
        return;
    }else if (pid == 0){
        if(execvp(token[0],token)<0){
            printf("\ncan't excute");
        }
        exit(0);
    }else{
        
        wait(NULL);
        //printf("child done ");
        return;
    }

}
	
	
	
	
char** part1(){

    char input[512];
    char** arrToken = malloc(50 * sizeof(char*));
    int i = 0;
	
	

    printf("\n +");
			
	//Control D
	fgets(input, 512, stdin); 

	if(feof(stdin)){

        printf(">>> exit shell \n");
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
