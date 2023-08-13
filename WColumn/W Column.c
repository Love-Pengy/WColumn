#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dllist.h"
#include <stdbool.h>
#include <ctype.h>
//max W's 200
#define MAXWS 200
//max string size in chars
#define MAXSTRSIZE 80

    //get rid of whitespace in strings
    char * stringParse(char *pString){
        //printf("DEBUG1!!%s\n", pString);
        int size = strlen(pString);
        //printf("size: %d\n", size);
        char *newString = malloc(sizeof(char) * size);
        int j = 0;
        for(int i = 0; i < size; i++){
            if(!isspace(pString[i])){
                newString[i-j] = pString[i];
        }
            else{
                j++;
            }
     }
     //printf("DEBUG2!!%s\n", newString);
     return(newString);
    }

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

    //check if user is a returning user (0 = false);
    int checkReturnUser(){
        char ans;
        printf("Are you a returning user? (T/F)\n");
        scanf(" %c", &ans);
        if((ans == 'T') || (ans == 't')){
            return(1);
        }
        else{
            return(0);
        }
        }

    //get dir of W's
    char *getDir(){
        char *hold = malloc(sizeof(char) * 160);
        printf("What is the Directory where your W's are held?\n");
        scanf(" %s", hold);
        return(hold);
    }
    

    dllist loadWs(FILE *fp){
       int size = 0;
       if(fp != NULL){
        fseek(fp, 0, SEEK_END);
        size = ftell(fp);

        if(size == 0){
          int returning = 0;
          returning = checkReturnUser();
          if(returning == 0){
            dllist d = initList();
            return(d);
          }
          else{
            int finLoop = 0;
            while(finLoop == 0){
            char* dir = getDir();
            fp = fopen(dir, "a+");
                if(fp != NULL){
                fseek(fp, 0, SEEK_END);
                size = ftell(fp);
                    if(size == 0){
                        char hold = ' ';
                        printf("That File is Empty! Would You Like To Continue With This Directory?(Y/N) ");
                        scanf(" %c", &hold);
                            if(hold == 'Y'){
                                dllist d = initList();
                                return(d);
                            }
                            else{
                                continue;
                            }
                        printf("File is empty \n");
                        continue;
                    }
                    else{
                        dllist d = initList();
                        rewind(fp);
                        //printf("test: %d\n", fp==NULL);
                        char* sTemp = malloc(sizeof(char) * 81);
                         while(1){
                            fgets(sTemp, 80, fp);
                                if(feof(fp)){
                                    break;
                                }
                                else{
                                    sTemp = stringParse(sTemp);
                                    addList(sTemp, d);
                                 }
                            fgets(sTemp, 80, fp);
                            sTemp = stringParse(sTemp);
                            addList(sTemp, d);
            }
            return(d);
                    }
                }
                else{
                     printf("ERR: NOT A VALID FILE\n");
                     continue;
                }
            }
          }
        }
        else{
            dllist d = initList();
            rewind(fp);
            char* sTemp = malloc(sizeof(char) * 81);
            while(1){
                //printf("here\n");
                fgets(sTemp, 80, fp);
                if(feof(fp)){
                    break;
                }
                else{
                    sTemp = stringParse(sTemp);
                    addList(sTemp, d);
                }

            }
            fgets(sTemp, 80, fp);
            stringParse(sTemp);
            addList(sTemp, d);
            return(d);
        }
    }
       else{
        printf("ERR: FILE POINTER RETURNED NULL CHECK FILE PERMISSIONS\n");
        return(NULL);
    }
    return(NULL);
        
    }

    
int main(void){
    introInstruct();
    FILE* fptr;
    fptr = fopen("wcolhold.txt", "a+");
    //a+ opens file for both reading and appending d
    dllist wList = loadWs(fptr);
    printList(wList);
    printf("End.");
    return 0; 
}