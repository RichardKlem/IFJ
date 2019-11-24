/* ******************************* IFJ_scanner.c ****************************
 *  Predmet: Formalni jazyky a prekladace (IFJ) - FIT VUT v Brne
 *  Nazev projektu: Implementace prekladace imperativniho jazyka IFJ19
 *  Autori: Beranek Tomas (xberan46) - vedouci
 *          Haderka Martin (xhader00)
 *          Slobodnik Simon (xslobo06)
 *          Klem Richard (xklemr00)
 *  Datum vytvoreni: 12.10.2019
 * ************************************************************************** */

/**
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * ______________!!! W I P !!!______________
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "IFJ_scanner.h"
#include "IFJ_stack.h"
#include "IFJ_error.h"
#include "IFJ_precedence_table.h"
#include "IFJ_precedence_DLL.h"
#include "IFJ_precedence_syntactic_analysis.h"

/**
 *  nactu si tokeny, pak je nacpu na zasobnik podle precedecni tabulky
 *  nasledne je zpracuji podle pravidel, jedu zprava doleva
**/


/**
 * @brief funkce loadExpr naplni zasobnik vstupnimi tokeny vcetne tokenu predanych od rekurzivniho sestupu (RS)
 * @param stack ukazatel na zasobnik pro PSA
 * @param first prvni token od RS
 * @param second druhy token od RS
 * @return vraci posledni token typu KEYWORD nebo EOL
 */
token_t loadExpr(FILE * src_file, tExprDLList * expr_DLL, expr_token_t * first_token, expr_token_t * second_token)
{
    if(expr_DLL == NULL)
        error_exit(ERROR_INTERNAL);
    if(first_token != NULL)
        exprDLInsertLast(expr_DLL, *first_token);
    if(second_token != NULL)
        exprDLInsertLast(expr_DLL, *second_token);

    token_t act_token = get_token(src_file); //nacteni tokenu ze vstupu
    //dokud nenactu keyword, eol nebo eof
    while (act_toke.type != TOKEN_KEYWORD || ||act_token.type != TOKEN_EOL || act_token.type != TOKEN_EOF)
    {
        exprDLInsertLast(expr_DLL, act_token);
        act_token = get_token(src_file);
    }
    return act_token;
}

/**
 * @brief funkce get_prec_value
 * @param type_of_token zpracuje typ tokenu
 * @return vraci index do precedencni tabulky
 */
int get_prec_value(token_type type_of_token)
{
    int index = -1;
    switch (type_of_token){
        case TOKEN_MATH_PLUS:
        case TOKEN_MATH_MINUS:
            index = 0;
            break;
        case TOKEN_MATH_MUL:
        case TOKEN_MATH_DIV:
        case TOKEN_MATH_INT_DIV:
            index = 1;
            break;
        case TOKEN_LEFT_BRACKET:
            index = 2;
            break;
        case TOKEN_RIGHT_BRACKET:
            index = 3;
            break;
        case TOKEN_LESS:
        case TOKEN_LESS_EQ:
        case TOKEN_GREATER:
        case TOKEN_GREATER_EQ:
        case TOKEN_EQ:
        case TOKEN_NOT_EQ:
            index = 4;
            break;
        //case FUNC: index = 5; break; --zatim nepodporovane
        case TOKEN_ID:
            index = 6;
            break;
        case TOKEN_DOLAR:
            index = 7;
            break;
        default:
            error_exit(ERROR_SYNTAX);
            break;
    }
    return index;
}

/**
 * @brief funkce get_precedence na zjisteni co se ma provest, shift, reduce, equal(push)
 * @param first prvni operand
 * @param second druhy operand
 * @return vraci jednu z precedencnich hodnot typu precedence_rule
 */
precedence_rule get_precedence(tExprElem first, tExprElem second)
{
    int i = -1;
    int j = -1;
    i = get_prec_value(first.exprToken.token.type);
    j = get_prec_value(second.exprToken.token.type);
    precedence_rule precedence = precedence_table[i][j];
    return precedence;
}

