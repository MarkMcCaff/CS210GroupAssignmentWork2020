#include <stdio.h>

int size = 5;
char* history[5];
int top = -1;

int isEmpty (){
    if(top ==-1){
        return 1;
    }else{
        return 0;
    }
}

int isFull(){
    if(top == size){
        return 1;
    }else{
        return 0;
    }
}
char* peek() {
   return history[top];
}

void push(char* data) {

   if(!isFull()) {
      top = top + 1;   
      history[top] = data;
   } else {
      printf("Could not insert data, Stack is full.\n");
   }
}

int main() {
   // push items on to the stack 
   push("ls");
   push("pwd");
   push("ls -if");
   push("setptah /bin");
   push("cd");
   push("exit");
   push("error");
   

   printf("Element at top of the stack: %s\n" ,peek());
   printf("Elements: \n");

   // print stack data 
   
   for(int i =0; i<=top;i++){
       printf("%s\n", history[i]);
   }

   printf("Stack full: %s\n" , isFull()?"true":"false");
   printf("Stack empty: %s\n" , isEmpty()?"true":"false");
   
   return 0;
}