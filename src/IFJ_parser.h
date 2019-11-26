/* ******************************* IFJ_scanner.h ****************************
 *  Predmet: Formalni jazyky a prekladace (IFJ) - FIT VUT v Brne
 *  Nazev projektu: Implementace prekladace imperativniho jazyka IFJ19
 *  Autori: Beranek Tomas (xberan46) - vedouci
 *          Haderka Martin (xhader00)
 *          Slobodnik Simon (xslobo06)
 *          Klem Richard (xklemr00)
 *  Datum vytvoreni: 23.11.2019
 * ************************************************************************** */

#ifndef IFJ_PARSER_H_INCLUDED
#define IFJ_PARSER_H_INCLUDED

void prog();
void st_list();
void stat();
void eol_opt();
void ret();
void param_list();
void param_next();
void expr_or_assign();
void arg_list();
void arg_next();
void fun_or_expr();
void fun_or_expr_2();

#endif // IFJ_PARSER_H_INCLUDED

/* konec souboru IFJ_scanner.h */
