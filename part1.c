#include <stdio.h>
#include <string.h>
int main(void){
		
		printf("Welcome to CS210 Group Shell Project \n");
		char input[512];
			
		
		char* pch;
		
		
		while(1){
			
			
			printf("+");
			
			
			if(!fgets(input, 512, stdin)){
				
			break;
			}
			else if(strcmp(input,"exit\n") == 0){
				
			break;
			}
			else{
			strtok(input, "\n");
	
			pch = strtok (input," \t,|><%;");
			while(pch != NULL){
				 printf ("%s\n",pch);
				 pch = strtok (NULL, " \t,|><%;");
			}
			}
			
			
}

		 return 0;
		
		
		
		
		}
		