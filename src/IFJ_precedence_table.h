/* ******************************* IFJ_precedence_table.h ****************************
 *  Predmet: Formalni jazyky a prekladace (IFJ) - FIT VUT v Brne
 *  Nazev projektu: Implementace prekladace imperativniho jazyka IFJ19
 *  Autori: Beranek Tomas (xberan46) - vedouci
 *          Haderka Martin (xhader00)
 *          Slobodnik Simon (xslobo06)
 *          Klem Richard (xklemr00)
 *  Datum vytvoreni: 27.11.2019
 * ************************************************************************** */

#ifndef IFJ_PRECEDENCE_TABLE_H
#define IFJ_PRECEDENCE_TABLE_H

#include <stdbool.h>

typedef enum {
    ERROR, REDUCE, EQUAL, SHIFT, XXXXXX
} precedence_rule;

extern precedence_rule precedence_table[8][8];


typedef struct tgenelem{
    token_t genToken;
    struct tgenelem * next;
} tGenElem;

typedef struct {
    tGenElem * top;
} tGenStack;

typedef struct expr_token_t {
    bool terminal;
    bool shifted;
    token_t token; //je definovan kdyz je terminal, jinak = NULL (muzu to dat na null???)
} expr_token_t;

typedef struct texprelem{
    expr_token_t exprToken;
    struct texprelem * next;
} tExprElem;

typedef struct {
    tExprElem * top;
} tExprStack;

void exprStackInit (tExprStack * s);
int exprStackEmpty (tExprStack * s);
expr_token_t exprStackTop (tExprStack * s);
void exprStackPop (tExprStack * s);
void exprStackPush (tExprStack * s, expr_token_t item);
expr_token_t * find_top_terminal(tExprStack * s);

void genStackInit (tGenStack * s);
int genStackEmpty (tGenStack * s);
token_t genStackTop (tGenStack * s);
void genStackPop (tGenStack * s);
void genStackPush (tGenStack * s, token_t item);

#endif //IFJ_PRECEDENCE_TABLE_H
/* konec souboru IFJ_precedence_table.h */
