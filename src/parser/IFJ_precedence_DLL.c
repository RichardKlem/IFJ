/* ******************************* IFJ_scanner.c ****************************
 *  Predmet: Formalni jazyky a prekladace (IFJ) - FIT VUT v Brne
 *  Nazev projektu: Implementace prekladace imperativniho jazyka IFJ19
 *  Autori: Beranek Tomas (xberan46) - vedouci
 *          Haderka Martin (xhader00)
 *          Slobodnik Simon (xslobo06)
 *          Klem Richard (xklemr00)
 *  Datum vytvoreni: 12.10.2019
 * ************************************************************************** */

#include <stdlib.h>
#include <stdbool.h>
#include "IFJ_scanner.h"
#include "IFJ_precedence_DLL.h"


void exprDLError() {

    fprintf (stderr,"*ERROR* PSA expression DLL\n");
    return;
}

void eInitList (tExprDLList *L) {

    L->First = NULL;
    L->Act = NULL;
    L->Last = NULL;
    return;
}

void exprDLDisposeList (tExprDLList *L) {

    if(L->First != NULL)
    {
        do
        {
            L->Act = L->Last->lptr;
            free(L->Last);
            L->Last = L->Act;
        } while(L->Last != NULL);
        L->First = NULL;
    }
}

void exprDLInsertFirst (tExprDLList *L, expr_token_t exprToken) {

    tExprDLElemPtr newElem = (tExprDLElemPtr) malloc (sizeof(tExprDLElemPtr));// tExprDLElemPtr je struktura definovaná jako pointer, jak tedy mallocovat
    if (newElem == NULL)    //Ověření zda alokace proběhla úspěšně
    {
        exprDLError();
        exit(1);
    }
    newElem->exprToken = exprToken;    //Přiřazení dat do nového prvku
    newElem->exprToken.terminal = true;
    newElem->exprToken.shifted = false;
    newElem->lptr = NULL;
    if(L->First != NULL)    //Pokud už seznam nějaké prvky má
    {
        newElem->rptr = L->First;
        L->First->lptr = newElem;
    }
    else    //Pokud je seznam prázdný
    {
        newElem->lptr = NULL;
        L->Last = newElem;
    }

    L->First = newElem;
    return;
}

void exprDLInsertLast(tExprDLList *L, expr_token_t exprToken) {

    tExprDLElemPtr newElem = (tExprDLElemPtr) malloc(sizeof(tExprDLElemPtr));
    if (newElem == NULL)
    {
        exprDLError();
        exit(1);
    }
    newElem->exprToken = exprToken;
    newElem->exprToken.terminal = true;
    newElem->exprToken.shifted = false;
    newElem->rptr = NULL;
    if(L->Last != NULL)
    {
        L->Last->rptr = newElem;
        newElem->lptr = L->Last;
    }
    else
    {
        newElem->lptr = NULL;
        L->First = newElem;
    }

    L->Last = newElem;
    return;
}

void exprDLFirst (tExprDLList *L) {

    L->Act = L->First;
    return;
}

void exprDLLast (tExprDLList *L) {

    L->Act = L->Last;
    return;
}

void exprDLCopyFirst (tExprDLList *L, expr_token_t *exprToken) {

    if(L->First == NULL)
    {
        exprDLError();
        return;
    }
    *exprToken = L->First->exprToken;
    return;

}

void exprDLCopyLast (tExprDLList *L, expr_token_t *exprToken) {

    if(L->Last == NULL)
    {
        exprDLError();
        return;
    }
    *exprToken = L->Last->exprToken;
    return;
}

void exprDLDeleteFirst (tExprDLList *L) {

    if (L->First == NULL)
    {
        return;
    }
    else if (L->First == L->Act)
    {
        L->Act = NULL;
    }
    tExprDLElemPtr tempElem;
    tempElem = L->First;
    L->First = L->First->rptr;
    L->First->lptr = NULL;
    free(tempElem);
    return;
}

