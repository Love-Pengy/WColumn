

//index starts at 0 (head) 
struct llist{
    node head;
    node next; 
    node prev; 
    int index; 
};

typedef struct llist *node;

//initiallizes doubly linked list 
node initList(){
    node new = malloc(sizeof(struct llist));
    new->head = NULL;
    new->next = NULL; 
    new->prev = NULL;
    new->index = 0;
}