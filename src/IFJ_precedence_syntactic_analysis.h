/* ******************************* IFJ_precedence_syntactic_analysis.h ****************************
 *  Predmet: Formalni jazyky a prekladace (IFJ) - FIT VUT v Brne
 *  Nazev projektu: Implementace prekladace imperativniho jazyka IFJ19
 *  Autori: Beranek Tomas (xberan46) - vedouci
 *          Haderka Martin (xhader00)
 *          Slobodnik Simon (xslobo06)
 *          Klem Richard (xklemr00)
 *  Datum vytvoreni: 27.11.2019
 * ************************************************************************** */
#ifndef IFJ_PRECEDENCE_SYNTACTIC_ANALYSIS_H
#define IFJ_PRECEDENCE_SYNTACTIC_ANALYSIS_H

#include "IFJ_precedence_table.h"
#include "IFJ_precedence_DLL.h"


/**
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * ______________!!! W I P !!!______________
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

//tExprStack psa_stack;
//extern ptr_psa_stack = &psa_stack;
typedef enum {
    BRACKETS, BINARY, RT_ERROR
}reduction_type;

token_t expressionParse(FILE * src_file, token_t* first, token_t* second, int num_of_tokens);
int reduce_by_rules(tExprStack *stack, int members_count);
token_t loadExpr(FILE * src_file, tExprDLList * expr_DLL, expr_token_t* first_token, expr_token_t* second_token, int * max_len);

#endif //IFJ_PRECEDENCE_SYNTACTIC_ANALYSIS_H
/* konec souboru IFJ_precedence_syntactic_analysis.h */
