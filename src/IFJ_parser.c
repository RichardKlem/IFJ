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
<stat>			|   |   |   |   |   |   |   |   |   |19*|19*|19*|   |   |19*     |5  |19*|   |   |   |   |      |      |   |11*    |        |17       |3       |          |16    |   |
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
#include "IFJ_builtin.h"
#include <string.h>
#include "IFJ_stack_string.h"

#define debug_print(...)
//printf(__VA_ARGS__)

token_t next_token; //globalni token
token_t first, second; //pomocne tokeny pro uchovani tokenu pri predavani ke zpracovani vyrazu
Record id_param; //pomocna struktura pro ukladani informaci o promennych
tBSTNodePtr symtable; //tabulka symbolu
tStack_sem stack_semantic; //zasobnik pro semantickou analyzu
tStack_sem stack_semantic_params; //pomocny zasobnik pro zpracovani parametru
int param_num, arg_num; //pocitadla argumentu (u volani fce) a parametru (u definice fce)
bool in_function = false, in_block = false;
char * fun_name = NULL; //pomocna promenna, pro uchovani jmena funkce ve ktere jsme zanoreni
int unique_number = 0; //unikatni pocitadlo pro generovani labelu
token_t assign_to; //pomocna promenna pro uchovani jmena promenne, do ktere se prirazuje
bool is_param = false;
int print_pop = 0;
int print_assign_one_var = 0;
int print_assign_fun = 0;
int in_cycle = 0;
tStack_string stack_instructions;

void prog(){
    debug_print("In main\n");
    next_token = get_token(stdin);

    //inicializace tabulky symbolu
    symtable_init(&symtable);
    //inicializace zasobniku na semantickou kontrolu
    stack_sem_init(&stack_semantic);
    stack_sem_init(&stack_semantic_params);
    stack_init_string(&stack_instructions);

    //vypsani hlavicky a vestavenych funkci
    generate_builtin();
    insert_builtin_to_symtable();

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
    debug_print("In st_list\n");
    //pravidlo 4
    if (next_token.type == TOKEN_DEDENT || next_token.type == TOKEN_EOF)
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
            ((in_function) && (next_token.type == TOKEN_KEYWORD && next_token.value.keyword_value == RETURN))){
                stat();
                eol_opt();
                st_list();
            }
    else
        error_exit(ERROR_SYNTAX);
}

