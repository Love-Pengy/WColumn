#include <stdio.h>
#include <windows.h>
#include <stdbool.h>
#include <conio.h>
int main (void){
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
    for(int i = 0; i < 10; i++){
        printf(" ");
    }

    printf("\e[1m");
    printf("\e[48;5;230m");
    printf("\e[38;5;232m");
    printf("EDIT: e");
    printf("\e[0m");
    for(int i = 0; i < 10; i++){
        printf(" ");
    }
    printf("\e[1m");
    printf("\e[48;5;230m");
    printf("\e[38;5;232m"); 
    printf("DELETE: d");
    printf("\e[0m");
    for(int i = 0; i < 10; i++){
        printf(" ");
    }
    printf("\e[1m");
    printf("\e[48;5;230m");
    printf("\e[38;5;232m"); 
    printf("EXIT: r");
    printf("\e[0m");
    printf("\n");
    //system("cls");






    char something = ' ';
    scanf("%c", &something);
    return(0);
}