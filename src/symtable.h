/* ******************************* symtable.h ****************************
 *  Predmet: Formalni jazyky a prekladace (IFJ) - FIT VUT v Brne
 *  Nazev projektu: Implementace prekladace imperativniho jazyka IFJ19
 *  Autori: Beranek Tomas (xberan46) - vedouci
 *          Haderka Martin (xhader00)
 *          Slobodnik Simon (xslobo06)
 *          Klem Richard (xklemr00)
 *  Datum vytvoreni: 23.11.2019
 * ************************************************************************** */

#ifndef SYMTABLE_H_INCLUDED
#define SYMTABLE_H_INCLUDED

#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<stdbool.h>
#include "IFJ_stack_string.h"

typedef struct record {
    bool is_variable;
    int param_num;
    tStack_string* undefined_functions; //zde se ukladaji funkce, ktere jeste nemely definici, ale byly pouzity v ji difinici
    bool global;
    bool local;
    bool read_from_global;
    bool forward_call;
} Record;

typedef struct tBSTNode {
	char* Key;
	Record BSTNodeCont;
	struct tBSTNode * LPtr;
	struct tBSTNode * RPtr;
} *tBSTNodePtr;


//globalni tabulka symbolu - nutne pro soubor error.c, ktery ji v pripade
//chyby korektne uvolni
extern tBSTNodePtr symtable;


void symtable_init(tBSTNodePtr *);
int symtable_search(tBSTNodePtr, char*, Record*);
void symtable_insert(tBSTNodePtr *, char*, Record);
void symtable_delete(tBSTNodePtr *, char*);
void symtable_dispose(tBSTNodePtr *);
void insert_builtin_to_symtable();

#endif // SYMTABLE_H_INCLUDED

/* konec souboru IFJ_scanner.h */
