
/* ******************************* c204.h *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/* ************************************************************************** */

/* TENTO SOUBOR, PROSÍME, NEUPRAVUJTE! */

#ifndef _INFIX2POSTFIX_H_
#define _INFIX2POSTFIX_H_

#include <stdio.h>
#include <stdlib.h>
#include "IFJ_scanner.h"
#include "IFJ_precedence_table.h"
//#include "c202.h"                          /* využívá příklad c202 - zásobník */


/* Konverzní funkce */
token_t * infix2postfix (token_t * input_infix_stack, int max_len);

#endif

/* Konec hlavičkového souboru c204.h */
