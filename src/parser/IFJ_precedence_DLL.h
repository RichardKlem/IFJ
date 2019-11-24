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

typedef struct expr_token_t {
    bool terminal;
    bool shifted;
    token_t token; //je definovan kdyz je terminal, jinak = NULL (muzu to dat na null???)
} expr_token_t;

typedef struct tExprDLElem {
    expr_token_t exprToken;
    struct tExprDLElem *lptr;
    struct tExprDLElem *rptr;
} *tExprDLElemPtr;

typedef struct {
    tExprDLElemPtr First;
    tExprDLElemPtr Act;
    tExprDLElemPtr Last;
} tExprDLList;

/* funkce dvousmerne vazaneho seznamu */
void exprDLInitList (tExprDLList * L);
void exprDLDisposeList (tExprDLList * L);
void exprDLInsertFirst (tExprDLList * L, expr_token_t exprToken);
void exprDLInsertLast(tExprDLList * L, expr_token_t exprToken);
void exprDLFirst (tExprDLList * L);
void exprDLLast (tExprDLList * L);
void exprDLCopyFirst (tExprDLList * L, expr_token_t * exprToken);
void exprDLCopyLast (tExprDLList * L, expr_token_t * exprToken);
void exprDLDeleteFirst (tExprDLList * L);
void exprDLDeleteLast (tExprDLList * L);
void exprDLPostDelete (tExprDLList * L);
void exprDLPreDelete (tExprDLList * L);
void exprDLPostInsert (tExprDLList * L, expr_token_t exprToken);
void exprDLPreInsert (tExprDLList * L, expr_token_t exprToken);
void exprDLCopy (tExprDLList * L, expr_token_t * exprToken);
void exprDLActualize (tExprDLList * L, expr_token_t exprToken);
void exprDLSucc (tExprDLList * L);
void exprDLPred (tExprDLList * L);
int exprDLActive (tExprDLList * L);

#endif //IFJ_IFJ_PRECEDENCE_exprDLL_H
