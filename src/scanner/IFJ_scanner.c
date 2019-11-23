/* ******************************* IFJ_scanner.c ****************************
 *  Predmet: Formalni jazyky a prekladace (IFJ) - FIT VUT v Brne
 *  Nazev projektu: Implementace prekladace imperativniho jazyka IFJ19
 *  Autori: Beranek Tomas (xberan46) - vedouci
 *          Haderka Martin (xhader00)
 *          Slobodnik Simon (xslobo06)
 *          Klem Richard (xklemr00)
 *  Datum vytvoreni: 12.10.2019
 * ************************************************************************** */

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "IFJ_stack.h"
#include "IFJ_scanner.h"
#include "IFJ_error.h"

//posune offset o 1 byte zpet, implicitní soubor je src_file
#define push_char_back(x) fseek(src_file, -(x), SEEK_CUR)

typedef enum {
    START,
    ESCAPE_CHAR,
    HEX_CHAR
} convert_to_str_fsm_states;

/*predpokladame, ze do funkce nepujdou jineho znaky nez 0-9,A-F,a-f
  pro jine znaky ma funkce nedefinvane chovani*/
char two_digit_hex_to_dec (char first, char second) {
    char dec = 0;

    if (isdigit(first))
        dec = 16 * (first - '0');
    else if (first >= 'A' && first <= 'F')
        dec = 16 * (first - 'A' + 10);
    else
        dec = 16 * (first - 'a' + 10);

    if (isdigit(second))
        dec += second - '0';
    else if (second >= 'A' && second <= 'F')
        dec += second - 'A' + 10;
    else
        dec += second - 'a' + 10;
    return dec;
}

/* funkce prevede retezec nacteny ze zdrojoveho souboru na retezec bez escape sekvenci
   a koncoveho apostrofu */
char* convert_to_str(char* input) {
    size_t shift_left = 0;
    int state = START;
    char* input_cp = input;

    if (input[strlen(input) - 1] == '"') { //pokud se jedna o literal uvozeny """string"""
        input[strlen(input) - 1] = '\0';
        input[strlen(input) - 1] = '\0';
        input[strlen(input) - 1] = '\0';
    }
    else //pokud se jedna o literal uvozeny 'string'
        input[strlen(input) - 1] = '\0'; //posledni znak (apostrof) nahradim ukoncujicim znakem

    while (*input_cp) {
        switch (state){
            case START:
                if (*input_cp == '\\')
                    state = ESCAPE_CHAR;
                else if (*input_cp == '\r') { //pokud se jedna o CR, tak musí nasledovat LF
                    *(input_cp - shift_left) = *input_cp;
                    if (*(input_cp + 1) != '\n') //zkontrolujeme, zda opravdu nasleduje LF
                        error_exit(ERROR_LEX);
                }
                else
                    *(input_cp - shift_left) = *input_cp; //kopirovani normalnich znaku na prislusne misto
                break;

            case ESCAPE_CHAR:
                if (*input_cp == 'x') {
                    shift_left += 3; //jelikoz nahradime ctyr-znakovou sekvenci za jeden znak
                    state = HEX_CHAR;
                    break;
                }

                shift_left += 1; //jelikoz nahradime dvou-znakovou sekvenci za jeden znak
                if (*input_cp == '\"')
                    *(input_cp - shift_left) = '\"';
                else if (*input_cp == '\'')
                    *(input_cp - shift_left) = '\'';
                else if (*input_cp == 'n')
                    *(input_cp - shift_left) = '\n';
                else if (*input_cp == 't')
                    *(input_cp - shift_left) = '\t';
                else if (*input_cp == '\\') //jina moznost nenastane, to je osetreno v hlavnim fsm
                    *(input_cp - shift_left) = '\\';
                else {
                    shift_left--;
                    *(input_cp - shift_left -1) = '\\';
                    *(input_cp - shift_left) = *input_cp;
                }
                state = START;
                break;

            case HEX_CHAR:
                input_cp++; //musime rucne posunout, jelikoz zpracovavame dva znaky najednou
                //vezmeme aktualni a predchozi znak, prevedeme je do desitkove soustavy a ulozime do pole
                *(input_cp - shift_left) = two_digit_hex_to_dec(*(input_cp - 1), *input_cp);
                state = START;
                break;
        } //switch
        input_cp++;
    } //while

    *(input_cp - shift_left) = '\0'; //posunuti ukoncovaciho znaku

    //realokace podle aktualni delky, delka noveho retezce je <= delka stareho
    return input;
}

