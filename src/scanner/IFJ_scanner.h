/* ******************************* IFJ_scanner.h ****************************
 *  Predmet: Formalni jazyky a prekladace (IFJ) - FIT VUT v Brne
 *  Nazev projektu: Implementace prekladace imperativniho jazyka IFJ19
 *  Autori: Beranek Tomas (xberan46) - vedouci
 *          Haderka Martin (xhader00)
 *          Slobodnik Simon (xslobo06)
 *          Klem Richard (xklemr00)
 *  Datum vytvoreni: 12.10.2019
 * ************************************************************************** */

#ifndef _IFJ_SCANNER_H_
#define _IFJ_SCANNER_H_

#include <stdio.h>

typedef enum {
    STATE_START,
    STATE_PLUS,
    STATE_MINUS,
    STATE_MUL,
    STATE_DIV,
    STATE_INT_DIV, //jedna se o operator "//" - celociselne deleni
    STATE_LESS,
    STATE_LESS_EQ,
    STATE_GREATER,
    STATE_GREATER_EQ,
    STATE_LEFT_BRACKET,
    STATE_RIGHT_BRACKET,
    STATE_COMMA,  //carka
    STATE_COLON,  //dvojtecka
    STATE_NOT_EQ,
    STATE_ASSIGNMENT,
    STATE_EQ,
    STATE_ID,
    STATE_EOF,
    STATE_MULTI_LINE_COMM,
    STATE_SINGLE_LINE_COMM,
    STATE_INDENT_DEDENT,
    STATE_INT,
    STATE_DOUBLE_EXP,
    STATE_DOUBLE_EXP_SIGN,
    STATE_DOUBLE_DECIMAL_POINT,
    STATE_DOUBLE_WITH_EXP,
    STATE_DOUBLE_WITHOUT_EXP,
    STATE_STRING,
    STATE_STRING_READ,
    STATE_STRING_BACKSLASH,
    STATE_STRING_HEX_START,
    STATE_STRING_HEX_END
} states;

typedef enum {
    TOKEN_EOL,
    TOKEN_MATH_PLUS,
    TOKEN_MATH_MINUS,
    TOKEN_MATH_MUL,
    TOKEN_MATH_DIV,
    TOKEN_MATH_INT_DIV,
    TOKEN_LESS,
    TOKEN_LESS_EQ,
    TOKEN_GREATER,
    TOKEN_GREATER_EQ,
    TOKEN_LEFT_BRACKET,
    TOKEN_RIGHT_BRACKET,
    TOKEN_COMMA,  //carka
    TOKEN_COLON,  //dvojtecka
    TOKEN_NOT_EQ,
    TOKEN_ASSIGNMENT,
    TOKEN_EQ,
    TOKEN_ID,
    TOKEN_KEYWORD,
    TOKEN_EOF,
    TOKEN_INDENT_DEDENT,
    TOKEN_INT,
    TOKEN_DOUBLE,
    TOKEN_STRING
} token_type;

typedef enum {
    DEF,
    ELSE,
    IF,
    NONE,
    PASS,
    RETURN,
    WHILE
} keyword;

typedef union {
    char* string;
    double double_value;
    int int_value;
    keyword keyword_value;
} token_value;

#define NO_PARAM (token_value)0 //pouzit, pokud token nema zadny parametr

typedef struct token_t {
    token_type type;
    token_value value;
} token_t;

token_t get_token(FILE* src_file);

#endif

 /* konec souboru IFJ_scanner.h */
