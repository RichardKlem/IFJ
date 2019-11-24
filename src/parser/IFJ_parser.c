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
                |+  |-  |/  |*  |// |<  |<= |>  |>= |str|dbl|int|!= |== |key-None|id |(  |)  |eol|,  |:  |indent|dedent|=  |key-def|key-else|key-while|key-pass|key-return|key-if|eof|
<prog>			|   |   |   |   |   |   |   |   |   |1  |1  |1  |   |   |1       |1  |1  |   |1  |   |   |      |      |   |1      |        |1        |1       |          |1     |1  |
<st-list>		|   |   |   |   |   |   |   |   |   |2  |2  |2  |   |   |2       |2  |2  |   |   |   |   |      |4     |   |2      |        |2        |2       |4         |2     |4  |
<stat>			|   |   |   |   |   |   |   |   |   |19*|19*|19*|   |   |19*     |5  |19*|   |   |   |   |      |      |   |11     |        |17       |3       |          |16    |   |
<eol-opt>		|   |   |   |   |   |   |   |   |   |10 |10 |10 |   |   |10      |10 |10 |   |9  |   |   |10    |10    |   |10     |        |10       |10      |10        |10    |10 |
<return>		|   |   |   |   |   |   |   |   |   |   |   |   |   |   |        |   |   |   |   |   |   |      |8     |   |       |        |         |        |7         |      |   |
<param-list>	|   |   |   |   |   |   |   |   |   |   |   |   |   |   |        |12 |   |13 |   |   |   |      |      |   |       |        |         |        |          |      |   |
<param-next>	|   |   |   |   |   |   |   |   |   |   |   |   |   |   |        |   |   |14 |   |15 |   |      |      |   |       |        |         |        |          |      |   |
<expr-or-assign>|18*|18*|18*|18*|18*|18*|18*|18*|18*|   |   |   |18*|18*|        |   |24 |   |29 |   |   |      |      |6  |       |        |         |        |          |      |   |
<arg-list>		|   |   |   |   |   |   |   |   |   |25*|25*|25*|   |   |25*     |25*|   |26 |   |   |   |      |      |   |       |        |         |        |          |      |   |
<arg-next>		|   |   |   |   |   |   |   |   |   |   |   |   |   |   |        |   |   |27 |   |28*|   |      |      |   |       |        |         |        |          |      |   |
<fun-or-expr>	|   |   |   |   |   |   |   |   |   |22*|22*|22*|   |   |22*     |20 |22*|   |29 |   |   |      |      |   |       |        |         |        |          |      |   |
<fun-or-expr-2>	|23*|23*|23*|23*|23*|23*|23*|23*|23*|   |   |   |23*|23*|        |   |21 |   |30 |   |   |      |      |   |       |        |         |        |          |      |   |

*/

#include <stdio.h>
#include "IFJ_scanner.h"
#include "IFJ_parser.h"
#include "../IFJ_error.h"
#include "symtable.h"

token_t next_token; //globalni token
Record id_param; //pomocna struktura pro ukladani informaci o promennych
tBSTNodePtr symtable;

void prog(){
    printf("In main\n");
    next_token = get_token(stdin);

    //inicializace tabulky symbolu
    symtable_init(&symtable);

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
            if (next_token.type == TOKEN_EOF){ //pokud se korekne zpracovali vsechny tokeny, koncime



                symtable_dispose(&symtable);
                return;
            }
            else
                error_exit(ERROR_SYNTAX);
    }
    else
        error_exit(ERROR_SYNTAX);
}

void st_list(){
    printf("In st_list\n");
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
            (next_token.type == TOKEN_KEYWORD && next_token.value.keyword_value == IF)){
                stat();
                eol_opt();
                st_list();
            }
    else
        error_exit(ERROR_SYNTAX);
}

