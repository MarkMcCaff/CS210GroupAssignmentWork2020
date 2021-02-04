#include <stdio.h>
#include <string.h>
	
	void part1();
	
int main(void){
		
		
		part1();
		return 0;
		
		}	
	
	
	
	
void part1(){
	
	printf("Welcome to CS210 Group Shell Project \n");
		char input[512];
			
		
		char* pch;
		
		
		while(1){
			
			
			printf("+");
			
			//Control D
			if(!fgets(input, 512, stdin)){
				
			break;
			}
			//Will break when u type "exit"
			else if(strcmp(input,"exit\n") == 0){
				
			break;
			}
			else{
			strtok(input, "\n");
	
			pch = strtok (input," \t,|><&;");
			while(pch != NULL){
				 printf ("%s\n",pch); //Just used for testing purposes,
				 pch = strtok (NULL, " \t,|><&;");
			}
			}
			
			
		}

		 	
	
}



		