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
#include "IFJ_stack_semantic.h"
//#include "c202.h"
#include "IFJ_builtin.h"
#include "c204.h"
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
token_t loadExpr(FILE * src_file, tExprDLList * expr_DLL, expr_token_t * first_token, expr_token_t * second_token, int * max_len)
{
    if(expr_DLL == NULL)
        error_exit(ERROR_INTERNAL);

    *max_len = 0;
    if(first_token != NULL){
        ////printf("VKLADAM do DL listu PRVNI token\n");
        exprDLInsertLast(expr_DLL, *first_token);
        ////printf("VLOZIL JSEM do DL listu PRVNI token\n");
        *max_len = 1;
        }
    if(second_token != NULL){
        ////printf("Vkladam do DL listu DRUHY token\n");
        exprDLInsertLast(expr_DLL, *second_token);
        *max_len = 2;
        }

    expr_token_t act_token;
    act_token.token = get_token(src_file); //nacteni tokenu ze vstupu

    //dokud nenactu keyword krome None, EOL nebo EOF
        while ((act_token.token.type != TOKEN_KEYWORD || act_token.token.value.keyword_value == NONE) && act_token.token.type != TOKEN_EOL && act_token.token.type != TOKEN_EOF && act_token.token.type != TOKEN_COLON)
    {
        /*//printf("Cyklim????\n");
        if(act_token.token.type == TOKEN_INT)
            //printf("%d\n", act_token.token.value.int_value);
        if(act_token.token.type == TOKEN_KEYWORD)
            //printf("%d\n", act_token.token.value.keyword_value);*/
        act_token.terminal = true;
        act_token.shifted = false;
        exprDLInsertLast(expr_DLL, act_token);
        act_token.token = get_token(src_file);
        (*max_len)++;
        ////printf("Cyklim za\n");
    }
    ////printf("PRED returnem\n");
    expr_token_t end_token;
    end_token.token.type = TOKEN_DOLAR;
    end_token.token.value.string = "$";
    end_token.terminal = true;
    end_token.shifted = false;
    exprDLInsertLast(expr_DLL, end_token);
    return act_token.token;
}

/**
 * @brief funkce inf2post_stack_gen zpracuje infix na postix
 * @param input_list DL list kde je jiz nacteny vstup
 * @param infix_array ukazatel na stack s tokeny v infix tvaru
 * @param postfix_array ukazatel na cilovy stack, kde bude input zpracovan do postfixu
 */
void inf2post_stack_gen(tExprDLList * input_list, token_t * infix_array, token_t * postfix_array, int * max_len)
{

    expr_token_t act_expr_token;
    token_t act_token;
    exprDLFirst(input_list);
    exprDLCopy(input_list, &act_expr_token);
    act_token = act_expr_token.token;
    int max_len_loc = 0;
    while (max_len_loc < *max_len)
    {
        infix_array[max_len_loc++] = act_token;

        exprDLSucc(input_list);
        if(input_list->Act != NULL)
            {exprDLCopy(input_list, &act_expr_token);
            act_token = act_expr_token.token;}
    }
    token_t end_token;
    end_token.type = TOKEN_DOLAR;
    end_token.value.string = "$";
    infix_array[max_len_loc] = end_token;
    infix2postfix(infix_array, postfix_array, *max_len + 1);
}

/**
 * @brief funkce get_prec_value
 * @param type_of_token zpracuje typ tokenu
 * @return vraci index do precedencni tabulky
 */
