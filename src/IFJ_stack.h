#ifndef _IFJ_STACK_H_
#define _IFJ_STACK_H_

typedef struct telem{
    int data;
    struct telem* next;
} tElem;

typedef struct {
        tElem* top;
} tStack;

void stackInit (tStack* s);
int stackEmpty (tStack* s);
int stackTop (tStack* s);
void stackPop (tStack* s);
void stackPush (tStack* s, int data);

#endif
