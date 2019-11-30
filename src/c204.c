
/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického
** výrazu do postfixového tvaru. Pro převod využijte zásobník (tStack),
** který byl implementován v rámci příkladu c202. Bez správného vyřešení
** příkladu c202 se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix .... konverzní funkce pro převod infixového výrazu
**                       na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**
**    untilLeftPar .... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/


#include "IFJ_scanner.h"
#include "IFJ_precedence_table.h"
#include "c204.h"

/*
** Pomocná funkce untilLeftPar.
** Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka
** bude také odstraněna. Pokud je zásobník prázdný, provádění funkce se ukončí.
**
** Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
** znaků postfix_gen_stack. Délka převedeného výrazu a též ukazatel na první volné
** místo, na které se má zapisovat, představuje parametr postLen.
**
** Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
** nadeklarovat a používat pomocnou proměnnou typu char.
*/
void untilLeftPar ( tGenStack * s, token_t * postfix_stack, unsigned * postLen)
{
    token_t tmp;
    while(genStackEmpty(s) == 0) //dokud JE to rovno nule, tak to znamena ze stack NENI prazdny
    {
        tmp = genStackTop(s);
        genStackPop(s);
        if(tmp.type == TOKEN_LEFT_BRACKET)
            break;  // kdyz dojde na levou zavorku, tak konci, zavorka je jiz z predchoziho kroku popnuta
        postfix_stack[(*postLen)++] = tmp;
    }
}

/*
** Pomocná funkce doOperation.
** Zpracuje operátor, který je předán parametrem c po načtení znaku ze
** vstupního pole znaků.
**
** Dle priority předaného operátoru a případně priority operátoru na
** vrcholu zásobníku rozhodneme o dalším postupu. Délka převedeného
** výrazu a taktéž ukazatel na první volné místo, do kterého se má zapisovat,
** představuje parametr postLen, výstupním polem znaků je opět postfix_gen_stack.
*/
void doOperation (tGenStack* s, token_t operator_token, token_t * postfix_stack, unsigned * postLen)
{
    if (genStackEmpty(s) != 0) // nerovna se nule znamena je prazdny
    {
        genStackPush(s, operator_token);
        return;
    }

    token_t tmp;
    tmp = genStackTop(s);  //prvni znak

    if (tmp.type == TOKEN_LEFT_BRACKET)
    {
        genStackPush(s, operator_token);
        return;
    }

    token_type tt = operator_token.type;
    if ((tt == TOKEN_MATH_MUL || tt == TOKEN_MATH_DIV || tt == TOKEN_MATH_INT_DIV) &&
    (tmp.type == TOKEN_MATH_PLUS || tmp.type == TOKEN_MATH_MINUS || tmp.type == TOKEN_EQ || tmp.type == TOKEN_NOT_EQ || tmp.type == TOKEN_LESS || tmp.type== TOKEN_GREATER || tmp.type== TOKEN_LESS_EQ || tmp.type== TOKEN_GREATER ))
    {
        genStackPush(s, operator_token);
        return;
    }
    if((tt == TOKEN_MATH_PLUS || tt == TOKEN_MATH_MINUS) && (tmp.type == TOKEN_EQ || tmp.type == TOKEN_NOT_EQ || tmp.type == TOKEN_LESS || tmp.type== TOKEN_GREATER || tmp.type== TOKEN_LESS_EQ || tmp.type== TOKEN_GREATER ))
    {
        genStackPush(s, operator_token);
        return;
    }
    postfix_stack[(*postLen)++] = tmp;
    genStackPop(s);

    doOperation(s, operator_token, postfix_stack, postLen);  //rekurze dokud se nepodari splnit podminka
}

/*
** Konverzní funkce infix2postfix.
** Čte infixový výraz ze vstupního řetězce infExpr a generuje
** odpovídající postfixový výraz do výstupního řetězce (postup převodu
** hledejte v přednáškách nebo ve studijní opoře). Paměť pro výstupní řetězec
** (o velikosti MAX_LEN) je třeba alokovat. Volající funkce, tedy
** příjemce konvertovaného řetězce, zajistí korektní uvolnění zde alokované
** paměti.
**
** Tvar výrazu:
** 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
**    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
**    násobení má stejnou prioritu jako dělení. Priorita násobení je
**    větší než priorita sčítání. Všechny operátory jsou binární
**    (neuvažujte unární mínus).
**
** 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
**    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
**
** 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
**    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
**    chybné zadání výrazu).
**
** 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen
**    ukončovacím znakem '='.
**
** 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
**
** Poznámky k implementaci
** -----------------------
** Jako zásobník použijte zásobník znaků tStack implementovaný v příkladu c202.
** Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
**
** Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
**
** Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
** char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
**
** Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
** nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
** by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
** ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
** řetězce konstantu NULL.
*/
void infix2postfix (token_t * input_infix_stack, token_t * postfix_array, int max_len)
{
    tGenStack * stack = (tGenStack *) malloc(sizeof(tGenStack));
    if(stack == NULL)
        return;

    /*token_t * postfix_gen_stack = (token_t *) malloc(max_len * sizeof(token_t));
    if(postfix_gen_stack == NULL)
    {
        free(stack);
        return NULL;
    }*/

    genStackInit(stack);

    unsigned infIndex = 0;
    unsigned postIndex = 0;  //postIndex je vlastne postLen
    token_t input_token = input_infix_stack[infIndex];  //prvni znak
    int max_len_loc = 0;
    while(max_len_loc < max_len)
    {
        token_type tt = input_token.type;
        if(tt == TOKEN_ID || tt == TOKEN_STRING || tt == TOKEN_INT || tt == TOKEN_DOUBLE || tt == TOKEN_KEYWORD)
            postfix_array[postIndex++] = input_token;

        else if(tt == TOKEN_LEFT_BRACKET)
            genStackPush(stack, input_token);

        else if(tt == TOKEN_MATH_PLUS || tt == TOKEN_MATH_MINUS || tt == TOKEN_MATH_MUL || tt == TOKEN_MATH_DIV || tt == TOKEN_MATH_INT_DIV ||
                tt == TOKEN_EQ || tt == TOKEN_NOT_EQ || tt == TOKEN_LESS || tt == TOKEN_GREATER || tt == TOKEN_LESS_EQ || tt == TOKEN_GREATER) {
            doOperation(stack, input_token, postfix_array, (&postIndex));
        }

        else if(tt == TOKEN_RIGHT_BRACKET)
            untilLeftPar(stack, postfix_array, (&postIndex));

        else if(tt == TOKEN_DOLAR)
        {
            token_t tmp;
            while(genStackEmpty(stack) == 0)  //dokud JE rovno 0 znamena ze NENI prazdny
            {
                tmp = genStackTop(stack);
                postfix_array[postIndex++] = tmp;
                genStackPop(stack);
            }
            token_t end_token;
            end_token.type = TOKEN_DOLAR;
            end_token.value.string = "$";
            postfix_array[postIndex++] = end_token;
            break;
        }
        input_token = input_infix_stack[++infIndex];
        max_len_loc++;
    }
    //postfix_gen_stack[postIndex] = '\0';

    free(stack);  //postfix_gen_stack neuvolnuji, protoze bych si smazal svuj vysledek
    //postfix_array = postfix_gen_stack;
}

/* Konec c204.c */
