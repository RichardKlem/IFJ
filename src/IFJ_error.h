/* ******************************* IFJ_error.h ****************************
 *  Predmet: Formalni jazyky a prekladace (IFJ) - FIT VUT v Brne
 *  Nazev projektu: Implementace prekladace imperativniho jazyka IFJ19
 *  Autori: Beranek Tomas (xberan46) - vedouci
 *          Haderka Martin (xhader00)
 *          Slobodnik Simon (xslobo06)
 *          Klem Richard (xklemr00)
 *  Datum vytvoreni: 12.10.2019
 * ************************************************************************** */

#ifndef _IFJ_ERROR_H_
#define _IFJ_ERROR_H_

typedef enum {
    ERROR_LEX,
    ERROR_SYNTAX,
    ERROR_SEM_UNDEF,
    ERROR_SEM_TYPE,
    ERROR_SEM_CALL,
    ERROR_SEM_OTHER,
    ERROR_DIVISION_BY_ZERO,
    ERROR_INTERNAL
} error_types;

void error_exit(int error_id);

#endif
/* konec souboru IFJ_error.h */
