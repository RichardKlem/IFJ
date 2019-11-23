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
#include "IFJ_precedence_syntactic_analysis.h"
#include "IFJ_scanner"
#include "IFJ_stack.h"
#include "IFJ_error.h"
#include "IFJ_precedence_table.h"
#include "IFJ_precedence_DLL.h"

/**
 *  nactu si tokeny, pak je nacpu na zasobnik podle precedecni tabulky
 *  nasledne je zpracuji podle pravidel, jedu zprava doleva
**/


/**
 * @brief funkce naplni zasobnik vstupnimi tokeny vcetne tokenu predanych od rekurzivniho sestupu (RS)
 * @param stack ukazatel na zasobnik pro PSA
 * @param first prvni token od RS
 * @param second druhy token od RS
 * @return vraci posledni token typu KEYWORD nebo EOL
 */
token_t loadExpr(FILE * src_file, tExprDLList * expr_DLL, token_t * first_token, token_t * second_token)
{
    if(expr_DLL == NULL)
        error_exit(ERROR_INTERNAL);
    if(first_token != NULL)
        exprDLInsertLast(expr_DLL, *first_token);
    if(second_token != NULL)
        exprDLInsertLast(expr_DLL, *second_token);

    token_t act_token = get_token(src_file);
    while (act_toke.type != TOKEN_KEYWORD || act_token.type != TOKEN_EOF)
    {
        exprDLInsertLast(expr_DLL, act_token);
        act_token = get_token(src_file);
    }
    return act_token;
}

precedence_rule get_precedence(tExprElem first, tExprElem second)
{
    int i = -1;
    int j = -1;

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
        first_expr_token = first;
        if(num_of_tokens == 2)
            second_expr_token = second;
    }

    tExprDLList psa_exprDLL; //DL list
    exprDLInitList(ptr_psa_exprDLL); //inicializace DL listu
    token_t last_token; //token ktery vratim do RS
    last_token = loadWholeexpression(src_file, &psa_exprDLL, first_expr_token, second_expr_token);

    tExprStack psa_stack;
    tExprElem end_token;
    end_token.exprToken.token.type = TOKEN_DOLAR;
    end_token.exprToken.token.value.string = "$";
    end_token.exprToken.terminal = true;
    //ptr_psa_stac is pointer to global PSA stack of type tExprStack
    exprStackPush(&psa_stack, end_token); //vlozim 'zarazku'
    tExprElem top_terminal;
    tExprElem input;
    do
    {
        top_terminal.exprToken = find_top_terminal(ptr_psa_stack); //nactu si nevrchnejsi terminal
        top_terminal.exprToken.terminal = true;
        exprDLCopyFirst(psa_exprDLL, &(input.exprToken.token)); //dostanu prvni token na vstupu
        exprDLDeleteFirst(psa_exprDLL); //zaroven ho i smazu, jiz je nacteny

        precedence = get_precedence(top_terminal, input)


    } while (top_terminal.exprToken.token.type != TOKEN_DOLAR && input.exprToken.token.type != TOKEN_DOLAR);
    return last_token; // kdyz vse probehne v poradku, vratim posledni token, aby mohl pokracovat RS
}

