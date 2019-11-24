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
#include <stdio.h>
#include <string.h>
#include "IFJ_inst_gen.h"

void generateInst(tDLElemPtr *List, enInstruction instType, void *arg1, void *arg2, void *arg3)
{
    //Vytvoříme 
    tInstruction instruction;
    instruction.arg1 = arg1;
    instruction.arg2 = arg2;
    instruction.arg3 = arg3;
    instruction.instType = instType;
    DLInsertLast(List, instruction);
}

void printInst(tDLList *List)
{
    if(List->First == NULL) //Pokud je seznam prazdny
    {
        //Nevipisujeme nic
        return;
    }
    tInstruction *tmpInst;   //Deklarace pomocne promenne pro nacitani instrukci
    while(DLActive(List))   //Dokud je list aktivni
    {
        DLCopy(List, tmpInst);  //kopirovani aktualni hodnoty
        switch (tmpInst->instType)
        {
            case MOVE:
                printf("MOVE %s %s\n",(char *) tmpInst->arg1,(char *) tmpInst->arg2);
                break;
            case CREATEFRAME:
                printf("CREATEFRAME\n");
                break;
            case PUSHFRAME:
                printf("POPFRAME\n");
                break;
            case DEFVAR:
                printf("DEFVAR %s\n",(char *) tmpInst->arg1);
                break;
            case CALL:
                printf("CALL %s\n",(char *) tmpInst->arg1);
                break;
            case RETURN:
                printf("RETURN\n");
                break;
            case PUSHS:
                printf("PUSHS %s\n",(char *) tmpInst->arg1);
                break;
            case POPS:
                printf("POPS %s\n",(char *) tmpInst->arg1);
                break;
            case CLEARS:
                printf("CLEARS\n");
                break;
            case ADD:
                printf("ADD %s %s %s\n",(char *) tmpInst->arg1, (char *) tmpInst->arg2, (char *) tmpInst->arg3);
                break;
            case SUB:
                printf("SUB %s %s %s\n",(char *) tmpInst->arg1, (char *) tmpInst->arg2, (char *) tmpInst->arg3);
                break;
            case MUL:
                printf("MUL %s %s %s\n",(char *) tmpInst->arg1, (char *) tmpInst->arg2, (char *) tmpInst->arg3);
                break;
            case DIV:
                printf("DIV %s %s %s\n",(char *) tmpInst->arg1, (char *) tmpInst->arg2, (char *) tmpInst->arg3);
                break;
            case IDIV:
                printf("IDIV %s %s %s\n",(char *) tmpInst->arg1, (char *) tmpInst->arg2, (char *) tmpInst->arg3);
                break;
            case ADDS:
                printf("ADDS\n");
                break;
            case SUBS:
                printf("SUBS\n");
                break;
            case MULS:
                printf("MULS\n");
                break;
            case DIVS:
                printf("DIVS\n");
                break;
            case IDIVS:
                printf("IDIVS\n");
                break;
            case LT:
                printf("LT %s %s %s\n", (char *) tmpInst->arg1, (char *) tmpInst->arg2, (char *) tmpInst->arg3);
                break;
            case GT:
                printf("LT %s %s %s\n", (char *) tmpInst->arg1, (char *) tmpInst->arg2, (char *) tmpInst->arg3);
                break;
            case EQ:
                printf("LT %s %s %s\n", (char *) tmpInst->arg1, (char *) tmpInst->arg2, (char *) tmpInst->arg3);
                break;
            case LTS:
                printf("LTS\n");
                break;
            case GTS:
                printf("GTS\n");
                break;
            case EQS:
                printf("EQS\n");
                break;
            case AND:
                printf("AND %s %s %s\n", (char *) tmpInst->arg1, (char *) tmpInst->arg2, (char *) tmpInst->arg3);
                break;
            case OR:
                printf("OR %s %s %s\n", (char *) tmpInst->arg1, (char *) tmpInst->arg2, (char *) tmpInst->arg3);
                break;
            case NOT:
                printf("NOT %s %s %s\n",(char *) tmpInst->arg1, (char *) tmpInst->arg2, (char *) tmpInst->arg3);
                break;
            case ANDS:
                printf("ANDS\n");
                break;
            case ORS:
                printf("ORS\n");
                break;
            case NOTS:
                printf("NOTS\n");
                break;
            case INT2FLOAT:
                printf("INT2FLOAT %s %s\n",(char *) tmpInst->arg1, (char *) tmpInst->arg2);
                break;
            case FLOAT2INT:
                printf("FLOAT2INT %s %s\n",(char *) tmpInst->arg1, (char *) tmpInst->arg2);
                break;
            case INT2CHAR:
                printf("INT2CHAR %s %s\n",(char *) tmpInst->arg1, (char *) tmpInst->arg2);
                break;
            case STRI2INT:
                printf("STRI2INT %s %s %s\n",(char *) tmpInst->arg1, (char *) tmpInst->arg2, (char *) tmpInst->arg3);
                break;
            case INT2FLOATS:
                printf("INT2FLOATS\n");
                break;
            case FLOAT2INTS:
                printf("FLOAT2INTS\n");
                break;
            case INT2CHARS:
                printf("INT2CHARS\n");
                break;
            case STRI2INTS:
                printf("STRI2INTS\n");
                break;
            case READ:
                printf("READ %s %s\n",(char *) tmpInst->arg1, (char *) tmpInst->arg2);
                break;
            case WRITE:
                printf("WRITE %s\n", (char *) tmpInst->arg1);
                break;
            case CONCAT:
                printf("CONCAT %s %s %s\n",(char *) tmpInst->arg1, (char *) tmpInst->arg2, (char *) tmpInst->arg3);
                break;
            case STRLEN:
                printf("STRLEN %s %s\n", (char *) tmpInst->arg1, (char *) tmpInst->arg2);
                break;
            case GETCHAR:
                printf("GETCHAR %s %s %s\n",(char *) tmpInst->arg1, (char *) tmpInst->arg2, (char *) tmpInst->arg3);
                break;
            case SETCHAR:
                printf("SETCHAR %s %s %s\n",(char *) tmpInst->arg1, (char *) tmpInst->arg2, (char *) tmpInst->arg3);
                break;
            case TYPE:
                printf("TYPE %s %s\n",(char *) tmpInst->arg1, (char *) tmpInst->arg2, (char *) tmpInst->arg3);
                break;
            case LABEL:
                printf("LABEL %s\n", (char *) tmpInst->arg1);
                break;
            case JUMP:
                printf("JUMP %s\n", (char *) tmpInst->arg1);
                break;
            case JUMPIFEQ:
                printf("JUMPIFEG %s %s %s\n", (char *) tmpInst->arg1, (char *) tmpInst->arg2, (char *) tmpInst->arg3);
                break;
            case JUMPIFNEQ:
                printf("JUMPIFNEG %s %s %s\n", (char *) tmpInst->arg1, (char *) tmpInst->arg2, (char *) tmpInst->arg3);
                break;
            case JUMPIFEQS:
                printf("JUMPIFEQS %s\n", (char *) tmpInst->arg1);
                break;
            case JUMPIFNEQS:
                printf("JUMPIFNEQS %s\n", (char *) tmpInst->arg1);
                break;
            case EXIT:
                printf("EXIT %s\n", (char *) tmpInst->arg1);
                break;
            case BREAK:
                printf("BREAK\n");
                break;
            case DPRINT:
                printf("DPRINT %s\n", (char *) tmpInst->arg1);
                break;
            case COMMENT:
                printf("#%s\n", (char *) tmpInst->arg1);
                break;
            case HEADER:
                printf(".IFJcode19\n");
                break;
        }
        DLSucc(List);   //Posunuti se na dalsi prvek
    }
    return;
}



void DLError() {

    fprintf (stderr,"*ERROR* Table of Instructions.\n");
    return;
}

void DLInitList (tDLList *L) {

    L->First = NULL;
    L->Act = NULL;
    L->Last = NULL;
    generateInst(L,HEADER,NULL,NULL,NULL);
    //TODO
    //generateInst();
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