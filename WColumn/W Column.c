#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dllist.h"
#include <stdbool.h>
#include <ctype.h>
#include <conio.h>
//max line size
#define LSIZE 300
//max W's 200
#define MAXWS 200
//max string size in chars
#define MAXSTRSIZE 80

    void userInputPrompt(void){
        //clear screen 
        system("cls");
        printf("You are about to enter input mode!\n");
        printf("To add a W you just type what you want and hit enter!\n");
        printf("To enter \"roaming mode\" you type ROAM and hit enter\n");
        printf("Within roaming mode you will be able to view past W's as well as edit/delete them!\n");
        printf("To fully exit type \"bye\" and it enter! This will exit input mode and give you your recap! \n");
        printf("~~~ Press Any Key To Continue ~~~");
        char hold;
        scanf("%c", &hold);
        printf("Got Input\n");
        system("cls");
    }

    char * userInputString(){
        printf("userInputString\n");
        return(NULL);
    }

    void userInputRoam(){
        printf("userInputRoam\n");
    }

    void userInputAction(void){
        //prompt user for how to input information
        userInputPrompt();
        //take user input and add it to the dllist
        userInputString();
        //starts roaming mode 
        userInputRoam();
    } 
    //if user wants to create file return 1 if they want to continue with default return 2
    int invalFilePrompt(void){
        int hold = 0;
        int returning = 0;
        while(1){
            if(returning == 0){
                printf("That file does not exist! Would you like to create this file (1), continue with the default directory (2), or give another directory? (3)\n");
            }
            else{
                printf("That was not one of the options! Please try again. \n");
            }
        scanf(" %d", &hold);
        if(hold > 0 || hold < 4){
            return(hold);
        }
        returning++;
        }
    }

    //add .txt to files without a file extension or change the file extention to .txt if one exists but isnt a text extention 
    char *textAdd(char *str){
        //printf("got to text add\n");
        int size = strlen(str);
        char *textAdd = malloc(sizeof(char)* (size + 4));
        if(strchr(str, '.') == NULL){
            strcpy(textAdd, str);
            strcat(textAdd, ".txt");
            return(textAdd);
        }
        char* end = malloc(sizeof(char) * 10);
        end = strchr(str, '.');
        int sizeEnd = strlen(end);
        for(int i = 0; i < size - sizeEnd; i++){
            textAdd[i] = str[i];
        }
        strcat(textAdd, ".txt");
        return(textAdd);
    }


    //Check for .txt file extention 1 if txt 0 if not
    int checkText(char *str){
        char * newString = malloc(sizeof(char) * strlen(str));
        strcpy(newString, str);
        char * token = strtok(newString, ".");
        char * lastToken = malloc(sizeof(char) * strlen(token));
        while( token != NULL ) {
            strcpy(lastToken, token);
            token = strtok(NULL, ".");
        }
        //printf("TEST WHILE: %s\n", lastToken);
        if(!strcmp(lastToken, "txt")){
            return 1;
        }
        return 0;
        //int size = strlen(str);
        //printf("TOKEN TEST: %d\n", str == NULL);
        //printf("TEST: %s\n", token);
        //printf("TEST2: (token size): %lld (str size): %lld \n", strlen(token), strlen(str));
        /*if(token == NULL || (strlen(token) == strlen(str))){
            printf("exit clause\n");
            return(0);
        }
    return(0);*/
    
    }

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
        printf("What is the file path to where your W's are held?\n");
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
            fp = fopen(dir, "r");
                if(fp != NULL){
                fseek(fp, 0, SEEK_END);
                size = ftell(fp);
                rewind(fp);
                char *strChecker = malloc(sizeof(char) * 160);
                fgets(strChecker, LSIZE, fp);
                strChecker = trailingNLDestroyer(strChecker);
                    if(size == 0 || (checkIdent(strChecker) == 0) || (!checkText(dir))){
                        int hold = 0;
                        printf("That File Is Empty Or Not Formatted Correctly! Would You Like To Continue With This Directory(1), continue with the default directory(2), or pick another directory(3)?\n");
                        printf("NOTE: Continuing with this directory will delete everything within this file. \n");
                    
                        scanf(" %d", &hold);
                            if(hold == 1){
                                //here it has it 
                                //printf("DIR CHECK: %s\n", dir);
                                if(!checkText(dir)){
                                    //printf("if\n\n");
                                    fclose(fp);
                                    fp = fopen(textAdd(dir), "w");
                                }
                                
                                else{
                                    //printf("else\n\n");
                                    fclose(fp);
                                    printf("else test: %s\n\n", dir);
                                    fp = fopen(dir, "w");
                                }
                                dllist d = initList();
                                fprintf(fp,"~W Column App By LovePengy~\n");
                                return(d);
                            }
                            else if(hold == 2){
                                fp = fopen("wcolhold.txt", "w"); 
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
                        fclose(fp);
                        fp = fopen(dir, "a+");
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
                    int grubbyHandGrabber; 
                    grubbyHandGrabber = invalFilePrompt();
                    if(grubbyHandGrabber == 1){
                        if(!checkText(dir)){
                                    fclose(fp);
                                    fp = fopen(textAdd(dir), "w");
                                }
                                dllist d = initList();
                                fprintf(fp,"~W Column App By LovePengy~\n");
                                return(d);
                    }

                    else if(grubbyHandGrabber == 2){
                        fp = fopen("wcolhold.txt", "w");
                        fprintf(fp, "~W Column App By LovePengy~\n");
                        dllist d = initList();
                        return(d);
                    }
                    else{
                     continue;
                    }
                }
            }
          }
        }

        else{
            //printf("\nHERE\n");
            dllist d = initList();
            fp = fopen("wcolhold.txt", "w");
            fprintf(fp, "~W Column App By LovePengy~\n");
            //rewind(fp);
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
            /*fgets(sTemp, LSIZE, fp);
            sTemp = trailingNLDestroyer(sTemp);
            sTemp = stringParse(sTemp);
            addList(sTemp, d);*/
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
    userInputAction();
    printf("End.");
    fclose(fptr);
    return 0; 
}