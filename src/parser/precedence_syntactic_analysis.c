#include "precedence_syntactic_analysis.h"

/**
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * ______________!!! W I P !!!______________
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

/**
 *  nactu si tokeny, pak je nacpu na zasobnik podle precedecni tabulky
 *  nasledne je zpracuji podle pravidel, jedu zprava doleva
**/

/**
 * @brief hlavni funkce, ktera se vola kdyz je potreba zpracovat vyraz
 *        muze byt predno 0 az 2 tokeny
 * @param first prvni token
 * @param second druhy token
 * @param num_of_tokens pocet tokenu, ktere byli predany
 */
void expressionParse(token_t first, token_t second, int num_of_tokens)
{
    expr_token_t first_expr_token;
    expr_token_t second_expr_token;

    if(num_of_tokens <= 1)
    {
        first_expr_token = first;
        if(num_of_tokens == 2)
            second_expr_token = second;
    }
}

