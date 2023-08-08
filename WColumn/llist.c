#include <stdlib.h>
#include <string.h>
 struct node{
    struct node *next; 
    struct node *prev; 
    char *data;
 };
 typedef struct node *dlNode;

struct header{
    struct node *head; 
    struct node *tail;
    int size; 
};
typedef struct header *dllist;

//initiallizes doubly linked list 
dllist initList(){
    dllist new = malloc(sizeof(struct header));
    new->head = NULL;
    new->tail = NULL;
    new->size = 0;
    return(new);
}

dlNode *createNode(char *str){
    dlNode newNode = malloc(sizeof(struct node));
    newNode->next = NULL;
    newNode->prev = NULL;
    strcpy(newNode->data, str);
    return(&newNode);
}

int listSize(dllist dl){
    return(dl->size);
}

//take string and header node and insert string into dllist at end
dllist addList(char *str, dllist dl){
    //if there are no nodes create one and set it as head
    if(dl->size == 0){
        dl->head = createNode(str);
        dl->size = 1;
        return(dl);
    }
    //if there is just a head then create node and put it after the head
    else if(dl->size == 1){
        dl->tail = createNode(str);
        dl->head->next = dl->tail;
        dl->tail->prev = dl->head;
        return(dl);
    }
    //if dllist is already created then add node at the end like normal 
    else{
        dlNode tmp; 
        tmp = dl->tail; 
        dl->tail = createNode(str);
        dl->tail->prev = tmp;
        tmp->next = dl->tail;
        return(dl);
    }
}

void clearList(dllist d){
    if(d->size == 0){
        free(d);
    }
    else if(d->size == 1){
        free(d->head);
        d->head = NULL;
    }
    else{
        dlNode tmp;
        dlNode destroyerOfWorlds;
        tmp = d->tail;
        while(tmp->prev != NULL){
            destroyerOfWorlds = tmp; 
            tmp = destroyerOfWorlds->prev;
            free(destroyerOfWorlds);
            d->size--;
        }
        if(d->size == 1){
            free(d->head);
            d->size--;
        }
        d->head = NULL;
        d->tail = NULL;
        d->size = 0;
    }
}