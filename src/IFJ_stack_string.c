#include "IFJ_stack_string.h"
#include "IFJ_error.h"
#include <stdlib.h>

void stack_init_string(tStack_string* s) {
	if (s == NULL)
        error_exit(ERROR_INTERNAL);
	else
		s->top = NULL;
}

int stack_empty_string(tStack_string* s) {
    if (s == NULL)
        error_exit(ERROR_INTERNAL);
    else
        return s->top == NULL;
}

int stack_top_string(tStack_string* s) {
	if (stack_empty_string(s) || stack_empty_string(s))
		error_exit(ERROR_INTERNAL);
	else
		return s->top->data;
}

void stack_pop_string(tStack_string* s) {
	if (s == NULL)
        error_exit(ERROR_INTERNAL);
	else if (!stack_empty_string(s)) {
            tElem_string* del = s->top;
            s->top = s->top->next;
            free (del);
        }
}

void stack_push_string(tStack_string* s, char * data) {
	if (s == NULL)
		error_exit(ERROR_INTERNAL);
	else {
        tElem_string* insert = (tElem_string*)malloc(sizeof(tElem_string));
        if (insert == NULL)
            error_exit(ERROR_INTERNAL);

        insert->data = data;
        insert->next = s->top;
        s->top = insert;
	}
}