void stat(){
    debug_print("In stat\n");
    //pravidlo 19*
    if (next_token.type == TOKEN_STRING || next_token.type == TOKEN_DOUBLE
        || next_token.type == TOKEN_INT || (next_token.type == TOKEN_KEYWORD && next_token.value.keyword_value == NONE)
        || next_token.type == TOKEN_LEFT_BRACKET){

        first = next_token;
        /*****PSA*******/
        next_token = expressionParse(stdin, &first, NULL, 1);

        if (next_token.type == TOKEN_EOL)
            next_token = get_token(stdin);
        else
            error_exit(ERROR_SYNTAX);
    }
    //pravidlo 5
    else if (next_token.type == TOKEN_ID){
        //uchovani identifikatoru
        assign_to = next_token;
        first = next_token;
        next_token = get_token(stdin);
        expr_or_assign();
        if (next_token.type == TOKEN_EOL)
            next_token = get_token(stdin);
        else
            error_exit(ERROR_SYNTAX);
    }
    //pravidlo 7*/8 sloucene a pouze za podminky, ze jsme ve funkci
    else if ((in_function) && (next_token.type == TOKEN_KEYWORD && next_token.value.keyword_value == RETURN)){
        ret(); //pouze presmerujeme do puvodniho pravidla 7/8
    }
    //pravidlo 11 - definice funkce
    else if ((!in_block) && (next_token.type == TOKEN_KEYWORD && next_token.value.keyword_value == DEF)){
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

        print_instruction("\nJUMP %s_END\n", fun_name);
        print_instruction("LABEL %s\n", fun_name);
        print_instruction("PUSHFRAME\n");
        print_instruction("DEFVAR LF@%%ret\n");
        print_instruction("MOVE LF@%%ret nil@nil\n");

        param_num = 0; //vynuluje pocet parametru
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

        //VYTVORENI ZARAZKY PRED ZACATKEM BLOKU
        //stack_sem_push se interne diva na globalni promennou param_num a zaroven vlozi funkci na symtable
        stack_sem_push(&stack_semantic, FUN_DEF, fun_name);
        in_function = true;
        in_block = true;

        print_instruction("\n#telo funkce\n\n");

        st_list();

        in_block = false;
        in_function = false;
        stack_sem_pop_until_block_start(&stack_semantic);

        print_instruction("POPFRAME\n");
        print_instruction("RETURN\n");
        print_instruction("LABEL %s_END\n", fun_name);

        if (next_token.type == TOKEN_DEDENT)
            next_token = get_token(stdin);
        else error_exit(ERROR_SYNTAX);
    }
    //pravidlo 17
    else if (next_token.type == TOKEN_KEYWORD && next_token.value.keyword_value == WHILE){
        int while_unique_label = unique_number;
        unique_number++;

        in_cycle++;
        print_instruction("\nLABEL WHILE%d\n", while_unique_label);
        print_instruction("#zpracovani vyrazu\n");

        next_token = get_token(stdin);
        first = next_token;
        /*****PSA*******/
        next_token = expressionParse(stdin, &first, NULL, 1);

        print_instruction("PUSHS int@0\n");
        print_instruction("JUMPIFEQS WHILE_END%d\n", while_unique_label);
        print_instruction("CLEARS\n");
        print_instruction("\n#telo while\n\n");

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

        in_block = true;
        st_list();
        in_block = false;

        print_instruction("JUMP WHILE%d\n", while_unique_label);
        print_instruction("LABEL WHILE_END%d\n", while_unique_label);
        print_instruction("CLEARS\n");

        in_cycle--;
        //zde se vypise telo cyklu (az po vypisu definici pouzitych promenny)
        tStack_string tmp_stack; //pomocny zasobnik na vypsani instrukci ve spravne poradi (obraceni)
        stack_init_string(&tmp_stack);

        if (!in_cycle){
            while (!stack_empty_string(&stack_instructions)){
                stack_push_string(&tmp_stack, stack_top_string(&stack_instructions));
                stack_pop_string(&stack_instructions);
            }
            while (!stack_empty_string(&tmp_stack)){
                printf("%s", stack_top_string(&tmp_stack));
                stack_pop_string(&tmp_stack);
            }
        }

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
        int if_unique_label = unique_number;
        unique_number++;
        in_cycle++;

        print_instruction("\n#zpracovani vyrazu\n");

        next_token = get_token(stdin);
        first = next_token;
        /*****PSA*******/
        next_token = expressionParse(stdin, &first, NULL, 1);

        print_instruction("PUSHS int@0\n");
        print_instruction("JUMPIFEQS ELSE%d\n", if_unique_label);
        print_instruction("\n#telo then\n\n");

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

        in_block = true;
        st_list();
        in_block = false;

        print_instruction("JUMP IF_END%d\n", if_unique_label);
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

        print_instruction("LABEL ELSE%d\n", if_unique_label);
        print_instruction("\n#telo else\n\n");

        in_block = true;
        st_list();
        in_block = false;

        print_instruction("LABEL IF_END%d\n", if_unique_label);
        print_instruction("CLEARS\n");
        in_cycle--;
        //zde se vypise telo cyklu (az po vypisu definici pouzitych promenny)
        tStack_string tmp_stack; //pomocny zasobnik na vypsani instrukci ve spravne poradi (obraceni)
        stack_init_string(&tmp_stack);

        if (!in_cycle){
            while (!stack_empty_string(&stack_instructions)){
                stack_push_string(&tmp_stack, stack_top_string(&stack_instructions));
                stack_pop_string(&stack_instructions);
            }
            while (!stack_empty_string(&tmp_stack)){
                printf("%s", stack_top_string(&tmp_stack));
                stack_pop_string(&tmp_stack);
            }
        }

        if (next_token.type == TOKEN_DEDENT)
            next_token = get_token(stdin);
        else error_exit(ERROR_SYNTAX);
    }
    else
        error_exit(ERROR_SYNTAX);
}

