/* ******************************* IFJ_stack_string.h ****************************
 *  Predmet: Formalni jazyky a prekladace (IFJ) - FIT VUT v Brne
 *  Nazev projektu: Implementace prekladace imperativniho jazyka IFJ19
 *  Autori: Beranek Tomas (xberan46) - vedouci
 *          Haderka Martin (xhader00)
 *          Slobodnik Simon (xslobo06)
 *          Klem Richard (xklemr00)
 *  Datum vytvoreni: 30.11.2019
 * ************************************************************************** */
#ifndef IFJ_STACK_STRING_H_INCLUDED
#define IFJ_STACK_STRING_H_INCLUDED

typedef struct telem_string{
    char * data;
    struct telem_string* next;
} tElem_string;

typedef struct {
        tElem_string* top;
} tStack_string;

void stack_init_string (tStack_string* s);
int stack_empty_string (tStack_string* s);
char * stack_top_string (tStack_string* s);
void stack_pop_string (tStack_string* s);
void stack_push_string (tStack_string* s, char * data);

#endif // IFJ_STACK_STRING_H_INCLUDED
/* konec souboru IFJ_stack_string.h */
