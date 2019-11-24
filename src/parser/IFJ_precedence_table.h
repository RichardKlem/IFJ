/**
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * ______________!!! W I P !!!______________
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

#ifndef IFJ_PRECEDENCE_TABLE_H
#define IFJ_PRECEDENCE_TABLE_H

typedef enum {
    ERROR, REDUCE, EQUAL, SHIFT, XXXXXX
} precedence_rule;

extern precedence_rule precedence_table[8][8];

typedef struct expr_token_t {
    bool terminal;
    bool shifted;
    token_t token; //je definovan kdyz je terminal, jinak = NULL (muzu to dat na null???)
} expr_token_t;

typedef struct texprelem{
    expr_token_t exprToken;
    struct texprelem* next;
} tExprElem;

typedef struct {
    tExprElem* top;
} tExprStack;

void exprStackInit (tExprStack* s);
int exprStackEmpty (tExprStack* s);
expr_token_t exprStackTop (tExprStack* s);
void exprStackPop (tExprStack* s);
void exprStackPush (tExprStack* s, expr_token_t item);
expr_token_t find_top_terminal(tExprStack* s);


#endif //IFJ_PRECEDENCE_TABLE_H
