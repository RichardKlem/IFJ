/* ******************************* IFJ_scanner.c ****************************
 *  Predmet: Formalni jazyky a prekladace (IFJ) - FIT VUT v Brne
 *  Nazev projektu: Implementace prekladace imperativniho jazyka IFJ19
 *  Autori: Beranek Tomas (xberan46) - vedouci
 *          Haderka Martin (xhader00)
 *          Slobodnik Simon (xslobo06)
 *          Klem Richard (xklemr00)
 *  Datum vytvoreni: 12.10.2019
 * ************************************************************************** */

#ifndef IFJ_IFJ_PRECEDENCE_ExprDLL_H
#define IFJ_IFJ_PRECEDENCE_ExprDLL_H

typedef struct tExprDLElem {
    tInstruction instr;
    struct tExprDLElem *lptr;
    struct tExprDLElem *rptr;
} *tExprDLElemPtr;

typedef struct {
    tExprDLElemPtr First;
    tExprDLElemPtr Act;
    tExprDLElemPtr Last;
} tExprDLList;
/* funkce pro spracovani instrukci*/
void generateInst(tExprDLElemPtr *List, enInstruction instType, void *arg1, void *arg2, void *arg3);
void printInst(tExprDLList *List);
/* funkce dvousmerne vazaneho seznamu */
void ExprDLInitList (tExprDLList *);
void ExprDLDisposeList (tExprDLList *);
void ExprDLInsertFirst (tExprDLList *, tInstruction);
void ExprDLInsertLast(tExprDLList *, tInstruction);
void ExprDLFirst (tExprDLList *);
void ExprDLLast (tExprDLList *);
void ExprDLCopyFirst (tExprDLList *, tInstruction *);
void ExprDLCopyLast (tExprDLList *, tInstruction *);
void ExprDLDeleteFirst (tExprDLList *);
void ExprDLDeleteLast (tExprDLList *);
void ExprDLPostDelete (tExprDLList *);
void ExprDLPreDelete (tExprDLList *);
void ExprDLPostInsert (tExprDLList *, tInstruction);
void ExprDLPreInsert (tExprDLList *, tInstruction);
void ExprDLCopy (tExprDLList *, tInstruction *);
void ExprDLActualize (tExprDLList *, tInstruction);
void ExprDLSucc (tExprDLList *);
void ExprDLPred (tExprDLList *);
int ExprDLActive (tExprDLList *);

#endif //IFJ_IFJ_PRECEDENCE_ExprDLL_H
