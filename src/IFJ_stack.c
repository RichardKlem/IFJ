/* ******************************* IFJ_stack.c ****************************
 *  Predmet: Formalni jazyky a prekladace (IFJ) - FIT VUT v Brne
 *  Nazev projektu: Implementace prekladace imperativniho jazyka IFJ19
 *  Autori: Beranek Tomas (xberan46) - vedouci
 *          Haderka Martin (xhader00)
 *          Slobodnik Simon (xslobo06)
 *          Klem Richard (xklemr00)
 *  Datum vytvoreni: 15.10.2019
 * ************************************************************************** */
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
/* konec souboru IFJ_stack.c */
