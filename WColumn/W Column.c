#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dllist.h"
#include <stdbool.h>
#include <ctype.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include "animations.h"
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
int originalWs = 0; 
char currentFilePath[300] = "";





int consoleEscapeCodeSetup(void){
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
        dwRequestedOutModes = ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        dwOutMode = dwOriginalOutMode | dwRequestedOutModes;
        if (!SetConsoleMode(hOut, dwOutMode))
        {
            return -1;
        }
    }

    DWORD dwInMode = dwOriginalInMode | dwRequestedInModes;
    if (!SetConsoleMode(hIn, dwInMode))
    {
        return -1;
    }
    return 0;
}

int getTotalCount(dllist d){
        return(listSize(d));
}
int getSessionCount(dllist d, int original){
        int hold = (listSize(d) - original);
        return(hold);
}


int stringLengthNoSpace(char *string){
    int length = 0; 
    for(int i = 0; i < (strlen(string)); i++){
        if(!(isspace(string[i]))){
            length++;
        }
    }
    return(length);
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

void createRoamUI(void){
    system("cls");
    for(int i = 0; i < 10; i++){
        printf(" ");
    }
    printf("\e[1m");
    printf("\e[48;5;230m");
    printf("\e[38;5;232m");  
    printf("MOVE: w & s");
    printf("\e[0m");
    printf("\e[?25l");
    for(int i = 0; i < 10; i++){
        printf(" ");
    }

    printf("\e[1m");
    printf("\e[48;5;230m");
    printf("\e[38;5;232m");
    printf("EDIT: e");
    printf("\e[0m");
    printf("\e[?25l");
    for(int i = 0; i < 10; i++){
        printf(" ");
    }
    printf("\e[1m");
    printf("\e[48;5;230m");
    printf("\e[38;5;232m"); 
    printf("DELETE: d");
    printf("\e[0m");
    printf("\e[?25l");
    for(int i = 0; i < 10; i++){
        printf(" ");
    }
    printf("\e[1m");
    printf("\e[48;5;230m");
    printf("\e[38;5;232m"); 
    printf("EXIT: r");
    printf("\e[0m");
    printf("\e[?25l");
    printf("\n");
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

void createBoxNoClear(void){
    fillTop();
    fillSides();
    fillBottom();
    printf("\e[5A");
    printf("\e[79D");
}

void createEditBoxNoClear(void){
    fillTop();
    fillSides();
    fillBottom();
    printf("\e[7A");
    printf("\e[79D");
}

char * userInputString(){
    createBox();
    char * input = malloc(sizeof(char) * 81);
    //reset style
    printf("\e[0m");
    printf("\e[?25l");
    gets(input);
    if(strlen(input) == 0){
        return(NULL);
    }
    return(input);
}

void userInputRoam(dllist list){
    //w = up (go from tail to head)
    //s = down (go from head to tail)
    //e = edit mode
    //d = delete 
    //r = exit
    int currentIndex = listSize(list);
    char holdCurrChar = ' ';
    int exit = 0;
    while(!exit){
        createRoamUI();
        createBoxNoClear();
        printf("%s", getItemAtIndex(list, currentIndex));
        holdCurrChar = getch();
        printf(" ");

        if((holdCurrChar == 'w') | (holdCurrChar == 'W')){
            system("cls");
            createRoamUI();
            createBoxNoClear();
            if((currentIndex != 1)){
                currentIndex--;
                continue;
            }
            else{
                for(int i = 0; i < 5; i++){
                    createRoamUI();
                    createBoxNoClear();
                    if((i % 2) == 0){
                        //red
                        printf("\e[38;5;196m");
                    }
                    else{
                        //white
                        printf("\e[38;5;255m");
                    }
                    for(int j = 0; j < 30; j++){
                        printf(" ");
                    }
                    printf("Hit The Oldest Item!");
                    Sleep(75);
                    system("cls");
                }
                printf("\e[25m");
                printf("\e[?25l");
                FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
                continue;
            }
        }   
        else if((holdCurrChar == 's') | (holdCurrChar == 'S')){
            system("cls");
            createRoamUI();
            createBoxNoClear();
            if((currentIndex != listSize(list))){
                currentIndex++;
                continue;
            }
            else{
                for(int i = 0; i < 5; i++){
                    createRoamUI();
                    createBoxNoClear();
                    if((i % 2) == 0){
                        printf("\e[38;5;196m");
                    }
                    else{
                        printf("\e[38;5;255m");
                    }
                    for(int j = 0; j < 30; j++){
                        printf(" ");
                    }
                    printf("Hit The Newest Item!");
                    Sleep(75);
                    system("cls");
                }
                printf("\e[25m");
                printf("\e[?25l");
                //clear inputs that happened during invalid period 
                FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
                continue;
            }
        }   
        else if((holdCurrChar == 'd') | (holdCurrChar == 'D')){
            system("cls");
            createRoamUI();
            createBoxNoClear();
            if((listSize(list) != 1)){
                if(currentIndex == listSize(list)){
                    removeAtIndex(list, currentIndex);
                    currentIndex--;
                    continue;
                }
                removeAtIndex(list, currentIndex);
                FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
                continue;
            }
            else{
                for(int i = 0; i < 5; i++){ 
                    createRoamUI();
                    createBoxNoClear();
                    if((i % 2) == 0){
                        printf("\e[38;5;196m");
                    }
                    else{
                        printf("\e[38;5;255m");
                    }
                    for(int j = 0; j < 30; j++){
                        printf(" ");
                    }
                    printf("!!Removing Last Item!!");
                    Sleep(75);
                    system("cls");
                    for(int j = 0; j < 30; j++){
                        printf(" ");
                    }
                }
                removeAtIndex(list, currentIndex);
                printf("\n\n\n\n\n");
                for(int i = 0; i < 30; i++){
                    printf(" ");
                }
                printf("Returning To Insert Mode");
                Sleep(2000);
                printf("\e[25m");
                printf("\e[?25l");
                exit = 1;
            }
        }   
        else if((holdCurrChar == 'r') || (holdCurrChar == 'R')){
            exit = 1;
        }
        else if((holdCurrChar == 'e') || (holdCurrChar == 'E')){
            char *buffer = malloc(sizeof(char) * LSIZE);
            system("cls");
            createRoamUI();
            createEditBoxNoClear();
            char * hold = getItemAtIndex(list, currentIndex);
            printf("\e[38;5;197m");
            printf("%s\n", hold);
            //move to the right one line (doesnt override boundary because nothing was written)
            printf("\e[1C");
            int size = stringLengthNoSpace(hold); 
            printf("\e[38;5;76m");
            for(int i = 0; i < size; i++){
                if((i % 2) == 0){
                    printf("v ");
                }
            }
            printf("\e[0m");
            printf("\n");
            printf("\e[1C");
            gets(buffer);
            if((buffer == NULL) || (buffer[0] == '\n')){
                continue;
            }
            replaceAtIndex(list, currentIndex, buffer);
        }
        else{ 
            continue;
        }
    }
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
}

void userInputAction(dllist list){
    manager bossyBoss = initManager();
    animationSetup(bossyBoss);
    for(int i = 0; i < MAXWS; i++){
        if(i != 0){
            setRandomCLIColor();
        }
        char* currentString = malloc(sizeof(char) * 82);
        if(i == 0){
            userInputPrompt();
        }
        if(i == 0){
            setRandomCLIColor();
        }
        currentString = userInputString();
        if(currentString == NULL){
            continue;
        }
        else if(!strcmp(currentString, "ROAM")){
            if(listSize(list) == 0){
                for(int i = 0; i < 5; i++){
                    createRoamUI();
                    createBoxNoClear();
                    if((i % 2) == 0){
                        printf("\e[38;5;196m");
                    }
                    else{
                        printf("\e[38;5;255m");
                    }
                    for(int j = 0; j < 30; j++){
                        printf(" ");
                    }
                    printf("Not Enough W's To ROAM!");
                    Sleep(75);
                    system("cls");
                }
                printf("\e[25m");
                printf("\e[?25l");
                FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
                continue;
                printf("\e[38;5;196m"); 
            }
            else{
                userInputRoam(list);
            }
        }
        else if(!strcmp(currentString, "bye!")){
            break;
        }
        else{
            addList(currentString, list);
            playRandAnimation(bossyBoss); 
            FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
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

char *textAdd(char *str){
    int size = strlen(str);
    char *textAdd = malloc(sizeof(char)* (size + 4));
    if(strchr(str, '.') == NULL){
        strcpy(textAdd, str);
        strcat(textAdd, ".txt");
        printf("NOT FINDING . CHAR");
        return(textAdd);
    }
    char* end = malloc(sizeof(char) * 10);
    //get last occurance
    end = strrchr(str, '.');
    int sizeEnd = strlen(end);
    for(int i = 0; i < (size - sizeEnd); i++){
        textAdd[i] = str[i];
    }
    textAdd[(strlen(str) - strlen(end))] = '\0';
    strcat(textAdd, ".txt");
    return(textAdd);
}


int checkText(char *str){
    char * newString = malloc(sizeof(char) * strlen(str));
    strcpy(newString, str);
    char * token = strtok(newString, ".");
    char * lastToken = malloc(sizeof(char) * strlen(token));
    while( token != NULL ) {
        strcpy(lastToken, token);
        token = strtok(NULL, ".");
    }
    if(!strcmp(lastToken, "txt")){
        return 1;
    }
    return 0;
}

int checkIdent(char *str){
    char strCheck[50];
    strcpy(strCheck, "~W Column App By LovePengy~");
    if(!strcmp(str, strCheck)){
        return(1); 
    }
    return(0);
}

char * stringParse(char *pString){
    int size = strlen(pString);
    char *newString = malloc(sizeof(char) * (size + 1));
    int j = 0;
    int spaceCount = 0;
    int countValid = 0;
    
    for(int i = 0; i < size; i++){
        if(!isspace(pString[i])){
            newString[i-j] = pString[i];
            spaceCount = 0;
            countValid++;
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

void introInstruct(){
    char hold;
    system("cls");
    printf("Hello! This app is inspired by Dr. K's talk about turning w's into l's (https://twitter.com/HealthyGamerGG/status/1615857210638180353?s=200).\nThis app is designed to help you keep track of your w's and put them in your respective \"column\"! \n");
    printf("~~Press any key to continue~~");
    hold = getch();
    system("cls");
    printf("You will first see a colored box show up that will allow you to input your W!\n");
    printf("~~Press any key to continue~~");
    hold = getch();
    system("cls");
}

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
        strcpy(currentFilePath, dir);
        fp = fopen(dir, "r");
            if(fp != NULL){
            fseek(fp, 0, SEEK_END);
            size = ftell(fp);
            rewind(fp);
            char *strChecker = malloc(sizeof(char) * 160);
            fgets(strChecker, LSIZE, fp);
            strChecker = trailingNLDestroyer(strChecker);
                if(size == 0 || (checkIdent(strChecker) == 0) || (!checkText(dir))){
                    printf("size: %d, ident: %d, checkText: %d\n", size, checkIdent(strChecker), (!checkText(dir)));
                    int hold = 0;
                    printf("That File Is Empty Or Not Formatted Correctly! Would You Like To Continue With This Directory(1), continue with the default directory(2), or pick another directory(3)?\n");
                    printf("NOTE: Continuing with this directory will delete everything within this file. \n"); 
                    scanf(" %d", &hold);
                        if(hold == 1){
                            if(!checkText(dir)){
                                fclose(fp);
                                fp = fopen(textAdd(dir), "w");
                                strcpy(currentFilePath, textAdd(dir));
                            }
                            
                            else{
                                fclose(fp);
                                fp = fopen(dir, "w");
                                strcpy(currentFilePath, dir);
                            }
                            dllist d = initList();
                            fprintf(fp,"~W Column App By LovePengy~\n");
                            return(d);
                        }
                        else if(hold == 2){
                            fp = fopen("wcolhold.txt", "w"); 
                            strcpy(currentFilePath, "wcolhold.txt");
                            dllist d = initList();
                            fprintf(fp,"~W Column App By LovePengy~\n");
                            return(d);
                        }
                        else{ 
                            printf("Invalid Option! Please Try Again!\n");
                            continue;
                        }
                    printf("File is empty \n");
                    continue;
                }
                else{
                    printf("HERE\n");
                    getch();
                    fclose(fp);
                    fp = fopen(dir, "a+");
                    strcpy(currentFilePath, dir);
                    dllist d = initList();
                    char* sTemp = malloc(sizeof(char) * 81);
                    fgets(sTemp, LSIZE, fp);
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
                                strcpy(currentFilePath, textAdd(dir));
                            }
                            dllist d = initList();
                            fprintf(fp,"~W Column App By LovePengy~\n");
                            return(d);
                }
                else if(grubbyHandGrabber == 2){
                    fp = fopen("wcolhold.txt", "w");
                    strcpy(currentFilePath, "wcolhold.txt");
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
        if(size == 0){
            dllist d = initList();
            fp = fopen("wcolhold.txt", "w");
            strcpy(currentFilePath, "wcolhold.txt");
            return(d);
        }
        if(checkIdent(strChecker2) && (size != 0)){
            dllist d = initList();
            char* sTemp = malloc(sizeof(char) * 81);
            rewind(fp);
            fgets(sTemp, 80, fp); 
            sTemp = malloc(sizeof(char) * 81);
            while(1){
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
            return(d);
        }
    }
}
    else{
    printf("ERR: FILE POINTER RETURNED NULL CHECK FILE PERMISSIONS\n");
    return(NULL);
}
return(NULL);
    
}



void dumpWs(dllist dl, FILE * fp){
        fclose(fp);
        fp = fopen(currentFilePath, "w");  
        fprintf(fp, "~W Column App By LovePengy~\n");
        if(listSize(dl) != 0){
            for(int i = 1; i < (listSize(dl)+1); i++){
                    fprintf(fp, "%s\n", getItemAtIndex(dl, i));
            }
        }
}






    
int main(void){
    //ensures that escape codes work properly regardless of what program runs before it 
    consoleEscapeCodeSetup();
    //hides cursor
    printf("\e[?25l");
    srand(time(NULL));
    introInstruct();
    FILE* fptr;
    fptr = fopen("wcolhold.txt", "a+");
    strcpy(currentFilePath, "wcolhold.txt");
    dllist wList = loadWs(fptr);
    originalWs = listSize(wList);    
    userInputAction(wList);
    system("cls");
    printf("The amount of W's this session was: %d\n", getSessionCount(wList, originalWs)); 
    printf("This means you have %d total W's!\n", getTotalCount(wList));
    printf("\e[?25h");
    dumpWs(wList, fptr);
    clearList(wList);
    fclose(fptr);
    return 0; 
}