void stat(){
    printf("In stat\n");
    //pravidlo 19*
    if (next_token.type == TOKEN_STRING){
        //TODO <expr1>
        if (next_token.type == TOKEN_EOL)
            next_token = get_token(stdin);
        else
            error_exit(ERROR_SYNTAX);
    }
    else if (next_token.type == TOKEN_DOUBLE){
        //TODO <expr1>
        if (next_token.type == TOKEN_EOL)
            next_token = get_token(stdin);
        else
            error_exit(ERROR_SYNTAX);
    }
    else if (next_token.type == TOKEN_INT){
        //TODO <expr1>
        if (next_token.type == TOKEN_EOL)
            next_token = get_token(stdin);
        else
            error_exit(ERROR_SYNTAX);
    }
    else if (next_token.type == TOKEN_KEYWORD && next_token.value.keyword_value == NONE){
        //TODO <expr1>
        if (next_token.type == TOKEN_EOL)
            next_token = get_token(stdin);
        else
            error_exit(ERROR_SYNTAX);
    }
    else if (next_token.type == TOKEN_LEFT_BRACKET){
        //TODO <expr1>
        if (next_token.type == TOKEN_EOL)
            next_token = get_token(stdin);
        else
            error_exit(ERROR_SYNTAX);
    }
    //pravidlo 5
    else if (next_token.type == TOKEN_ID){
        next_token = get_token(stdin);
        expr_or_assign();
        if (next_token.type == TOKEN_EOL)
            next_token = get_token(stdin);
        else
            error_exit(ERROR_SYNTAX);
    }
    //pravidlo 11
    else if (next_token.type == TOKEN_KEYWORD && next_token.value.keyword_value == DEF){
        next_token = get_token(stdin);
        if (next_token.type == TOKEN_ID)
            next_token = get_token(stdin);
        else error_exit(ERROR_SYNTAX);
        if (next_token.type == TOKEN_LEFT_BRACKET)
            next_token = get_token(stdin);
        else error_exit(ERROR_SYNTAX);
        param_list();
        if (next_token.type == TOKEN_RIGHT_BRACKET)
            next_token = get_token(stdin);
        else error_exit(ERROR_SYNTAX);
        if (next_token.type == TOKEN_COLON)
            next_token = get_token(stdin);
        else error_exit(ERROR_SYNTAX);
        if (next_token.type == TOKEN_EOL)
            next_token = get_token(stdin);
        else error_exit(ERROR_SYNTAX);
        eol_opt();
        if (next_token.type == TOKEN_INDENT)
            next_token = get_token(stdin);
        else error_exit(ERROR_SYNTAX);
        st_list();
        ret();
        if (next_token.type == TOKEN_DEDENT)
            next_token = get_token(stdin);
        else error_exit(ERROR_SYNTAX);
    }
    //pravidlo 17
    else if (next_token.type == TOKEN_KEYWORD && next_token.value.keyword_value == WHILE){
        next_token = get_token(stdin);
        //TODO <expr20>
        if (next_token.type == TOKEN_COLON)
            next_token = get_token(stdin);
        else error_exit(ERROR_SYNTAX);
        if (next_token.type == TOKEN_EOL)
            next_token = get_token(stdin);
        else error_exit(ERROR_SYNTAX);
        eol_opt();
        if (next_token.type == TOKEN_INDENT)
            next_token = get_token(stdin);
        else error_exit(ERROR_SYNTAX);
        st_list();
        if (next_token.type == TOKEN_DEDENT)
            next_token = get_token(stdin);
        else error_exit(ERROR_SYNTAX);
    }
    //pravidlo 3
    else if (next_token.type == TOKEN_KEYWORD && next_token.value.keyword_value == PASS){
        next_token = get_token(stdin);
        if (next_token.type == TOKEN_EOL) {
            next_token = get_token(stdin);
        } else error_exit(ERROR_SYNTAX);
    }
    //pravidlo 16
    else if (next_token.type == TOKEN_KEYWORD && next_token.value.keyword_value == IF){
        next_token = get_token(stdin);
        //TODO <expr20>
        if (next_token.type == TOKEN_COLON)
            next_token = get_token(stdin);
        else error_exit(ERROR_SYNTAX);
        if (next_token.type == TOKEN_EOL)
            next_token = get_token(stdin);
        else error_exit(ERROR_SYNTAX);
        eol_opt();
        if (next_token.type == TOKEN_INDENT)
            next_token = get_token(stdin);
        else error_exit(ERROR_SYNTAX);
        st_list();
        if (next_token.type == TOKEN_DEDENT)
            next_token = get_token(stdin);
        else error_exit(ERROR_SYNTAX);
        if (next_token.type == TOKEN_KEYWORD && next_token.value.keyword_value == ELSE)
            next_token = get_token(stdin);
        else error_exit(ERROR_SYNTAX);
        if (next_token.type == TOKEN_COLON)
            next_token = get_token(stdin);
        else error_exit(ERROR_SYNTAX);
        if (next_token.type == TOKEN_EOL)
            next_token = get_token(stdin);
        else error_exit(ERROR_SYNTAX);
        eol_opt();
        if (next_token.type == TOKEN_INDENT)
            next_token = get_token(stdin);
        else error_exit(ERROR_SYNTAX);
        st_list();
        if (next_token.type == TOKEN_DEDENT)
            next_token = get_token(stdin);
        else error_exit(ERROR_SYNTAX);
    }
    else
        error_exit(ERROR_SYNTAX);
}

