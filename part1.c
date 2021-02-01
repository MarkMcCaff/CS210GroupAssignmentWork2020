#include <stdio.h>
#include <string.h>
int main(void){
		
		printf("Welcome to CS210 Group Shell Project \n");
		char input[512];
			
		
		char* pch;
		
		
		while(1){
			
			printf("+");
			
			//Control D
			fgets(input, 512, stdin); 
			if(feof(stdin)){
				
				break;
			}
			//Will break when u type "exit"
			if(strcmp(input,"exit\n") == 0){
				
			break;
			}
			else{
			strtok(input, "\n");
	
			pch = strtok (input," \t,|><&;");
			while(pch != NULL){
				 printf ("%s\n",pch); //Just used for testing purposes,
				 pch = strtok (NULL, " \t,|><&;");
				 //printf ("%d", check);
			}
			}
			
			
}

		 return 0;
		
		
		
		
		}
		