#ifndef IFJ_STACK_STRING_H_INCLUDED
#define IFJ_STACK_STRING_H_INCLUDED

typedef struct telem_string{
    char * data;
    struct telem_string* next;
} tElem_string;

typedef struct {
        tElem_string* top;
} tStack_string;

void stack_init_string (tStack_string* s);
int stack_empty_string (tStack_string* s);
int stack_top_string (tStack_string* s);
void stack_pop_string (tStack_string* s);
void stack_push_string (tStack_string* s, int data);

#endif // IFJ_STACK_STRING_H_INCLUDED