void eol_opt(){
    printf("In eol_opt\n");
    //pravidlo 10
    if (next_token.type == TOKEN_STRING ||
        next_token.type == TOKEN_DOUBLE ||
        next_token.type == TOKEN_INT ||
        (next_token.type == TOKEN_KEYWORD && next_token.value.keyword_value == NONE) ||
        next_token.type == TOKEN_LEFT_BRACKET ||
        next_token.type == TOKEN_ID ||
        next_token.type == TOKEN_INDENT ||
        next_token.type == TOKEN_DEDENT ||
        (next_token.type == TOKEN_KEYWORD && next_token.value.keyword_value == DEF) ||
        (next_token.type == TOKEN_KEYWORD && next_token.value.keyword_value == WHILE) ||
        (next_token.type == TOKEN_KEYWORD && next_token.value.keyword_value == PASS) ||
        (next_token.type == TOKEN_KEYWORD && next_token.value.keyword_value == RETURN) ||
        (next_token.type == TOKEN_KEYWORD && next_token.value.keyword_value == IF) ||
        next_token.type == TOKEN_EOF)
        /*DO NOTHING*/;
    //pravidlo 9
    else if (next_token.type == TOKEN_EOL){
        next_token = get_token(stdin);
        eol_opt();
    }
    else
        error_exit(ERROR_SYNTAX);
}

void ret(){
    printf("In ret\n");
    //pravidlo 8
    if (next_token.type == TOKEN_DEDENT)
        /*DO NOTHING*/;
    //pravidlo 7
    else if (next_token.type == TOKEN_KEYWORD && next_token.value.keyword_value == RETURN) {
        next_token = get_token(stdin);
        //TODO <expr20>
        if (next_token.type == TOKEN_EOL) {
            next_token = get_token(stdin);
            eol_opt();
        }
        else
            error_exit(ERROR_SYNTAX);
    }
    else
        error_exit(ERROR_SYNTAX);
}

void param_list(){
    printf("In param_list\n");
    //pravidlo 12
    if (next_token.type == TOKEN_ID) {
        next_token = get_token(stdin);
        param_next();
    }
    //pravidlo 13
    else if (next_token.type == TOKEN_RIGHT_BRACKET)
        /* DO NOTHING */;
    else
        error_exit(ERROR_SYNTAX);
}

void param_next(){
    printf("In param_next\n");
    //pravidlo 14
    if (next_token.type == TOKEN_RIGHT_BRACKET)
        /* DO NOTHING */;
    //pravidlo 15
    else if (next_token.type == TOKEN_COMMA) {
        next_token = get_token(stdin);
        if (next_token.type == TOKEN_ID){
            next_token = get_token(stdin);
            param_next();
        }
        else error_exit(ERROR_SYNTAX);
    }
    else
        error_exit(ERROR_SYNTAX);
}

void expr_or_assign() {
    printf("In expr_or_assign\n");
    //pravidlo 18*
    if (next_token.type == TOKEN_MATH_PLUS || next_token.type == TOKEN_MATH_MINUS ||
        next_token.type == TOKEN_MATH_DIV || next_token.type == TOKEN_MATH_MUL ||
        next_token.type == TOKEN_MATH_INT_DIV || next_token.type == TOKEN_LESS ||
        next_token.type == TOKEN_LESS_EQ || next_token.type == TOKEN_GREATER ||
        next_token.type == TOKEN_GREATER_EQ || next_token.type == TOKEN_NOT_EQ ||
        next_token.type == TOKEN_EQ) {
            //TODO <expr2>
            ;
    }
    //pravidlo 24
    else if (next_token.type == TOKEN_LEFT_BRACKET){
        next_token = get_token(stdin);
        arg_list();
        if (next_token.type == TOKEN_RIGHT_BRACKET) {
            next_token = get_token(stdin);
        } else error_exit(ERROR_SYNTAX);
    }
    //pravidlo 29
    else if (next_token.type == TOKEN_EOL) {
        /*DO NOTHING*/;
    }
    //pravidlo 6
    else if (next_token.type == TOKEN_ASSIGNMENT) {
        next_token = get_token(stdin);
        fun_or_expr();
        }
    else
        error_exit(ERROR_SYNTAX);
}

