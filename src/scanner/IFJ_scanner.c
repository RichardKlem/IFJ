/* ******************************* IFJ_scanner.c ****************************
 *  Pøedmìt: Formální jazyky a pøekladaèe (IFJ) - FIT VUT v Brnì
 *  Název projektu: Implementace pøekladaèe imperativního jazyka IFJ19
 *  Autoøi: Beránek Tomáš (xberan46) - vedoucí
 *          Haderka Martin (xhader00)
 *          Slobodník Šimon (xslobo06)
 *          Klem Richard (xklemr00)
 *  Datum vytvoření: 12.10.2019
 * ************************************************************************** */

#include "IFJ_scanner.h"

token_t create_token(int token_id, token_value value){ //TODO upravit podle struktury tokenu
    token_t token;
    token.type = token_id;
    token.value = value;
    return token;
}


token_t get_token(FILE* src_file){
    //TODO nacitani

    int next_char = EOF;
    int state = STATE_START;

    while (1){
        next_char = fgetc(src_file);    //nacteni dalsiho znaku
        if (next_char == EOF)
            exit(0);

        switch(state){
        case STATE_START:
            if (next_char == '+') {state = STATE_PLUS; break;}
            if (next_char == '-') {state = STATE_MINUS;}
            if (next_char == '*') {state = STATE_MUL;}
            if (next_char == '/') {state = STATE_DIV;}
            //TODO otatni stavy
            break;
        case STATE_PLUS:
            return create_token(TOKEN_MATH_PLUS, NO_PARAM);
        case STATE_MINUS:
            return create_token(TOKEN_MATH_MINUS, NO_PARAM);
        case STATE_MUL:
            return create_token(TOKEN_MATH_MUL, NO_PARAM);
        case STATE_DIV:
            if (next_char == '/')
                state = STATE_INT_DIV;
            else
                return create_token(TOKEN_MATH_DIV, NO_PARAM);
            break;

        case STATE_INT_DIV:
            return create_token(TOKEN_MATH_INT_DIV, NO_PARAM);



        default:
            break;
        } //switch
    } //while

} //get_token


 /* konec souboru IFJ_scanner.c */
