/* ******************************* IFJ_builtin.c ****************************
 *  Predmet: Formalni jazyky a prekladace (IFJ) - FIT VUT v Brne
 *  Nazev projektu: Implementace prekladace imperativniho jazyka IFJ19
 *  Autori: Beranek Tomas (xberan46) - vedouci
 *          Haderka Martin (xhader00)
 *          Slobodnik Simon (xslobo06)
 *          Klem Richard (xklemr00)
 *  Datum vytvoreni: 27.11.2019
 * ************************************************************************** */

#include <stdio.h>
#include "IFJ_builtin.h"
#include "symtable.h"
#include "IFJ_error.h"
#include "IFJ_scanner.h"
#include "stdlib.h"
#include <string.h>
#include "IFJ_parser.h"

/*
    a = inputi     READ a int
    a = inputf      READ a float
    a = inputs      READ a string

    print(a)        WRITE a
    print(a, b)     WRITE a
                    WRITE b

    a = len(b)      STRLEN a b
    a = len("abc")  STRLEN a string@abc

    a = substr(retezec, zacatek, pocet)     DEFVAR  GF@tmp
                                            MOVE a string@
                                            DEFVAR GF@tmp2
                                            MOVE GF@tmp2 zacatek
                                            DEFVAR GF@tmp3
                                            ADD GF@tmp3 zacatek pocet
                                            LABEL while
                                            LT GF@tmp GF@tmp2 GF@tmp3
                                            JUMPIFEQ end_while GF@tmp bool@false
                                            GETCHAR tmp retezec GF@tmp2
                                            CONCAT a GF@tmp
                                            ADD GF@tmp2 GF@tmp2 int@1
                                            JUMP wjile
                                            LABEL end_while

    a = ord(řetězec. pozice)    STRI2INT a řetězec pozice

    a = chr(ASCII_kod)      INT2CHAR a ASCII_kod
*/
void generate_builtin()
{
    //Hlavicka
    print_instruction(".IFJcode19\n");
    //Promenná pro kontrolu deklarací
    print_instruction("DEFVAR GF@$tmp_def\n");
    //Definovani vestavene funkce inputi
    print_instruction("#def inputi()\n");
    print_instruction("JUMP end_$inputi\n");
    print_instruction("LABEL inputi\n");
    print_instruction("PUSHFRAME\n");
    print_instruction("DEFVAR LF@%cret\n", '%');
    print_instruction("MOVE LF@%cret nil@nil\n", '%');
    print_instruction("READ LF@%cret int\n", '%');
    print_instruction("DEFVAR LF@tmp\n");
    print_instruction("TYPE LF@tmp LF@%%ret\n");
    print_instruction("JUMPIFEQ $is_int LF@tmp string@int\n");
    print_instruction("MOVE LF@%%ret nil@nil\n");
    print_instruction("LABEL $is_int\n");
    print_instruction("POPFRAME\n");
    print_instruction("RETURN\n");
    print_instruction("LABEL end_$inputi\n");
    print_instruction("\n");

    //Definovani vestavene funkce inputf
    print_instruction("#def inputf()\n");
    print_instruction("JUMP end_$inputf\n");
    print_instruction("LABEL inputf\n");
    print_instruction("PUSHFRAME\n");
    print_instruction("DEFVAR LF@%cret\n", '%');
    print_instruction("MOVE LF@%cret nil@nil\n", '%');
    print_instruction("READ LF@%cret float\n", '%');
    print_instruction("DEFVAR LF@tmp\n");
    print_instruction("TYPE LF@tmp LF@%%ret\n");
    print_instruction("JUMPIFEQ $is_float LF@tmp string@float\n");
    print_instruction("MOVE LF@%%ret nil@nil\n");
    print_instruction("LABEL $is_float\n" );
    print_instruction("POPFRAME\n");
    print_instruction("RETURN\n");
    print_instruction("LABEL end_$inputf\n");
    print_instruction("\n");

    //Definovani vestavene funkce inputs
    print_instruction("#def inputs()\n");
    print_instruction("JUMP end_$inputs\n");
    print_instruction("LABEL inputs\n");
    print_instruction("PUSHFRAME\n");
    print_instruction("DEFVAR LF@%cret\n", '%');
    print_instruction("MOVE LF@%cret nil@nil\n", '%');
    print_instruction("READ LF@%cret string\n", '%');
    print_instruction("DEFVAR LF@tmp\n");
    print_instruction("TYPE LF@tmp LF@%%ret\n");
    print_instruction("JUMPIFEQ $is_string LF@tmp string@string\n");
    print_instruction("MOVE LF@%%ret nil@nil\n");
    print_instruction("LABEL $is_string\n");
    print_instruction("POPFRAME\n");
    print_instruction("RETURN\n");
    print_instruction("LABEL end_$inputs\n");
    print_instruction("\n");

    //Definovani vestavene funkce write
    print_instruction("#def print()\n");
    print_instruction("JUMP end_$print\n");
    print_instruction("LABEL print\n");
    print_instruction("PUSHFRAME\n");
    print_instruction("DEFVAR LF@%cret\n", '%');
    print_instruction("MOVE LF@%cret nil@nil\n", '%');
    print_instruction("DEFVAR LF@param\n");
    print_instruction("MOVE LF@param LF@%c1\n", '%');
    print_instruction("DEFVAR LF@param$type\n");
    print_instruction("TYPE LF@param$type LF@param\n");
    print_instruction("JUMPIFEQ $none LF@param$type string@nil\n");
    print_instruction("WRITE LF@param\n");
    print_instruction("POPFRAME\n");
    print_instruction("RETURN\n");
    print_instruction("LABEL $none\n");
    print_instruction("WRITE string@None\n");
    print_instruction("POPFRAME\n");
    print_instruction("RETURN\n");
    print_instruction("LABEL end_$print\n");
    print_instruction("\n");

    //Definovani vestavene funkce len
    print_instruction("#def len()\n");
    print_instruction("JUMP end_$len\n");
    print_instruction("LABEL len\n");
    print_instruction("PUSHFRAME\n");
    print_instruction("DEFVAR LF@%cret\n", '%');
    print_instruction("MOVE LF@%cret nil@nil\n", '%');
    print_instruction("DEFVAR LF@param\n");
    print_instruction("MOVE LF@param LF@%c1\n", '%');

    print_instruction("DEFVAR LF@param$type\n");
    print_instruction("TYPE LF@param$type LF@param\n");
    print_instruction("JUMPIFNEQ $operation_error LF@param$type string@string\n");

    print_instruction("STRLEN LF@%cret LF@param\n", '%');
    print_instruction("POPFRAME\n");
    print_instruction("RETURN\n");
    print_instruction("LABEL end_$len\n");
    print_instruction("\n");

    //Definovani vestavene funkce substr
    print_instruction("#def substr()\n");
    print_instruction("JUMP end_$substr\n");
    print_instruction("LABEL substr\n");
    print_instruction("PUSHFRAME\n");
    print_instruction("DEFVAR LF@%cret\n", '%');
    print_instruction("MOVE LF@%cret string@\n", '%');
    print_instruction("DEFVAR LF@param1\n");
    print_instruction("MOVE LF@param1 LF@%c1\n", '%');
    print_instruction("DEFVAR LF@param2\n");
    print_instruction("MOVE LF@param2 LF@%c2\n", '%');
    print_instruction("DEFVAR LF@param3\n");
    print_instruction("MOVE LF@param3 LF@%c3\n", '%');
    print_instruction("ADD LF@param3 LF@param3 LF@param2\n");
    print_instruction("DEFVAR LF@$tmp\n");
    print_instruction("LABEL $while_substr\n");
    print_instruction("LT LF@$tmp LF@param2 LF@param3\n");
    print_instruction("JUMPIFNEQ $end_while_substr LF@$tmp bool@true\n");
    print_instruction("GETCHAR LF@$tmp LF@param1 LF@param2\n");
    print_instruction("CONCAT LF@%cret LF@%cret LF@$tmp\n", '%', '%');
    print_instruction("ADD LF@param2 LF@param2 int@1\n");
    print_instruction("JUMP $while_substr\n");
    print_instruction("LABEL $end_while_substr\n");
    print_instruction("POPFRAME\n");
    print_instruction("RETURN\n");
    print_instruction("LABEL end_$substr\n");
    print_instruction("\n");

    //Definice vestavene funkce ord
    print_instruction("#def ord()\n");
    print_instruction("JUMP end_$ord\n");
    print_instruction("LABEL ord\n");
    print_instruction("PUSHFRAME\n");
    print_instruction("DEFVAR LF@%cret\n", '%');
    print_instruction("MOVE LF@%cret nil@nil\n", '%');
    print_instruction("DEFVAR LF@param1\n");
    print_instruction("MOVE LF@param1 LF@%c1\n", '%');
    print_instruction("DEFVAR LF@param2\n");
    print_instruction("MOVE LF@param2 LF@%c2\n", '%');
    print_instruction("STRI2INT LF@%cret LF@param1 LF@param2\n", '%');
    print_instruction("POPFRAME\n");
    print_instruction("RETURN\n");
    print_instruction("LABEL end_$ord\n");
    print_instruction("\n");

    //Definice vestavene funkce chr
    print_instruction("#def chr()\n");
    print_instruction("JUMP end_$chr\n");
    print_instruction("LABEL chr\n");
    print_instruction("PUSHFRAME\n");
    print_instruction("DEFVAR LF@%cret\n", '%');
    print_instruction("MOVE LF@%cret nil@nil\n", '%');
    print_instruction("DEFVAR LF@param1\n");
    print_instruction("MOVE LF@param1 LF@%c1\n", '%');
    print_instruction("INT2CHAR LF@%cret LF@param1\n", '%');
    print_instruction("POPFRAME\n");
    print_instruction("RETURN\n");
    print_instruction("LABEL end_$chr\n");
    print_instruction("\n");

    //Definovani funkce pro zpracovani vyrazu
    print_instruction("#def do_operation()\n");
    print_instruction("JUMP end_of_def$do_operation\n");
    print_instruction("LABEL $do_operation\n");
    print_instruction("PUSHFRAME\n");
    print_instruction("DEFVAR LF@param1\n");
    print_instruction("POPS LF@param1\n");
    print_instruction("DEFVAR LF@param2\n");
    print_instruction("POPS LF@param2\n");
    print_instruction("DEFVAR LF@param3\n");
    print_instruction("POPS LF@param3\n");
    print_instruction("DEFVAR LF@param2$type\n");
    print_instruction("TYPE LF@param2$type LF@param2\n");
    print_instruction("DEFVAR LF@param3$type\n");
    print_instruction("TYPE LF@param3$type LF@param3\n");
    print_instruction("JUMPIFEQ $operation_plus LF@param1 string@+\n");
    print_instruction("JUMPIFEQ $operation_min LF@param1 string@-\n");
    print_instruction("JUMPIFEQ $operation_mul LF@param1 string@*\n");
    print_instruction("JUMPIFEQ $operation_div LF@param1 string@/\n");
    print_instruction("JUMPIFEQ $operation_intdiv LF@param1 string@//\n");
    print_instruction("JUMPIFEQ $operation_less LF@param1 string@<\n");
    print_instruction("JUMPIFEQ $operation_greater LF@param1 string@>\n");
    print_instruction("JUMPIFEQ $operation_eq LF@param1 string@==\n");
    print_instruction("JUMPIFEQ $operation_not_eq LF@param1 string@!=\n");
    print_instruction("JUMPIFEQ $operation_less_eq LF@param1 string@<=\n");
    print_instruction("JUMPIFEQ $operation_greater_eq LF@param1 string@>=\n");


    print_instruction("LABEL $operation_plus\n");
    //None
    print_instruction("JUMPIFEQ $operation_error LF@param2$type string@nil\n");
    print_instruction("JUMPIFEQ $operation_error LF@param3$type string@nil\n");

    print_instruction("DEFVAR LF@tmp\n");
    print_instruction("JUMPIFNEQ $operation_plus_notsame LF@param2$type LF@param3$type\n");
    print_instruction("JUMPIFEQ $operation_plus_same_int LF@param2$type string@int\n");
    print_instruction("JUMPIFEQ $operation_plus_same_int LF@param2$type string@float\n");
    print_instruction("CONCAT LF@tmp LF@param3 LF@param2\n");
    print_instruction("PUSHS LF@tmp\n");
    print_instruction("JUMP $end_do_operation\n");
    print_instruction("LABEL $operation_plus_same_int\n");
    print_instruction("ADD LF@tmp LF@param2 LF@param3\n");
    print_instruction("PUSHS LF@tmp\n");
    print_instruction("JUMP $end_do_operation\n");
    print_instruction("LABEL $operation_plus_notsame\n");
    print_instruction("JUMPIFEQ $operation_error LF@param2$type string@string\n");
    print_instruction("JUMPIFEQ $operation_error LF@param3$type string@string\n");
    print_instruction("JUMPIFEQ $operation_plus_notsame_1int LF@param2$type string@int\n");
    print_instruction("INT2FLOAT LF@tmp LF@param3\n");
    print_instruction("ADD LF@tmp LF@param2 LF@tmp\n");
    print_instruction("JUMP $operation_plus_notsame_push\n");
    print_instruction("LABEL $operation_plus_notsame_1int\n");
    print_instruction("INT2FLOAT LF@tmp LF@param2\n");
    print_instruction("ADD LF@tmp LF@param3 LF@tmp\n");
    print_instruction("LABEL $operation_plus_notsame_push\n");
    print_instruction("PUSHS LF@tmp\n");
    print_instruction("JUMP $end_do_operation\n");


    print_instruction("LABEL $operation_min\n");
    //None
    print_instruction("JUMPIFEQ $operation_error LF@param2$type string@nil\n");
    print_instruction("JUMPIFEQ $operation_error LF@param3$type string@nil\n");

    print_instruction("JUMPIFEQ $operation_error LF@param2$type string@string\n");
    print_instruction("JUMPIFEQ $operation_error LF@param3$type string@string\n");
    print_instruction("DEFVAR LF@tmp\n");
    print_instruction("JUMPIFNEQ $operation_min_notsame LF@param2$type LF@param3$type\n");
    print_instruction("SUB LF@tmp LF@param3 LF@param2\n");
    print_instruction("PUSHS LF@tmp\n");
    print_instruction("JUMP $end_do_operation\n");
    print_instruction("LABEL $operation_min_notsame\n");
    print_instruction("JUMPIFEQ $operation_min_notsame_1int LF@param2$type string@int\n");
    print_instruction("INT2FLOAT LF@tmp LF@param3\n");
    print_instruction("SUB LF@tmp LF@tmp LF@param2\n");
    print_instruction("PUSHS LF@tmp\n");
    print_instruction("JUMP $end_do_operation\n");
    print_instruction("LABEL $operation_min_notsame_1int\n");
    print_instruction("INT2FLOAT LF@tmp LF@param2\n");
    print_instruction("SUB LF@tmp LF@param3 LF@tmp\n");
    print_instruction("PUSHS LF@tmp\n");
    print_instruction("JUMP $end_do_operation\n");

    print_instruction("LABEL $operation_mul\n");
    //None
    print_instruction("JUMPIFEQ $operation_error LF@param2$type string@nil\n");
    print_instruction("JUMPIFEQ $operation_error LF@param3$type string@nil\n");

    print_instruction("JUMPIFEQ $operation_error LF@param2$type string@string\n");
    print_instruction("JUMPIFEQ $operation_error LF@param3$type string@string\n");
    print_instruction("DEFVAR LF@tmp\n");
    print_instruction("JUMPIFNEQ $operation_mul_notsame LF@param2$type LF@param3$type\n");
    print_instruction("MUL LF@tmp LF@param2 LF@param3\n");
    print_instruction("PUSHS LF@tmp\n");
    print_instruction("JUMP $end_do_operation\n");
    print_instruction("LABEL $operation_mul_notsame\n");
    print_instruction("JUMPIFEQ $operation_mul_notsame_1int LF@param2$type string@int\n");
    print_instruction("INT2FLOAT LF@tmp LF@param3\n");
    print_instruction("MUL LF@tmp LF@tmp LF@param2\n");
    print_instruction("PUSHS LF@tmp\n");
    print_instruction("JUMP $end_do_operation\n");
    print_instruction("LABEL $operation_mul_notsame_1int\n");
    print_instruction("INT2FLOAT LF@tmp LF@param2\n");
    print_instruction("MUL LF@tmp LF@tmp LF@param3\n");
    print_instruction("PUSHS LF@tmp\n");
    print_instruction("JUMP $end_do_operation\n");

    print_instruction("LABEL $operation_div\n");
    //None
    print_instruction("JUMPIFEQ $operation_error LF@param2$type string@nil\n");
    print_instruction("JUMPIFEQ $operation_error LF@param3$type string@nil\n");

    print_instruction("DEFVAR LF@tmp\n");
    print_instruction("DEFVAR LF@tmp2\n");
    print_instruction("JUMPIFEQ $operation_error LF@param2$type string@string\n");
    print_instruction("JUMPIFEQ $operation_error LF@param3$type string@string\n");
    print_instruction("JUMPIFNEQ $operation_div_notsame LF@param2$type LF@param3$type\n");
    print_instruction("JUMPIFEQ $both_int LF@param2$type string@int\n" );
    print_instruction("JUMPIFEQ $operation_error_divzero LF@param2 float@0x0p+0\n");
    print_instruction("DIV LF@tmp LF@param3 LF@param2\n");
    print_instruction("PUSHS LF@tmp\n");
    print_instruction("JUMP $end_do_operation\n");
    print_instruction("LABEL $both_int\n");
    print_instruction("INT2FLOAT LF@tmp LF@param2\n");
    print_instruction("INT2FLOAT LF@tmp2 LF@param3\n");
    print_instruction("JUMPIFEQ $operation_error_divzero LF@tmp float@0x0p+0\n");
    print_instruction("DIV LF@tmp LF@tmp2 LF@tmp\n");
    print_instruction("PUSHS LF@tmp\n");
    print_instruction("JUMP $end_do_operation\n");
    print_instruction("LABEL $operation_div_notsame\n");
    print_instruction("JUMPIFEQ $operation_div_notsame_1int LF@param2$type string@int\n");
    print_instruction("INT2FLOAT LF@tmp LF@param3\n");
    print_instruction("JUMPIFEQ $operation_error_divzero LF@param2 float@0x0p+0\n");
    print_instruction("DIV LF@tmp LF@tmp LF@param2\n");
    print_instruction("PUSHS LF@tmp\n");
    print_instruction("JUMP $end_do_operation\n");
    print_instruction("LABEL $operation_div_notsame_1int\n");
    print_instruction("INT2FLOAT LF@tmp LF@param2\n");
    print_instruction("JUMPIFEQ $operation_error_divzero LF@tmp float@0x0p+0\n");
    print_instruction("DIV LF@tmp LF@param3 LF@tmp\n");
    print_instruction("PUSHS LF@tmp\n");
    print_instruction("JUMP $end_do_operation\n");

    print_instruction("LABEL $operation_intdiv\n");
    //None
    print_instruction("JUMPIFEQ $operation_error LF@param2$type string@nil\n");
    print_instruction("JUMPIFEQ $operation_error LF@param3$type string@nil\n");

    print_instruction("DEFVAR LF@tmp\n");
    print_instruction("MOVE LF@tmp nil@nil\n");
    print_instruction("DEFVAR LF@tmp2\n");
    print_instruction("MOVE LF@tmp2 nil@nil\n");
    print_instruction("JUMPIFEQ $operation_error LF@param2$type string@string\n");
    print_instruction("JUMPIFEQ $operation_error LF@param3$type string@string\n");
    print_instruction("JUMPIFNEQ $operation_idiv_notsame LF@param2$type LF@param3$type\n");
    print_instruction("JUMPIFEQ both_float LF@param2$type string@float\n" );
    print_instruction("JUMPIFEQ $operation_error_divzero LF@tmp int@0\n");
    print_instruction("IDIV LF@tmp LF@param3 LF@param2\n");
    print_instruction("PUSHS LF@tmp\n");
    print_instruction("JUMP $end_do_operation\n");
    print_instruction("LABEL both_float\n");
    print_instruction("FLOAT2INT LF@tmp LF@param2\n");
    print_instruction("FLOAT2INT LF@tmp2 LF@param3\n");
    print_instruction("JUMPIFEQ $operation_error_divzero LF@tmp2 int@0\n");
    print_instruction("IDIV LF@tmp LF@tmp2 LF@tmp\n");
    print_instruction("PUSHS LF@tmp\n");
    print_instruction("JUMP $end_do_operation\n");
    print_instruction("LABEL $operation_idiv_notsame\n");
    print_instruction("JUMPIFEQ $operation_idiv_notsame_1int LF@param2$type string@int\n");
    print_instruction("FLOAT2INT LF@tmp LF@param2\n");
    print_instruction("JUMPIFEQ $operation_error_divzero LF@tmp int@0\n");
    print_instruction("IDIV LF@tmp LF@tmp LF@param2\n");
    print_instruction("PUSHS LF@tmp\n");
    print_instruction("JUMP $end_do_operation\n");
    print_instruction("LABEL $operation_idiv_notsame_1int\n");
    print_instruction("FLOAT2INT LF@tmp LF@param3\n");
    print_instruction("JUMPIFEQ $operation_error_divzero LF@param3 int@0\n");
    print_instruction("IDIV LF@tmp LF@param3 LF@tmp\n");
    print_instruction("PUSHS LF@tmp\n");
    print_instruction("JUMP $end_do_operation\n");

    //<
    print_instruction("LABEL $operation_less\n");
    //None
    print_instruction("JUMPIFEQ $operation_error LF@param2$type string@nil\n");
    print_instruction("JUMPIFEQ $operation_error LF@param3$type string@nil\n");

    print_instruction("DEFVAR LF@tmp_bool\n");
    print_instruction("JUMPIFNEQ $operation_less_notsame LF@param2$type LF@param3$type\n");
    print_instruction("LT LF@tmp_bool LF@param3 LF@param2\n");
    print_instruction("JUMPIFEQ $operation_less_true bool@true LF@tmp_bool\n");
    print_instruction("JUMPIFEQ $operation_less_false bool@false LF@tmp_bool\n");

    print_instruction("LABEL $operation_less_notsame\n");
    print_instruction("JUMPIFEQ $operation_error LF@param2$type string@string\n");
    print_instruction("JUMPIFEQ $operation_error LF@param3$type string@string\n");

    print_instruction("JUMPIFEQ $operation_less_notsame_1int LF@param2$type string@int\n");
    print_instruction("INT2FLOAT LF@param3 LF@param3\n");
    print_instruction("LT LF@tmp_bool LF@param3 LF@param2\n");
    print_instruction("JUMPIFEQ $operation_less_true bool@true LF@tmp_bool\n");
    print_instruction("JUMPIFEQ $operation_less_false bool@false LF@tmp_bool\n");

    print_instruction("LABEL $operation_less_notsame_1int\n");
    print_instruction("INT2FLOAT LF@param2 LF@param2\n");
    print_instruction("LT LF@tmp_bool LF@param3 LF@param2\n");
    print_instruction("JUMPIFEQ $operation_less_true bool@true LF@tmp_bool\n");
    print_instruction("JUMPIFEQ $operation_less_false bool@false LF@tmp_bool\n");

    //true
    print_instruction("LABEL $operation_less_true\n");
    print_instruction("PUSHS int@1\n");
    print_instruction("JUMP $end_do_operation\n");
    //false
    print_instruction("LABEL $operation_less_false\n");
    print_instruction("PUSHS int@0\n");
    print_instruction("JUMP $end_do_operation\n");

    //>
    print_instruction("LABEL $operation_greater\n");
    //None
    print_instruction("JUMPIFEQ $operation_error LF@param2$type string@nil\n");
    print_instruction("JUMPIFEQ $operation_error LF@param3$type string@nil\n");

    print_instruction("DEFVAR LF@tmp_bool\n");
    print_instruction("JUMPIFNEQ $operation_greater_notsame LF@param2$type LF@param3$type\n");
    print_instruction("GT LF@tmp_bool LF@param3 LF@param2\n");
    print_instruction("JUMPIFEQ $operation_greater_true bool@true LF@tmp_bool\n");
    print_instruction("JUMPIFEQ $operation_greater_false bool@false LF@tmp_bool\n");

    print_instruction("LABEL $operation_greater_notsame\n");
    print_instruction("JUMPIFEQ $operation_error LF@param2$type string@string\n");
    print_instruction("JUMPIFEQ $operation_error LF@param3$type string@string\n");

    print_instruction("JUMPIFEQ $operation_greater_notsame_1int LF@param2$type string@int\n");
    print_instruction("INT2FLOAT LF@param3 LF@param3\n");
    print_instruction("GT LF@tmp_bool LF@param3 LF@param2\n");
    print_instruction("JUMPIFEQ $operation_greater_true bool@true LF@tmp_bool\n");
    print_instruction("JUMPIFEQ $operation_greater_false bool@false LF@tmp_bool\n");

    print_instruction("LABEL $operation_greater_notsame_1int\n");
    print_instruction("INT2FLOAT LF@param2 LF@param2\n");
    print_instruction("GT LF@tmp_bool LF@param3 LF@param2\n");
    print_instruction("JUMPIFEQ $operation_greater_true bool@true LF@tmp_bool\n");
    print_instruction("JUMPIFEQ $operation_greater_false bool@false LF@tmp_bool\n");

    //true
    print_instruction("LABEL $operation_greater_true\n");
    print_instruction("PUSHS int@1\n");
    print_instruction("JUMP $end_do_operation\n");
    //false
    print_instruction("LABEL $operation_greater_false\n");
    print_instruction("PUSHS int@0\n");
    print_instruction("JUMP $end_do_operation\n");

    //==
    print_instruction("LABEL $operation_eq\n");
    print_instruction("DEFVAR LF@tmp_bool\n");

    print_instruction("JUMPIFNEQ $operation_eq_notsame LF@param2$type LF@param3$type\n");

    print_instruction("EQ LF@tmp_bool LF@param2 LF@param3\n");
    print_instruction("JUMPIFEQ $operation_eq_true bool@true LF@tmp_bool\n");
    print_instruction("JUMPIFEQ $operation_eq_false bool@false LF@tmp_bool\n");

    print_instruction("LABEL $operation_eq_notsame\n");
    //None
    print_instruction("JUMPIFEQ $operation_eq_false LF@param2$type string@nil\n" );
    print_instruction("JUMPIFEQ $operation_eq_false LF@param3$type string@nil\n" );

    print_instruction("JUMPIFEQ $operation_error LF@param2$type string@string\n");
    print_instruction("JUMPIFEQ $operation_error LF@param3$type string@string\n");

    print_instruction("JUMPIFEQ $operation_eq_notsame_1int LF@param2$type string@int\n");
    print_instruction("INT2FLOAT LF@param3 LF@param3\n");
    print_instruction("EQ LF@tmp_bool LF@param2 LF@param3\n");
    print_instruction("JUMPIFEQ $operation_eq_true bool@true LF@tmp_bool\n");
    print_instruction("JUMPIFEQ $operation_eq_false bool@false LF@tmp_bool\n");

    print_instruction("LABEL $operation_eq_notsame_1int\n");
    print_instruction("INT2FLOAT LF@param2 LF@param2\n");
    print_instruction("EQ LF@tmp_bool LF@param2 LF@param3\n");
    print_instruction("JUMPIFEQ $operation_eq_true bool@true LF@tmp_bool\n");
    print_instruction("JUMPIFEQ $operation_eq_false bool@false LF@tmp_bool\n");


    //true
    print_instruction("LABEL $operation_eq_true\n");
    print_instruction("PUSHS int@1\n");
    print_instruction("JUMP $end_do_operation\n");
    //false
    print_instruction("LABEL $operation_eq_false\n");
    print_instruction("PUSHS int@0\n");
    print_instruction("JUMP $end_do_operation\n");

    //!=
    print_instruction("LABEL $operation_not_eq\n");
    print_instruction("DEFVAR LF@tmp_bool\n");
    print_instruction("JUMPIFNEQ $operation_not_eq_notsame LF@param2$type LF@param3$type\n");
    print_instruction("EQ LF@tmp_bool LF@param2 LF@param3\n");
    print_instruction("JUMPIFNEQ $operation_not_eq_true bool@true LF@tmp_bool\n");
    print_instruction("JUMPIFNEQ $operation_not_eq_false bool@false LF@tmp_bool\n");

    print_instruction("LABEL $operation_not_eq_notsame\n");
    //None
    print_instruction("JUMPIFEQ $operation_eq_true LF@param2$type string@nil\n" );
    print_instruction("JUMPIFEQ $operation_eq_true LF@param2$type string@nil\n" );

    print_instruction("JUMPIFEQ $operation_error LF@param2$type string@string\n");
    print_instruction("JUMPIFEQ $operation_error LF@param3$type string@string\n");

    print_instruction("JUMPIFEQ $operation_not_eq_notsame_1int LF@param2$type string@int\n");
    print_instruction("INT2FLOAT LF@param3 LF@param3\n");
    print_instruction("EQ LF@tmp_bool LF@param2 LF@param3\n");
    print_instruction("JUMPIFNEQ $operation_not_eq_true bool@true LF@tmp_bool\n");
    print_instruction("JUMPIFNEQ $operation_not_eq_false bool@false LF@tmp_bool\n");

    print_instruction("LABEL $operation_not_eq_notsame_1int\n");
    print_instruction("INT2FLOAT LF@param2 LF@param2\n");
    print_instruction("EQ LF@tmp_bool LF@param2 LF@param3\n");
    print_instruction("JUMPIFNEQ $operation_not_eq_true bool@true LF@tmp_bool\n");
    print_instruction("JUMPIFNEQ $operation_not_eq_false bool@false LF@tmp_bool\n");

    //true
    print_instruction("LABEL $operation_not_eq_true\n");
    print_instruction("PUSHS int@1\n");
    print_instruction("JUMP $end_do_operation\n");
    //false
    print_instruction("LABEL $operation_not_eq_false\n");
    print_instruction("PUSHS int@0\n");
    print_instruction("JUMP $end_do_operation\n");

    //<=
    print_instruction("LABEL $operation_less_eq\n");
    //None
    print_instruction("JUMPIFEQ $operation_error LF@param2$type string@nil\n");
    print_instruction("JUMPIFEQ $operation_error LF@param3$type string@nil\n");

    print_instruction("DEFVAR LF@tmp_bool\n");
    print_instruction("JUMPIFNEQ $operation_less_eq_notsame LF@param2$type LF@param3$type\n");
    print_instruction("GT LF@tmp_bool LF@param3 LF@param2\n");
    print_instruction("JUMPIFNEQ $operation_less_eq_true bool@true LF@tmp_bool\n");
    print_instruction("JUMPIFNEQ $operation_less_eq_false bool@false LF@tmp_bool\n");

    print_instruction("LABEL $operation_less_eq_notsame\n");
    print_instruction("JUMPIFEQ $operation_error LF@param2$type string@string\n");
    print_instruction("JUMPIFEQ $operation_error LF@param3$type string@string\n");

    print_instruction("JUMPIFEQ $operation_less_eq_notsame_1int LF@param2$type string@int\n");
    print_instruction("INT2FLOAT LF@param3 LF@param3\n");
    print_instruction("GT LF@tmp_bool LF@param3 LF@param2\n");
    print_instruction("JUMPIFNEQ $operation_less_eq_true bool@true LF@tmp_bool\n");
    print_instruction("JUMPIFNEQ $operation_less_eq_false bool@false LF@tmp_bool\n");

    print_instruction("LABEL $operation_less_eq_notsame_1int\n");
    print_instruction("INT2FLOAT LF@param2 LF@param2\n");
    print_instruction("GT LF@tmp_bool LF@param3 LF@param2\n");
    print_instruction("JUMPIFNEQ $operation_less_eq_true bool@true LF@tmp_bool\n");
    print_instruction("JUMPIFNEQ $operation_less_eq_false bool@false LF@tmp_bool\n");

    //true
    print_instruction("LABEL $operation_less_eq_true\n");
    print_instruction("PUSHS int@1\n");
    print_instruction("JUMP $end_do_operation\n");
    //false
    print_instruction("LABEL $operation_less_eq_false\n");
    print_instruction("PUSHS int@0\n");
    print_instruction("JUMP $end_do_operation\n");

    //>=
    print_instruction("LABEL $operation_greater_eq\n");
    //None
    print_instruction("JUMPIFEQ $operation_error LF@param2$type string@nil\n");
    print_instruction("JUMPIFEQ $operation_error LF@param3$type string@nil\n");

    print_instruction("DEFVAR LF@tmp_bool\n");
    print_instruction("JUMPIFNEQ $operation_greater_eq_notsame LF@param2$type LF@param3$type\n");
    print_instruction("LT LF@tmp_bool LF@param3 LF@param2\n");
    print_instruction("JUMPIFNEQ $operation_greater_eq_true bool@true LF@tmp_bool\n");
    print_instruction("JUMPIFNEQ $operation_greater_eq_false bool@false LF@tmp_bool\n");

    print_instruction("LABEL $operation_greater_eq_notsame\n");
    print_instruction("JUMPIFEQ $operation_error LF@param2$type string@string\n");
    print_instruction("JUMPIFEQ $operation_error LF@param3$type string@string\n");

    print_instruction("JUMPIFEQ $operation_greater_eq_notsame_1int LF@param2$type string@int\n");
    print_instruction("INT2FLOAT LF@param3 LF@param3\n");
    print_instruction("LT LF@tmp_bool LF@param3 LF@param2\n");
    print_instruction("JUMPIFNEQ $operation_greater_eq_true bool@true LF@tmp_bool\n");
    print_instruction("JUMPIFNEQ $operation_greater_eq_false bool@false LF@tmp_bool\n");

    print_instruction("LABEL $operation_greater_eq_notsame_1int\n");
    print_instruction("INT2FLOAT LF@param2 LF@param2\n");
    print_instruction("LT LF@tmp_bool LF@param3 LF@param2\n");
    print_instruction("JUMPIFNEQ $operation_greater_eq_true bool@true LF@tmp_bool\n");
    print_instruction("JUMPIFNEQ $operation_greater_eq_false bool@false LF@tmp_bool\n");

    //true
    print_instruction("LABEL $operation_greater_eq_true\n");
    print_instruction("PUSHS int@1\n");
    print_instruction("JUMP $end_do_operation\n");
    //false
    print_instruction("LABEL $operation_greater_eq_false\n");
    print_instruction("PUSHS int@0\n");
    print_instruction("JUMP $end_do_operation\n");


    print_instruction("LABEL $operation_error\n");
    print_instruction("DPRINT int@4\n");
    print_instruction("EXIT int@4\n"); //error_exit(4)

    print_instruction("LABEL $operation_error_divzero\n");
    print_instruction("DPRINT int@9\n");
    print_instruction("EXIT int@9\n"); //snad ok
    //end_do_operation
    print_instruction("LABEL $end_do_operation\n");

    print_instruction("POPFRAME\n");
    print_instruction("RETURN\n");
    print_instruction("LABEL end_of_def$do_operation\n");
    print_instruction("\n");


}

