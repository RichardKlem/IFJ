/* ******************************* IFJ_scanner.c ****************************
 *  Predmet: Formalni jazyky a prekladace (IFJ) - FIT VUT v Brne
 *  Nazev projektu: Implementace prekladace imperativniho jazyka IFJ19
 *  Autori: Beranek Tomas (xberan46) - vedouci
 *          Haderka Martin (xhader00)
 *          Slobodnik Simon (xslobo06)
 *          Klem Richard (xklemr00)
 *  Datum vytvoreni: 12.10.2019
 * ************************************************************************** */

#include <ctype.h>
#include "IFJ_scanner.h"
#include "IFJ_error.h"
#include <string.h>
#include <stdlib.h>

//posune offset o 1 byte zpet, implicitní soubor je src_file
#define push_char_back(x) fseek(src_file, -(x), SEEK_CUR)

token_t create_token(int token_id, token_value value){ //TODO upravit podle struktury tokenu
    token_t token;
    token.type = token_id;
    token.value = value;
    return token;
}

char *load_to_str(FILE *src_file, size_t chars_loaded_cnt){
    char *str = (char *)malloc(chars_loaded_cnt + 1);
    if (str == NULL)
        error_exit(ERROR_INTERNAL);

    //zkopiruje chars_loaded znaku z souboru do str
    for (size_t i = 0; i < chars_loaded_cnt; i++)
        str[i] = fgetc(src_file);
    str[chars_loaded_cnt] = '\0';
    return str;
}

token_t get_token(FILE* src_file){
    int next_char = EOF;
    int state = STATE_START;
    size_t chars_loaded_cnt = 0;
    token_value value;
    value.string = NULL;

    while (1){
        next_char = fgetc(src_file);    //nacteni dalsiho znaku

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
            if (isdigit(next_char)) {state = STATE_INT; break;}
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

        case STATE_INT:
            chars_loaded_cnt++;

            if (isdigit(next_char))
                state = STATE_INT;
            else if (next_char == '.')
                state = STATE_DOUBLE_DECIMAL_POINT;
            else if (next_char == 'E' || next_char == 'e')
                state = STATE_DOUBLE_EXP;
            else
                push_char_back(chars_loaded_cnt);
                chars_loaded_cnt--;
                value.string = load_to_str(src_file, chars_loaded_cnt);
                return create_token(TOKEN_INT, value);
            break;

        case STATE_DOUBLE_DECIMAL_POINT:
            if (isdigit(next_char)){
                chars_loaded_cnt;
                state = STATE_DOUBLE_WITHOUT_EXP;
            } else
                error_exit(ERROR_LEX);
            break;

        default:
            break;
        } //switch
    } //while

} //get_token


 /* konec souboru IFJ_scanner.c */
