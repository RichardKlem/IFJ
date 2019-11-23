/* ******************************* IFJ_parser.c ****************************
 *  Predmet: Formalni jazyky a prekladace (IFJ) - FIT VUT v Brne
 *  Nazev projektu: Implementace prekladace imperativniho jazyka IFJ19
 *  Autori: Beranek Tomas (xberan46) - vedouci
 *          Haderka Martin (xhader00)
 *          Slobodnik Simon (xslobo06)
 *          Klem Richard (xklemr00)
 *  Datum vytvoreni: 23.11.2019
 * ************************************************************************** */

/*

LL tabulka
                |+  |-  |/  |*  |// |<  |<= |>  |>= |str|dbl|int|!= |== |key-None|id |(  |)  |eol|,  |:  |id |indent|dedent|=  |key-def|key-else|key-while|key-pass|key-return|key-if|eof|
<prog>			|   |   |   |   |   |   |   |   |   |1  |1  |1  |   |   |1       |   |1  |   |1  |   |   |1  |      |      |   |1      |        |1        |1       |          |1     |1  |
<st-list>		|   |   |   |   |   |   |   |   |   |2  |2  |2  |   |   |2       |2  |2  |   |   |   |   |   |      |4     |   |2      |        |2        |2       |4         |2     |4  |
<stat>			|   |   |   |   |   |   |   |   |   |19*|19*|19*|   |   |19*     |5  |19*|   |   |   |   |   |      |      |   |11     |        |17       |3       |          |16    |   |
<eol-opt>		|   |   |   |   |   |   |   |   |   |10 |10 |10 |   |   |10      |   |10 |   |9  |   |   |10 |10    |10    |   |10     |        |10       |10      |10        |10    |10 |
<return>		|   |   |   |   |   |   |   |   |   |   |   |   |   |   |        |   |   |   |   |   |   |   |      |8     |   |       |        |         |        |7         |      |   |
<param-list>	|   |   |   |   |   |   |   |   |   |   |   |   |   |   |        |12 |   |13 |   |   |   |   |      |      |   |       |        |         |        |          |      |   |
<param-next>	|   |   |   |   |   |   |   |   |   |   |   |   |   |   |        |   |   |14 |   |15 |   |   |      |      |   |       |        |         |        |          |      |   |
<expr-or-assign>|18*|18*|18*|18*|18*|18*|18*|18*|18*|   |   |   |18*|18*|        |   |24 |   |29 |   |   |   |      |      |6  |       |        |         |        |          |      |   |
<arg-list>		|   |   |   |   |   |   |   |   |   |25*|25*|25*|   |   |25*     |25*|   |26 |   |   |   |   |      |      |   |       |        |         |        |          |      |   |
<arg-next>		|   |   |   |   |   |   |   |   |   |   |   |   |   |   |        |   |   |27 |   |28*|   |   |      |      |   |       |        |         |        |          |      |   |
<fun-or-expr>	|   |   |   |   |   |   |   |   |   |22*|22*|22*|   |   |22*     |   |22*|   |   |   |   |20 |      |      |   |       |        |         |        |          |      |   |
<fun-or-expr-2>	|23*|23*|23*|23*|23*|23*|23*|23*|23*|   |   |   |23*|23*|        |21 |   |   |   |   |   |   |      |      |   |       |        |         |        |          |      |   |

*/

#include <stdio.h>
#include "IFJ_scanner.h"
#include "IFJ_parser.h"

token_t next_token; //globalni token

void prog(){
    next_token = get_token(stdin);

    //pravidlo 1
    if (next_token.type == TOKEN_STRING ||
        next_token.type == TOKEN_DOUBLE ||
        next_token.type == TOKEN_INT ||
        (next_token.type == TOKEN_KEYWORD && next_token.value.keyword_value == NONE) ||
        next_token.type == TOKEN_LEFT_BRACKET ||
        next_token.type == TOKEN_EOL ||
        next_token.type == TOKEN_ID ||
        (next_token.type == TOKEN_KEYWORD && next_token.value.keyword_value == DEF) ||
        (next_token.type == TOKEN_KEYWORD && next_token.value.keyword_value == WHILE) ||
        (next_token.type == TOKEN_KEYWORD && next_token.value.keyword_value == PASS) ||
        (next_token.type == TOKEN_KEYWORD && next_token.value.keyword_value == IF) ||
        next_token.type == TOKEN_EOF){
            eol_opt();
            st_list();
            if (next_token.type == TOKEN_EOF) //pokud se korekne zpracovali vsechny tokeny, koncime
                return;
            else
                error_exit(ERROR_SYNTAX);
    }
    else
        error_exit(ERROR_SYNTAX);
}

void st_list(){
    //pravidlo 4
    if (next_token.type == TOKEN_DEDENT ||
        (next_token.type == TOKEN_KEYWORD && next_token.value.keyword_value == RETURN) ||
        next_token.type == TOKEN_EOF)
        /*DO NOTHING*/;
    //pravidlo 2
    else if (next_token.type == TOKEN_STRING ||
            next_token.type == TOKEN_DOUBLE ||
            next_token.type == TOKEN_INT ||
            (next_token.type == TOKEN_KEYWORD && next_token.value.keyword_value == NONE) ||
            next_token.type == TOKEN_ID ||
            next_token.type == TOKEN_LEFT_BRACKET ||
            (next_token.type == TOKEN_KEYWORD && next_token.value.keyword_value == DEF) ||
            (next_token.type == TOKEN_KEYWORD && next_token.value.keyword_value == WHILE) ||
            (next_token.type == TOKEN_KEYWORD && next_token.value.keyword_value == PASS) ||
            (next_token.type == TOKEN_KEYWORD && next_token.value.keyword_value == IF) ||){
                stat();
                eol_opt();
                st_list();
            }
    else
        error_exit(ERROR_SYNTAX);
}

void stat(){
    //pravidlo 19*
    if (next_token.type == TOKEN_STRING){
        next_token = get_token(stdin);
        //TODO
        if (next_token.type == TOKEN_EOL)
            next_token = get_token(stdin);
    }
    else if (next_token.type == TOKEN_DOUBLE){
        next_token = get_token(stdin);

        //TODO

        if (next_token.type == TOKEN_EOL)
            next_token = get_token(stdin);

    }
}










/* konec souboru IFJ_parser.c */
