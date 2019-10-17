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
#include "scanner/IFJ_scanner.h"

int main () {
    FILE* src_file = fopen("test.src", "r");
    if (src_file == NULL)
        return 10;

    token_t token = get_token(src_file);
    printf("%d\n", token.type);


    return 0;
}

/* konec souboru main.c */
