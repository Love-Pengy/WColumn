#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
//#include <unistd.h>
char baseDir[30] = "animations/"; 

int DEBUG = 0;

int colors[] = {51, 84, 213, 227};

struct anim{
        int size; 
        char ** frames; 


}; 
typedef struct anim * animation; 


struct animationManager{
        int numAnimations; 
        animation * anims;

};
typedef struct animationManager * manager; 

void setColor(void){
        int randomVal = (rand() % (sizeof(colors)/sizeof(int))); 
        if(DEBUG){
                printf("SETCOLOR: %d\n", randomVal);
        }
        printf("\e[38;5;%dm", colors[randomVal]); 
}

void resetColor(void){
        printf("\e[0m");
}
//OUR SAVIOR, THE CHARMING TRAILINGNLDESTROYER, THE SAVER OF WORLDS, DESTROYER OF EVIL;
char * trailingNLDestroyer(char *str){
    //strcspn returns the value where the inputed value is found
    str[strcspn(str, "\n")] = '\0';
    return(str);
}

void playRandAnimation(manager manage){
        srand(time(NULL)); 
        FILE * holdFp;
        char * hold;
        int picker = (rand() % manage->numAnimations);
        //printf("PICKER: %d\n", manage->numAnimations);
        //int whatever = 0;
        //scanf("%d", &whatever);
        if(DEBUG){
                picker = 2;
                printf("NUM ANIMATIONS: %d, RAND VALUE: %d\n", manage->numAnimations, picker); 
                int something = 0;
                scanf("%d", &something);
        }
        system("cls");
        setColor();
        for(int i = 0; i < manage->anims[picker]->size; i++){ 
                holdFp = fopen(manage->anims[picker]->frames[i], "r");
                if(DEBUG){
                        printf("TEST Dir:  %s, Index: %d\n", manage->anims[picker]->frames[i], i);
                }
                fseek(holdFp, 0, SEEK_END); 
                int size = ftell(holdFp); 
                rewind(holdFp); 
                hold = malloc(size +1); 
                fread(hold, size, 1, holdFp); 
                printf("%s", hold);
                Sleep(50);
                system("cls");
        }
        resetColor();
}

void append(animation appendee, char* newPath){
        strcpy(appendee->frames[appendee->size], newPath);
        appendee->size++;
        if(DEBUG){
                printf("APPEND Dir: %s, Size: %d\n", appendee->frames[appendee->size], appendee->size);

        }   
}


//filenames will come from a file within the folder for the animation
//take basedir and add the next file name onto it
char * animationCreator(char * dir, char * fileName){
        char * hold = malloc(sizeof(char) * (strlen(dir) + strlen(fileName))); 
        strcpy(hold, dir);
        strcat(hold, fileName);
        if(DEBUG){
                printf("AnimationCreator Dir: %s, dir: %s, fileName: %s",  hold, dir, fileName);
        }
        return(hold);
        //int numFrames = 0;
        //printf("TEST: %s\n", filePath);
}

animation initAnimation(void){
       animation new = malloc(sizeof(struct anim)); 
       new->size = 0;
       new->frames = malloc(sizeof(char *) * 10);
       for(int i = 0; i < 10; i++){
                new->frames[i] = malloc(sizeof(char) * 100);
        }
       return(new);
}

manager initManager(void){
        manager new = malloc(sizeof(struct animationManager)); 
        new->numAnimations = 0; 
        new->anims = malloc(sizeof(struct anim) * 10);
        return(new);
}

void animationSetup(manager boss){
        char * filePath = malloc(sizeof(char) * 100);
        FILE * fptr = fopen("animations/manage.txt", "r");
        char * hold = malloc(sizeof(char) * 100);
        animation currAnim = initAnimation();
        char * newBaseDir = malloc(sizeof(char) * 50); 
        char * animFolder = malloc(sizeof(char) * 100);
        fgets(animFolder, 50, fptr);
        trailingNLDestroyer(animFolder);
        strcat(animFolder, "/");
        strcpy(newBaseDir, baseDir);
        strcat(newBaseDir, animFolder);
        while(!feof(fptr)){
                fgets(hold, 99, fptr);
                trailingNLDestroyer(hold);
                if(DEBUG){
                printf("animationSetup original hold: %s\n", hold);
                }
                if(hold[0] == '~'){
                        boss->anims[boss->numAnimations] = currAnim;
                        boss->numAnimations++;
                        currAnim = initAnimation();
                        fgets(animFolder, 50, fptr);
                        trailingNLDestroyer(animFolder); 
                        strcat(animFolder, "/");
                        strcpy(newBaseDir, baseDir); 
                        strcat(newBaseDir, animFolder);
                        continue;
                }
                hold = animationCreator(newBaseDir, hold);
                if(DEBUG){
                        printf("animationSetup anim directory: %s\n", hold);
                }
                append(currAnim, hold);
        }
                //boss->anims[boss->numAnimations] = currAnim; 
                //boss->numAnimations++; 


}



