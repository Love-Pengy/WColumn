struct anim; 
typedef struct anim * animation; 

struct animationManager; 
typedef struct animationManager * manager; 

void playRandAnimation(manager); 

void append(animation, char*); 

char * animationCreator(char *, char *); 

animation initAnimation(void); 

manager initManager(void); 

void animationSetup(manager); 

void playRandAnimation(manager); 
