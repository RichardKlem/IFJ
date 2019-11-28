/* ******************************* IFJ_stack_semantic.c ****************************
 *  Predmet: Formalni jazyky a prekladace (IFJ) - FIT VUT v Brne
 *  Nazev projektu: Implementace prekladace imperativniho jazyka IFJ19
 *  Autori: Beranek Tomas (xberan46) - vedouci
 *          Haderka Martin (xhader00)
 *          Slobodnik Simon (xslobo06)
 *          Klem Richard (xklemr00)
 *  Datum vytvoreni: 25.11.2019
 * ************************************************************************** */

#include "IFJ_stack_semantic.h"
#include "IFJ_error.h"
#include <stdlib.h>
#include "symtable.h"

void stack_sem_init (tStack_sem* s) {
	if (s == NULL)
        error_exit(ERROR_INTERNAL);
	else
		s->top = NULL;
}

int stack_sem_empty (tStack_sem* s) {
    if (s == NULL)
        error_exit(ERROR_INTERNAL);
    else
        return s->top == NULL;
}

tdata stack_sem_top (tStack_sem* s) {
	if (s == NULL || stack_sem_empty(s))
		error_exit(ERROR_INTERNAL);
	else
		return s->top->data;
}

void stack_sem_pop (tStack_sem* s) {
	if (s == NULL)
        error_exit(ERROR_INTERNAL);
	else if (!stack_sem_empty(s)) {
            tElem_sem* del = s->top;
            s->top = s->top->next;
            free (del);
        }
}

void stack_sem_push (tStack_sem* s, taction action, char * name) {
	if (s == NULL)
		error_exit(ERROR_INTERNAL);

    tElem_sem* insert = (tElem_sem*)malloc(sizeof(tElem_sem));
    if (insert == NULL)
        error_exit(ERROR_INTERNAL);

    insert->data.name = name;
    insert->data.action = action;
    insert->next = s->top;
    s->top = insert;

	//uprava tabulky symbolu
    Record tmp;

    switch (action) {
    case FUN_DEF:
        //pokud jiz je v tabulce fce nebo promenna se stejnym jmenem a je definovana => chyba
        if (symtable_search(symtable, name, &tmp) && tmp.defined)
            error_exit(ERROR_SEM_UNDEF);
        else{ //pokud v tabulce neni nebo neni definovana nahradime jeji hodnoty/vlozime jeji hodnoty
            tmp.defined = 1;
            tmp.is_variable = 0;
            tmp.param_num = param_num; //globalni prommena do ktere zapisuje parser
            symtable_insert(&symtable, name, tmp); //prepiseme hodnoty v symtable
            stack_sem_push(&stack_semantic, BLOCK_START, NULL); //vlozeni zacatku bloku

            //nutne vlozit parametry, ktere jsou nacteny v stack_semantic_params, budou tam v opacnem poradi
            tdata data;

            while (!stack_sem_empty(&stack_semantic_params)){
                data = stack_sem_top(&stack_semantic_params);
                stack_sem_pop(&stack_semantic_params);
                stack_sem_push(&stack_semantic, data.action, data.name);
            }
        }
        break;

    case FUN_CALL:
        //pokud funkce je v symtable, je definovana, jedna se opravdu o funkci a ma shodny pocet parametru
        if (symtable_search(symtable, name, &tmp) && tmp.defined && !tmp.is_variable){
            if (tmp.param_num == arg_num)
                /*DO NOTHING, EVERYTHING IS FINE*/;
            else
                error_exit(ERROR_SEM_CALL);
        }
        else
            error_exit(ERROR_SEM_UNDEF);
        break;

    case BLOCK_START:
        /* POUZE VLOZ NA ZASOBNIK */
        break;

    case VAR_DEF:
        //musime overit, zda neexistuje definovana fce se stejnym jmenem
        if (symtable_search(symtable, name, &tmp)){ //pokud je dany symbol v tabulce
            if (!tmp.is_variable && tmp.defined) //pokud se jedna o funkci a je definovana
                error_exit(ERROR_SEM_UNDEF);
            else { //pokud uz bylo v tabulce, ale neni to fce nebo neni definovana, jenom upravime parametry
                tmp.defined++;
                tmp.is_variable = 1;
                symtable_insert(&symtable, name, tmp);
            }
        }
        else { //pokud neni v tabulce, musime rucne nastavit parametry
            tmp.defined = 1;
            tmp.is_variable = 1;
            symtable_insert(&symtable, name, tmp);
        }
        break;

    case VAR_USE:
            //promenna musi byt v tabulce a byt definovana
            if (symtable_search(symtable, name, &tmp) && tmp.is_variable && tmp.defined)
                /*DO NOTHING, EVERYTHING IS FINE*/;
            else
                error_exit(ERROR_SEM_UNDEF);
        break;
    }
}

void stack_sem_pop_until_block_start(tStack_sem* s) {
    tdata data = stack_sem_top(s);
    Record tmp;

    while (data.action != BLOCK_START){
        if (data.action == FUN_DEF || data.action == VAR_DEF) {
            symtable_search(symtable, data.name, &tmp);
            //snizime pocitadlo definici, pokud se jedna o funkci, jde na 0, pokud o promennou snizi se o 1
            tmp.defined--;
            symtable_insert(&symtable, data.name, tmp);
        }

        stack_sem_pop(s);
        data = stack_sem_top(s);
    }
    stack_sem_pop(s); //popneme i BLOCK_START
}

/* konec souboru IFJ_stack_semantic.c */