int get_prec_value(token_t token)
{
    int index = -1;
    token_type type_of_token = token.type;
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
        case TOKEN_INT:
        case TOKEN_DOUBLE:
        case TOKEN_STRING:
            index = 6;
            break;
        case TOKEN_KEYWORD:
            if (token.value.keyword_value == NONE)
                index = 6;
            else
            {
                //printf("Chci zjistit precedenci tokenu KEYWORD, ktery nnei NONE\n");
                error_exit(ERROR_SYNTAX);
            }
            break;
        case TOKEN_DOLAR:
            index = 7;
            break;
        default:
            //printf("dostal jsem neocekavany token\n");
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
precedence_rule get_precedence(expr_token_t first, expr_token_t second)
{
    int i = -1;
    int j = -1;
    i = get_prec_value(first.token);
    j = get_prec_value(second.token);
    precedence_rule precedence = precedence_table[i][j];
    return precedence;
}

/**
 * @brief funkce get_top_shifted_terminal vraci nejvrchnejsi terminal, ktery je shifted
 * @param s ukazatel na PSA zasobnik
 * @return vraci raci nejvrchnejsi terminal s atributem shifted
 */
int get_members_count(tExprStack * s)
{
    if (s == NULL)
        error_exit(ERROR_INTERNAL);
    else {
        tExprElem * top_terminal= s->top;
        int members_count = 0; //prvni to nikdy nebude, takze se aspon jednou navyssi pocitadlo v cyklu
        while (top_terminal->exprToken.terminal == false || top_terminal->exprToken.shifted == false)
        {
            if (top_terminal->next == NULL) //uz neni zadny prvek a my jsme nenasli zadny terminal
                error_exit(ERROR_INTERNAL);
            top_terminal = top_terminal->next;
            members_count++;
        }
        if (top_terminal->exprToken.terminal == true && top_terminal->exprToken.shifted == true){
            top_terminal->exprToken.shifted = false;
            return members_count;
        }
        else
        {
            //printf("get_members_count funkce nema zadny terminal\n");
            error_exit(ERROR_SYNTAX); //neni zadny terminal, to je chyba
        }
    }
}

/**
 * @brief funkce reduce_var_val redukuje terminal identifikator/promennou anebo konstantu na NEterminal
 * @param stack ukazatel na PSA zasobnik
 */
void reduce_var_val(tExprStack * stack)
{
    if (stack == NULL)
    {
        //printf("neni stack ve funkci reduce_var_val\n");
        error_exit(ERROR_INTERNAL);
    }

    // zjisteni zda byla promenna deklarovana
    if (stack->top->exprToken.token.type == TOKEN_ID)
        stack_sem_push(&stack_semantic, VAR_USE, stack->top->exprToken.token.value.string);

    stack->top->exprToken.terminal = false; //prenastavim terminal na NEterminal
}

/**
 * @brief funkce reduce_brackets redukuje posloupnost (terminal NEterminal terminal) na jeden NEterminal
 * @param stack ukazatel na PSA zasobnik
 */
void reduce_brackets(tExprStack * stack)
{
    if (stack == NULL)
    {
        //printf("neni stack ve funkci reduce_brackets\n");
        error_exit(ERROR_INTERNAL);
    }
    exprStackPop(stack);
    exprStackPop(stack);
    stack->top->exprToken.terminal = false;
}

/**
 * @brief funkce reduce_bin_op redukuje osloupnost (NEterminal terminal NEterminal) na jeden NEterminal
 * @param stack ukazatel na PSA zasobnik
 */
void reduce_bin_op(tExprStack * stack)
{
    if (stack == NULL)
    {
        //printf("neni stack ve funkci reduce_bin_op\n");
        error_exit(ERROR_INTERNAL);
    }
    //TODO kontolovat operator
    exprStackPop(stack);
    exprStackPop(stack);
    stack->top->exprToken.terminal = false;
}

/**
 * @brief funkce get_type_of_reduction vraci typ vyrazu, ktery se bude zpracovavat
 * @param stack ukazatel na PSA zasobnik
 * @return vraci BRACKETS kdyz se jedna o (E) nebo BINARY kdyz se jedna o E op E jinak RT_ERROR
 */
reduction_type get_type_of_reduction(tExprStack * stack)
{
    int first = stack->top->exprToken.terminal;
    int second = stack->top->next->exprToken.terminal;
    int third = stack->top->next->next->exprToken.terminal;

    if (first == 1 && second == 0 && third ==1)
        return BRACKETS;
    else if (first == 0 && second == 1 && third ==0)
        return BINARY;
    else
        return RT_ERROR;
}

/**
 * @brief funkce reduce_by_rules zpracuje tokeny na PSA zasobniku podle pravidel, provadi PRAVY rozbor
 * @param stack ukazatel na PSA zasobnik
 * @param members_count pocet operandu kteri budou zasazeni redukci
 * @return vraci 1, kdyz je vse v poradku, jinak 0
 */
int reduce_by_rules(tExprStack * stack, int members_count)
{
    //kdyz reduji jeden, je to bud konstanta nebo promenna
    if(members_count == 1)
        reduce_var_val(stack);
    else if(members_count == 3)
    {
        reduction_type type_of_reduction = get_type_of_reduction(stack);
        if(type_of_reduction == BRACKETS)
            reduce_brackets(stack);
        else if(type_of_reduction == BINARY)
            reduce_bin_op(stack);
        else
            error_exit(ERROR_SYNTAX);
    }
    else
        error_exit(ERROR_SYNTAX);
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
    ////printf("\nfirst-%d",first->type);
    expr_token_t prvni_expr_token;
    expr_token_t druhy_expr_token;
    expr_token_t* first_expr_token = &prvni_expr_token;
    expr_token_t* second_expr_token = &druhy_expr_token;

    if(num_of_tokens < 0 || num_of_tokens > 2 || src_file == NULL)
        error_exit(ERROR_INTERNAL);
    if(num_of_tokens >= 1)
    {   if (first == NULL)
            error_exit(ERROR_INTERNAL);
        else
        {
            first_expr_token->token = *first;
            first_expr_token->terminal = true;
            first_expr_token->shifted = false;

            if(num_of_tokens == 2){
                if (second == NULL)
                    error_exit(ERROR_INTERNAL);
                else
                {
                    second_expr_token->token = *second;
                    second_expr_token->terminal = true;
                    second_expr_token->shifted = false;
                }
            }
            else
                second_expr_token = NULL;
        }
    }
    else
    {
        first_expr_token = NULL;
        second_expr_token = NULL;
    }

    tExprDLList psa_exprDLL; //dvojsmerne vazany list v kterem bude cely vstup v tokenech
    exprDLInitList(&psa_exprDLL); //inicializace DL listu
    token_t last_token; //token ktery vratim do RS
    int max_len;
    last_token = loadExpr(src_file, &psa_exprDLL, first_expr_token, second_expr_token, &max_len);

    max_len++;
    token_t * infix_array = (token_t *) malloc(max_len * sizeof(token_t));
    token_t * postfix_array = (token_t *) malloc(max_len * sizeof(token_t));


    inf2post_stack_gen(&psa_exprDLL, infix_array, postfix_array, &max_len);

    /*for(int i =0; i < max_len; ++i)
    {
        printf("%d\n", (postfix_array[i]).type);
    }*/
    tExprStack psa_stack;
    exprStackInit(&psa_stack);

    expr_token_t end_token;
    end_token.token.type = TOKEN_DOLAR;
    end_token.token.value.string = "$";
    end_token.terminal = true;
    end_token.shifted = false;
    exprStackPush(&psa_stack, end_token); //vlozim 'zarazku' $

    expr_token_t top_terminal_obj;
    expr_token_t * top_terminal = &top_terminal_obj;
    expr_token_t input_obj;
    expr_token_t * input = &input_obj;

    //prvni inicializace promennych
    top_terminal = find_top_terminal(&psa_stack); //nactu si nevrchnejsi terminal
    exprDLCopyFirst(&psa_exprDLL, input); //dostanu prvni expr_token s atributem temrinal=true na vstupu
    exprDLDeleteFirst(&psa_exprDLL); //zaroven ho i smazu, jiz je nacteny
    do
    {
        //zjisteni precedecniho pravidla
        precedence_rule precedence = get_precedence(*top_terminal, *input);
        ////printf("\n%d \ntt-%d \ninput-%d \n\n", precedence, top_terminal->token.type, input->token.type);
        // < :zaměň a za a< na zásobníku & push(b) & přečti další symbol b ze vstupu
        if(precedence == SHIFT){
            top_terminal->shifted = true; //zamen a za a<
            exprStackPush(&psa_stack, *input); //push(b)

            top_terminal = find_top_terminal(&psa_stack); //nactu si nevrchnejsi terminal
            exprDLCopyFirst(&psa_exprDLL, input); //dostanu prvni token na vstupu
            exprDLDeleteFirst(&psa_exprDLL); //zaroven ho i smazu, jiz je nacteny
        }

        // = :push(b) & přečti další symbol b ze vstupu
        else if(precedence == EQUAL){
            exprStackPush(&psa_stack, *input); //push(b)

            top_terminal = find_top_terminal(&psa_stack); //nactu si nevrchnejsi terminal
            exprDLCopyFirst(&psa_exprDLL, input); //dostanu prvni token na vstupu
            exprDLDeleteFirst(&psa_exprDLL); //zaroven ho i smazu, jiz je nacteny
        }

        //if <y je na vrcholu zásobníku && r: A -> y € P
        //      then zaměň <y za A && vypiš r na výstup
        //else chyba
        else if(precedence == REDUCE){
            //TODO zavolat funkci, ktera najde shifted prvek nejblize vrcholu zasobniku
            // napriklad pro  $<E+<i to je +< a pro $<E+E to je $<
            // po te se odstrani atribut shifted a vse napravo se musi zpracovat,
            // kdyz pro to neni pravidlo, tak se jedna o chybu

            //zjistim pocet operandu na PSA stacku, ktere budu redukovat , nejvrchnejsi shifted terminal je touto funkci prenastavem na shifted == false
            int members_count = get_members_count(&psa_stack);
            ////printf("%d\n", members_count);
            //reduce_by_rules vraci 1 kdyz je vse ok, jinak 0
            if(! reduce_by_rules(&psa_stack, members_count)){
                //printf("PO redukci\n");
                error_exit(ERROR_SYNTAX); // kdyz vrati 0, tak nastala chyba => ERROR_SYNTAX
            }
        top_terminal = find_top_terminal(&psa_stack);
        }
        else if(precedence == ERROR)
        {
            //printf("Precedence vratila ERROR\n");
            error_exit(ERROR_SYNTAX);
        }

    } while (top_terminal->token.type != TOKEN_DOLAR || input->token.type != TOKEN_DOLAR);
    ////printf("\nJDU VEN\n");
    print_stack(postfix_array);//volani generovani mezikodu k zpracovani postfix vyrazu
    return last_token; // kdyz vse probehne v poradku, vratim posledni token, aby mohl pokracovat RS
}
