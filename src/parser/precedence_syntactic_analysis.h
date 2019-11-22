#ifndef IFJ_PRECEDENCE_SYNTACTIC_ANALYSIS_H
#define IFJ_PRECEDENCE_SYNTACTIC_ANALYSIS_H

#include "IFJ_stack.h"
tStack psa_stack;
extern ptr_psa_stac = &psa_stack;

void expressionParse(token_t *args);
void reduce_by_rules(tStack *stack);


#endif //IFJ_PRECEDENCE_SYNTACTIC_ANALYSIS_H
