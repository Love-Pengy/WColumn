#include "dllist.c"



int main (void){

    char *str = "something";
    dllist list = initList();
    //dlNode nodeT = createNode(str);
    addList(str, list);
    str = "something2";
    addList(str, list);


    printList(list);
    printf("end\n");
    return 0;
}