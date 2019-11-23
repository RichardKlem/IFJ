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

//funkce prevede jmeno promenne na unikatni klic
int name_to_key(char * name);

typedef struct record {
    char * name;
    int was_defined;
    int is_variable;
    int param_num;
} Record;

typedef struct tBSTNode {
	int Key;
	Record BSTNodeCont;
	struct tBSTNode * LPtr;
	struct tBSTNode * RPtr;
} *tBSTNodePtr;

void BSTInit(tBSTNodePtr *);
int BSTSearch(tBSTNodePtr, char*, Record*);
void BSTInsert(tBSTNodePtr *, char*, Record);
void BSTDelete(tBSTNodePtr *, char*);
void BSTDispose(tBSTNodePtr *);

#endif // SYMTABLE_H_INCLUDED

/* konec souboru IFJ_scanner.h */
