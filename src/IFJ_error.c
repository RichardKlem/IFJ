/* ******************************* IFJ_error.c ****************************
 *  Predmet: Formalni jazyky a prekladace (IFJ) - FIT VUT v Brne
 *  Nazev projektu: Implementace prekladace imperativniho jazyka IFJ19
 *  Autori: Beranek Tomas (xberan46) - vedouci
 *          Haderka Martin (xhader00)
 *          Slobodnik Simon (xslobo06)
 *          Klem Richard (xklemr00)
 *  Datum vytvoreni: 12.10.2019
 * ************************************************************************** */

#include "IFJ_error.h"
#include "IFJ_parser.h"
#include "symtable.h"
#include "IFJ_stack_semantic.h"
#include <stdio.h>
#include <stdlib.h>

void error_exit(int error_id) {
  int error_num = 0;
  switch (error_id) {
    case ERROR_LEX:
        fprintf(stderr, "%s\n", "1 -  chyba v programu v ramci lexikalni analyzy.\n");
        error_num = 1;
        break;
    case ERROR_SYNTAX:
        fprintf(stderr, "%s\n", "2 - chyba v programu v ramci syntakticke analyzy.\n");
        error_num = 2;
        break;
    case ERROR_SEM_UNDEF:
        fprintf(stderr, "%s\n", "3 - semanticka chyba v programu – nedefinovaná funkce/promenna, pokus o redefinici funkce/promenne, atp.\n");
        error_num = 3;
        break;
    case ERROR_SEM_TYPE:
        fprintf(stderr, "%s\n", "4 - semanticka/behova chyba typové kompatibility v aritmetickych, retezcovych a relacnich vyrazech.\n");
        error_num = 4;
        break;
    case ERROR_SEM_CALL:
        fprintf(stderr, "%s\n", "5 - semanticka chyba v programu – spatny pocet parametru u volani funkce.\n");
        error_num = 5;
        break;
    case ERROR_SEM_OTHER:
        fprintf(stderr, "%s\n", "6 - ostatni semanticke chyby.\n");
        error_num = 6;
        break;
    case ERROR_DIVISION_BY_ZERO:
        fprintf(stderr, "%s\n", "9 - behova chyba deleni nulou.\n");
        error_num = 9;
        break;
    case ERROR_INTERNAL:
        fprintf(stderr, "%s\n", "99 - interni chyba prekladace tj. neovlivnena vstupnim programem (napr. chyba alokace pameti, atd.).\n");
        error_num = 99;
        break;
  }

    //uvolneni zasobniku pro sematickou analyzu
    while (!stack_sem_empty(&stack_semantic))
        stack_sem_pop(&stack_semantic);

    //uvolneni tabulky symbolu
    symtable_dispose(&symtable);
    exit(error_num);
}
/* konec souboru IFJ_error.c */
