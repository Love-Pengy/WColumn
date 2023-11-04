struct node;
typedef struct node *dlNode;

struct header;
typedef struct header *dllist;


dllist initList();
dlNode createNode(char *);
int listSize(dllist);
dllist addList(char *, dllist);
void clearList(dllist);
void removeAtIndex(dllist, int);
void printList(dllist);
void replaceAtIndex(dllist, int, char *);
char * getItemAtIndex(dllist, int);
