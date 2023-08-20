#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

 char * stringParse(char *pString){
        //printf("DEBUG1!!%s\n", pString);
        int size = strlen(pString);
        //printf("size: %d\n", size);
        char *newString = malloc(sizeof(char) * (size + 1));
        int j = 0;
        int spaceCount = 0;
        for(int i = 0; i < size; i++){
            if(!(isspace(pString[i]))){
                newString[i-j] = pString[i];
                spaceCount = 0;
                //printf("Added: %c\n", pString[i]);
                //printf("NewString: %s\n", newString);
        }
        else if((isspace(pString[i])) && (spaceCount == 0)){
            newString[i-j] = pString[i];
            spaceCount++;
        }
            else{
                j++;
                spaceCount++;
            }
     }
     //printf("DEBUG2!!%s\n", newString);
     newString[size-j] = '\0';
     return(newString);
    }


int main (void){
    FILE * fp = fopen("wcolhold.txt", "a+");
    char *string = malloc(sizeof(char) * 160);
    int i = 0;
    fgets(string, 80, fp);
    string = stringParse(string);
    printf("STRING %d: %s\n", i, string);
    i++;
    fgets(string, 80, fp);
    string = stringParse(string);
    printf("STRING %d: %s\n", i, string);
    printf("got to the end\n");
    return 0; 
}