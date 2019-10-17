/* ******************************* IFJ_scanner.c ****************************
 *  Pøedmìt: Formální jazyky a pøekladaèe (IFJ) - FIT VUT v Brnì
 *  Název projektu: Implementace pøekladaèe imperativního jazyka IFJ19
 *  Autoøi: Beránek Tomáš (xberan46) - vedoucí
 *          Haderka Martin (xhader00)
 *          Slobodník Šimon (xslobo06)
 *          Klem Richard (xklemr00)
 *  Datum vytvoření: 12.10.2019
 * ************************************************************************** */

#include <ctype.h>
#include "IFJ_scanner.h"

//posune offset o 1 byte zpet, implicitní soubor je src_file
#define push_char_back(x) fseek(src_file, -(x), SEEK_CUR)

token_t create_token(int token_id, token_value value){ //TODO upravit podle struktury tokenu
    token_t token;
    token.type = token_id;
    token.value = value;
    return token;
}


token_t get_token(FILE* src_file){
    int next_char = EOF;
    int state = STATE_START;

    while (1){
        next_char = fgetc(src_file);    //nacteni dalsiho znaku
        if (next_char == EOF)   //pri vytvoreni stavu EOF smazat
            exit(0);

        switch(state){
        case STATE_START:
            if (next_char == '+') {state = STATE_PLUS; break;} //break vlozen, aby se zbytecne nevyhodnocovali podminky
            if (next_char == '-') {state = STATE_MINUS; break;}
            if (next_char == '*') {state = STATE_MUL; break;}
            if (next_char == '/') {state = STATE_DIV; break;}
            if (next_char == '<') {state = STATE_LESS; break;}
            if (next_char == '>') {state = STATE_GREATER; break;}
            if (next_char == '(') {state = STATE_LEFT_BRACKET; break;}
            if (next_char == ')') {state = STATE_RIGHT_BRACKET; break;}
            if (next_char == EOF) {state = STATE_EOF; break;}
            //TODO otatni stavy
            break;

        case STATE_PLUS:
            push_char_back(1);
            return create_token(TOKEN_MATH_PLUS, NO_PARAM);

        case STATE_MINUS:
            push_char_back(1);
            return create_token(TOKEN_MATH_MINUS, NO_PARAM);

        case STATE_MUL:
            push_char_back(1);
            return create_token(TOKEN_MATH_MUL, NO_PARAM);

        case STATE_DIV:
            if (next_char == '/')
                state = STATE_INT_DIV;
            else {
                push_char_back(1);
                return create_token(TOKEN_MATH_DIV, NO_PARAM);
            }
            break;

        case STATE_INT_DIV:
            push_char_back(1);
            return create_token(TOKEN_MATH_INT_DIV, NO_PARAM);

        case STATE_LESS:
            if (next_char == '=')
                state = STATE_LESS_EQ;
            else {
                push_char_back(1);
                return create_token(TOKEN_LESS, NO_PARAM);
            }
            break;

        case STATE_LESS_EQ:
            push_char_back(1);
            return create_token(TOKEN_LESS_EQ, NO_PARAM);

        case STATE_GREATER:
            if (next_char == '=')
                state = STATE_GREATER_EQ;
            else {
                push_char_back(1);
                return create_token(TOKEN_GREATER, NO_PARAM);
            }
            break;

        case STATE_GREATER_EQ:
            push_char_back(1);
            return create_token(TOKEN_GREATER_EQ, NO_PARAM);

        case STATE_LEFT_BRACKET:
            push_char_back(1);
            return create_token(TOKEN_LEFT_BRACKET, NO_PARAM);

        case STATE_RIGHT_BRACKET:
            push_char_back(1);
            return create_token(TOKEN_RIGHT_BRACKET, NO_PARAM);

        case STATE_EOF:
            return create_token(TOKEN_EOF, NO_PARAM);

        default:
            break;
        } //switch
    } //while

} //get_token


 /* konec souboru IFJ_scanner.c */
