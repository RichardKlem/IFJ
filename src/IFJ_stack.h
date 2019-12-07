/* ******************************* IFJ_stack.h ****************************
 *  Predmet: Formalni jazyky a prekladace (IFJ) - FIT VUT v Brne
 *  Nazev projektu: Implementace prekladace imperativniho jazyka IFJ19
 *  Autori: Beranek Tomas (xberan46) - vedouci
 *          Haderka Martin (xhader00)
 *          Slobodnik Simon (xslobo06)
 *          Klem Richard (xklemr00)
 *  Datum vytvoreni: 15.10.2019
 * ************************************************************************** */
#ifndef _IFJ_STACK_H_
#define _IFJ_STACK_H_

typedef struct telem{
    int data;
    struct telem* next;
} tElem;

typedef struct {
        tElem* top;
} tStack;

void stackInit (tStack* s);
int stackEmpty (tStack* s);
int stackTop (tStack* s);
void stackPop (tStack* s);
void stackPush (tStack* s, int data);

#endif
/* konec souboru IFJ_stack.h */