/**
 * @brief hlavni funkce, ktera se vola kdyz je potreba zpracovat vyraz
 *        muze byt predno 0 az 2 tokeny, kdyz nastane syntaxticka chyba, tak vyhodi chybu
 * @param first prvni token
 * @param second druhy token
 * @param num_of_tokens pocet tokenu, ktere byli predany
 * @param src_file
 * @return vraci posledni token typu KEYWORD nebo EOL
 */
token_t expressionParse(FILE * src_file, token_t * first, token_t * second, int num_of_tokens)
{
    expr_token_t* first_expr_token = NULL;
    expr_token_t* second_expr_token = NULL;

    //
    if(num_of_tokens < 0 || num_of_tokens > 2 || src_file == NULL)
        error_exit(ERROR_INTERNAL);
    if(num_of_tokens >= 1)
    {
        first_expr_token->token = first;
        first_expr_token->terminal = true;
        first_expr_token->shifted = false;
        if(num_of_tokens == 2){
            second_expr_token->token = second;
            second_expr_token->terminal = true;
            second_expr_token->shifted = false;
        }
    }

    tExprDLList psa_exprDLL; //dvojsmerne vazany list v kterem bude cely vstup v tokenech
    exprDLInitList(ptr_psa_exprDLL); //inicializace DL listu
    token_t last_token; //token ktery vratim do RS
    last_token = loadExpr(src_file, &psa_exprDLL, first_expr_token, second_expr_token);

    tExprStack psa_stack;
    tExprElem end_token;
    end_token.exprToken.token.type = TOKEN_DOLAR;
    end_token.exprToken.token.value.string = "$";
    end_token.exprToken.terminal = true;
    exprStackPush(&psa_stack, end_token); //vlozim 'zarazku'

    tExprElem top_terminal;
    tExprElem input;

    //prvni inicializace promennych
    top_terminal.exprToken = find_top_terminal(ptr_psa_stack); //nactu si nevrchnejsi terminal
    top_terminal.exprToken.terminal = true;
    exprDLCopyFirst(psa_exprDLL, &(input.exprToken.token)); //dostanu prvni token na vstupu
    exprDLDeleteFirst(psa_exprDLL); //zaroven ho i smazu, jiz je nacteny
    input.exprToken.terminal = true;
    do
    {
        precedence_rule precedence = get_precedence(top_terminal, input);
            // < :zaměň a za a< na zásobníku & push(b) & přečti další symbol b ze vstupu
        if(precedence == SHIFT){

            //TODO zamen a za a<
            exprStackPush(psa_stack, input.exprToken);
            top_terminal.exprToken = find_top_terminal(ptr_psa_stack); //nactu si nevrchnejsi terminal
            top_terminal.exprToken.terminal = true;
            exprDLCopyFirst(psa_exprDLL, &(input.exprToken.token)); //dostanu prvni token na vstupu
            exprDLDeleteFirst(psa_exprDLL); //zaroven ho i smazu, jiz je nacteny
            input.exprToken.terminal = true;
        }
        else if(precedence == EQUAL){
            // = :push(b) & přečti další symbol b ze vstupu
            exprStackPush(psa_stack, input.exprToken);
            top_terminal.exprToken = find_top_terminal(ptr_psa_stack); //nactu si nevrchnejsi terminal
            top_terminal.exprToken.terminal = true;
            exprDLCopyFirst(psa_exprDLL, &(input.exprToken.token)); //dostanu prvni token na vstupu
            exprDLDeleteFirst(psa_exprDLL); //zaroven ho i smazu, jiz je nacteny
            input.exprToken.terminal = true;
        }

    } while (top_terminal.exprToken.token.type != TOKEN_DOLAR && input.exprToken.token.type != TOKEN_DOLAR);
    return last_token; // kdyz vse probehne v poradku, vratim posledni token, aby mohl pokracovat RS
}

