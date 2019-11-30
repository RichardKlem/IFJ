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
#include "IFJ_scanner.h"
#include "IFJ_error.h"
#include "IFJ_stack.h"
#include "IFJ_precedence_table.h"
//______Gen stack functions_________
void genStackInit (tGenStack* s) {
    if (s == NULL)
        error_exit(ERROR_INTERNAL);
    else
        s->top = NULL;
}

int genStackEmpty (tGenStack* s) {
    if (s == NULL)
        error_exit(ERROR_INTERNAL);
    else
        return s->top == NULL;
}

token_t genStackTop (tGenStack* s) {
    if (genStackEmpty(s))
        error_exit(ERROR_INTERNAL);
    else
        return s->top->genToken;
}

void genStackPop (tGenStack* s) {
    if (s == NULL)
        error_exit(ERROR_INTERNAL);
    else if (!genStackEmpty(s)) {
        tGenElem* del = s->top;
        s->top = s->top->next;
        free (del);
    }
}

void genStackPush (tGenStack* s, token_t item) {
    if (s == NULL)
        error_exit(ERROR_INTERNAL);
    else {
        tGenElem* insert = (tGenElem *)malloc(sizeof(tGenElem));
        if (insert == NULL)
            error_exit(ERROR_INTERNAL);

        insert->genToken= item;
        insert->next = s->top;
        s->top = insert;
    }
}

//________Expression stack functions___________
void exprStackInit (tExprStack* s) {
    if (s == NULL)
        error_exit(ERROR_INTERNAL);
    else
        s->top = NULL;
}

int exprStackEmpty (tExprStack* s) {
    if (s == NULL)
        error_exit(ERROR_INTERNAL);
    else
        return s->top == NULL;
}

expr_token_t exprStackTop (tExprStack* s) {
    if (exprStackEmpty(s) || exprStackEmpty(s))
        error_exit(ERROR_INTERNAL);
    else
        return s->top->exprToken;
}

void exprStackPop (tExprStack* s) {
    if (s == NULL)
        error_exit(ERROR_INTERNAL);
    else if (!exprStackEmpty(s)) {
        tExprElem* del = s->top;
        s->top = s->top->next;
        free (del);
    }
}

void exprStackPush (tExprStack* s, expr_token_t item) {
    if (s == NULL)
        error_exit(ERROR_INTERNAL);
    else {
        tExprElem* insert = (tExprElem*)malloc(sizeof(tExprElem));
        if (insert == NULL)
            error_exit(ERROR_INTERNAL);

        insert->exprToken = item;
        insert->next = s->top;
        s->top = insert;
    }
}

expr_token_t * find_top_terminal(tExprStack* s)
{
    if (s == NULL)
        error_exit(ERROR_INTERNAL);
    else {
        tExprElem * top_terminal= s->top;
        while (top_terminal->exprToken.terminal == false)
        {
            if (top_terminal->next == NULL) //uz neni zadny prvek a my jsme nenasli zadny terminal
            {   //printf("TOP TERMINAL FUNCTION KDYZ UZ NENNI ZADNY PRVEK A JA NEMAM TERMINAL\n");
                error_exit(ERROR_SYNTAX);}
            top_terminal = top_terminal->next;
        }
        if (top_terminal->exprToken.terminal == true)
            return &(top_terminal->exprToken);
        else
        {    //printf("prosli jsme cely stack a zadny temrinal zde neni ackoli by mel byt\n");
            error_exit(ERROR_SYNTAX);//prosli jsme cely stack a zadny temrinal zde neni ackoli by mel byt
        }
    }
}

precedence_rule precedence_table[8][8] =
         {
           //  |  +-  | // / *|   (   |    )  |  rel  |   fc  |   var |    $  |
/*|   +-   |*/ {REDUCE, SHIFT,  SHIFT,  REDUCE, REDUCE, XXXXXX, SHIFT,  REDUCE},
/*| // / * |*/ {REDUCE, REDUCE, SHIFT,  REDUCE, REDUCE, XXXXXX, SHIFT,  REDUCE},
/*|   (    |*/ {SHIFT,  SHIFT,  SHIFT,  EQUAL,  SHIFT,  SHIFT,  SHIFT,  ERROR },
/*|   )    |*/ {REDUCE, REDUCE, ERROR,  REDUCE, REDUCE, XXXXXX, ERROR,  REDUCE},
/*|  rel   |*/ {SHIFT,  SHIFT,  SHIFT,  REDUCE, ERROR,  XXXXXX, SHIFT,  REDUCE},
/*|  fc    |*/ {ERROR,  ERROR,  EQUAL,  ERROR,  ERROR,  ERROR,  ERROR,  REDUCE},
/*|  var   |*/ {REDUCE, REDUCE, ERROR,  REDUCE, REDUCE, ERROR,  ERROR,  REDUCE},
/*|   $    |*/ {SHIFT,  SHIFT,  SHIFT,  ERROR,  SHIFT,  SHIFT,  SHIFT,  ERROR }};
