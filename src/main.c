/* ******************************* main.c ***********************************
 *  Předmět: Formální jazyky a překladače (IFJ) - FIT VUT v Brně
 *  Název projektu: Implementace překladače imperativního jazyka IFJ19
 *  Autoři: Beránek Tomáš (xberan46) - vedoucí
 *          Haderka Martin (xhader00)
 *          Slobodník Šimon (xslobo06)
 *          Klem Richard (xklem00)
 *  Datum vytvoření: 12.10.2019
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
