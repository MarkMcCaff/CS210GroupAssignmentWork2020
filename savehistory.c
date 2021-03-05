void saveHistory(){
    FILE* fPtr;
    fPtr = fopen("/home/tmb19188/Documents/Project/ThisSucks.txt", "w");
    if(fPtr == NULL)
    {
        //File does not exist
        printf("Unable to create requested file.\n");
    }
    else
    {
        for(int i = 0; i < 20; i++){
            fputs(history[i], fPtr);
        }
        printf("Command History Saved.\n");
    }
    fclose(fPtr);

}
