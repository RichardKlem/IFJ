/* ******************************* IFJ_scanner.c ****************************
 *  Predmet: Formalni jazyky a prekladace (IFJ) - FIT VUT v Brne
 *  Nazev projektu: Implementace prekladace imperativniho jazyka IFJ19
 *  Autori: Beranek Tomas (xberan46) - vedouci
 *          Haderka Martin (xhader00)
 *          Slobodnik Simon (xslobo06)
 *          Klem Richard (xklemr00)
 *  Datum vytvoreni: 12.10.2019
 * ************************************************************************** */

#include "instGen.h"

void generateInst(tDLElemPtr *List, enInstruction instType, void *arg1, void *arg2, void *arg3)
{
    tInstruction instruction;
    instruction.arg1 = arg1;
    instruction.arg2 = arg2;
    instruction.arg3 = arg3;
    instruction.instType = instType;
    DLInsertLast(List, instruction)
}

void printInst(tDLList *List)
{
    if(List->First == NULL) //Pokud je seznam prazdny
    {
        //TODO
        return;
    }
    tInstruction tmpInst;   //Deklarace pomocne promenne pro nacitani instrukci
    while(DLActive(List))
    {
        DLCopy(List, tmpInst);
        switch (tmpInst.instType)
        {
            case MOVE:
                printf("MOVE %s %s\n",(char) tmpInst->arg1,(char) tmpInst->arg2);
                break;
            case CREATEFRAME:
                printf("CREATEFRAME\n");
                break;
            case PUSHFRAME:
                printf("POPFRAME\n");
                break;
            case DEFVAR:
                printf("DEFVAR %s\n",(char) tmpInst->arg1);
                break;
            case CALL:
                printf("CALL %s\n",(char) tmpInst->arg1);
                break;
            case RETURN:
                printf("RETURN\n");
                break;
            case PUSHS:
                printf("PUSHS %s\n",(char) tmpInst.arg1);
                break;
        }
    }
}

void DLError() {

    fprintf (stderr,"*ERROR* Table of Instructions.\n");
    return;
}

void DLInitList (tDLList *L) {

    L->First = NULL;
    L->Act = NULL;
    L->Last = NULL;
    return;

}

void DLDisposeList (tDLList *L) {

    if (L->First == NULL)
    {
        DLError();
        return;
    }

    tDLElemPtr *tempElem = L->Last;
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

void DLInsertFirst (tDLList *L, tInstruction val) {

    tDLElemPtr newElem = (tDLElemPtr) malloc (sizeof(tDLElemPtr));// tDLElemPtr je struktura definovaná jako pointer, jak tedy mallocovat
    if (newElem == NULL)    //Ověření zda alokace proběhla úspěšně
    {
        DLError();
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

void DLInsertLast(tDLList *L, tInstruction val) {

    tDLElemPtr newElem = (tDLElemPtr) malloc(sizeof(tDLElemPtr));
    if (newElem == NULL)
    {
        DLError();
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

void DLFirst (tDLList *L) {

    L->Act = L->First;
    return;
}

void DLLast (tDLList *L) {

    L->Act = L->Last;
    return;
}

void DLCopyFirst (tDLList *L, tInstruction *val) {

    if(L->First == NULL)
    {
        DLError();
        return;
    }
    *val = L->First->instr;
    return;

}

void DLCopyLast (tDLList *L, tInstruction *val) {

    if(L->Last == NULL)
    {
        DLError();
        return;
    }
    *val = L->Last->instr;
    return;
}

void DLDeleteFirst (tDLList *L) {

    if (L->First == NULL)
    {
        return;
    }
    else if (L->First == L->Act)
    {
        L->Act = NULL;
    }
    tDLElemPtr tempElem;
    tempElem = L->First;
    L->First = L->First->rptr;
    L->First->lptr = NULL;
    free(tempElem);
    return;
}

void DLDeleteLast (tDLList *L) {

    if (L->Last == NULL)
    {
        return;
    }
    else if (L->Last == L->Act)
    {
        L->Act = NULL;
    }
    tDLElemPtr tempElem;
    tempElem = L->Last;
    L->Last = L->Last->lptr;
    L->Last->rptr = NULL;
    free(tempElem);
    return;
}

void DLPostDelete (tDLList *L) {

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
        tDLElemPtr tempElem;
        tempElem = L->Act->rptr;
        L->Act->rptr = L->Act->rptr->rptr;
        L->Act->rptr->rptr->lptr = L->Act;
        free(tempElem);
    }
    return;
}

void DLPreDelete (tDLList *L) {

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
        tDLElemPtr tempElem;
        tempElem = L->Act->lptr;
        L->Act->lptr = L->Act->lptr->lptr;
        L->Act->lptr->lptr->rptr = L->Act;
        free(tempElem);
    }
    return;
}

void DLPostInsert (tDLList *L, tInstruction val) {

    if(L->Act == NULL)
    {
        return;
    }

    tDLElemPtr newElem = (tDLElemPtr) malloc(sizeof(tDLElemPtr));
    if (newElem == NULL)
    {
        DLError();
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

void DLPreInsert (tDLList *L, tInstruction val) {

    if(L->Act == NULL)
    {
        return;
    }

    tDLElemPtr newElem = (tDLElemPtr) malloc(sizeof(tDLElemPtr));
    if (newElem == NULL)
    {
        DLError();
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

void DLCopy (tDLList *L, tInstruction *val) {

    if(DLActive(L) == 0)
    {
        DLError();
        return;
    }
    *val = L->Act->instr;
    return;
}

void DLActualize (tDLList *L, tInstruction val) {

    if(L->Act == NULL)
    {
        return;
    }
    L->Act->instr = val;
    return;
}

void DLSucc (tDLList *L) {

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


void DLPred (tDLList *L) {

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

int DLActive (tDLList *L) {

    if(L->Act != NULL)
    {
        return 1;
    }
    else
    {
        return 0;
    }

}