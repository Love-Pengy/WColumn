#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dllist.h"
#include <stdbool.h>
#include <ctype.h>
//max line size
#define LSIZE 300
//max W's 200
#define MAXWS 200
//max string size in chars
#define MAXSTRSIZE 80

    //Check For Identifier 0 if no identifier 1 if identifier 
    int checkIdent(char *str){
        char strCheck[50];
        //printf("TEST: %s\n", str);
        strcpy(strCheck, "~W Column App By LovePengy~");
        if(!strcmp(str, strCheck)){
            return(1); 
        }
        return(0);
    }

    char * trailingNLDestroyer(char *str){
        //strcspn returns the value where the inputed value is found
        str[strcspn(str, "\n")] = '\0';
        return(str);
    }
    //get rid of whitespace in strings
    char * stringParse(char *pString){
        //printf("DEBUG1!!%s\n", pString);
        int size = strlen(pString);
        //printf("Something: %d\n", size);
        //printf("size: %d\n", size);
        char *newString = malloc(sizeof(char) * (size + 1));
        int j = 0;
        int spaceCount = 0;
        int countValid = 0;
        
        for(int i = 0; i < size; i++){
            if(!isspace(pString[i])){
                newString[i-j] = pString[i];
                spaceCount = 0;
                countValid++;
                //printf("Added: %c\n", pString[i]);
                //printf("NewString: %s\n", newString);
        }
        else if((spaceCount == 0) && (countValid > 0)){
            newString[i-j] = pString[i];
            spaceCount++;
        }
            else{
                j++;
            }
     }
     newString[size-j] = '\0';
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
        rewind(fp);
        char *strChecker2 = malloc(sizeof(char) * 160);
        fgets(strChecker2, LSIZE, fp);
        strChecker2 = trailingNLDestroyer(strChecker2);

        if(size == 0 || !checkIdent(strChecker2)){
          int returning = 0;
          returning = checkReturnUser();
          if(returning == 0){
            dllist d = initList();
            rewind(fp);
            fprintf(fp, "~W Column App By LovePengy~\n");
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
                rewind(fp);
                char *strChecker = malloc(sizeof(char) * 160);
                fgets(strChecker, LSIZE, fp);
                strChecker = trailingNLDestroyer(strChecker);
                    if(size == 0 || (checkIdent(strChecker) == 0)){
                        char hold = ' ';
                        printf("That File Is Empty Or Not Formatted Correctly! Would You Like To Continue With This Directory?(Y/N) ");
                        scanf(" %c", &hold);
                            if(hold == 'Y'){
                                dllist d = initList();
                                fprintf(fp,"~W Column App By LovePengy~\n");
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
                        //printf("test: %d\n", fp==NULL);
                        char* sTemp = malloc(sizeof(char) * 81);
                         while(1){
                            fgets(sTemp, LSIZE, fp);
                            sTemp = trailingNLDestroyer(sTemp);
                                if(feof(fp)){
                                    break;
                                }
                                else{
                                    sTemp = stringParse(sTemp);
                                    addList(sTemp, d);
                                 }
                            fgets(sTemp, 80, fp);
                            sTemp = trailingNLDestroyer(sTemp);
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
            //printf("\nHERE\n");
            dllist d = initList();
            rewind(fp);
            //fp = freopen(NULL, "w", fp);
            char* sTemp = malloc(sizeof(char) * 81);
            fgets(sTemp, 80, fp);
            sTemp = malloc(sizeof(char) * 81);
            while(1){
                //printf("here\n");
                fgets(sTemp, 80, fp);
                sTemp = trailingNLDestroyer(sTemp);
                if(feof(fp)){
                    break;
                }
                else{
                    sTemp = stringParse(sTemp);
                    addList(sTemp, d);
                }

            }
            fgets(sTemp, LSIZE, fp);
            sTemp = trailingNLDestroyer(sTemp);
            sTemp = stringParse(sTemp);
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