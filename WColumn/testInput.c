#include <stdio.h> 
#include <stdlib.h>
#include <conio.h>

int main (void){
    //printf("\n");
    //printf("\x1b[1;31mHello");
    int exit = 0;
    char hold = ' ';
    char *test = malloc(sizeof(char) * 81);
    int index = 0;
    while(exit != 1){
        scanf("%c", &hold);
        printf("scanned once\n");
        if(hold == '\n' || index == 80){
            exit = 1; 
            continue;
        }
        test[index] = hold;
        index++;
    }
    test[++index] = '\0';
    printf("TEST: %s", test);

    return(0);

}