int iskeyword(char *s){
    if (!strcmp((const char*)s, "if"))
        return IF;
    if (!strcmp((const char*)s, "else"))
        return ELSE;
    if (!strcmp((const char*)s, "return"))
        return RETURN;
    if (!strcmp((const char*)s, "def"))
        return DEF;
    if (!strcmp((const char*)s, "while"))
        return WHILE;
    if (!strcmp((const char*)s, "None"))
        return NONE;
    if (!strcmp((const char*)s, "pass"))
        return PASS;
    return NOT_A_KEYWORD; //else
}

int str_to_int(char* string_number) {
    long int int_number = 0;

    while (*string_number) {
        if (int_number > (int)((unsigned)(-1)/2)) {
            return 1;
        }
        int_number *= 10;
        int_number += (string_number[0]-48);
        string_number++;
    }

    return int_number;
}

token_t create_token(int token_id, token_value value){
    token_t token;
    token.type = token_id;
    int tmp = 0;

    switch (token_id){
        case TOKEN_DOUBLE:
            token.value.double_value = strtod(value.string , NULL);
            if (errno == ERANGE)
                error_exit(ERROR_SEM_TYPE); //pri preteceni nebo podteceni
            free(value.string);
            break;

        case TOKEN_ID:
            if (tmp = iskeyword(value.string)) { //pokud se jedna o klicove slovo, tak je v tmp jeho id, jinak 0
                token.type = TOKEN_KEYWORD;
                token.value.keyword_value = tmp;
                free(value.string);
            }
            else     //pokud se jedna o identifikator pouze uloz jeho jmeno
                token.value.string = value.string;
            break;

        case TOKEN_STRING:
            token.value.string = convert_to_str(value.string);
            break;

        case TOKEN_INT:
            token.value.int_value = str_to_int(value.string);
            break;


        default:
            token.value = value;
            break;
    } //switch

    return token;
}

char *load_to_str(FILE *src_file, size_t chars_loaded_cnt){
    char *str = (char *)malloc(chars_loaded_cnt + 1);
    if (str == NULL)
        error_exit(ERROR_INTERNAL);

    //zkopiruje chars_loaded znaku z souboru do str
    for (size_t i = 0; i < chars_loaded_cnt; i++)
        str[i] = fgetc(src_file);
    str[chars_loaded_cnt] = '\0';
    return str;
}

//funkce zjisti zda na vstupu ze souboru src_file je zacatek/konec
//viceradkoveho komentare (sekvence znaku """)
//funkce se vrati do puvodniho stavu bufferu souboru
int multi_line_comm_follow(FILE* src_file, int next_char){
    int next_next_char = fgetc(src_file);
    int next_next_next_char = fgetc(src_file);
    if ((next_char == '"') && (next_next_char == '"') && (next_next_next_char == '"')) {
        //push_char_back(2); //multiline comment fix
        return 1;
    }
    else {
        push_char_back(2);
        return 0;
    }
}

