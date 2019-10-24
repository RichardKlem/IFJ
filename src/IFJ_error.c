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
#include <stdio.h>

void error_exit(int error_id) {
  switch (error_id) {
    case ERROR_LEX:
      fprintf(stderr, "%s\n", "1 -  chyba v programu v ramci lexikalni analyzy.\n");
      exit(1);
    case ERROR_SYNTAX:
      fprintf(stderr, "%s\n", "2 - chyba v programu v ramci syntakticke analyzy.\n");
      exit(2);
    case ERROR_SEM_UNDEF:
      fprintf(stderr, "%s\n", "3 - semanticka chyba v programu – nedefinovaná funkce/promenna, pokus o redefinici funkce/promenne, atp.\n");
      exit(3);
    case ERROR_SEM_TYPE:
      fprintf(stderr, "%s\n", "4 - semanticka/behova chyba typové kompatibility v aritmetickych, retezcovych a relacnich vyrazech.\n");
      exit(4);
    case ERROR_SEM_CALL:
      fprintf(stderr, "%s\n", "5 - semanticka chyba v programu – spatny pocet parametru u volani funkce.\n");
      exit(5);
    case ERROR_SEM_OTHER:
      fprintf(stderr, "%s\n", "6 - ostatni semanticke chyby.\n");
      exit(6);
    case ERROR_DIVISION_BY_ZERO:
      fprintf(stderr, "%s\n", "9 - behova chyba deleni nulou.\n");
      exit(9);
    case ERROR_INTERNAL:
      fprintf(stderr, "%s\n", "99 - interni chyba prekladace tj. neovlivnena vstupnim programem (napr. chyba alokace pameti, atd.).\n");
      exit(99);
  }
}
/* konec souboru IFJ_error.c */