void call_inputi()
{
    print_instruction("CREATEFRAME\n");
    print_instruction("CALL $inputi\n");
}

void call_inputf()
{
    print_instruction("CREATEFRAME\n");
    print_instruction("CALL $inputf\n");
}

void call_inputs()
{
    print_instruction("CREATEFRAME\n");
    print_instruction("CALL $inputs\n");
}

void call_write(char* arg)
{
    print_instruction("CREATEFRAME\n");
    print_instruction("DEFVAR TF@%c1\n", '%');
    print_instruction("MOVE TF@%c1 %s\n", '%', arg);
    print_instruction("CALL print\n");
}

void call_len(char* arg)
{
    print_instruction("CREATEFRAME\n");
    print_instruction("DEFVAR TF@%c1\n", '%');
    print_instruction("MOVE TF@%c1 %s\n", '%', arg);
    print_instruction("CALL $len\n");
}

void call_substr(char* arg1, char* arg2, char* arg3)
{
    print_instruction("CREATEFRAME\n");
    print_instruction("DEFVAR TF@%c1\n", '%');
    print_instruction("MOVE TF@%c1 %s\n", '%', arg1);
    print_instruction("DEFVAR TF@%c2\n", '%');
    print_instruction("MOVE TF@%c2 %s\n", '%', arg2);
    print_instruction("DEFVAR TF@%c3\n", '%');
    print_instruction("MOVE TF@%c3 %s\n", '%', arg3);
    print_instruction("CALL $substr\n");
}

