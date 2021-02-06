#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

int main (){
    
    char * tokens[] = {"./stage1",NULL}; 

    pid_t pid = fork();

    if (pid < 0){
        perror("failed to create processssssssssss");
    }else if(pid == 0){

        if(execve(tokens[0],(char**)tokens, NULL)<0){

            perror("-> failed to excute");
        }
    }else{

        wait(NULL);
        return 0;
    }

    return 0;

}