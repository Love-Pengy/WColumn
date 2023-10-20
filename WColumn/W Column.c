#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dllist.h"
#include <stdbool.h>
#include <ctype.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
//max line size
#define LSIZE 300
//max W's 200
#define MAXWS 200
//max string size in chars
#define MAXSTRSIZE 80

//global value for randColor
int color = 0; 
//global randBool for randColor 
bool randColor = false;

//go back to this later cause you have negative clue what this does but it works :3
//https://learn.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences
void consoleEscapeCodeSetup(void){
    // Set output mode to handle virtual terminal sequences
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE)
    {
        return false;
    }
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    if (hIn == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    DWORD dwOriginalOutMode = 0;
    DWORD dwOriginalInMode = 0;
    if (!GetConsoleMode(hOut, &dwOriginalOutMode))
    {
        return false;
    }
    if (!GetConsoleMode(hIn, &dwOriginalInMode))
    {
        return false;
    }

    DWORD dwRequestedOutModes = ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
    DWORD dwRequestedInModes = ENABLE_VIRTUAL_TERMINAL_INPUT;

    DWORD dwOutMode = dwOriginalOutMode | dwRequestedOutModes;
    if (!SetConsoleMode(hOut, dwOutMode))
    {
        // we failed to set both modes, try to step down mode gracefully.
        dwRequestedOutModes = ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        dwOutMode = dwOriginalOutMode | dwRequestedOutModes;
        if (!SetConsoleMode(hOut, dwOutMode))
        {
            // Failed to set any VT mode, can't do anything here.
            return -1;
        }
    }

    DWORD dwInMode = dwOriginalInMode | dwRequestedInModes;
    if (!SetConsoleMode(hIn, dwInMode))
    {
        // Failed to set VT input mode, can't do anything here.
        return -1;
    }
}
//returns the int that corresponds to the next color. 
void randColorDuplicateDestroyer(void){
    if((randColor != true) && (color != 4)){
        color++; 
    }
    else if(randColor == true){
        randColor = false; 
        color = 0;
    }
    else{
    color = (rand() % (4) + 1);
    randColor = true;
    }
}

void setRandomCLIColor(void){
    randColorDuplicateDestroyer();
    if(color == 0){
        //Blue
        printf("\e[38;5;123m");
    }
    else if(color == 1){
        //Pink
        printf("\e[38;5;219m");
    }
    else if(color == 2){
        //Green
        printf("\e[38;5;48m");
    }
    else if(color == 3){
        //Yellow
        printf("\e[38;5;227m");
    }
    else{
        //Porple
        printf("\e[38;5;171m");
    }
}

void userInputPrompt(void){
    //clear screen 
    system("cls");
    printf("You are about to enter input mode!\n");
    printf("To add a W you just type what you want and hit enter!\n");
    printf("To enter \"roaming mode\" you type ROAM and hit enter\n");
    printf("Within roaming mode you will be able to view past W's as well as edit/delete them!\n");
    printf("To fully exit type \"bye!\" and it enter! This will exit input mode and give you your recap! \n");
    printf("~~~ Press Any Key To Continue ~~~");
    char hold;
    scanf("%c", &hold);
    printf("Got Input\n");
    system("cls");
}

void fillTop(void){
    //printf("\e[38;5;123m");
    //printf("\e[38;5;219m");
    printf(" ");
    for(int i = 0; i < 79; i++){
        printf("-");
    }
    printf("\n");
}

void fillSides(void){
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 79; j++){
            if(j == 0){
                printf("|");
            }
            else if(j == 78){
                printf("|");
            }
            printf(" ");
        }
        printf("\n");
    }
}
void fillBottom(void){
    printf(" ");
    for(int i = 0; i < 79; i++){
        printf("-");
    }
}

void createBox(void){
    system("cls");
    fillTop();
    fillSides();
    fillBottom();
    //move up 5 lines
    printf("\e[5A");
    //move left 79 lines
    printf("\e[79D");
}
char * userInputString(){
    createBox();
    char * input = malloc(sizeof(char) * 81);
    //reset style
    printf("\e[0m");
    gets(input);
    //printf("Inputed String: %s\n", test);
    return(input);
}

void userInputRoam(){
    //need function to create box an write roamed string in
    printf("userInputRoam\n");
}

void userInputAction(dllist list){ 
    for(int i = 0; i < MAXWS; i++){
        //printf("userinputAction for loop top");
        setRandomCLIColor();
        char* currentString = malloc(sizeof(char) * 82);
        if(i == 0){
            //prompt user for how to input information
            userInputPrompt();
        }
        //take user input and add it to the dllist
        currentString = userInputString();
        if(currentString == NULL){
            continue;
        }
        else if(!strcmp(currentString, "ROAM")){
            //starts roaming mode 
            userInputRoam();
        }
        else if(!strcmp(currentString, "bye!")){
            break;
        }
        else{
            addList(currentString, list);
        }
    }
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
    system("cls");
    printf("You will see your W column show up, when that happens you will have the opportunity to type in all of your W's into your column! whether it be big or small a W is a W! Go ahead and type it out! After each W you type in hit enter and your box will close and hold the W for later viewing! When done you can type -1 and you will get your recap for the day!\n");
    printf("~~Press any key to continue~~");
    scanf("%c", &hold);
    system("cls");
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
    //sets random seed to a random value 
    srand(time(NULL));
    introInstruct();
    FILE* fptr;
    fptr = fopen("wcolhold.txt", "a+");
    //a+ opens file for both reading and appending d
    dllist wList = loadWs(fptr);
    /*for(int i = 0; i < 10; i++){
        setRandomCLIColor();
    }*/
    userInputAction(wList);
    system("cls");
    printList(wList);
    printf("End.");
    fclose(fptr);
    return 0; 
}