void call_ord(char* arg1, char* arg2)
{
    print_instruction("CREATEFRAME\n");
    print_instruction("DEFVAR TF@%c1\n", '%');
    print_instruction("MOVE TF@%c1 %s\n", '%', arg1);
    print_instruction("DEFVAR TF@%c2\n", '%');
    print_instruction("MOVE TF@%c2 %s\n", '%', arg2);
    print_instruction("CALL $ord\n");
}

void call_chr(char* arg)
{
    print_instruction("CREATEFRAME\n");
    print_instruction("DEFVAR TF@%c1\n", '%');
    print_instruction("MOVE TF@%c1 %s\n", '%', arg);
    print_instruction("CALL $chr\n");
}

char * convert_str_to_ifjcode_str(char * input){
    int size = 0;
    char * input_cp = input;
    char * tmp;
    int size_alloc = 20;
    char * output = malloc(20); //akolokujeme po blocích o velikosti 20


    while (*input) {
        if (size > (size_alloc - 5)){ //jakmile se priblizime k naplneni reallocujeme
            tmp = malloc(size_alloc + 20);
            if (tmp == NULL)
                error_exit(ERROR_INTERNAL);
            strcpy(tmp, output);
            free(output);
            output = tmp;
            size_alloc += 20;
        }

        if (*input == ' '){ //nahrazeni mezery \032
         output[size] = '\\';
         size++;
         output[size] = '0';
         size++;
         output[size] = '3';
         size++;
         output[size] = '2';
         size++;
        }
        else if (*input == '\n'){ //nahrazeni EOL \010
         output[size] = '\\';
         size++;
         output[size] = '0';
         size++;
         output[size] = '1';
         size++;
         output[size] = '0';
         size++;
        }
        else if (*input == '\t'){ //nahrazeni tab \009
         output[size] = '\\';
         size++;
         output[size] = '0';
         size++;
         output[size] = '0';
         size++;
         output[size] = '9';
         size++;
        }
        else if (*input == '\\'){ //nahrazeni \\ \092
         output[size] = '\\';
         size++;
         output[size] = '0';
         size++;
         output[size] = '9';
         size++;
         output[size] = '2';
         size++;
        }
        else if (*input == '#'){ //nahrazeni # \035
         output[size] = '\\';
         size++;
         output[size] = '0';
         size++;
         output[size] = '3';
         size++;
         output[size] = '5';
         size++;
        }
        else
        {
            output[size] = *input;
            size++;
        }
        input++;
    }
    output[size] = '\0';
    //free(input_cp);
    return output;
}

