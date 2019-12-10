/* ******************************* main.c ***********************************
 *  Predmet: Formalni jazyky a prekladace (IFJ) - FIT VUT v Brne
 *  Nazev projektu: Implementace prekladace imperativniho jazyka IFJ19
 *  Autori: Beranek Tomas (xberan46) - vedouci
 *          Haderka Martin (xhader00)
 *          Slobodnik Simon (xslobo06)
 *          Klem Richard (xklemr00)
 *  Datum vytvoreni: 12.10.2019
 * ************************************************************************** */

#include <stdio.h>
#include "IFJ_scanner.h"
#include "IFJ_error.h"
#include "IFJ_parser.h"

int main () {

    //zavolaní prvního pravidla
    prog();

    return 0;
}

/* konec souboru main.c */
