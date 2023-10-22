#include "dllist.h"
#include <stdio.h>


int main (void){

    char *str = "1";
    dllist list = initList();
    dlNode nodeT = createNode(str);
    addList(str, list);
    removeAtIndex(list, 1);
    printf("size: %d\n", listSize(list));
    printList(list);
    addList("2", list);
    addList("3", list);
    addList("4", list);
    removeAtIndex(list, 1); 
    printf("size: %d\n", listSize(list));
    printList(list);
    removeAtIndex(list, listSize(list));
    printf("size: %d\n", listSize(list));
    printList(list);
    clearList(list);
    addList("1", list);
    addList("2", list);
    addList("3", list);
    addList("4", list);
    addList("5", list);
    printf("NEW LIST AFTER CLEAR: ");
    printList(list);
    removeAtIndex(list, 2);
    printf("size: %d\n", listSize(list));
    printList(list);
    clearList(list);    
    addList("1", list);
    addList("2", list);
    addList("3", list);
    addList("4", list);
    addList("5", list);
    printf("NEW LIST AFTER CLEAR: ");
    printList(list);
    replaceAtIndex(list, 2, "changed");
    printf("size: %d\n", listSize(list));
    printList(list);
    replaceAtIndex(list, 1, "changed head");
    printf("size: %d\n", listSize(list));
    printList(list);
    replaceAtIndex(list, listSize(list), "changed tail");
    printf("size: %d\n", listSize(list));
    printList(list);
    printf("PRINTING LIST USING GETITEM: %s, %s, %s, %s, %s\n", (getItemAtIndex(list, 1)), getItemAtIndex(list, 2), getItemAtIndex(list, 3), getItemAtIndex(list, 4), getItemAtIndex(list, 5));
    /*clearList(list);
    str = "2";
    addList(str, list);
    printf("size: %d\n", listSize(list));

    str = "3";
    addList(str, list);
    str = "4";
    addList(str, list);
    str = "5";
    addList(str, list);
    str = "6";
    addList(str, list);
    str = "7";
    addList(str, list);
    str = "8";
    addList(str, list);
    printf("BEFORE DELETE: ");
    printList(list);
    deleteItem(list, 2);
    printf("AFTER DELETE AT INDEX 2: ");
    printList(list);
    deleteItem(list, 0);
    printf("AFTER DELETE AT INDEX 0: ");
    printList(list);
    //breaks here (last index)
    deleteItem(list, 4);
    printf("AFTER DELETE AT INDEX 4: ");
    printList(list);*/
    printf("end\n");
    return 0;
}