token_t get_token(FILE* src_file) {
    int next_char = -10; //hodnota, ktera nemuze z fgetc nastat
    int state = STATE_START;
    static int spaces_cnt = 0;
    size_t chars_loaded_cnt = 0;
    token_value value;
    value.string = NULL;
    static int first_token = 1;
    static int generate_indent = 0;
    static int start_with_indentation = 1;
    static tStack stack_indent;
    static int eof_was_loaded = 0;


    if (first_token) {
            first_token = 0; //dalsi tokeny uz se neprovede
            stackInit(&stack_indent);
            stackPush(&stack_indent, 0); //vlozeni pomocne 0
    }

    //pokud se v predchozim behu neprestaly geenrovat DEDENTY, skoc zpet do smycky generovani
    if (generate_indent) {
        generate_indent = 0;
        goto DEDENT;
    }
    else
        spaces_cnt = 0;

    //pri zpracovani prvniho radku zaciname kontrolou odsazeni
    //nebo pri zpracovani noveho radku
    if (start_with_indentation) {
        state = STATE_INDENT_DEDENT;
        start_with_indentation = 0; //pro pristi zavolani uz se bude zacinat v STATE_START
    }

    while (1) {
        next_char = fgetc(src_file);    //nacteni dalsiho znaku

        if (eof_was_loaded == 1) {
            state = STATE_INDENT_DEDENT;
        }

        if (next_char == EOF) { //pridani EOL pred EOF
            eof_was_loaded = 1;
        }

        switch(state) {
        //TODO usporadat napr. podle principu Hammingova kodu pro vetsi rychlost
        case STATE_START:
            if (next_char == '+') {state = STATE_PLUS; break;} //break vlozen, aby se zbytecne nevyhodnocovali podminky
            if (next_char == '-') {state = STATE_MINUS; break;}
            if (next_char == '*') {state = STATE_MUL; break;}
            if (next_char == '/') {state = STATE_DIV; break;}
            if (next_char == '<') {state = STATE_LESS; break;}
            if (next_char == '>') {state = STATE_GREATER; break;}
            if (next_char == '(') {state = STATE_LEFT_BRACKET; break;}
            if (next_char == ')') {state = STATE_RIGHT_BRACKET; break;}
            if (next_char == EOF) {state = STATE_EOF; break;}
            if (isdigit(next_char)) {state = STATE_INT; chars_loaded_cnt++; break;}
            if (next_char == ',') {state = STATE_COMMA; break;}
            if (next_char == ':') {state = STATE_COLON; break;}
            if (next_char == '!') {state = STATE_NOT_EQ; break;}
            if (next_char == '=') {state = STATE_ASSIGNMENT; break;}
            if (next_char == '#') {state = STATE_SINGLE_LINE_COMM; break;}
            if (next_char == '_' || isalpha(next_char)) {state = STATE_ID; chars_loaded_cnt++; break;}
            if (next_char == '\'') {state = STATE_STRING_READ; break;}
            if (next_char == ' ' || next_char == '\t') {state = STATE_START; break;}
            if (next_char == '\n') {
                start_with_indentation = 1;
                return create_token(TOKEN_EOL, NO_PARAM);
            }
            if (next_char == '\r') {state = STATE_CRLF; break;}
            if (multi_line_comm_follow(src_file, next_char)) {
                state = STATE_MULTI_LINE_LITERAL;
                break;
            }
            error_exit(ERROR_LEX); //jiny vstupni znak -> CHYBA

        case STATE_MULTI_LINE_LITERAL:
          chars_loaded_cnt++;
            if (multi_line_comm_follow(src_file, next_char)) {//pokud je konec
                    chars_loaded_cnt += 2; //musime precit i nasledujici 2 uvozovky
                    push_char_back(chars_loaded_cnt); //simulace stavu STATE_STRING
                    value.string = load_to_str(src_file, chars_loaded_cnt);
                    return create_token(TOKEN_STRING, value);
            }
            else if (next_char == EOF)
                error_exit(ERROR_LEX);
            else if (next_char > (char)31 || next_char == '"' || next_char == '\'' ||
                     next_char ==  '\n' || next_char == '\t' || next_char == '\\' || next_char == '\r')
                state = STATE_MULTI_LINE_LITERAL;
            else
                error_exit(ERROR_LEX);
            break;

        case STATE_CRLF:
            if (next_char == '\n')
                state = STATE_START;
            else
                error_exit(ERROR_LEX);

        case STATE_INDENT_DEDENT:
            if (next_char == ' ') {
                spaces_cnt++;
                state = STATE_INDENT_DEDENT;
            }
            else if (next_char == '\t') { //pokud je nacten tabulator, nejedna se indent/dedent
                state = STATE_EMPTY_LINE; //muze nasledovat jedine prazdny radek nebo komentare
                spaces_cnt = 0;
            }
            else if (next_char == '\n') { //pokud nacteme EOL, nechame reseni na stavu START
                state = STATE_START;
                push_char_back(1);
                spaces_cnt = 0;
            }
            else if (multi_line_comm_follow(src_file, next_char)) { //pokud zacina komentar (na vstupu je """)
                state = STATE_MULTI_LINE_COMM;
                spaces_cnt = 0;
            }
            else if (next_char == '#') {
                state = STATE_SINGLE_LINE_COMM;
                spaces_cnt = 0;
            }
            else {  //pokud nasleduje nejaky prikaz
                if (next_char == EOF) {
                    eof_was_loaded = 1;
                    spaces_cnt = 0;
                }
                else
                    push_char_back(1);

                if (spaces_cnt > stackTop(&stack_indent)) { //pokud je odsazeni vetsi na na vrcholu zasbinku -> INDENT
                    stackPush(&stack_indent, spaces_cnt);
                    return create_token(TOKEN_INDENT, NO_PARAM);
                }
                else if (spaces_cnt == stackTop(&stack_indent)) { //pokud je odsazeni stejne, tak nedelame nic
                    state = STATE_START;
                    spaces_cnt = 0;
                    if (next_char == EOF)
                        return create_token(TOKEN_EOF, NO_PARAM);
                }
                else {  //pokud je odsazeni mensi nez na zaobniku, vyjimame ze zasobniku, dokud nenarazime na
                        //stejnou hodnotu, pri kazdem vyjmuti generujeme dedent, pokud hodnotu nenajdeme -> CHYBA

                    while (spaces_cnt < stackTop(&stack_indent)) {
                        generate_indent = 1;
                        stackPop(&stack_indent);
                        return create_token(TOKEN_DEDENT, NO_PARAM);
                        DEDENT:; //sem skocime, abychom mohli jednoduse pokracovat v generovani
                    }
                    //kontrola spravnosti odsazeni
                    if (spaces_cnt != stackTop(&stack_indent)) //pokud se nejedna o hledanou hodnotu -> CHYBA
                        error_exit(ERROR_LEX);

                    if (next_char == EOF || eof_was_loaded) //eof mohl byt nacten pri predchozim generovani dedentu
                        return create_token(TOKEN_EOF, NO_PARAM);
                    else
                        state = STATE_START;
                    spaces_cnt = 0;
                }
            }
            break;

        case STATE_EMPTY_LINE:
            if (next_char == EOF)
                state = STATE_START;
            else if (next_char == '\n') { //nechame reseni na stavu START
                push_char_back(1);
                state = STATE_START;
            }
            else if (next_char == '#')
                state = STATE_SINGLE_LINE_COMM;
            else if (multi_line_comm_follow(src_file, next_char))
                state = STATE_MULTI_LINE_COMM;
            else if (next_char == ' ' || next_char == '\t')
                state = STATE_EMPTY_LINE;
            else
                error_exit(ERROR_LEX); //pokud byl na zacatku radku tabulator a pokracuje necim jinym
            break;                     //nez tab/space/EOL/komentarem -> CHYBA

        case STATE_MULTI_LINE_COMM:
            if (next_char == EOF) //pokud skoncil soubor nechame reseni na stavu START
                state = STATE_START;
            else if (next_char != '"') //jsme stale v komentari
                state = STATE_MULTI_LINE_COMM;
            else if (multi_line_comm_follow(src_file, next_char)) //kontrola zda nenasleduje """
                state = STATE_MULTI_LINE_COMM_AFTER;
            else   //nejednalo se o """, ale pouze o " nebo ""
                state = STATE_MULTI_LINE_COMM;
            break;

        case STATE_MULTI_LINE_COMM_AFTER:
            if (next_char == ' ' || next_char == '\t') //za viceradkovych komentarem mohou nasledovat pouze space znaky
                state = STATE_MULTI_LINE_COMM_AFTER;
            else if (next_char == EOF)
                state = STATE_START;
            else if (next_char == '\n') { //nechame reseni na stavu START
                push_char_back(1);
                state = STATE_START;
            }
            else
                error_exit(ERROR_LEX); //za komentarem byl prikaz -> CHYBA
            break;

        case STATE_PLUS:
            push_char_back(1);
            return create_token(TOKEN_MATH_PLUS, NO_PARAM);

        case STATE_MINUS:
            push_char_back(1);
            return create_token(TOKEN_MATH_MINUS, NO_PARAM);

        case STATE_MUL:
            push_char_back(1);
            return create_token(TOKEN_MATH_MUL, NO_PARAM);

        case STATE_DIV:
            if (next_char == '/')
                state = STATE_INT_DIV;
            else {
                push_char_back(1);
                return create_token(TOKEN_MATH_DIV, NO_PARAM);
            }
            break;

        case STATE_INT_DIV:
            push_char_back(1);
            return create_token(TOKEN_MATH_INT_DIV, NO_PARAM);

        case STATE_LESS:
            if (next_char == '=')
                state = STATE_LESS_EQ;
            else {
                push_char_back(1);
                return create_token(TOKEN_LESS, NO_PARAM);
            }
            break;

        case STATE_LESS_EQ:
            push_char_back(1);
            return create_token(TOKEN_LESS_EQ, NO_PARAM);

        case STATE_GREATER:
            if (next_char == '=')
                state = STATE_GREATER_EQ;
            else {
                push_char_back(1);
                return create_token(TOKEN_GREATER, NO_PARAM);
            }
            break;

        case STATE_GREATER_EQ:
            push_char_back(1);
            return create_token(TOKEN_GREATER_EQ, NO_PARAM);

        case STATE_LEFT_BRACKET:
            push_char_back(1);
            return create_token(TOKEN_LEFT_BRACKET, NO_PARAM);

        case STATE_RIGHT_BRACKET:
            push_char_back(1);
            return create_token(TOKEN_RIGHT_BRACKET, NO_PARAM);

        case STATE_EOF:
            return create_token(TOKEN_EOF, NO_PARAM);

        case STATE_INT:
            chars_loaded_cnt++;

            if (isdigit(next_char))
                state = STATE_INT;
            else if (next_char == '.')
                state = STATE_DOUBLE_DECIMAL_POINT;
            else if (next_char == 'E' || next_char == 'e')
                state = STATE_DOUBLE_EXP;
            else {
                push_char_back(chars_loaded_cnt);
                chars_loaded_cnt--;
                value.string = load_to_str(src_file, chars_loaded_cnt);
                return create_token(TOKEN_INT, value);
            }
            break;

        case STATE_DOUBLE_DECIMAL_POINT:
            if (isdigit(next_char)){
                chars_loaded_cnt++;
                state = STATE_DOUBLE_WITHOUT_EXP;
            }
            else
                error_exit(ERROR_LEX);
            break;

        case STATE_DOUBLE_WITHOUT_EXP:
            chars_loaded_cnt++;
            if (isdigit(next_char))
                state = STATE_DOUBLE_WITHOUT_EXP;
            else if (next_char == 'E' ||  next_char == 'e')
                state = STATE_DOUBLE_EXP;
            else {
                push_char_back(chars_loaded_cnt);
                chars_loaded_cnt--;
                value.string = load_to_str(src_file, chars_loaded_cnt);
                return create_token(TOKEN_DOUBLE, value);
            }
            break;

        case STATE_DOUBLE_EXP:
            chars_loaded_cnt++;
            if (isdigit(next_char))
                state = STATE_DOUBLE_WITH_EXP;
            else if (next_char == '+' || next_char == '-')
                state = STATE_DOUBLE_EXP_SIGN;
            else
                error_exit(ERROR_LEX);
            break;

        case STATE_DOUBLE_EXP_SIGN:
            chars_loaded_cnt++;
            if (isdigit(next_char))
                state = STATE_DOUBLE_WITH_EXP;
            else
                error_exit(ERROR_LEX);
            break;

        case STATE_DOUBLE_WITH_EXP:
            chars_loaded_cnt++;
            if (isdigit(next_char))
                state = STATE_DOUBLE_WITH_EXP;
            else {
                push_char_back(chars_loaded_cnt);
                chars_loaded_cnt--;
                value.string = load_to_str(src_file, chars_loaded_cnt);
                return create_token(TOKEN_DOUBLE, value);
            }
            break;

        case STATE_COMMA:
            push_char_back(1);
            return create_token(TOKEN_COMMA, NO_PARAM);

        case STATE_COLON:
            push_char_back(1);
            return create_token(TOKEN_COLON, NO_PARAM);

        case STATE_NOT_EQ:
            if (next_char == '=') //tento if simuluje mezistav
                return create_token(TOKEN_NOT_EQ, NO_PARAM);
            else
                error_exit(ERROR_LEX);

        case STATE_ASSIGNMENT:
            if (next_char == '=')
                state = STATE_EQ;
            else {
                push_char_back(1);
                return create_token(TOKEN_ASSIGNMENT, NO_PARAM);
            }
            break;

        case STATE_EQ:
            push_char_back(1);
            return create_token(TOKEN_EQ, NO_PARAM);

        case STATE_SINGLE_LINE_COMM:
            if (next_char == EOF) {
                push_char_back(1);
                state = STATE_START;
            }
            else if (next_char != '\n')
                state = STATE_SINGLE_LINE_COMM; //jsme stale v komentari
            else {
                push_char_back(1);
                state = STATE_START;
            }
            break;

        case STATE_ID:
            chars_loaded_cnt++;
            if (next_char == '_' || isalnum(next_char))
                state = STATE_ID;
            else {
                push_char_back(chars_loaded_cnt);
                chars_loaded_cnt--;
                value.string = load_to_str(src_file, chars_loaded_cnt);
                return create_token(TOKEN_ID, value);
            }
            break;

        case STATE_STRING_READ:
            chars_loaded_cnt++;
            if (next_char == '\'') {
                push_char_back(chars_loaded_cnt); //simulace stavu STATE_STRING
                value.string = load_to_str(src_file, chars_loaded_cnt);
                return create_token(TOKEN_STRING, value);
            }
            else if (next_char == '\\')
                state = STATE_STRING_BACKSLASH;
            else if (next_char > (char)31)
                state = STATE_STRING_READ;
            else
                error_exit(ERROR_LEX);
            break;

        case STATE_STRING_BACKSLASH:
            chars_loaded_cnt++;
            if (next_char == '\\' || next_char == '"' || next_char == '\''
                || next_char == 'n' || next_char == 't') //uvazujeme, ze toto jsou jedine pripustne escape sekvence
                state = STATE_STRING_READ;
            else if (next_char == 'x')
                state = STATE_STRING_HEX_START;
            else if (next_char > (char)31)
                state = STATE_STRING_READ;
            else
                error_exit(ERROR_LEX);
            break;

        case STATE_STRING_HEX_START:
            chars_loaded_cnt++;
            if (isdigit(next_char) || (next_char >= 'A' && next_char <= 'F') ||
                (next_char >= 'a' && next_char <= 'f'))
                state = STATE_STRING_HEX_END;
            else
                error_exit(ERROR_LEX);
            break;

        case STATE_STRING_HEX_END:
            chars_loaded_cnt++;
            if (isdigit(next_char) || (next_char >= 'A' && next_char <= 'F') ||
                (next_char >= 'a' && next_char <= 'f'))
                state = STATE_STRING_READ;
            else
                error_exit(ERROR_LEX);
            break;

        default:
            error_exit(ERROR_INTERNAL);
            break;
        } //switch
    } //while

} //get_token


 /* konec souboru IFJ_scanner.c */
