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
//global count for original W's
int originalWs = 0; 
//go back to this later cause you have negative clue what this does but it works :3
//https://learn.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences
//HANDLE type is a reference that allows you to access a resource
int consoleEscapeCodeSetup(void){
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
    return 0;
}

int getTotalCount(dllist d){
        return(listSize(d));
}
int getSessionCount(dllist d, int original){
        int hold = (listSize(d) - original);
        if(hold < 0){
                return(0);
        }
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

void createRoamUI(void){
    system("cls");
    for(int i = 0; i < 10; i++){
        printf(" ");
    }
    printf("\e[1m");
    printf("\e[48;5;230m");
    printf("\e[38;5;232m");  
      //fflush(stdout);
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
    //move up 5 lines
    printf("\e[5A");
    //move left 79 lines
    printf("\e[79D");
}

void createEditBoxNoClear(void){
    fillTop();
    fillSides();
    fillBottom();
    //move up 5 lines
    printf("\e[7A");
    //move left 79 lines
    printf("\e[79D");
}

char * userInputString(){
    createBox();
    char * input = malloc(sizeof(char) * 81);
    //reset style
    printf("\e[0m");
    printf("\e[?25l");
    gets(input);
    //printf("Inputed String: %s\n", input);
    if(strlen(input) == 0){
        return(NULL);
    }
    //printf("INPUT: %lld", (strlen(input)));
    //printf("~~~~~~~~~~");
    //int something = 0;
    //scanf("%d", &something);
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
        //need function to create box an write roamed string in
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
                    //stop for .05 seconds
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
                    printf("Hit The Newest Item!");
                    //stop for .05 seconds
                    Sleep(75);
                    system("cls");
                }
                //try to read in a string to stop getche from getting the inputs that were given during this time
                printf("\e[25m");
                printf("\e[?25l");
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
                    printf("!!Removing Last Item!!");
                    //stop for .05 seconds
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
    //flushes the input given in the console (AKA fflush() but actually works)
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
}

void userInputAction(dllist list){
        manager bossyBoss = initManager();
        animationSetup(bossyBoss);
    for(int i = 0; i < MAXWS; i++){
        //printf("userinputAction for loop top");
        if(i != 0){
            setRandomCLIColor();
        }
        char* currentString = malloc(sizeof(char) * 82);
        if(i == 0){
            //prompt user for how to input information
            userInputPrompt();
        }
        //take user input and add it to the dllist
        if(i == 0){
            setRandomCLIColor();
        }
        currentString = userInputString();
        if(currentString == NULL){
            continue;
        }
        else if(!strcmp(currentString, "ROAM")){
            //starts roaming mode 
            userInputRoam(list);
        }
        else if(!strcmp(currentString, "bye!")){
            break;
        }
        else{
            addList(currentString, list);
            playRandAnimation(bossyBoss); 
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
        if(checkIdent(strChecker2)){
            dllist d = initList();
            return(d);
        }
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



void dumpWs(dllist dl, FILE * fp){
        rewind(fp);
        for(int i = 0; i < listSize(dl); i++){
                fprintf(fp, "%s\n", getItemAtIndex(dl, i+1));
        }
}






    
int main(void){
    //ensures that escape codes work properly regardless of what program runs before it 
    consoleEscapeCodeSetup();
    //hides cursor
    printf("\e[?25l");
    //to reenable it use printf("\e[?25h");
    //sets random seed to a random value 
    srand(time(NULL));
    introInstruct();
    FILE* fptr;
    fptr = fopen("wcolhold.txt", "a+");
    //a+ opens file for both reading and appending d
    dllist wList = loadWs(fptr);
    originalWs = listSize(wList);    
    userInputAction(wList);
    system("cls");
    //printList(wList);
    printf("The amount of W's this session was: %d\n", getSessionCount(wList, originalWs)); 
    printf("This means you have %d total W's!\n", getTotalCount(wList));
    printf("\e[?25h");
    dumpWs(wList, fptr);
    clearList(wList);
    fclose(fptr);
    return 0; 
}