void exprDLDeleteLast (tExprDLList *L) {

    if (L->Last == NULL)
    {
        return;
    }
    else if (L->Last == L->Act)
    {
        L->Act = NULL;
    }
    tExprDLElemPtr tempElem;
    tempElem = L->Last;
    L->Last = L->Last->lptr;
    L->Last->rptr = NULL;
    free(tempElem);
    return;
}

void exprDLPostDelete (tExprDLList *L) {

    if ((L->Act == L->Last) || (L->Act == NULL))
    {
        return;
    }
    if(L->Act->rptr == L->Last)
    {
        free(L->Act->rptr);
        L->Last = L->Act;
        L->Act->rptr = NULL;
    }
    else
    {
        tExprDLElemPtr tempElem;
        tempElem = L->Act->rptr;
        L->Act->rptr = L->Act->rptr->rptr;
        L->Act->rptr->rptr->lptr = L->Act;
        free(tempElem);
    }
    return;
}

void exprDLPreDelete (tExprDLList *L) {

    if ((L->Act == L->First) || (L->Act == NULL))
    {
        return;
    }
    if(L->Act->lptr == L->First)
    {
        free(L->Act->lptr);
        L->First = L->Act;
        L->Act->lptr = NULL;
    }
    else
    {
        tExprDLElemPtr tempElem;
        tempElem = L->Act->lptr;
        L->Act->lptr = L->Act->lptr->lptr;
        L->Act->lptr->lptr->rptr = L->Act;
        free(tempElem);
    }
    return;
}

void exprDLPostInsert (tExprDLList *L, expr_token_t exprToken) {

    if(L->Act == NULL)
    {
        return;
    }

    tExprDLElemPtr newElem = (tExprDLElemPtr) malloc(sizeof(tExprDLElemPtr));
    if (newElem == NULL)
    {
        exprDLError();
        exit(1);
    }
    newElem->exprToken= exprToken;
    newElem->lptr = L->Act;
    newElem->rptr = L->Act->rptr;
    L->Act->rptr = newElem;
    if(L->Act == L->Last)
    {
        L->Last = newElem;
    }
    else
    {
        newElem->rptr->lptr = newElem;
    }
    return;
}

void exprDLPreInsert (tExprDLList *L, expr_token_t exprToken) {

    if(L->Act == NULL)
    {
        return;
    }

    tExprDLElemPtr newElem = (tExprDLElemPtr) malloc(sizeof(tExprDLElemPtr));
    if (newElem == NULL)
    {
        exprDLError();
    }

    newElem->exprToken = exprToken;
    newElem->rptr = L->Act;
    newElem->lptr = L->Act->lptr;
    L->Act->lptr = newElem;
    if(L->Act == L->First)
    {
        L->First = newElem;
    }
    else
    {
        newElem->lptr->rptr = newElem;
    }
    return;
}

void exprDLCopy (tExprDLList *L, expr_token_t *exprToken) {

    if(exprDLActive(L) == 0)
    {
        exprDLError();
        return;
    }
    *exprToken = L->Act->exprToken;
    return;
}

void exprDLActualize (tExprDLList *L, expr_token_t exprToken) {

    if(L->Act == NULL)
    {
        return;
    }
    L->Act->exprToken = exprToken;
    return;
}

void exprDLSucc (tExprDLList *L) {

    if(L->Act == NULL)
    {
        return;
    }
    if(L->Act == L->Last)
    {
        L->Act = NULL;
    }
    else
    {
        L->Act = L->Act->rptr;
    }
    return;

}


void exprDLPred (tExprDLList *L) {

    if(L->Act == NULL)
    {
        return;
    }
    if(L->Act == L->First)
    {
        L->Act = NULL;
    }
    else
    {
        L->Act = L->Act->lptr;
    }
    return;
}

int exprDLActive (tExprDLList *L) {

    if(L->Act != NULL)
    {
        return 1;
    }
    else
    {
        return 0;
    }

}