void arg_list() {
    printf("In arg_list\n");
    //pravidlo 25*
    if (next_token.type == TOKEN_STRING){
        next_token = get_token(stdin);
        arg_next();
    }
    else if (next_token.type == TOKEN_DOUBLE){
        next_token = get_token(stdin);
        arg_next();
    }
    else if (next_token.type == TOKEN_INT){
        next_token = get_token(stdin);
        arg_next();
    }
    else if (next_token.type == TOKEN_KEYWORD && next_token.value.keyword_value == NONE){
        next_token = get_token(stdin);
        arg_next();
    }
    else if (next_token.type == TOKEN_ID){
        next_token = get_token(stdin);
        arg_next();
    }
    //pravidlo 26
    else if (next_token.type == TOKEN_RIGHT_BRACKET)
        /*DO NOTHING*/;
    else
        error_exit(ERROR_SYNTAX);
}

void arg_next() {
    printf("In arg_next\n");
    //pravidlo 27
    if (next_token.type == TOKEN_RIGHT_BRACKET)
        /*DO NOTHING*/;
    //pravidlo 28*
    else if (next_token.type == TOKEN_COMMA) {
        next_token = get_token(stdin);
        if (next_token.type == TOKEN_STRING) {
            next_token = get_token(stdin);
            arg_next();
        }
        else if (next_token.type == TOKEN_DOUBLE) {
            next_token = get_token(stdin);
            arg_next();
        }
        else if (next_token.type == TOKEN_INT) {
            next_token = get_token(stdin);
            arg_next();
        }
        else if (next_token.type == TOKEN_KEYWORD && next_token.value.keyword_value == NONE) {
            next_token = get_token(stdin);
            arg_next();
        }
        else if (next_token.type == TOKEN_ID) {
            next_token = get_token(stdin);
            arg_next();
        }
        else
            error_exit(ERROR_SYNTAX);
    }
    else
        error_exit(ERROR_SYNTAX);
}

void fun_or_expr() {
    printf("In fun_or_expr\n");
    //pravidlo 22*
    if (next_token.type == TOKEN_STRING) {
        //TODO <expr1>
    }
    else if (next_token.type == TOKEN_DOUBLE) {
        //TODO <expr1>
    }
    else if (next_token.type == TOKEN_INT) {
        //TODO <expr1>
    }
    else if (next_token.type == TOKEN_KEYWORD && next_token.value.keyword_value == NONE) {
        //TODO <expr1>
    }
    else if (next_token.type == TOKEN_LEFT_BRACKET) {
        //TODO <expr1>
    }
    //pravidlo 20
    else if (next_token.type == TOKEN_ID){
        next_token = get_token(stdin);
        //fun_or_expr(); CHYBA
        fun_or_expr2();
    }
    //pravidlo 29
    else if (next_token.type == TOKEN_EOL)
        /*DO NOTHING*/;
    else
        error_exit(ERROR_SYNTAX);
}

void fun_or_expr2() {
    printf("In fun_or_expr2\n");
    //pravidlo 23*
    if (next_token.type == TOKEN_MATH_PLUS || next_token.type == TOKEN_MATH_MINUS ||
        next_token.type == TOKEN_MATH_DIV || next_token.type == TOKEN_MATH_MUL ||
        next_token.type == TOKEN_MATH_INT_DIV || next_token.type == TOKEN_LESS ||
        next_token.type == TOKEN_LESS_EQ || next_token.type == TOKEN_GREATER ||
        next_token.type == TOKEN_GREATER_EQ || next_token.type == TOKEN_NOT_EQ ||
        next_token.type == TOKEN_EQ) {
            //TODO <expr2>
    }
    //pravidlo 21
    else if (next_token.type == TOKEN_LEFT_BRACKET) {
        next_token = get_token(stdin);
        arg_list();
        if (next_token.type == TOKEN_RIGHT_BRACKET)
            next_token = get_token(stdin);
        else
            error_exit(ERROR_SYNTAX);
    }
    //pravidlo 30
    else if (next_token.type == TOKEN_EOL)
        /*DO NOTHING*/;
    else
        error_exit(ERROR_SYNTAX);
}


/* konec souboru IFJ_parser.c */
