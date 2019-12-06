/* ******************************* IFJ_scanner.h ****************************
 *  Predmet: Formalni jazyky a prekladace (IFJ) - FIT VUT v Brne
 *  Nazev projektu: Implementace prekladace imperativniho jazyka IFJ19
 *  Autori: Beranek Tomas (xberan46) - vedouci
 *          Haderka Martin (xhader00)
 *          Slobodnik Simon (xslobo06)
 *          Klem Richard (xklemr00)
 *  Datum vytvoreni: 23.11.2019
 * ************************************************************************** */

#ifndef IFJ_PARSER_H_INCLUDED
#define IFJ_PARSER_H_INCLUDED

#include "IFJ_stack_string.h"

void prog();
void st_list();
void stat();
void eol_opt();
void ret();
void param_list();
void param_next();
void expr_or_assign();
void arg_list();
void arg_next();
void fun_or_expr();
void fun_or_expr_2();

extern int in_cycle;
extern tStack_string stack_instructions;
extern char *buffer;

#define print_instruction(...) if (in_cycle){                           \
                                    /* zde muze nastat problem s velikosti bufferu - TODO */ \
                                    sprintf(buffer, __VA_ARGS__);       \
                                    char *tmp = malloc(strlen(buffer) + 1); \
                                    if (!tmp) error_exit(ERROR_INTERNAL); \
                                    strcpy(tmp, buffer); \
                                    stack_push_string(&stack_instructions, tmp); \
                                }                                       \
                                else                                    \
                                    printf(__VA_ARGS__)


#endif // IFJ_PARSER_H_INCLUDED

/* konec souboru IFJ_scanner.h */
