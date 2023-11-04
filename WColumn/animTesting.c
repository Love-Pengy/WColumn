#include <stdio.h>
#include <stdlib.h>
/*
struct filearr{
        int size; 
        char ** filePaths;
};
typedef struct filearr * files; 

void playAnimation(FILE ** fp){

        //take in an array of file pointers and play through them one by one printing the value and then pausing in between each one

}

void append(files filesStruct, char* newPath){
        filesStruct->filePaths[filesStruct->size] = newPath; 
}

void animationCreator(char * dir, char * fileName){
        

}
*/
int main (void){

        char test[100] = "animations/success1/frame1";
        FILE * testFp = fopen(test, "r");
        fseek(testFp, 0, SEEK_END); 
        int size = ftell(testFp);
        rewind(testFp); 

        char * hold = malloc(size + 1); 
        fread(hold, size, 1, testFp); 
        printf("%s", hold); 
        



















return(0);
}
