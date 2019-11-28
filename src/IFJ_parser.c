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
#include "IFJ_error.h"
#include "symtable.h"
#include "IFJ_stack_semantic.h"
#include "IFJ_precedence_syntactic_analysis.h"

token_t next_token; //globalni token
token_t first, second; //pomocne tokeny pro uchovani tokenu pri predavani ke zpracovani vyrazu
Record id_param; //pomocna struktura pro ukladani informaci o promennych
tBSTNodePtr symtable; //tabulka symbolu
tStack_sem stack_semantic; //zasobnik pro semantickou analyzu
tStack_sem stack_semantic_params;
int param_num, arg_num, fun_def_nesting = 0;

void prog(){
    printf("In main\n");
    next_token = get_token(stdin);

    //inicializace tabulky symbolu
    symtable_init(&symtable);
    //inicializace zasobniku na semantickou kontrolu
    stack_sem_init(&stack_semantic);
    stack_sem_init(&stack_semantic_params);

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


                while (!stack_sem_empty(&stack_semantic))
                    stack_sem_pop(&stack_semantic);
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
        //(next_token.type == TOKEN_KEYWORD && next_token.value.keyword_value == RETURN) ||
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
            (next_token.type == TOKEN_KEYWORD && next_token.value.keyword_value == IF) ||
            ((fun_def_nesting > 0) && (next_token.type == TOKEN_KEYWORD && next_token.value.keyword_value == RETURN))){
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
    if (next_token.type == TOKEN_STRING || next_token.type == TOKEN_DOUBLE
        || next_token.type == TOKEN_INT || (next_token.type == TOKEN_KEYWORD && next_token.value.keyword_value == NONE)
        || next_token.type == TOKEN_LEFT_BRACKET){
            first = next_token;
            /*****PSA*******/
            next_token = expressionParse(stdin, &first, NULL, 1);
/*******************************************************************************************
        SIMULACE RESENI VYRAZU first
            //TODO <expr1>

        if (first.type == TOKEN_ID)
            stack_sem_push(&stack_semantic, VAR_USE, first.value.string);
        next_token = get_token(stdin);

        while (next_token.type == TOKEN_MATH_PLUS || next_token.type == TOKEN_MATH_MINUS ||
        next_token.type == TOKEN_MATH_DIV || next_token.type == TOKEN_MATH_MUL ||
        next_token.type == TOKEN_MATH_INT_DIV || next_token.type == TOKEN_LESS ||
        next_token.type == TOKEN_LESS_EQ || next_token.type == TOKEN_GREATER ||
        next_token.type == TOKEN_GREATER_EQ || next_token.type == TOKEN_NOT_EQ ||
        next_token.type == TOKEN_EQ || next_token.type == TOKEN_LEFT_BRACKET ||
        next_token.type == TOKEN_RIGHT_BRACKET || next_token.type == TOKEN_STRING ||
        next_token.type == TOKEN_INT || next_token.type == TOKEN_DOUBLE ||
        next_token.type == TOKEN_ID) {
            if (next_token.type == TOKEN_ID)
                stack_sem_push(&stack_semantic, VAR_USE, next_token.value.string);
            next_token = get_token(stdin);
        }

/*********************************************************************************************/
        if (next_token.type == TOKEN_EOL)
            next_token = get_token(stdin);
        else
            error_exit(ERROR_SYNTAX);
    }
    //pravidlo 5
    else if (next_token.type == TOKEN_ID){
        //uchovani identifikatoru
        first = next_token;
        next_token = get_token(stdin);
        expr_or_assign();
        if (next_token.type == TOKEN_EOL)
            next_token = get_token(stdin);
        else
            error_exit(ERROR_SYNTAX);
    }
    //pravidlo 7*/8 sloucene a pouze za podminky, ze jsme ve funkci
    else if ((fun_def_nesting > 0) && (next_token.type == TOKEN_KEYWORD && next_token.value.keyword_value == RETURN)){
        ret(); //pouze presmerujeme do puvodniho pravidla 7/8
    }
    //pravidlo 11 - definice funkce
    else if (next_token.type == TOKEN_KEYWORD && next_token.value.keyword_value == DEF){
        char * fun_name; //pomocna promenna pro uchovani jmena funkce

        next_token = get_token(stdin);
        if (next_token.type == TOKEN_ID) {
            fun_name = next_token.value.string;
            next_token = get_token(stdin);
        }
        else
            error_exit(ERROR_SYNTAX);

        if (next_token.type == TOKEN_LEFT_BRACKET)
            next_token = get_token(stdin);
        else error_exit(ERROR_SYNTAX);

        fun_def_nesting++; //zacina blok prikazu fce, muze nastat return
        //VYTVORENI ZARAZKY PRED ZACATKEM BLOKU
        param_num = 0; //vynuluje pocet parametru
        param_list();
        //stack_sem_push se interne diva na globalni promennou param_num a zaroven vlozi funkci na symtable
        stack_sem_push(&stack_semantic, FUN_DEF, fun_name);

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
       // ret();
        fun_def_nesting--; //konci blok fce, pokud je fun_def_nesting > 0, znamena to, ze jsme stale v definici funkce

        if (next_token.type == TOKEN_DEDENT) {
            //konec definice funkce, uvolnime semanticky zasobnik
            stack_sem_pop_until_block_start(&stack_semantic);

            next_token = get_token(stdin);
        }
        else error_exit(ERROR_SYNTAX);
    }
    //pravidlo 17
    else if (next_token.type == TOKEN_KEYWORD && next_token.value.keyword_value == WHILE){
        next_token = get_token(stdin);
        first = next_token;
        /*****PSA*******/
        next_token = expressionParse(stdin, &first, NULL, 1);
/********************************************************************************************
        SIMULACE RESENI VYRAZU first
            //TODO <expr1>

        if (first.type == TOKEN_ID)
            stack_sem_push(&stack_semantic, VAR_USE, first.value.string);
        next_token = get_token(stdin);

        while (next_token.type == TOKEN_MATH_PLUS || next_token.type == TOKEN_MATH_MINUS ||
        next_token.type == TOKEN_MATH_DIV || next_token.type == TOKEN_MATH_MUL ||
        next_token.type == TOKEN_MATH_INT_DIV || next_token.type == TOKEN_LESS ||
        next_token.type == TOKEN_LESS_EQ || next_token.type == TOKEN_GREATER ||
        next_token.type == TOKEN_GREATER_EQ || next_token.type == TOKEN_NOT_EQ ||
        next_token.type == TOKEN_EQ || next_token.type == TOKEN_LEFT_BRACKET ||
        next_token.type == TOKEN_RIGHT_BRACKET || next_token.type == TOKEN_STRING ||
        next_token.type == TOKEN_INT || next_token.type == TOKEN_DOUBLE ||
        next_token.type == TOKEN_ID) {
            if (next_token.type == TOKEN_ID)
                stack_sem_push(&stack_semantic, VAR_USE, next_token.value.string);
            next_token = get_token(stdin);
        }

/*********************************************************************************************/
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

        //VYTVORENI ZARAZKY PRED ZACATKEM BLOKU
        stack_sem_push(&stack_semantic, BLOCK_START, NULL);
        st_list();
        stack_sem_pop_until_block_start(&stack_semantic);

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
        first = next_token;
        /*****PSA*******/
        next_token = expressionParse(stdin, &first, NULL, 1);
/********************************************************************************************
        SIMULACE RESENI VYRAZU first
            //TODO <expr1>

        if (first.type == TOKEN_ID)
            stack_sem_push(&stack_semantic, VAR_USE, first.value.string);
        next_token = get_token(stdin);

        while (next_token.type == TOKEN_MATH_PLUS || next_token.type == TOKEN_MATH_MINUS ||
        next_token.type == TOKEN_MATH_DIV || next_token.type == TOKEN_MATH_MUL ||
        next_token.type == TOKEN_MATH_INT_DIV || next_token.type == TOKEN_LESS ||
        next_token.type == TOKEN_LESS_EQ || next_token.type == TOKEN_GREATER ||
        next_token.type == TOKEN_GREATER_EQ || next_token.type == TOKEN_NOT_EQ ||
        next_token.type == TOKEN_EQ || next_token.type == TOKEN_LEFT_BRACKET ||
        next_token.type == TOKEN_RIGHT_BRACKET || next_token.type == TOKEN_STRING ||
        next_token.type == TOKEN_INT || next_token.type == TOKEN_DOUBLE ||
        next_token.type == TOKEN_ID) {
            if (next_token.type == TOKEN_ID)
                stack_sem_push(&stack_semantic, VAR_USE, next_token.value.string);
            next_token = get_token(stdin);
        }

/*********************************************************************************************/
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

        //VYTVORENI ZARAZKY PRED ZACATKEM BLOKU
        stack_sem_push(&stack_semantic, BLOCK_START, NULL);
        st_list();
        stack_sem_pop_until_block_start(&stack_semantic);

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

        //VYTVORENI ZARAZKY PRED ZACATKEM BLOKU
        stack_sem_push(&stack_semantic, BLOCK_START, NULL);
        st_list();
        stack_sem_pop_until_block_start(&stack_semantic);

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
        if (next_token.type != TOKEN_EOL && next_token.type != TOKEN_EOF) {
            /*****PSA*******/
            next_token = expressionParse(stdin, &next_token, NULL, 1);
        }


/********************************************************************************************
        SIMULACE RESENI VYRAZU
            //TODO <expr0>

        while (next_token.type == TOKEN_MATH_PLUS || next_token.type == TOKEN_MATH_MINUS ||
        next_token.type == TOKEN_MATH_DIV || next_token.type == TOKEN_MATH_MUL ||
        next_token.type == TOKEN_MATH_INT_DIV || next_token.type == TOKEN_LESS ||
        next_token.type == TOKEN_LESS_EQ || next_token.type == TOKEN_GREATER ||
        next_token.type == TOKEN_GREATER_EQ || next_token.type == TOKEN_NOT_EQ ||
        next_token.type == TOKEN_EQ || next_token.type == TOKEN_LEFT_BRACKET ||
        next_token.type == TOKEN_RIGHT_BRACKET || next_token.type == TOKEN_STRING ||
        next_token.type == TOKEN_INT || next_token.type == TOKEN_DOUBLE ||
        next_token.type == TOKEN_ID) {
            if (next_token.type == TOKEN_ID)
                stack_sem_push(&stack_semantic, VAR_USE, next_token.value.string);
            next_token = get_token(stdin);
        }

/*********************************************************************************************/
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
        param_num++;
        stack_sem_push(&stack_semantic_params, VAR_DEF, next_token.value.string);
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
            param_num++;
            stack_sem_push(&stack_semantic_params, VAR_DEF, next_token.value.string);
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
        second = next_token;
        /*****PSA*******/
        next_token = expressionParse(stdin, &first, &second, 2);
/********************************************************************************************
        SIMULACE RESENI VYRAZU first,second
            //TODO <expr2> */
/*
        if (first.type == TOKEN_ID)
            stack_sem_push(&stack_semantic, VAR_USE, first.value.string);

        if (second.type == TOKEN_ID)
            stack_sem_push(&stack_semantic, VAR_USE, second.value.string);
        next_token = get_token(stdin);

        while (next_token.type == TOKEN_MATH_PLUS || next_token.type == TOKEN_MATH_MINUS ||
        next_token.type == TOKEN_MATH_DIV || next_token.type == TOKEN_MATH_MUL ||
        next_token.type == TOKEN_MATH_INT_DIV || next_token.type == TOKEN_LESS ||
        next_token.type == TOKEN_LESS_EQ || next_token.type == TOKEN_GREATER ||
        next_token.type == TOKEN_GREATER_EQ || next_token.type == TOKEN_NOT_EQ ||
        next_token.type == TOKEN_EQ || next_token.type == TOKEN_LEFT_BRACKET ||
        next_token.type == TOKEN_RIGHT_BRACKET || next_token.type == TOKEN_STRING ||
        next_token.type == TOKEN_INT || next_token.type == TOKEN_DOUBLE ||
        next_token.type == TOKEN_ID) {
            if (next_token.type == TOKEN_ID)
                stack_sem_push(&stack_semantic, VAR_USE, next_token.value.string);
            next_token = get_token(stdin);
        }
*/
/*********************************************************************************************/
            ;
    }
    //pravidlo 24
    else if (next_token.type == TOKEN_LEFT_BRACKET){
        next_token = get_token(stdin);

        arg_num = 0;
        arg_list();
        stack_sem_push(&stack_semantic, FUN_CALL, first.value.string);

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
        //ulozeni jmena promenne, jelikoz nejdrive semanticky zkontroluje vyraz
        char * tmp = first.value.string;

        fun_or_expr();

        //bylo prirazeno do promenne, je nutna vlozit ji do stack_semantic
        stack_sem_push(&stack_semantic, VAR_DEF, tmp);
    }
    else
        error_exit(ERROR_SYNTAX);
}

