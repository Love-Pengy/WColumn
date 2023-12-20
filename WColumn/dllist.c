#include <stdlib.h>
#include <string.h>
#include <stdio.h>
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

dllist initList(){
    dllist new = malloc(sizeof(struct header));
    new->head = NULL;
    new->tail = NULL;
    new->size = 0;
    return(new);
}

dlNode createNode(char *str){
    if(strlen(str) == 0){
        printf("CREATENODE PASSED EMPTY STRING\n");
        return(NULL);
    }
    dlNode newNode = malloc(sizeof(struct node));
    newNode->data = malloc(sizeof(char)*80);
    (newNode)->next = NULL;
    (newNode)->prev = NULL;
    strcpy((newNode)->data, str);
    return(newNode);
}

int listSize(dllist dl){
    return(dl->size);
}

dllist addList(char *str, dllist dl){
    if(strcmp(str, "") == 0){
        printf("ADDLIST PASSED NULL STRING\n");
        return(NULL);
    }
    if(dl->size == 0){
        dl->head = createNode(str);
        dl->size = 1;
        return(dl);
    }
    else if(dl->size == 1){
        dl->tail = createNode(str);
        dl->head->next = dl->tail;
        dl->tail->prev = dl->head;
        dl->size++;
        return(dl);
    }
    else{
        dlNode tmp; 
        tmp = dl->tail; 
        dl->tail = createNode(str);
        dl->tail->prev = tmp;
        tmp->next = dl->tail;
        dl->size++;
        return(dl);
    }
}

void clearList(dllist d){
    if(d->size == 0){
        
    }
    else if(d->size == 1){
        free(d->head);
        d->head = NULL;
        d->size = 0;
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

void printList(dllist d){
    if((listSize(d) == 0) || (d == NULL)){
        printf("EMPTY!!: {}\n");
    }
    else if(listSize(d) == 1){
        printf("{%s}\n", d->head->data);
    }
    else{
        dlNode ptr = d->head;
        printf("{");
        while(ptr->next != NULL){
            printf("%s,", ptr->data);
            ptr = ptr->next;
        }
        printf("%s}\n", ptr->data);
    }
}

void removeAtIndex(dllist list, int index){
    if(list == NULL){
        printf("deleteItem passed NULL dllist\n");
    }
    else if((index > listSize(list)) || (index < 1)){
        printf("deleteItem passed invalid index\n");
    }
    else if(listSize(list) == 1){
        clearList(list);
    }
    else if(index == 1){
        dlNode newHead = list->head->next;
        free(list->head);
        list->head = newHead;
        list->size--;
    }
    else{
        dlNode delPtr = list->head;
        for(int i = 0; i < (index - 1); i++){
            delPtr = delPtr->next; 
        }
        if(delPtr->next != NULL){
            dlNode prevPtr = delPtr->prev; 
            dlNode nextPtr = delPtr->next; 
            prevPtr->next = nextPtr; 
            nextPtr->prev = prevPtr; 
            free(delPtr);
            list->size--;
        }
        if(delPtr->next == NULL){
            dlNode prevPtr = delPtr->prev;
            free(list->tail);
            list->tail = prevPtr;
            list->size--;
        }
    }
}

void replaceAtIndex(dllist list, int index, char* string){
    if((index > listSize(list)) || (index < 1)){
        printf("addAtIndex passed invalid index\n");
    }
    else if((string == NULL) || (!strcmp(string, " ")) || (!strcmp(string, "\n"))){
        printf("addAtIndex passed invalid string\n");
    } 
    else if(index == 1){
        strcpy(list->head->data, string);
    }
    else if(index == listSize(list)){
        strcpy(list->tail->data, string);
}
    else{
        dlNode ptr = list->head;
        for(int i = 0; i < (index - 1); i++){
            ptr = ptr->next;
        }
        strcpy(ptr->data, string); 
    }
}

char * getItemAtIndex(dllist list, int index){
    if((index < 1) || (index > listSize(list))){
        printf("getItemAtIndex passed invalid index: %d", index);
        return(NULL);
    }
    else{ 
        dlNode ptr = list->head; 
        for(int i = 0; i < (index - 1); i++){
            ptr = ptr->next;
        }
        return(ptr->data);
    }
}

