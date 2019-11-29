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
#include "stdbool.h"
#include "IFJ_stack_string.h"

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

void check_for_fun_def(char *name){
    Record tmp;

    symtable_search(symtable, name, &tmp);

    //nezavisi na zadnych funkcich
    if (stack_empty_string(tmp.undefined_functions))
        return;

    //prochazime vsechny funkce, na kterych je aktualni funkce zavisla a kontrolujeem zda jsou definovane a jejich zavislosti taky
    while (!stack_empty_string(tmp.undefined_functions)) {
        name = stack_top_string(tmp.undefined_functions);
        if (symtable_search(symtable, name, &tmp) && !tmp.is_variable && tmp.global && !tmp.forward_call)
            check_for_fun_def(name);
        else
            error_exit(ERROR_SEM_UNDEF);
        stack_pop_string(tmp.undefined_functions);
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
        if (symtable_search(symtable, name, &tmp)) { //je v symtable
            if (tmp.is_variable) { //je promenna
                if (tmp.global || tmp.local) //je definovana
                    error_exit(ERROR_SEM_UNDEF);
                else { //neni definovana
                    tmp.global = true;
                    tmp.is_variable = false;
                    tmp.param_num = param_num; //globalni prommena do ktere zapisuje parser
                    tmp.local = false;
                    tmp.forward_call = false;
                    tmp.undefined_functions = malloc(sizeof(*tmp.undefined_functions));
                    stack_init_string(tmp.undefined_functions);
                    symtable_insert(&symtable, name, tmp); //prepiseme hodnoty v symtable
                    stack_sem_push(&stack_semantic, BLOCK_START, NULL); //vlozeni zacatku bloku
                }
            }
            else{ //neni promenna
                if (!tmp.forward_call) //uz je definovana
                    error_exit(ERROR_SEM_UNDEF);
                else { //byla jenom volana
                    if (tmp.param_num != param_num) //neshoduje se pocet arguemntu predchoziho volani a pocet parametru teto definice
                        error_exit(ERROR_SEM_CALL);
                    else {
                        tmp.forward_call = false;
                        symtable_insert(&symtable, name, tmp);
                        stack_sem_push(&stack_semantic, BLOCK_START, NULL);
                    }
                }
            }
        }
        else{ //pokud v tabulce neni
            tmp.global = true;
            tmp.is_variable = false;
            tmp.param_num = param_num; //globalni prommena do ktere zapisuje parser
            tmp.local = false;
            tmp.forward_call = false;
            tmp.undefined_functions = malloc(sizeof(*tmp.undefined_functions));
            stack_init_string(tmp.undefined_functions);
            symtable_insert(&symtable, name, tmp); //prepiseme hodnoty v symtable
            stack_sem_push(&stack_semantic, BLOCK_START, NULL); //vlozeni zacatku bloku
        }
        //nutne vlozit parametry, ktere jsou nacteny v stack_semantic_params, budou tam v opacnem poradi
        tdata data;

        //pro nacteni parametru uz bereme, ze jsme ve funkci
        in_function = true;

        while (!stack_sem_empty(&stack_semantic_params)){
            data = stack_sem_top(&stack_semantic_params);
            stack_sem_pop(&stack_semantic_params);
            stack_sem_push(&stack_semantic, VAR_DEF, data.name);
        }
        break;

    case FUN_CALL:
        if (in_function){ //pokud se vola z funkce
            if (symtable_search(symtable, name, &tmp)){ //pokud je v tabulce zaznam
                if (tmp.is_variable){ //pokud existuje promenna stejneho jmena
                    if (tmp.global || tmp.local) //je definovana
                        error_exit(ERROR_SEM_UNDEF);
                    else { //neni definovana
                        tmp.is_variable = false;
                        tmp.global = true;
                        tmp.local = false;
                        tmp.param_num = arg_num;
                        tmp.undefined_functions = malloc(sizeof(*tmp.undefined_functions));
                        stack_init_string(tmp.undefined_functions);
                        tmp.read_from_global = false;
                        tmp.forward_call = true;
                        symtable_insert(&symtable, name, tmp);

                        //jeste pridame do zavislosti funkce, ve ktere je volana
                        symtable_search(symtable, fun_name, &tmp);
                        stack_push_string(tmp.undefined_functions, name);
                    }
                }
                else { //neni promenna
                    if (tmp.param_num != arg_num)
                        error_exit(ERROR_SEM_CALL);
                }
            }
            else { //neni v tabulce symbolu
                tmp.is_variable = false;
                tmp.global = true;
                tmp.local = false;
                tmp.param_num = arg_num;
                tmp.undefined_functions = malloc(sizeof(*tmp.undefined_functions));
                stack_init_string(tmp.undefined_functions);
                tmp.read_from_global = false;
                tmp.forward_call = true;
                symtable_insert(&symtable, name, tmp);

                //jeste pridame do zavislosti funkce, ve ktere je volana
                symtable_search(symtable, fun_name, &tmp);
                stack_push_string(tmp.undefined_functions, name);
            }
        }
        else { //neni ve funkci
            if (symtable_search(symtable, name, &tmp) && !tmp.is_variable && tmp.global) { //pokud odpovida definici v symtable vse ok
                if (tmp.param_num == arg_num) {
                    //musime se jeste podivat, zda jsou definovane vsechny zavisle fce
                    check_for_fun_def(name);
                }
                else
                    error_exit(ERROR_SEM_CALL);
            }
            else
                error_exit(ERROR_SEM_UNDEF);
        }

        break;

    case BLOCK_START:
        /* POUZE VLOZ NA ZASOBNIK */
        break;

    case VAR_DEF:
        //musime overit, zda neexistuje definovana fce se stejnym jmenem
        if (symtable_search(symtable, name, &tmp)){ //pokud je dany symbol v tabulce
            if (!tmp.is_variable && tmp.global) //pokud se jedna o funkci a je definovana
                error_exit(ERROR_SEM_UNDEF);
            else { //pokud uz bylo v tabulce, ale neni to fce nebo neni definovana, jenom upravime parametry
                if (in_function) { //pokud se nachazime v lokalni tabulce symbolu
                    if (tmp.read_from_global) //pokud jsme z ni cetli a byla globalni tak error pri lokalni definici
                        error_exit(ERROR_SEM_UNDEF);
                    tmp.is_variable = true;
                    tmp.local = true;
                    symtable_insert(&symtable, name, tmp);
                }
                else { //pokud jsme na globalni urovni, nedelame nic
                    tmp.global = true;
                    tmp.read_from_global = false;
                    tmp.local = false;
                    symtable_insert(&symtable, name, tmp);
                }
            }
        }
        else { //pokud neni v tabulce, musime rucne nastavit parametry
            if (in_function){ //pokud je lokalni
                tmp.global = false;
                tmp.is_variable = true;
                tmp.local = true;
                tmp.read_from_global = false;
                symtable_insert(&symtable, name, tmp);
            }
            else{ //pokud je globalni
                tmp.global = true;
                tmp.is_variable = true;
                tmp.local = false;
                tmp.read_from_global = false;
                symtable_insert(&symtable, name, tmp);
            }
        }
        break;

    case VAR_USE:
            //promenna musi byt v tabulce a byt definovana
            if (symtable_search(symtable, name, &tmp) && tmp.is_variable && (tmp.global || tmp.local)){
                if (in_function){
                    if (tmp.local)
                        /*DO NOTHING, EVERYTHING IS FINE*/;
                    else {
                        tmp.read_from_global = true; //cteme z globalni, musime si pohlidat, ze ji nebudeme lokane definovat
                        symtable_insert(&symtable, name, tmp);
                    }
                }
                else{
                    /*DO NOTHING, EVERYTHING IS FINE*/;
                }
            }
            else
                error_exit(ERROR_SEM_UNDEF);
        break;
    }
}

void stack_sem_pop_until_block_start(tStack_sem* s) {
    tdata data = stack_sem_top(s);
    Record tmp;

    while (data.action != BLOCK_START){
        if (data.action == VAR_DEF || data.action == VAR_USE) {
            symtable_search(symtable, data.name, &tmp);
            //snizime pocitadlo definici, pokud se jedna o funkci, jde na 0, pokud o promennou snizi se o 1
            tmp.local = false;
            tmp.read_from_global = false;
            symtable_insert(&symtable, data.name, tmp);
        }

        stack_sem_pop(s);
        data = stack_sem_top(s);
    }
    stack_sem_pop(s); //popneme i BLOCK_START
}

/* konec souboru IFJ_stack_semantic.c */