void print_stack(token_t *sem_array)
{
    for (; sem_array->type != TOKEN_DOLAR; sem_array++) {
        switch (sem_array->type) {
            case TOKEN_INT:
                print_instruction("PUSHS int@%d\n", sem_array->value.int_value);
                break;
            case TOKEN_DOUBLE:
                print_instruction("PUSHS float@%a\n", sem_array->value.double_value);
                break;
            case TOKEN_STRING:
                print_instruction("PUSHS string@%s\n", convert_str_to_ifjcode_str(sem_array->value.string));
                break;
            case TOKEN_ID:
                ;
                int frame = get_frame(sem_array->value.string);
                if (frame == 1) {
                    print_instruction("PUSHS GF@%s\n", sem_array->value.string);
                } else {
                    print_instruction("PUSHS LF@%s\n", sem_array->value.string);
                }
                break;

            case TOKEN_KEYWORD:
                if (sem_array->value.keyword_value == NONE) {
                    print_instruction("PUSHS nil@nil\n");
                } else {
                    error_exit(ERROR_SYNTAX); //dostanu jiný KEYWORD token než NONE
                }
                break;

            case TOKEN_MATH_PLUS:
                print_instruction("CREATEFRAME\n");
                print_instruction("PUSHS string@+\n");
                print_instruction("CALL $do_operation\n");
                break;
            case TOKEN_MATH_MINUS:
                print_instruction("CREATEFRAME\n");
                print_instruction("PUSHS string@-\n");
                print_instruction("CALL $do_operation\n");
                break;
            case TOKEN_MATH_MUL:
                print_instruction("CREATEFRAME\n");
                print_instruction("PUSHS string@*\n");
                print_instruction("CALL $do_operation\n");
                break;
            case TOKEN_MATH_DIV:
                print_instruction("CREATEFRAME\n");
                print_instruction("PUSHS string@/\n");
                print_instruction("CALL $do_operation\n");
                break;
            case TOKEN_MATH_INT_DIV:
                print_instruction("CREATEFRAME\n");
                print_instruction("PUSHS string@//\n");
                print_instruction("CALL $do_operation\n");
                break;
            case TOKEN_LESS:
                print_instruction("CREATEFRAME\n");
                print_instruction("PUSHS string@<\n");
                print_instruction("CALL $do_operation\n");
                break;
            case TOKEN_LESS_EQ:
                print_instruction("CREATEFRAME\n");
                print_instruction("PUSHS string@<=\n");
                print_instruction("CALL $do_operation\n");
                break;
            case TOKEN_GREATER:
                print_instruction("CREATEFRAME\n");
                print_instruction("PUSHS string@>\n");
                print_instruction("CALL $do_operation\n");
                break;
            case TOKEN_GREATER_EQ:
                print_instruction("CREATEFRAME\n");
                print_instruction("PUSHS string@>=\n");
                print_instruction("CALL $do_operation\n");
                break;
            case TOKEN_EQ:
                print_instruction("CREATEFRAME\n");
                print_instruction("PUSHS string@==\n");
                print_instruction("CALL $do_operation\n");
                break;
            case TOKEN_NOT_EQ:
                print_instruction("CREATEFRAME\n");
                print_instruction("PUSHS string@!=\n");
                print_instruction("CALL $do_operation\n");
                break;
            default:
                error_exit(ERROR_SYNTAX);
                break;
        }
    }
}
/* konec souboru IFJ_builtin.c */
