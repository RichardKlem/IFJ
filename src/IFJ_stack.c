#include "IFJ_stack.h"
#include "IFJ_error.h"
#include <stdlib.h>

void stackInit (tStack* s) {
	if (s == NULL)
        error_exit(ERROR_INTERNAL);
	else
		s->top = NULL;
}

int stackEmpty (tStack* s) {
    if (s == NULL)
        error_exit(ERROR_INTERNAL);
    else
        return s->top == NULL;
}

int stackTop (tStack* s) {
	if (stackEmpty(s) || stackEmpty(s))
		error_exit(ERROR_INTERNAL);
	else
		return s->top->data;
}

void stackPop (tStack* s) {
	if (s == NULL)
        error_exit(ERROR_INTERNAL);
	else if (!stackEmpty(s)) {
            tElem* del = s->top;
            s->top = s->top->next;
            free (del);
        }
}

void stackPush (tStack* s, int data) {
	if (s == NULL)
		error_exit(ERROR_INTERNAL);
	else {
        tElem* insert = (tElem*)malloc(sizeof(tElem));
        if (insert == NULL)
            error_exit(ERROR_INTERNAL);

        insert->data = data;
        insert->next = s->top;
        s->top = insert;
	}
}
