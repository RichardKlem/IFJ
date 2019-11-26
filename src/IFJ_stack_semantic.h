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

typedef struct telem{
    tdata data;
    struct telem* next;
} tElem;

typedef struct {
        tElem* top;
} tStack;

extern tStack stack_semantic;
extern tStack stack_semantic_params;
extern int param_num;
extern int arg_num;

void stack_sem_init (tStack* s);
int stack_sem_empty (tStack* s);
tdata stack_sem_top (tStack* s);
void stack_sem_pop (tStack* s);
void stack_sem_push (tStack* s, taction action, char * name);
void stack_sem_pop_until_block_start(tStack* s);


#endif // IFJ_STACK_SEMANTIC_H_INCLUDED

/* konec souboru IFJ_stack_semantic.h */
