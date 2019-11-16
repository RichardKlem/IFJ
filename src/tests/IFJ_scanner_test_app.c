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
#include "IFJ_scanner.h"

int main (int argc, char *argv[]) {
    if (argc != 2){
        fprintf(stderr, "Zadejte pouze relativni/absolutni cestu k souboru bez mezer!\n");
        return -1;
    }

    FILE* src_file = fopen(argv[1], "r");
    if (src_file == NULL)
        return -1;

    token_t token = get_token(src_file);
    while (token.type != TOKEN_EOF){
        printf("TOKEN(");
        switch (token.type){
        case TOKEN_EOL:
            printf("EOL");
            break;

        case TOKEN_MATH_PLUS:
            printf("+");
            break;

        case TOKEN_MATH_MINUS:
            printf("-");
            break;

        case TOKEN_MATH_MUL:
            printf("*");
            break;

        case TOKEN_MATH_DIV:
            printf("/");
            break;

        case TOKEN_MATH_INT_DIV:
            printf("//");
            break;

        case TOKEN_LESS:
            printf("<");
            break;

        case TOKEN_LESS_EQ:
            printf("<=");
            break;

        case TOKEN_GREATER:
            printf(">");
            break;

        case TOKEN_GREATER_EQ:
            printf(">=");
            break;

        case TOKEN_LEFT_BRACKET:
            printf("(");
            break;

        case TOKEN_RIGHT_BRACKET:
            printf(")");
            break;

        case TOKEN_COMMA:
            printf(",");
            break;

        case TOKEN_COLON:
            printf(":");
            break;

        case TOKEN_NOT_EQ:
            printf("!=");
            break;

        case TOKEN_ASSIGNMENT:
            printf("=");
            break;

        case TOKEN_EQ:
            printf("==");
            break;

        case TOKEN_ID:
            printf("IDENTIFIKATOR");
            break;

        case TOKEN_KEYWORD:
            printf("KLICOVE_SLOVO");
            break;

        case TOKEN_INDENT:
            printf("INDENT");
            break;

        case TOKEN_DEDENT:
            printf("DEDENT");
            break;

        case TOKEN_INT:
            printf("INTEGER");
            break;

        case TOKEN_DOUBLE:
            printf("DOUBLE");
            break;

        case TOKEN_STRING:
            printf("STRING, %s", token.value.string);
            break;

        default:
            printf("CHYBNY_TYP_TOKENU");
            break;
        } //switch
        printf(")\n");
        token = get_token(src_file);
    } //while

    printf("TOKEN(EOF)\n");

    return 0;
}

/* konec souboru IFJ_scanner_test_app.c */
