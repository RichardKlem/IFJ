//
// Created by Richa on 21-Nov-19.
//

#ifndef IFJ_PRECEDENCE_TABLE_H
#define IFJ_PRECEDENCE_TABLE_H

typedef enum {
    ERROR, REDUCE, EQUAL, SHIFT
} precedence_rule;

extern precedence_rule precedence_table[8][8];

#endif //IFJ_PRECEDENCE_TABLE_H
