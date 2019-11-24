#ifndef IFJ_PRECEDENCE_SYNTACTIC_ANALYSIS_H
#define IFJ_PRECEDENCE_SYNTACTIC_ANALYSIS_H


/**
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * ______________!!! W I P !!!______________
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

//tExprStack psa_stack;
//extern ptr_psa_stack = &psa_stack;

void expressionParse(FILE * src_file, token_t* first, token_t* second, int num_of_tokens);
void reduce_by_rules(tExprStack *stack);
token_t loadExpr(FILE * src_file, tExprDLList expr_DLL, expr_token_t* first_token, expr_token_t* second_token);

#endif //IFJ_PRECEDENCE_SYNTACTIC_ANALYSIS_H
