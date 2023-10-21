struct node;
typedef struct node *dlNode;

struct header;
typedef struct header *dllist;


dllist initList();
dlNode createNode(char *);
int listSize(dllist);
dllist addList(char *, dllist);
void clearList(dllist);
void deleteItem(dllist, int);
void printList(dllist);
