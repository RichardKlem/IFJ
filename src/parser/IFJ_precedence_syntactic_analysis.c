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

#include "IFJ_precedence_syntactic_analysis.h"
#include "IFJ_stack.h"
#include "IFJ_error.h"
#include "IFJ_precedence_table.h"
#include "inst_gen.h"

/**
 *  nactu si tokeny, pak je nacpu na zasobnik podle precedecni tabulky
 *  nasledne je zpracuji podle pravidel, jedu zprava doleva
**/


/**
 * @brief funkce naplni zasobnik vstupnimi tokeny vcetne tokenu predanych od rekurzivniho sestupu (RS)
 * @param stack ukazatel na zasobnik pro PSA
 * @param first prvni token od RS
 * @param second druhy token od RS
 */
tExprStack * loadWholeExpression(t, token_t* first, token_t* second)
{
    if(stack == NULL)
        error_exit(ERROR_INTERNAL);
    if(first != NULL)


}

/**
 * @brief hlavni funkce, ktera se vola kdyz je potreba zpracovat vyraz
 *        muze byt predno 0 az 2 tokeny, kdyz nastane syntaxticka chyba, tak vyhodi chybu
 * @param first prvni token
 * @param second druhy token
 * @param num_of_tokens pocet tokenu, ktere byli predany
 */
void expressionParse(token_t* first, token_t* second, int num_of_tokens)
{
    expr_token_t* first_expr_token = NULL;
    expr_token_t* second_expr_token = NULL;

    //
    if(num_of_tokens < 0 || num_of_tokens > 2)
        error_exit(ERROR_INTERNAL);
    if(num_of_tokens >= 1)
    {
        first_expr_token = first;
        if(num_of_tokens == 2)
            second_expr_token = second;
    }

    //ptr_psa_stac is pointer to global PSA stack of type tExprStack
    exprStackPush(ptr_psa_stack, '$'); //vlozim 'zarazku'
    while(true)
    {
        if(first_expr_token != NULL)

    }
}

