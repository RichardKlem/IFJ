
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

#include "c204.h"

/*
** Pomocná funkce untilLeftPar.
** Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka
** bude také odstraněna. Pokud je zásobník prázdný, provádění funkce se ukončí.
**
** Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
** znaků postExpr. Délka převedeného výrazu a též ukazatel na první volné
** místo, na které se má zapisovat, představuje parametr postLen.
**
** Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
** nadeklarovat a používat pomocnou proměnnou typu char.
*/
void untilLeftPar ( tStack* s, char* postExpr, unsigned* postLen )
{
    char tmp;
    while(stackEmpty(s) == 0) //dokud JE to rovno nule, tak to znamena ze stack NENI prazdny
    {
        stackTop(s, &tmp);
        stackPop(s);
        if(tmp == '(')
            break;  // kdyz dojde na levou zavorku, tak konci, zavorka je jiz z predchoziho kroku popnuta
        postExpr[(*postLen)++] = tmp;
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
** představuje parametr postLen, výstupním polem znaků je opět postExpr.
*/
void doOperation ( tStack* s, char c, char* postExpr, unsigned* postLen )
{
    if (stackEmpty(s) != 0) // nerovna se nule znamena je prazdny
    {
        stackPush(s, c);
        return;
    }

    char tmp;
    stackTop(s, &tmp);  //prvni znak

    if ((tmp) == '(')
    {
        stackPush(s, c);
        return;
    }

    if ((c == '*' || c == '/') && (tmp == '+' || tmp == '-'))
    {
        stackPush(s, c);
        return;
    }

    postExpr[(*postLen)++] = tmp;
    stackPop(s);

    doOperation(s, c, postExpr, postLen);  //rekurze dokud se nepodari splnit podminka
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
tGenStack infix2postfix (tGenStack * input_infix_stack, int max_len)
{
    tGenStack * stack = (tGenStack *) malloc(sizeof(tGenStack));
    if(stack == NULL)
        return NULL;

    tGenStack * postfix_gen_stack = (tGenStack *) malloc(max_len * sizeof(tGenStack));
    if(postExpr == NULL)
    {
        free(stack);
        return NULL;
    }

    stackInit(stack);

    unsigned infIndex = 0;
    unsigned postIndex = 0;  //postIndex je vlastne postLen
    char inputChar = input_infix_stack[infIndex];  //prvni znak

    while(inputChar != '\0')
    {
        if(('a' <= inputChar && inputChar <= 'z') || ('A' <= inputChar && inputChar <= 'Z') || ('0' <= inputChar && inputChar <= '9'))
            postExpr[postIndex++] = inputChar;

        else if(inputChar == '(')
            stackPush(stack, inputChar);

        else if(inputChar == '+' || inputChar == '-' || inputChar == '*' || inputChar == '/') {
            doOperation(stack, inputChar, postExpr, (&postIndex));
        }

        else if(inputChar == ')')
            untilLeftPar(stack, postExpr, (&postIndex));

        else if(inputChar == '=')
        {
            char tmp;
            while(stackEmpty(stack) == 0)  //dokud JE rovno 0 znamena ze NENI prazdny
            {
                stackTop(stack, &tmp);
                postExpr[postIndex++] = tmp;
                stackPop(stack);
            }
            postExpr[postIndex++] = '=';
            break;
        }
        inputChar = input_infix_stack[++infIndex];
    }
    postExpr[postIndex] = '\0';

    free(stack);  //postExpr neuvolnuji, protoze bych si smazal svuj vysledek
    return postExpr;
}

/* Konec c204.c */
