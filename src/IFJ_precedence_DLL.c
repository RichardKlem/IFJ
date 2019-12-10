/* ******************************* IFJ_precedence_DLL.c ****************************
 *  Predmet: Formalni jazyky a prekladace (IFJ) - FIT VUT v Brne
 *  Nazev projektu: Implementace prekladace imperativniho jazyka IFJ19
 *  Autori: Beranek Tomas (xberan46) - vedouci
 *          Haderka Martin (xhader00)
 *          Slobodnik Simon (xslobo06)
 *          Klem Richard (xklemr00)
 *  Datum vytvoreni: 27.11.2019
 * ************************************************************************** */

#include <stdlib.h>
#include <stdbool.h>
#include "IFJ_scanner.h"
#include "IFJ_precedence_table.h"
#include "IFJ_precedence_DLL.h"


void exprDLError() {

    //fprintf (stderr,"*ERROR* PSA expression DLL\n");
}

void exprDLInitList (tExprDLList *L) {

    L->First = NULL;
    L->Act = NULL;
    L->Last = NULL;
}

void exprDLDisposeList (tExprDLList *L) {

    if(L->First != NULL)
    {
        do
        {
            L->Act = L->Last->lptr;
            //free(L->Last);
            L->Last = L->Act;
        } while(L->Last != NULL);
        L->First = NULL;
    }
}

void exprDLInsertFirst (tExprDLList *L, expr_token_t exprToken) {

        tExprDLElemPtr newElem = malloc(sizeof(struct tExprDLElem));
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
}

void exprDLInsertLast(tExprDLList *L, expr_token_t exprToken) {

    tExprDLElemPtr newElem = malloc(sizeof(struct tExprDLElem));
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
}

void exprDLFirst (tExprDLList *L) {

    L->Act = L->First;
}

void exprDLLast (tExprDLList *L) {

    L->Act = L->Last;
}

void exprDLCopyFirst (tExprDLList *L, expr_token_t *exprToken) {

    if(L->First == NULL)
    {
        exprDLError();
        return;
    }
    *exprToken = L->First->exprToken;
}

void exprDLCopyLast (tExprDLList *L, expr_token_t *exprToken) {

    if(L->Last == NULL)
    {
        exprDLError();
        return;
    }
    *exprToken = L->Last->exprToken;
}

void exprDLDeleteFirst (tExprDLList *L) {
    if(L->First != NULL)
    {
        if(L->First == L->Act)
            L->Act = NULL;
        if(L->First == L->Last)
        {
            //free(L->First);
            L->First = NULL;
            L->Last = NULL;
        }
        else
        {
            (L->First->rptr)->lptr = NULL;
            L->First = L->First->rptr;
            //free((L->First)->lptr);
        }
    }
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
    //free(tempElem);
}

void exprDLPostDelete (tExprDLList *L) {

    if ((L->Act == L->Last) || (L->Act == NULL))
    {
        return;
    }
    if(L->Act->rptr == L->Last)
    {
        //free(L->Act->rptr);
        L->Last = L->Act;
        L->Act->rptr = NULL;
    }
    else
    {
        tExprDLElemPtr tempElem;
        tempElem = L->Act->rptr;
        L->Act->rptr = L->Act->rptr->rptr;
        L->Act->rptr->rptr->lptr = L->Act;
        //free(tempElem);
    }
}

void exprDLPreDelete (tExprDLList *L) {

    if ((L->Act == L->First) || (L->Act == NULL))
    {
        return;
    }
    if(L->Act->lptr == L->First)
    {
        //free(L->Act->lptr);
        L->First = L->Act;
        L->Act->lptr = NULL;
    }
    else
    {
        tExprDLElemPtr tempElem;
        tempElem = L->Act->lptr;
        L->Act->lptr = L->Act->lptr->lptr;
        L->Act->lptr->lptr->rptr = L->Act;
        //free(tempElem);
    }
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
}

void exprDLCopy (tExprDLList *L, expr_token_t *exprToken) {

    if(exprDLActive(L) == 0)
    {
        exprDLError();
        return;
    }
    *exprToken = L->Act->exprToken;

}

void exprDLActualize (tExprDLList *L, expr_token_t exprToken) {

    if(L->Act == NULL)
    {
        return;
    }
    L->Act->exprToken = exprToken;
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
/* konec souboru IFJ_precedence_DLL.c */
