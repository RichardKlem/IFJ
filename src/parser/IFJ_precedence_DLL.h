/* ******************************* IFJ_scanner.c ****************************
 *  Predmet: Formalni jazyky a prekladace (IFJ) - FIT VUT v Brne
 *  Nazev projektu: Implementace prekladace imperativniho jazyka IFJ19
 *  Autori: Beranek Tomas (xberan46) - vedouci
 *          Haderka Martin (xhader00)
 *          Slobodnik Simon (xslobo06)
 *          Klem Richard (xklemr00)
 *  Datum vytvoreni: 12.10.2019
 * ************************************************************************** */

#ifndef IFJ_IFJ_PRECEDENCE_exprDLL_H
#define IFJ_IFJ_PRECEDENCE_exprDLL_H

typedef struct tExprDLElem {
    token_t token;
    struct tExprDLElem *lptr;
    struct tExprDLElem *rptr;
} *tExprDLElemPtr;

typedef struct {
    tExprDLElemPtr First;
    tExprDLElemPtr Act;
    tExprDLElemPtr Last;
} tExprDLList;

/* funkce dvousmerne vazaneho seznamu */
void exprDLInitList (tExprDLList *);
void exprDLDisposeList (tExprDLList *);
void exprDLInsertFirst (tExprDLList *, token_t);
void exprDLInsertLast(tExprDLList *, token_t);
void exprDLFirst (tExprDLList *);
void exprDLLast (tExprDLList *);
void exprDLCopyFirst (tExprDLList *, token_t *);
void exprDLCopyLast (tExprDLList *, token_t *);
void exprDLDeleteFirst (tExprDLList *);
void exprDLDeleteLast (tExprDLList *);
void exprDLPostDelete (tExprDLList *);
void exprDLPreDelete (tExprDLList *);
void exprDLPostInsert (tExprDLList *, token_t);
void exprDLPreInsert (tExprDLList *, token_t);
void exprDLCopy (tExprDLList *, token_t *);
void exprDLActualize (tExprDLList *, token_t);
void exprDLSucc (tExprDLList *);
void exprDLPred (tExprDLList *);
int exprDLActive (tExprDLList *);

#endif //IFJ_IFJ_PRECEDENCE_exprDLL_H
