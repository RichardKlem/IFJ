/* ******************************* IFJ_scanner_test_app.c ***********************************
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

int main (int argc, char *argv[]) {
    if (argc != 2){
        fprintf(stderr, "Zadejte pouze relativni/absolutni cestu k souboru bez mezer!\n")
        return -1;
    }

    FILE* src_file = fopen(argv[1], "r");
    if (src_file == NULL)
        return -1;

    token_t token = get_token(src_file);
    while (token.type != TOKEN_EOF){


    }

    return 0;
}

/* konec souboru IFJ_scanner_test_app.c */