void arg_list() {
    printf("In arg_list\n");
    //pravidlo 25*
    if (next_token.type == TOKEN_STRING){
        arg_num++;
        next_token = get_token(stdin);
        arg_next();
    }
    else if (next_token.type == TOKEN_DOUBLE){
        arg_num++;
        next_token = get_token(stdin);
        arg_next();
    }
    else if (next_token.type == TOKEN_INT){
        arg_num++;
        next_token = get_token(stdin);
        arg_next();
    }
    else if (next_token.type == TOKEN_KEYWORD && next_token.value.keyword_value == NONE){
        arg_num++;
        next_token = get_token(stdin);
        arg_next();
    }
    else if (next_token.type == TOKEN_ID){
        arg_num++;
        stack_sem_push(&stack_semantic, VAR_USE, next_token.value.string);
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
            arg_num++;
            next_token = get_token(stdin);
            arg_next();
        }
        else if (next_token.type == TOKEN_DOUBLE) {
            arg_num++;
            next_token = get_token(stdin);
            arg_next();
        }
        else if (next_token.type == TOKEN_INT) {
            arg_num++;
            next_token = get_token(stdin);
            arg_next();
        }
        else if (next_token.type == TOKEN_KEYWORD && next_token.value.keyword_value == NONE) {
            arg_num++;
            next_token = get_token(stdin);
            arg_next();
        }
        else if (next_token.type == TOKEN_ID) {
            arg_num++;
            stack_sem_push(&stack_semantic, VAR_USE, next_token.value.string);
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
    if (next_token.type == TOKEN_STRING || next_token.type == TOKEN_DOUBLE
        || next_token.type == TOKEN_INT || (next_token.type == TOKEN_KEYWORD && next_token.value.keyword_value == NONE)
        || next_token.type == TOKEN_LEFT_BRACKET) {
        first = next_token;
        /*****PSA*******/
        next_token = expressionParse(stdin, &first, NULL, 1);
/********************************************************************************************
        SIMULACE RESENI VYRAZU first
            //TODO <expr> */
/*
        if (first.type == TOKEN_ID)
            stack_sem_push(&stack_semantic, VAR_USE, first.value.string);
        next_token = get_token(stdin);

        while (next_token.type == TOKEN_MATH_PLUS || next_token.type == TOKEN_MATH_MINUS ||
        next_token.type == TOKEN_MATH_DIV || next_token.type == TOKEN_MATH_MUL ||
        next_token.type == TOKEN_MATH_INT_DIV || next_token.type == TOKEN_LESS ||
        next_token.type == TOKEN_LESS_EQ || next_token.type == TOKEN_GREATER ||
        next_token.type == TOKEN_GREATER_EQ || next_token.type == TOKEN_NOT_EQ ||
        next_token.type == TOKEN_EQ || next_token.type == TOKEN_LEFT_BRACKET ||
        next_token.type == TOKEN_RIGHT_BRACKET || next_token.type == TOKEN_STRING ||
        next_token.type == TOKEN_INT || next_token.type == TOKEN_DOUBLE ||
        next_token.type == TOKEN_ID) {
            if (next_token.type == TOKEN_ID)
                stack_sem_push(&stack_semantic, VAR_USE, next_token.value.string);
            next_token = get_token(stdin);
        }

/*********************************************************************************************/
    }
    //pravidlo 20
    else if (next_token.type == TOKEN_ID){
        first = next_token;
        next_token = get_token(stdin);
        fun_or_expr_2();
    }
    //pravidlo 29
    //else if (next_token.type == TOKEN_EOL)
    //    /*DO NOTHING*/;
    else
        error_exit(ERROR_SYNTAX);
}

void fun_or_expr_2() {
    printf("In fun_or_expr2\n");
    //pravidlo 23*
    if (next_token.type == TOKEN_MATH_PLUS || next_token.type == TOKEN_MATH_MINUS ||
        next_token.type == TOKEN_MATH_DIV || next_token.type == TOKEN_MATH_MUL ||
        next_token.type == TOKEN_MATH_INT_DIV || next_token.type == TOKEN_LESS ||
        next_token.type == TOKEN_LESS_EQ || next_token.type == TOKEN_GREATER ||
        next_token.type == TOKEN_GREATER_EQ || next_token.type == TOKEN_NOT_EQ ||
        next_token.type == TOKEN_EQ) {
            second = next_token;
            /*****PSA*******/
            next_token = expressionParse(stdin, &first, &second, 2);
/********************************************************************************************
        SIMULACE RESENI VYRAZU first,second
            //TODO <expr2>

        if (first.type == TOKEN_ID)
            stack_sem_push(&stack_semantic, VAR_USE, first.value.string);

        if (second.type == TOKEN_ID)
            stack_sem_push(&stack_semantic, VAR_USE, second.value.string);
        next_token = get_token(stdin);

        while (next_token.type == TOKEN_MATH_PLUS || next_token.type == TOKEN_MATH_MINUS ||
        next_token.type == TOKEN_MATH_DIV || next_token.type == TOKEN_MATH_MUL ||
        next_token.type == TOKEN_MATH_INT_DIV || next_token.type == TOKEN_LESS ||
        next_token.type == TOKEN_LESS_EQ || next_token.type == TOKEN_GREATER ||
        next_token.type == TOKEN_GREATER_EQ || next_token.type == TOKEN_NOT_EQ ||
        next_token.type == TOKEN_EQ || next_token.type == TOKEN_LEFT_BRACKET ||
        next_token.type == TOKEN_RIGHT_BRACKET || next_token.type == TOKEN_STRING ||
        next_token.type == TOKEN_INT || next_token.type == TOKEN_DOUBLE ||
        next_token.type == TOKEN_ID) {
            if (next_token.type == TOKEN_ID)
                stack_sem_push(&stack_semantic, VAR_USE, next_token.value.string);
            next_token = get_token(stdin);
        }

/*********************************************************************************************/
    }
    //pravidlo 21
    else if (next_token.type == TOKEN_LEFT_BRACKET) {
        next_token = get_token(stdin);

        arg_num = 0;
        arg_list();
        stack_sem_push(&stack_semantic, FUN_CALL, first.value.string);
        arg_num = 0;

        if (next_token.type == TOKEN_RIGHT_BRACKET)
            next_token = get_token(stdin);
        else
            error_exit(ERROR_SYNTAX);
    }
    //pravidlo 30
    else if (next_token.type == TOKEN_EOL)
        stack_sem_push(&stack_semantic, VAR_USE, first.value.string); //pouze kontrola samotneho identifikatoru
    else
        error_exit(ERROR_SYNTAX);
}
/* konec souboru IFJ_parser.c */
