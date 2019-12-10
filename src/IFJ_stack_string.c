/* ******************************* IFJ_stack_string.c ****************************
 *  Predmet: Formalni jazyky a prekladace (IFJ) - FIT VUT v Brne
 *  Nazev projektu: Implementace prekladace imperativniho jazyka IFJ19
 *  Autori: Beranek Tomas (xberan46) - vedouci
 *          Haderka Martin (xhader00)
 *          Slobodnik Simon (xslobo06)
 *          Klem Richard (xklemr00)
 *  Datum vytvoreni: 30.11.2019
 * ************************************************************************** */
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

char * stack_top_string(tStack_string* s) {
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
            //free (del);
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
/* konec souboru IFJ_stack_string.c */