void eol_opt(){
    debug_print("In eol_opt\n");
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
    debug_print("In ret\n");
    //pravidlo 8
    if (next_token.type == TOKEN_DEDENT) {
        print_instruction("POPFRAME\n");
        print_instruction("RETURN\n");
    }
    //pravidlo 7
    else if (next_token.type == TOKEN_KEYWORD && next_token.value.keyword_value == RETURN) {
        next_token = get_token(stdin);
        if (next_token.type != TOKEN_EOL && next_token.type != TOKEN_EOF) {
            /*****PSA*******/
            first = next_token;
            next_token = expressionParse(stdin, &first, NULL, 1);

            print_instruction("POPS LF@%%ret\n");
            print_instruction("POPFRAME\n");
            print_instruction("RETURN\n");
        }
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
    debug_print("In param_list\n");
    //pravidlo 12
    if (next_token.type == TOKEN_ID) {
        param_num++;
        stack_sem_push(&stack_semantic_params, DO_NOTHING, next_token.value.string);

        print_instruction("DEFVAR LF@%s\n", next_token.value.string);
        print_instruction("MOVE LF@%s LF@%%%d\n", next_token.value.string, param_num);

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
    debug_print("In param_next\n");
    //pravidlo 14
    if (next_token.type == TOKEN_RIGHT_BRACKET)
        /* DO NOTHING */;
    //pravidlo 15
    else if (next_token.type == TOKEN_COMMA) {
        next_token = get_token(stdin);
        if (next_token.type == TOKEN_ID){
            param_num++;
            stack_sem_push(&stack_semantic_params, DO_NOTHING, next_token.value.string);

            print_instruction("DEFVAR LF@%s\n", next_token.value.string);
            print_instruction("MOVE LF@%s LF@%%%d\n", next_token.value.string, param_num);

            next_token = get_token(stdin);
            param_next();
        }
        else error_exit(ERROR_SYNTAX);
    }
    else
        error_exit(ERROR_SYNTAX);
}

void expr_or_assign() {
    debug_print("In expr_or_assign\n");
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
    }
    //pravidlo 24
    else if (next_token.type == TOKEN_LEFT_BRACKET){
        next_token = get_token(stdin);

        if (strcmp(first.value.string, "print"))
            print_instruction("\nCREATEFRAME\n");

        arg_num = 0;
        arg_list();
        stack_sem_push(&stack_semantic, FUN_CALL, first.value.string);

        if (strcmp(first.value.string, "print"))
            print_instruction("CALL %s\n", first.value.string);

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
        //char * tmp = first.value.string;

        fun_or_expr();

        //bylo prirazeno do promenne, je nutna vlozit ji do stack_semantic
        stack_sem_push(&stack_semantic, VAR_DEF, assign_to.value.string);

        if (print_assign_fun) {
            if (get_frame(assign_to.value.string))
                print_instruction("MOVE GF@%s TF@%%ret\n", assign_to.value.string);
            else
                print_instruction("MOVE LF@%s TF@%%ret\n", assign_to.value.string);
            print_assign_fun = 0;
        }

        if (print_pop) {
            if (get_frame(assign_to.value.string))
                print_instruction("POPS GF@%s\n", assign_to.value.string);
            else
                print_instruction("POPS LF@%s\n", assign_to.value.string);
            print_instruction("CLEARS\n");
            print_pop = 0;
        }

        if (print_assign_one_var) {
            if (get_frame(assign_to.value.string))
                print_instruction("\nMOVE GF@%s ", assign_to.value.string);
            else
                print_instruction("\nMOVE LF@%s ", assign_to.value.string);

            if (get_frame(first.value.string))
                print_instruction("GF@%s\n", first.value.string);
            else
                print_instruction("LF@%s\n", first.value.string);
            print_assign_one_var = 0;
        }
    }
    else
        error_exit(ERROR_SYNTAX);
}

void arg_list() {
    debug_print("In arg_list\n");
    //pravidlo 25*
    if (next_token.type == TOKEN_STRING){
        arg_num++;

        if (!strcmp(first.value.string, "print")) {
            print_instruction("CREATEFRAME\n");
            arg_num = 1;
        }

        print_instruction("DEFVAR TF@%%%d\n", arg_num);
        print_instruction("MOVE TF@%%%d string@%s\n", arg_num, convert_str_to_ifjcode_str(next_token.value.string));

        if (!strcmp(first.value.string, "print"))
            print_instruction("CALL print\n");

        next_token = get_token(stdin);
        arg_next();
    }
    else if (next_token.type == TOKEN_DOUBLE){
        arg_num++;

        if (!strcmp(first.value.string, "print")) {
            print_instruction("CREATEFRAME\n");
            arg_num = 1;
        }

        print_instruction("DEFVAR TF@%%%d\n", arg_num);
        print_instruction("MOVE TF@%%%d float@%a\n", arg_num, next_token.value.double_value);

        if (!strcmp(first.value.string, "print"))
            print_instruction("CALL print\n");

        next_token = get_token(stdin);
        arg_next();
    }
    else if (next_token.type == TOKEN_INT){
        arg_num++;

        if (!strcmp(first.value.string, "print")) {
            print_instruction("CREATEFRAME\n");
            arg_num = 1;
        }

        print_instruction("DEFVAR TF@%%%d\n", arg_num);
        print_instruction("MOVE TF@%%%d int@%d\n", arg_num, next_token.value.int_value);

        if (!strcmp(first.value.string, "print"))
            print_instruction("CALL print\n");

        next_token = get_token(stdin);
        arg_next();
    }
    else if (next_token.type == TOKEN_KEYWORD && next_token.value.keyword_value == NONE){
        arg_num++;

        if (!strcmp(first.value.string, "print")) {
            print_instruction("CREATEFRAME\n");
            arg_num = 1;
        }

        print_instruction("DEFVAR TF@%%%d\n", arg_num);
        print_instruction("MOVE TF@%%%d string@None\n", arg_num);

        if (!strcmp(first.value.string, "print"))
            print_instruction("CALL print\n");

        next_token = get_token(stdin);
        arg_next();
    }
    else if (next_token.type == TOKEN_ID){
        arg_num++;
        stack_sem_push(&stack_semantic, VAR_USE, next_token.value.string);

        if (!strcmp(first.value.string, "print")) {
            print_instruction("CREATEFRAME\n");
            arg_num = 1;
        }

        print_instruction("DEFVAR TF@%%%d\n", arg_num);
        if (get_frame(next_token.value.string))
            print_instruction("MOVE TF@%%%d GF@%s\n", arg_num, next_token.value.string);
        else
            print_instruction("MOVE TF@%%%d LF@%s\n", arg_num, next_token.value.string);

        if (!strcmp(first.value.string, "print"))
            print_instruction("CALL print\n");

        next_token = get_token(stdin);
        arg_next();
    }
    //pravidlo 26
    else if (next_token.type == TOKEN_RIGHT_BRACKET){
        /*DO NOTHING*/
        if (!strcmp(first.value.string, "print"))
                print_instruction("WRITE string@\\010\n"); //vypsani odradkovani za kazdym printf
    }
    else
        error_exit(ERROR_SYNTAX);
}

void arg_next() {
    debug_print("In arg_next\n");
    //pravidlo 27
    if (next_token.type == TOKEN_RIGHT_BRACKET){
        /*DO NOTHING*/
        if (!strcmp(first.value.string, "print"))
                print_instruction("WRITE string@\\010\n"); //vypsani odradkovani za kazdym printf
    }
    //pravidlo 28*
    else if (next_token.type == TOKEN_COMMA) {
        next_token = get_token(stdin);
        if (next_token.type == TOKEN_STRING) {
            arg_num++;

            if (!strcmp(first.value.string, "print")) {
                print_instruction("WRITE string@\\032\n"); //vypsani mezery mezi termy
                print_instruction("CREATEFRAME\n");
                arg_num = 1;
            }

            print_instruction("DEFVAR TF@%%%d\n", arg_num);
            print_instruction("MOVE TF@%%%d string@%s\n", arg_num, convert_str_to_ifjcode_str(next_token.value.string));

            if (!strcmp(first.value.string, "print"))
                print_instruction("CALL print\n");

            next_token = get_token(stdin);
            arg_next();
        }
        else if (next_token.type == TOKEN_DOUBLE) {
            arg_num++;

            if (!strcmp(first.value.string, "print")) {
                print_instruction("WRITE string@\\032\n"); //vypsani mezery mezi termy
                print_instruction("CREATEFRAME\n");
                arg_num = 1;
            }

            print_instruction("DEFVAR TF@%%%d\n", arg_num);
            print_instruction("MOVE TF@%%%d float@%a\n", arg_num, next_token.value.double_value);

            if (!strcmp(first.value.string, "print"))
                print_instruction("CALL print\n");

            next_token = get_token(stdin);
            arg_next();
        }
        else if (next_token.type == TOKEN_INT) {
            arg_num++;

            if (!strcmp(first.value.string, "print")) {
                print_instruction("WRITE string@\\032\n"); //vypsani mezery mezi termy
                print_instruction("CREATEFRAME\n");
                arg_num = 1;
            }

            print_instruction("DEFVAR TF@%%%d\n", arg_num);
            print_instruction("MOVE TF@%%%d int@%d\n", arg_num, next_token.value.int_value);

            if (!strcmp(first.value.string, "print"))
                print_instruction("CALL print\n");

            next_token = get_token(stdin);
            arg_next();
        }
        else if (next_token.type == TOKEN_KEYWORD && next_token.value.keyword_value == NONE) {
            arg_num++;

            if (!strcmp(first.value.string, "print")) {
                print_instruction("WRITE string@\\032\n"); //vypsani mezery mezi termy
                print_instruction("CREATEFRAME\n");
                arg_num = 1;
            }

            print_instruction("DEFVAR TF@%%%d\n", arg_num);
            print_instruction("MOVE TF@%%%d string@None\n", arg_num);

            if (!strcmp(first.value.string, "print"))
                print_instruction("CALL print\n");

            next_token = get_token(stdin);
            arg_next();
        }
        else if (next_token.type == TOKEN_ID) {
            arg_num++;
            stack_sem_push(&stack_semantic, VAR_USE, next_token.value.string);

            if (!strcmp(first.value.string, "print")) {
                print_instruction("WRITE string@\\032\n"); //vypsani mezery mezi termy
                print_instruction("CREATEFRAME\n");
                arg_num = 1;
            }

            print_instruction("DEFVAR TF@%%%d\n", arg_num);
            if (get_frame(next_token.value.string))
                print_instruction("MOVE TF@%%%d GF@%s\n", arg_num, next_token.value.string);
            else
                print_instruction("MOVE TF@%%%d LF@%s\n", arg_num, next_token.value.string);

            if (!strcmp(first.value.string, "print"))
                print_instruction("CALL print\n");

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
    debug_print("In fun_or_expr\n");
    //pravidlo 22*
    if (next_token.type == TOKEN_STRING || next_token.type == TOKEN_DOUBLE
        || next_token.type == TOKEN_INT || (next_token.type == TOKEN_KEYWORD && next_token.value.keyword_value == NONE)
        || next_token.type == TOKEN_LEFT_BRACKET) {
        first = next_token;
        /*****PSA*******/
        next_token = expressionParse(stdin, &first, NULL, 1);

        print_pop = 1;
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
    debug_print("In fun_or_expr2\n");
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
            /*
            if (get_frame(assign_to.value.string))
                print_instruction("\nPOPS GF@%s\n", assign_to.value.string);
            else
                print_instruction("\nPOPS LF@%s\n", assign_to.value.string);
            print_instruction("CLEARS\n");*/
            print_pop = 1;


    }
    //pravidlo 21
    else if (next_token.type == TOKEN_LEFT_BRACKET) {
        next_token = get_token(stdin);

        if (strcmp(first.value.string, "print"))
            print_instruction("\nCREATEFRAME\n");

        arg_num = 0;
        arg_list();
        stack_sem_push(&stack_semantic, FUN_CALL, first.value.string);
        arg_num = 0;

        if (strcmp(first.value.string, "print"))
            print_instruction("CALL %s\n", first.value.string);

        print_assign_fun = 1;

        if (next_token.type == TOKEN_RIGHT_BRACKET)
            next_token = get_token(stdin);
        else
            error_exit(ERROR_SYNTAX);
    }
    //pravidlo 30
    else if (next_token.type == TOKEN_EOL) {
        stack_sem_push(&stack_semantic, VAR_USE, first.value.string); //pouze kontrola samotneho identifikatoru

        print_assign_one_var = 1;
    }
    else
        error_exit(ERROR_SYNTAX);
}
/* konec souboru IFJ_parser.c */
