/* ******************************* IFJ_scanner.h ****************************
 *  Pøedmìt: Formální jazyky a pøekladaèe (IFJ) - FIT VUT v Brnì
 *  Název projektu: Implementace pøekladaèe imperativního jazyka IFJ19
 *  Autoøi: Beránek Tomáš (xberan46) - vedoucí
 *          Haderka Martin (xhader00)
 *          Slobodník Šimon (xslobo06)
 *          Klem Richard (xklemr00)
 *  Datum vytvoření: 12.10.2019
 * ************************************************************************** */

#ifndef _IFJ_SCANNER_H_
#define _IFJ_SCANNER_H_

#include <stdio.h>

typedef struct token_t {
    int type;
                        //TODO
} token_t;

token_t get_token(FILE* src_file);

#define STATE_START 0
#define STATE_PLUS 1
#define STATE_MINUS 2
#define STATE_MUL 3
#define STATE_DIV 4
#define STATE_INT_DIV 5//jedna se o operator "//" - celociselne deleni
//TODO dodelat ostatni stavy


#define TOKEN_MATH_PLUS 0
#define TOKEN_MATH_MINUS 1
#define TOKEN_MATH_MUL 2
#define TOKEN_MATH_DIV 3
#define TOKEN_MATH_INT_DIV 4
//TODO dodelat ostatni tokeny

#endif

 /* konec souboru IFJ_scanner.h */
