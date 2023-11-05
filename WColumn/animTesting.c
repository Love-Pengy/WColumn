#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char baseDir[12] = "animations/"; 

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

void playRandAnimation(manager manage){
        srand(1); 
        FILE * holdFp;
        char * hold; 
        int picker = (rand() % manage->numAnimations);
        for(int i = 0; i < manage->anims[picker]->size; i++){ 
                holdFp = fopen(manage->anims[picker]->frames[i], "r");
                fseek(holdFp, 0, SEEK_END); 
                int size = ftell(holdFp); 
                rewind(holdFp); 
                hold = malloc(size +1); 
                fread(hold, size, 1, holdFp); 
                printf("%s", hold); 
        }
}

void append(animation appendee, char* newPath){
        appendee->frames[appendee->size] = newPath; 
        appendee->size++;
}


//filenames will come from a file within the folder for the animation
//take basedir and add the next file name onto it
char * animationCreator(char * dir, char * fileName){
        char * hold = malloc(sizeof(char) * (strlen(dir) + strlen(fileName))); 
        strcpy(hold, dir);
        strcat(hold, fileName);
        return(hold);
}

animation initAnimation(void){
       animation new = malloc(sizeof(struct anim)); 
       new->size = 0; 
       for(int i = 0; i < 10; i++){
                new->frames[i] = malloc(sizeof(char) * 50);
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
        char * filePath = malloc(sizeof(char) * 24);
        strcpy(filePath,  baseDir);
        strcat(filePath, "manage.txt");
        FILE * fptr = fopen(filePath, "r"); 
        char * hold = malloc(sizeof(char) * 100);
        animation currAnim = initAnimation();
        int numFrames = 0;
        while(!feof(fptr)){
                fgets(hold, 99, fptr); 
                if(hold[0] == '~'){
                        boss->anims[boss->numAnimations] = currAnim;
                        boss->numAnimations++;
                        currAnim = initAnimation();
                        continue;
                }
                hold = animationCreator(baseDir, hold);
                append(currAnim, hold);
        }
                boss->anims[boss->numAnimations] = currAnim; 
                boss->numAnimations++; 


}



int main (void){
//within this file thre is a ~ and then the folder in which the animation resides
//then the frame names



}
