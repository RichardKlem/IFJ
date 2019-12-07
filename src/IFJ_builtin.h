/* ******************************* IFJ_builtin.h ****************************
 *  Predmet: Formalni jazyky a prekladace (IFJ) - FIT VUT v Brne
 *  Nazev projektu: Implementace prekladace imperativniho jazyka IFJ19
 *  Autori: Beranek Tomas (xberan46) - vedouci
 *          Haderka Martin (xhader00)
 *          Slobodnik Simon (xslobo06)
 *          Klem Richard (xklemr00)
 *  Datum vytvoreni: 27.11.2019
 * ************************************************************************** */
#ifndef IFJ_BUILTIN_H
#define IFJ_BUILTIN_H

#include "IFJ_scanner.h"

void generate_builtin();
void call_inputi();
void call_inputf();
void call_inputs();
void call_write(char* arg);
void call_len(char* arg);
void call_substr(char* arg1, char* arg2, char* arg3);
void call_ord(char* arg1, char* arg2);
void call_chr(char* arg);
void print_stack(token_t *sem_array);
char * convert_str_to_ifjcode_str(char * input);

#endif //IFJ_BUILTIN_H
/* konec souboru IFJ_builtin.h */
