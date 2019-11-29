/* ******************************* IFJ_stack_semantic.h ****************************
 *  Predmet: Formalni jazyky a prekladace (IFJ) - FIT VUT v Brne
 *  Nazev projektu: Implementace prekladace imperativniho jazyka IFJ19
 *  Autori: Beranek Tomas (xberan46) - vedouci
 *          Haderka Martin (xhader00)
 *          Slobodnik Simon (xslobo06)
 *          Klem Richard (xklemr00)
 *  Datum vytvoreni: 25.11.2019
 * ************************************************************************** */

#ifndef IFJ_STACK_SEMANTIC_H_INCLUDED
#define IFJ_STACK_SEMANTIC_H_INCLUDED

#include <stdbool.h>

typedef enum {
    FUN_DEF,
    FUN_CALL,
    VAR_DEF,
    VAR_USE,
    BLOCK_START
} taction;

typedef struct tdata {
    taction action;
    char * name;
} tdata;

typedef struct telem_sem{
    tdata data;
    struct telem_sem* next;
} tElem_sem;

typedef struct {
        tElem_sem* top;
} tStack_sem;

extern tStack_sem stack_semantic;
extern tStack_sem stack_semantic_params;
extern int param_num;
extern int arg_num;
extern char * fun_name;
extern bool in_function;

void stack_sem_init (tStack_sem* s);
int stack_sem_empty (tStack_sem* s);
tdata stack_sem_top (tStack_sem* s);
void stack_sem_pop (tStack_sem* s);
void stack_sem_push (tStack_sem* s, taction action, char * name);
void stack_sem_pop_until_block_start(tStack_sem* s);


#endif // IFJ_STACK_SEMANTIC_H_INCLUDED

/* konec souboru IFJ_stack_semantic.h */
