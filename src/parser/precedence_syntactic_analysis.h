#ifndef IFJ_PRECEDENCE_SYNTACTIC_ANALYSIS_H
#define IFJ_PRECEDENCE_SYNTACTIC_ANALYSIS_H

#include "IFJ_stack.h"
tStack psa_stack;
ptr_psa_stac = &psa_stack;

token_t find_fisrt_terminal(ptr_psa_stac);

typedef enum {
    REDUCE, EQUAL, SHIFT, ERROR
} precedence_rule;

precedence_rule precdence_table = [8][8];

#endif //IFJ_PRECEDENCE_SYNTACTIC_ANALYSIS_H
