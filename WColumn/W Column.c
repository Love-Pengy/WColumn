#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dllist.h"
//max W's 200
#define MAXWS 200
//max string size in chars
#define MAXSTRSIZE 80

    //prompt user for the first time
    void introInstruct(){
        char hold;
        printf("Hello! This app is inspired by Dr. K's talk about turning w's into l's (https://twitter.com/HealthyGamerGG/status/1615857210638180353?s=200).\n This app is designed to help you keep track of your w's and put them in your respective \"column\"! \n");
        printf("~~Press any key to continue~~");
        scanf("%c", &hold);
        printf("You will see your W column show up, when that happens you will have the opportunity to type in all of your W's into your column! whether it be big or small a W is a W! Go ahead and type it out! After each W you type in hit enter and your box will close and hold the W for later viewing! When done you can type -1 and you will get your recap for the day!\n");
        printf("~~Press any key to continue~~");
        scanf("%c", &hold);
    }

    dllist loadWs(FILE* fp){
       int size = 0;
       if(fp != NULL){
        fseek(fp, 0, SEEK_END);
        size = ftell(fp);
        if(size == 0){
          dllist d = initList();
          return(d);
        }
        else{
            dllist d = initList();
            rewind(fp);
            char* sTemp = " ";
            while(1){
                fgets(sTemp, 80, fp);
                if(feof(fp)){
                    break;
                }
                else{
                    addList(sTemp, d);
                }

            }
            return(d);
        }
    }
       else{
        printf("ERR: FILE POINTER RETURNED NULL CHECK FILE PERMISSIONS\n");
        return NULL;
    }
    }

int main(void){
    introInstruct();
    FILE* fptr; 
    //a+ opens file for both reading and appending 
    fptr = fopen("wcolhold.txt", "a+");
    dllist wList = loadWs(fptr);
    printList(wList);
    fclose(fptr);
    printf("End.");
    return 0; 
}