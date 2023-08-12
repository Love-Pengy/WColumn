#include "dllist.c"



int main (void){

    char *str = "something";
    dllist list = initList();
    dlNode nodeT = createNode(str);
    addList(str, list);
    clearList(list);
    str = "something2";
    addList(str, list);
    printf("size: %d\n", listSize(list));
    printList(list);
    printf("end\n");
    return 0;
}