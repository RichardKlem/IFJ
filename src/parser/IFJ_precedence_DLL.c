/* ******************************* IFJ_scanner.c ****************************
 *  Predmet: Formalni jazyky a prekladace (IFJ) - FIT VUT v Brne
 *  Nazev projektu: Implementace prekladace imperativniho jazyka IFJ19
 *  Autori: Beranek Tomas (xberan46) - vedouci
 *          Haderka Martin (xhader00)
 *          Slobodnik Simon (xslobo06)
 *          Klem Richard (xklemr00)
 *  Datum vytvoreni: 12.10.2019
 * ************************************************************************** */

#include "IFJ_precedence_ExprDLL.h"


void ExprDLError() {

    fprintf (stderr,"*ERROR* Table of Instructions.\n");
    return;
}

void ExprDLInitList (tExprDLList *L) {

    L->First = NULL;
    L->Act = NULL;
    L->Last = NULL;
    return;

}

void ExprDLDisposeList (tExprDLList *L) {

    if (L->First == NULL)
    {
        ExprDLError();
        return;
    }

    tExprDLElemPtr *tempElem = L->Last;
    while (L->Last != L->First)
    {
        L->Last->lptr = L->Last;
        free(tempElem);
    }
    *tempElem = L->First;
    L->First = NULL;
    L->Act = NULL;
    L->Last = NULL;
    free(tempElem);
    return;
}

void ExprDLInsertFirst (tExprDLList *L, tInstruction val) {

    tExprDLElemPtr newElem = (tExprDLElemPtr) malloc (sizeof(tExprDLElemPtr));// tExprDLElemPtr je struktura definovaná jako pointer, jak tedy mallocovat
    if (newElem == NULL)    //Ověření zda alokace proběhla úspěšně
    {
        ExprDLError();
        exit(1);
    }
    newElem->instr = val;    //Přiřazení dat do nového prvku
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

void ExprDLInsertLast(tExprDLList *L, tInstruction val) {

    tExprDLElemPtr newElem = (tExprDLElemPtr) malloc(sizeof(tExprDLElemPtr));
    if (newElem == NULL)
    {
        ExprDLError();
        exit(1);
    }
    newElem->instr = val;
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

void ExprDLFirst (tExprDLList *L) {

    L->Act = L->First;
    return;
}

void ExprDLLast (tExprDLList *L) {

    L->Act = L->Last;
    return;
}

void ExprDLCopyFirst (tExprDLList *L, tInstruction *val) {

    if(L->First == NULL)
    {
        ExprDLError();
        return;
    }
    *val = L->First->instr;
    return;

}

void ExprDLCopyLast (tExprDLList *L, tInstruction *val) {

    if(L->Last == NULL)
    {
        ExprDLError();
        return;
    }
    *val = L->Last->instr;
    return;
}

void ExprDLDeleteFirst (tExprDLList *L) {

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

void ExprDLDeleteLast (tExprDLList *L) {

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

void ExprDLPostDelete (tExprDLList *L) {

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

void ExprDLPreDelete (tExprDLList *L) {

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

void ExprDLPostInsert (tExprDLList *L, tInstruction val) {

    if(L->Act == NULL)
    {
        return;
    }

    tExprDLElemPtr newElem = (tExprDLElemPtr) malloc(sizeof(tExprDLElemPtr));
    if (newElem == NULL)
    {
        ExprDLError();
        exit(1);
    }
    newElem->instr= val;
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

void ExprDLPreInsert (tExprDLList *L, tInstruction val) {

    if(L->Act == NULL)
    {
        return;
    }

    tExprDLElemPtr newElem = (tExprDLElemPtr) malloc(sizeof(tExprDLElemPtr));
    if (newElem == NULL)
    {
        ExprDLError();
    }

    newElem->instr = val;
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

void ExprDLCopy (tExprDLList *L, tInstruction *val) {

    if(ExprDLActive(L) == 0)
    {
        ExprDLError();
        return;
    }
    *val = L->Act->instr;
    return;
}

void ExprDLActualize (tExprDLList *L, tInstruction val) {

    if(L->Act == NULL)
    {
        return;
    }
    L->Act->instr = val;
    return;
}

void ExprDLSucc (tExprDLList *L) {

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


void ExprDLPred (tExprDLList *L) {

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

int ExprDLActive (tExprDLList *L) {

    if(L->Act != NULL)
    {
        return 1;
    }
    else
    {
        return 0;
    }