/* ******************************* IFJ_scanner.c ****************************
 *  Predmet: Formalni jazyky a prekladace (IFJ) - FIT VUT v Brne
 *  Nazev projektu: Implementace prekladace imperativniho jazyka IFJ19
 *  Autori: Beranek Tomas (xberan46) - vedouci
 *          Haderka Martin (xhader00)
 *          Slobodnik Simon (xslobo06)
 *          Klem Richard (xklemr00)
 *  Datum vytvoreni: 12.10.2019
 * ************************************************************************** */
 /**
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * ______________!!! W I P !!!______________
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

#include <stdlib.h>
#include <stdbool.h>
#include "IFJ_error.h"
#include "IFJ_stack.h"
#include "IFJ_precedence_table.h"

void exprStackInit (tExprStack* s) {
    if (s == NULL)
        error_exit(ERROR_INTERNAL);
    else
        s->top = NULL;
}

int epxrStackEmpty (tExprStack* s) {
    if (s == NULL)
        error_exit(ERROR_INTERNAL);
    else
        return s->top == NULL;
}

int exprStackTop (tExprStack* s) {
    if (stackEmpty(s) || stackEmpty(s))
        error_exit(ERROR_INTERNAL);
    else
        return s->top->data;
}

void exprStackPop (tExprStack* s) {
    if (s == NULL)
        error_exit(ERROR_INTERNAL);
    else if (!stackEmpty(s)) {
        tElem* del = s->top;
        s->top = s->top->next;
        free (del);
    }
}

void exprStackPush (tExprStack* s, expr_token_t item) {
    if (s == NULL)
        error_exit(ERROR_INTERNAL);
    else {
        tElem* insert = (tExprElem*)malloc(sizeof(tExprElem));
        if (insert == NULL)
            error_exit(ERROR_INTERNAL);

        insert->exprToken = item;
        insert->next = s->top;
        s->top = insert;
    }
}

expr_token_t* find_top_terminal(tExprStack* s)
{
    if (s == NULL)
        error_exit(ERROR_INTERNAL);
    else {
        tExprElem* top_terminal= &(s->top);
        while (top_terminal->exprToken.terminal == false)
        {
            if (top_terminal->next == NULL) //uz neni zadny prvek a my jsme nenasli zadny terminal
                error_exit(ERROR_INTERNAL);
            top_terminal = top_terminal->next;
        }
        return top_terminal;
    }
}

precedence_table =
         {
        //  | +-   |   /*  |   (   |    )  |   r   |   fc  |   var |    $  |
/*|  +- |*/ {REDUCE, SHIFT,  SHIFT,  REDUCE, XXXXXX, XXXXXX, SHIFT,  REDUCE},
/*|  /* |*/ {REDUCE, REDUCE, SHIFT,  REDUCE, XXXXXX, XXXXXX, SHIFT,  REDUCE},
/*|  (  |*/ {SHIFT,  XXXXXX, SHIFT,  EQUAL,  XXXXXX, XXXXXX, SHIFT,  ERROR },
/*|  )  |*/ {REDUCE, XXXXXX, ERROR,  REDUCE, XXXXXX, XXXXXX, ERROR,  REDUCE},
/*| rel |*/ {SHIFT,  XXXXXX, XXXXXX, XXXXXX, XXXXXX, XXXXXX, SHIFT,  REDUCE},
/*| fc  |*/ {ERROR,  ERROR,  EQUAL,  ERROR,  ERROR,  ERROR,  ERROR,  REDUCE},
/*| var |*/ {REDUCE, REDUCE, ERROR,  REDUCE, REDUCE, ERROR,  ERROR,  REDUCE},
/*|  $  |*/ {SHIFT,  SHIFT,  SHIFT,  ERROR,  SHIFT,  ERROR,  SHIFT,  ERROR }};