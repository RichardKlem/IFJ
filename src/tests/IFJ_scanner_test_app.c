/* ******************************* main.c ***********************************
 *  Pøedmìt: Formální jazyky a pøekladaèe (IFJ) - FIT VUT v Brnì
 *  Název projektu: Implementace pøekladaèe imperativního jazyka IFJ19
 *  Autoøi: Beránek Tomáš (xberan46) - vedoucí
 *          Haderka Martin (xhader00)
 *          Slobodník Šimon (xslobo06)
 *          Klem Richard (xklemr00)
 *  Datum vytvoøení: 16.10.2019
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

/* konec souboru main.c */
