//vestavene funkce
#include <stdio.h>
#include "IFJ_builtin.h"
#include "symtable.h"
#include "IFJ_error.h"
#include "IFJ_scanner.h"
#include "stdlib.h"
#include <string.h>

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
    printf(".IFJcode19\n");
    //Definovani vestavene funkce inputi
    printf("#def inputi()\n");
    printf("JUMP end_$inputi\n");
    printf("LABEL inputi\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@%cret\n", '%');
    printf("MOVE LF@%cret nil@nil\n", '%');
    printf("READ LF@%cret int\n", '%');
    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL end_$inputi\n");
    printf("\n");

    //Definovani vestavene funkce inputf
    printf("#def inputf()\n");
    printf("JUMP end_$inputf\n");
    printf("LABEL inputf\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@%cret\n", '%');
    printf("MOVE LF@%cret nil@nil\n", '%');
    printf("READ LF@%cret float\n", '%');
    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL end_$inputf\n");
    printf("\n");

    //Definovani vestavene funkce inputs
    printf("#def inputs()\n");
    printf("JUMP end_$inputs\n");
    printf("LABEL inputs\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@%cret\n", '%');
    printf("MOVE LF@%cret nil@nil\n", '%');
    printf("READ LF@%cret string\n", '%');
    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL end_$inputs\n");
    printf("\n");

    //Definovani vestavene funkce write
    printf("#def print()\n");
    printf("JUMP end_$print\n");
    printf("LABEL print\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@%cret\n", '%');
    printf("MOVE LF@%cret nil@nil\n", '%');
    printf("DEFVAR LF@param\n");
    printf("MOVE LF@param LF@%c1\n", '%');
    printf("WRITE LF@param\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL end_$print\n");
    printf("\n");

    //Definovani vestavene funkce len
    printf("#def len()\n");
    printf("JUMP end_$len\n");
    printf("LABEL len\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@%cret\n", '%');
    printf("MOVE LF@%cret nil@nil\n", '%');
    printf("DEFVAR LF@param\n");
    printf("MOVE LF@param LF@%c1\n", '%');
    printf("STRLEN LF@%cret LF@param\n", '%');
    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL end_$len\n");
    printf("\n");

    //Definovani vestavene funkce substr
    printf("#def substr()\n");
    printf("JUMP end_$substr\n");
    printf("LABEL substr\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@%cret\n", '%');
    printf("MOVE LF@%cret string@\n", '%');
    printf("DEFVAR LF@param1\n");
    printf("MOVE LF@param1 LF@%c1\n", '%');
    printf("DEFVAR LF@param2\n");
    printf("MOVE LF@param2 LF@%c2\n", '%');
    printf("DEFVAR LF@param3\n");
    printf("MOVE LF@param3 LF@%c3\n", '%');
    printf("ADD LF@param3 LF@param3 LF@param2\n");
    printf("DEFVAR LF@$tmp\n");
    printf("LABEL $while_substr\n");
    printf("LT LF@tmp LF@param2 LF@param3\n");
    printf("JUMPIFNEQ $end_while_substr LF@tmp bool@true\n");
    printf("GETCHAR LF@tmp LF@param1 LF@param2\n");
    printf("CONCAT LF@%cret LF@%cret LF@tmp\n", '%', '%');
    printf("ADD LF@param2 LF@param2 int@1\n");
    printf("JUMP $while_substr\n");
    printf("LABEL $end_while_substr\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL end_$substr\n");
    printf("\n");

    //Definice vestavene funkce ord
    printf("#def ord()\n");
    printf("JUMP end_$ord\n");
    printf("LABEL ord\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@%cret\n", '%');
    printf("MOVE LF@%cret nil@nil\n", '%');
    printf("DEFVAR LF@param1\n");
    printf("MOVE LF@param1 LF@%c1\n", '%');
    printf("DEFVAR LF@param2\n");
    printf("MOVE LF@param2 LF@%c2\n", '%');
    printf("STRI2INT LF@%cret LF@param1 LF@param2\n", '%');
    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL end_$ord\n");
    printf("\n");

    //Definice vestavene funkce chr
    printf("#def chr()\n");
    printf("JUMP end_$chr\n");
    printf("LABEL chr\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@%cret\n", '%');
    printf("MOVE LF@%cret nil@nil\n", '%');
    printf("DEFVAR LF@param1\n");
    printf("MOVE LF@param1 LF@%c1\n", '%');
    printf("INT2CHAR LF@%cret LF@param1\n", '%');
    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL end_$chr\n");
    printf("\n");

    //Definovani funkce pro zpracovani vyrazu
    printf("#def do_operation()\n");
    printf("JUMP end_of_def$do_operation\n");
    printf("LABEL $do_operation\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@param1\n");
    printf("POPS LF@param1\n");
    printf("DEFVAR LF@param2\n");
    printf("POPS LF@param2\n");
    printf("DEFVAR LF@param3\n");
    printf("POPS LF@param3\n");
    printf("DEFVAR LF@param2$type\n");
    printf("TYPE LF@param2$type LF@param2\n");
    printf("DEFVAR LF@param3$type\n");
    printf("TYPE LF@param3$type LF@param3\n");
    printf("JUMPIFEQ $operation_plus LF@param1 string@+\n");
    printf("JUMPIFEQ $operation_min LF@param1 string@-\n");
    printf("JUMPIFEQ $operation_mul LF@param1 string@*\n");
    printf("JUMPIFEQ $operation_div LF@param1 string@/\n");
    printf("JUMPIFEQ $operation_intdiv LF@param1 string@//\n");
    printf("JUMPIFEQ $operation_less LF@param1 string@<\n");
    printf("JUMPIFEQ $operation_greater LF@param1 string@>\n");
    printf("JUMPIFEQ $operation_eq LF@param1 string@==\n");
    printf("JUMPIFEQ $operation_not_eq LF@param1 string@!=\n");
    printf("JUMPIFEQ $operation_less_eq LF@param1 string@<=\n");
    printf("JUMPIFEQ $operation_greater_eq LF@param1 string@>=\n");


    printf("LABEL $operation_plus\n");
    printf("DEFVAR LF@tmp\n");
    printf("JUMPIFNEQ $operation_plus_notsame LF@param2$type LF@param3$type\n");
    printf("JUMPIFEQ $operation_plus_same_int LF@param2$type string@int\n");
    printf("JUMPIFEQ $operation_plus_same_int LF@param2$type string@float\n");
    printf("CONCAT LF@tmp LF@param2 LF@param3\n");
    printf("PUSHS LF@tmp\n");
    printf("JUMP $end_do_operation\n");
    printf("LABEL $operation_plus_same_int\n");
    printf("ADD LF@tmp LF@param2 LF@param3\n");
    printf("PUSHS LF@tmp\n");
    printf("JUMP $end_do_operation\n");
    printf("LABEL $operation_plus_notsame\n");
    printf("JUMPIFEQ $operation_error LF@param2$type string@string\n");
    printf("JUMPIFEQ $operation_error LF@param3$type string@string\n");
    printf("JUMPIFEQ $operation_plus_notsame_1int LF@param2$type string@int\n");
    printf("INT2FLOAT LF@tmp LF@param3\n");
    printf("ADD LF@tmp LF@param2 LF@tmp\n");
    printf("JUMP $operation_plus_notsame_push\n");
    printf("LABEL $operation_plus_notsame_1int\n");
    printf("INT2FLOAT LF@tmp LF@param2\n");
    printf("ADD LF@tmp LF@param3 LF@tmp\n");
    printf("LABEL $operation_plus_notsame_push\n");
    printf("PUSHS LF@tmp\n");
    printf("JUMP $end_do_operation\n");


    printf("LABEL $operation_min\n");
    printf("JUMPIFEQ $operation_error LF@param2$type string@string\n");
    printf("JUMPIFEQ $operation_error LF@param3$type string@string\n");
    printf("DEFVAR LF@tmp\n");
    printf("JUMPIFNEQ $operation_min_notsame LF@param2$type LF@param3$type\n");
    printf("SUB LF@tmp LF@param3 LF@param2\n");
    printf("PUSHS LF@tmp\n");
    printf("JUMP $end_do_operation\n");
    printf("LABEL $operation_min_notsame\n");
    printf("JUMPIFEQ $operation_min_notsame_1int LF@param2$type string@int\n");
    printf("INT2FLOAT LF@tmp LF@param3\n");
    printf("SUB LF@tmp LF@tmp LF@param2\n");
    printf("PUSHS LF@tmp\n");
    printf("JUMP $end_do_operation\n");
    printf("LABEL $operation_min_notsame_1int\n");
    printf("INT2FLOAT LF@tmp LF@param2\n");
    printf("SUB LF@tmp LF@param3 LF@tmp\n");
    printf("PUSHS LF@tmp\n");
    printf("JUMP $end_do_operation\n");

    printf("LABEL $operation_mul\n");
    printf("JUMPIFEQ $operation_error LF@param2$type string@string\n");
    printf("JUMPIFEQ $operation_error LF@param3$type string@string\n");
    printf("DEFVAR LF@tmp\n");
    printf("JUMPIFNEQ $operation_mul_notsame LF@param2$type LF@param3$type\n");
    printf("MUL LF@tmp LF@param2 LF@param3\n");
    printf("PUSHS LF@tmp\n");
    printf("JUMP $end_do_operation\n");
    printf("LABEL $operation_mul_notsame\n");
    printf("JUMPIFEQ $operation_mul_notsame_1int LF@param2$type string@int\n");
    printf("INT2FLOAT LF@tmp LF@param3\n");
    printf("MUL LF@tmp LF@tmp LF@param2\n");
    printf("PUSHS LF@tmp\n");
    printf("JUMP $end_do_operation\n");
    printf("LABEL $operation_mul_notsame_1int\n");
    printf("INT2FLOAT LF@tmp LF@param2\n");
    printf("MUL LF@tmp LF@tmp LF@param3\n");
    printf("PUSHS LF@tmp\n");
    printf("JUMP $end_do_operation\n");

    printf("LABEL $operation_div\n");
    printf("DEFVAR LF@tmp\n");
    printf("DEFVAR LF@tmp2\n");
    printf("JUMPIFEQ $operation_error LF@param2$type string@string\n");
    printf("JUMPIFEQ $operation_error LF@param3$type string@string\n");
    printf("JUMPIFNEQ $operation_div_notsame LF@param2$type LF@param3$type\n");
    printf("JUMPIFEQ $both_int LF@param2$type string@int\n" );
    printf("JUMPIFEQ $operation_error_divzero LF@param2 float@0x0p+0\n");
    printf("DIV LF@tmp LF@param3 LF@param2\n");
    printf("PUSHS LF@tmp\n");
    printf("JUMP $end_do_operation\n");
    printf("LABEL $both_int\n");
    printf("INT2FLOAT LF@tmp LF@param2\n");
    printf("INT2FLOAT LF@tmp2 LF@param3\n");
    printf("JUMPIFEQ $operation_error_divzero LF@tmp2 float@0x0p+0\n");
    printf("DIV LF@tmp LF@tmp2 LF@tmp\n");
    printf("PUSHS LF@tmp\n");
    printf("JUMP $end_do_operation\n");
    printf("LABEL $operation_div_notsame\n");
    printf("JUMPIFEQ $operation_div_notsame_1int LF@param2$type string@int\n");
    printf("INT2FLOAT LF@param3 LF@tmp\n");
    printf("JUMPIFEQ $operation_error_divzero LF@tmp float@0x0p+0\n");
    printf("DIV LF@tmp LF@tmp LF@param2\n");
    printf("PUSHS LF@tmp\n");
    printf("JUMP $end_do_operation\n");
    printf("LABEL $operation_div_notsame_1int\n");
    printf("INT2FLOAT LF@tmp LF@param2\n");
    printf("JUMPIFEQ $operation_error_divzero LF@param3 float@0x0p+0\n");
    printf("DIV LF@tmp LF@param3 LF@tmp\n");
    printf("PUSHS LF@tmp\n");
    printf("JUMP $end_do_operation\n");

    printf("LABEL $operation_intdiv\n");
    printf("DEFVAR LF@tmp\n");
    printf("MOVE LF@tmp nil@nil\n");
    printf("DEFVAR LF@tmp2\n");
    printf("MOVE LF@tmp2 nil@nil\n");
    printf("JUMPIFEQ $operation_error LF@param2$type string@string\n");
    printf("JUMPIFEQ $operation_error LF@param3$type string@string\n");
    printf("JUMPIFNEQ $operation_idiv_notsame LF@param2$type LF@param3$type\n");
    printf("JUMPIFEQ both_float LF@param2$type string@float\n" );
    printf("JUMPIFEQ $operation_error_divzero LF@tmp int@0\n");
    printf("IDIV LF@tmp LF@param3 LF@param2\n");
    printf("PUSHS LF@tmp\n");
    printf("JUMP $end_do_operation\n");
    printf("LABEL both_float\n");
    printf("FLOAT2INT LF@tmp LF@param2\n");
    printf("FLOAT2INT LF@tmp2 LF@param3\n");
    printf("JUMPIFEQ $operation_error_divzero LF@tmp2 int@0\n");
    printf("IDIV LF@tmp LF@tmp2 LF@tmp\n");
    printf("PUSHS LF@tmp\n");
    printf("JUMP $end_do_operation\n");
    printf("LABEL $operation_idiv_notsame\n");
    printf("JUMPIFEQ $operation_idiv_notsame_1int LF@param2$type string@int\n");
    printf("FLOAT2INT LF@tmp LF@param2\n");
    printf("JUMPIFEQ $operation_error_divzero LF@tmp int@0\n");
    printf("IDIV LF@tmp LF@tmp LF@param2\n");
    printf("PUSHS LF@tmp\n");
    printf("JUMP $end_do_operation\n");
    printf("LABEL $operation_idiv_notsame_1int\n");
    printf("FLOAT2INT LF@tmp LF@param3\n");
    printf("JUMPIFEQ $operation_error_divzero LF@param3 int@0\n");
    printf("IDIV LF@tmp LF@param3 LF@tmp\n");
    printf("PUSHS LF@tmp\n");
    printf("JUMP $end_do_operation\n");

    //<
    printf("LABEL $operation_less\n");

    printf("DEFVAR LF@tmp_bool\n");
    printf("JUMPIFNEQ $operation_less_notsame LF@param2$type LF@param3$type\n");
    printf("LT LF@tmp_bool LF@param2 LF@param3\n");
    printf("JUMPIFNEQ $operation_less_true bool@true LF@tmp_bool\n");
    printf("JUMPIFNEQ $operation_less_false bool@false LF@tmp_bool\n");

    printf("LABEL $operation_less_notsame\n");
    printf("JUMPIFEQ $operation_error LF@param2$type string@string\n");
    printf("JUMPIFEQ $operation_error LF@param3$type string@string\n");

    printf("JUMPIFEQ $operation_less_notsame_1int LF@param2$type string@int\n");
    printf("INT2FLOAT LF@param3 LF@param3\n");
    printf("LT LF@tmp_bool LF@param2 LF@param3\n");
    printf("JUMPIFNEQ $operation_less_true bool@true LF@tmp_bool\n");
    printf("JUMPIFNEQ $operation_less_false bool@false LF@tmp_bool\n");

    printf("LABEL $operation_less_notsame_1int\n");
    printf("INT2FLOAT LF@param2 LF@param2\n");
    printf("LT LF@tmp_bool LF@param2 LF@param3\n");
    printf("JUMPIFNEQ $operation_less_true bool@true LF@tmp_bool\n");
    printf("JUMPIFNEQ $operation_less_false bool@false LF@tmp_bool\n");

    //true
    printf("LABEL $operation_less_true\n");
    printf("PUSHS int@1\n");
    printf("JUMP $end_do_operation\n");
    //false
    printf("LABEL $operation_less_false\n");
    printf("PUSHS int@0\n");
    printf("JUMP $end_do_operation\n");

    //>
    printf("LABEL $operation_greater\n");
    printf("DEFVAR LF@tmp_bool\n");
    printf("JUMPIFNEQ $operation_greater_notsame LF@param2$type LF@param3$type\n");
    printf("GT LF@tmp_bool LF@param2 LF@param3\n");
    printf("JUMPIFNEQ $operation_greater_true bool@true LF@tmp_bool\n");
    printf("JUMPIFNEQ $operation_greater_false bool@false LF@tmp_bool\n");

    printf("LABEL $operation_greater_notsame\n");
    printf("JUMPIFNEQ $operation_error LF@param2$type string@string\n");
    printf("JUMPIFNEQ $operation_error LF@param3$type string@string\n");

    printf("JUMPIFEQ $operation_greater_notsame_1int LF@param2$type string@int\n");
    printf("INT2FLOAT LF@param3 LF@param3\n");
    printf("GT LF@tmp_bool LF@param2 LF@param3\n");
    printf("JUMPIFNEQ $operation_greater_true bool@true LF@tmp_bool\n");
    printf("JUMPIFNEQ $operation_greater_false bool@false LF@tmp_bool\n");

    printf("LABEL $operation_greater_notsame_1int\n");
    printf("INT2FLOAT LF@param2 LF@param2\n");
    printf("GT LF@tmp_bool LF@param2 LF@param3\n");
    printf("JUMPIFNEQ $operation_greater_true bool@true LF@tmp_bool\n");
    printf("JUMPIFNEQ $operation_greater_false bool@false LF@tmp_bool\n");

    //true
    printf("LABEL $operation_greater_true\n");
    printf("PUSHS int@1\n");
    printf("JUMP $end_do_operation\n");
    //false
    printf("LABEL $operation_greater_false\n");
    printf("PUSHS int@0\n");
    printf("JUMP $end_do_operation\n");

    //==
    printf("LABEL $operation_eq\n");
    printf("DEFVAR LF@tmp_bool\n");
    printf("JUMPIFNEQ $operation_eq_notsame LF@param2$type LF@param3$type\n");
    printf("EQ LF@tmp_bool LF@param2 LF@param3\n");
    printf("JUMPIFEQ $operation_eq_true bool@true LF@tmp_bool\n");
    printf("JUMPIFEQ $operation_eq_false bool@false LF@tmp_bool\n");

    printf("LABEL $operation_eq_notsame\n");
    printf("JUMPIFEQ $operation_error LF@param2$type string@string\n");
    printf("JUMPIFEQ $operation_error LF@param3$type string@string\n");

    printf("JUMPIFEQ $operation_eq_notsame_1int LF@param2$type string@int\n");
    printf("INT2FLOAT LF@param3 LF@param3\n");
    printf("EQ LF@tmp_bool LF@param2 LF@param3\n");
    printf("JUMPIFEQ $operation_eq_true bool@true LF@tmp_bool\n");
    printf("JUMPIFEQ $operation_eq_false bool@false LF@tmp_bool\n");

    printf("LABEL $operation_eq_notsame_1int\n");
    printf("INT2FLOAT LF@param2 LF@param2\n");
    printf("EQ LF@tmp_bool LF@param2 LF@param3\n");
    printf("JUMPIFEQ $operation_eq_true bool@true LF@tmp_bool\n");
    printf("JUMPIFEQ $operation_eq_false bool@false LF@tmp_bool\n");

    //true
    printf("LABEL $operation_eq_true\n");
    printf("PUSHS int@1\n");
    printf("JUMP $end_do_operation\n");
    //false
    printf("LABEL $operation_eq_false\n");
    printf("PUSHS int@0\n");
    printf("JUMP $end_do_operation\n");

    //!=
    printf("LABEL $operation_not_eq\n");
    printf("DEFVAR LF@tmp_bool\n");
    printf("JUMPIFNEQ $operation_not_eq_notsame LF@param2$type LF@param3$type\n");
    printf("EQ LF@tmp_bool LF@param2 LF@param3\n");
    printf("JUMPIFNEQ $operation_not_eq_true bool@true LF@tmp_bool\n");
    printf("JUMPIFNEQ $operation_not_eq_false bool@false LF@tmp_bool\n");

    printf("LABEL $operation_not_eq_notsame\n");
    printf("JUMPIFEQ $operation_error LF@param2$type string@string\n");
    printf("JUMPIFEQ $operation_error LF@param3$type string@string\n");

    printf("JUMPIFEQ $operation_not_eq_notsame_1int LF@param2$type string@int\n");
    printf("INT2FLOAT LF@param3 LF@param3\n");
    printf("EQ LF@tmp_bool LF@param2 LF@param3\n");
    printf("JUMPIFNEQ $operation_not_eq_true bool@true LF@tmp_bool\n");
    printf("JUMPIFNEQ $operation_not_eq_false bool@false LF@tmp_bool\n");

    printf("LABEL $operation_not_eq_notsame_1int\n");
    printf("INT2FLOAT LF@param2 LF@param2\n");
    printf("EQ LF@tmp_bool LF@param2 LF@param3\n");
    printf("JUMPIFNEQ $operation_not_eq_true bool@true LF@tmp_bool\n");
    printf("JUMPIFNEQ $operation_not_eq_false bool@false LF@tmp_bool\n");

    //true
    printf("LABEL $operation_not_eq_true\n");
    printf("PUSHS int@1\n");
    printf("JUMP $end_do_operation\n");
    //false
    printf("LABEL $operation_not_eq_false\n");
    printf("PUSHS int@0\n");
    printf("JUMP $end_do_operation\n");

    //<=
    printf("LABEL $operation_less_eq\n");
    printf("DEFVAR LF@tmp_bool\n");
    printf("JUMPIFNEQ $operation_less_eq_notsame LF@param2$type LF@param3$type\n");
    printf("GT LF@tmp_bool LF@param2 LF@param3\n");
    printf("JUMPIFNEQ $operation_less_eq_true bool@true LF@tmp_bool\n");
    printf("JUMPIFNEQ $operation_less_eq_false bool@false LF@tmp_bool\n");

    printf("LABEL $operation_less_eq_notsame\n");
    printf("JUMPIFEQ $operation_error LF@param2$type string@string\n");
    printf("JUMPIFEQ $operation_error LF@param3$type string@string\n");

    printf("JUMPIFEQ $operation_less_eq_notsame_1int LF@param2$type string@int\n");
    printf("INT2FLOAT LF@param3 LF@param3\n");
    printf("GT LF@tmp_bool LF@param2 LF@param3\n");
    printf("JUMPIFNEQ $operation_less_eq_true bool@true LF@tmp_bool\n");
    printf("JUMPIFNEQ $operation_less_eq_false bool@false LF@tmp_bool\n");

    printf("LABEL $operation_less_eq_notsame_1int\n");
    printf("INT2FLOAT LF@param2 LF@param2\n");
    printf("GT LF@tmp_bool LF@param2 LF@param3\n");
    printf("JUMPIFNEQ $operation_less_eq_true bool@true LF@tmp_bool\n");
    printf("JUMPIFNEQ $operation_less_eq_false bool@false LF@tmp_bool\n");

    //true
    printf("LABEL $operation_less_eq_true\n");
    printf("PUSHS int@1\n");
    printf("JUMP $end_do_operation\n");
    //false
    printf("LABEL $operation_less_eq_false\n");
    printf("PUSHS int@0\n");
    printf("JUMP $end_do_operation\n");

    //>=
    printf("LABEL $operation_greater_eq\n");
    printf("DEFVAR LF@tmp_bool\n");
    printf("JUMPIFNEQ $operation_greater_eq_notsame LF@param2$type LF@param3$type\n");
    printf("LT LF@tmp_bool LF@param2 LF@param3\n");
    printf("JUMPIFNEQ $operation_greater_eq_true bool@true LF@tmp_bool\n");
    printf("JUMPIFNEQ $operation_greater_eq_false bool@false LF@tmp_bool\n");

    printf("LABEL $operation_greater_eq_notsame\n");
    printf("JUMPIFEQ $operation_error LF@param2$type string@string\n");
    printf("JUMPIFEQ $operation_error LF@param3$type string@string\n");

    printf("JUMPIFEQ $operation_greater_eq_notsame_1int LF@param2$type string@int\n");
    printf("INT2FLOAT LF@param3 LF@param3\n");
    printf("LT LF@tmp_bool LF@param2 LF@param3\n");
    printf("JUMPIFNEQ $operation_greater_eq_true bool@true LF@tmp_bool\n");
    printf("JUMPIFNEQ $operation_greater_eq_false bool@false LF@tmp_bool\n");

    printf("LABEL $operation_greater_eq_notsame_1int\n");
    printf("INT2FLOAT LF@param2 LF@param2\n");
    printf("LT LF@tmp_bool LF@param2 LF@param3\n");
    printf("JUMPIFNEQ $operation_greater_eq_true bool@true LF@tmp_bool\n");
    printf("JUMPIFNEQ $operation_greater_eq_false bool@false LF@tmp_bool\n");

    //true
    printf("LABEL $operation_greater_eq_true\n");
    printf("PUSHS int@1\n");
    printf("JUMP $end_do_operation\n");
    //false
    printf("LABEL $operation_greater_eq_false\n");
    printf("PUSHS int@0\n");
    printf("JUMP $end_do_operation\n");


    printf("LABEL $operation_error\n");
    printf("DPRINT int@4\n");
    printf("EXIT int@4\n"); //error_exit(4)

    printf("LABEL $operation_error_divzero\n");
    printf("DPRINT int@9\n");
    printf("EXIT int@9\n"); //snad ok
    //end_do_operation
    printf("LABEL $end_do_operation\n");

    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL end_of_def$do_operation\n");
    printf("\n");


}

void call_inputi()
{
    printf("CREATEFRAME\n");
    printf("CALL $inputi\n");
}

void call_inputf()
{
    printf("CREATEFRAME\n");
    printf("CALL $inputf\n");
}

void call_inputs()
{
    printf("CREATEFRAME\n");
    printf("CALL $inputs\n");
}

void call_write(char* arg)
{
    printf("CREATEFRAME\n");
    printf("DEFVAR TF@%c1\n", '%');
    printf("MOVE TF@%c1 %s\n", '%', arg);
    printf("CALL print\n");
}

void call_len(char* arg)
{
    printf("CREATEFRAME\n");
    printf("DEFVAR TF@%c1\n", '%');
    printf("MOVE TF@%c1 %s\n", '%', arg);
    printf("CALL $len\n");
}

void call_substr(char* arg1, char* arg2, char* arg3)
{
    printf("CREATEFRAME\n");
    printf("DEFVAR TF@%c1\n", '%');
    printf("MOVE TF@%c1 %s\n", '%', arg1);
    printf("DEFVAR TF@%c2\n", '%');
    printf("MOVE TF@%c2 %s\n", '%', arg2);
    printf("DEFVAR TF@%c3\n", '%');
    printf("MOVE TF@%c3 %s\n", '%', arg3);
    printf("CALL $substr\n");
}

void call_ord(char* arg1, char* arg2)
{
    printf("CREATEFRAME\n");
    printf("DEFVAR TF@%c1\n", '%');
    printf("MOVE TF@%c1 %s\n", '%', arg1);
    printf("DEFVAR TF@%c2\n", '%');
    printf("MOVE TF@%c2 %s\n", '%', arg2);
    printf("CALL $ord\n");
}

void call_chr(char* arg)
{
    printf("CREATEFRAME\n");
    printf("DEFVAR TF@%c1\n", '%');
    printf("MOVE TF@%c1 %s\n", '%', arg);
    printf("CALL $chr\n");
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
        else if (*input == '\n'){ //nahrazeni mezery \032
         output[size] = '\\';
         size++;
         output[size] = '0';
         size++;
         output[size] = '1';
         size++;
         output[size] = '0';
         size++;
        }
        else if (*input == '\t'){ //nahrazeni mezery \032
         output[size] = '\\';
         size++;
         output[size] = '0';
         size++;
         output[size] = '0';
         size++;
         output[size] = '9';
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
                printf("PUSHS int@%d\n", sem_array->value.int_value);
                break;
            case TOKEN_DOUBLE:
                printf("PUSHS float@%a\n", sem_array->value.double_value);
                break;
            case TOKEN_STRING:
                printf("PUSHS string@%s\n", sem_array->value.string);
                break;
            case TOKEN_ID:
                ;
                int frame = get_frame(sem_array->value.string);
                if (frame == 1) {
                    printf("PUSHS GF@%s\n", sem_array->value.string);
                } else {
                    printf("PUSHS LF@%s\n", sem_array->value.string);
                }
                break;

            case TOKEN_KEYWORD:
                if (sem_array->value.keyword_value == NONE) {
                    printf("PUSHS nil@nil\n");
                } else {
                    error_exit(ERROR_SYNTAX); //dostanu jiný KEYWORD token než NONE
                }

            case TOKEN_MATH_PLUS:
                printf("CREATEFRAME\n");
                printf("PUSHS string@+\n");
                printf("CALL $do_operation\n");
                break;
            case TOKEN_MATH_MINUS:
                printf("CREATEFRAME\n");
                printf("PUSHS string@-\n");
                printf("CALL $do_operation\n");
                break;
            case TOKEN_MATH_MUL:
                printf("CREATEFRAME\n");
                printf("PUSHS string@*\n");
                printf("CALL $do_operation\n");
                break;
            case TOKEN_MATH_DIV:
                printf("CREATEFRAME\n");
                printf("PUSHS string@/\n");
                printf("CALL $do_operation\n");
                break;
            case TOKEN_MATH_INT_DIV:
                printf("CREATEFRAME\n");
                printf("PUSHS string@//\n");
                printf("CALL $do_operation\n");
                break;
            case TOKEN_LESS:
                printf("CREATEFRAME\n");
                printf("PUSHS string@<\n");
                printf("CALL $do_operation\n");
                break;
            case TOKEN_LESS_EQ:
                printf("CREATEFRAME\n");
                printf("PUSHS string@<=\n");
                printf("CALL $do_operation\n");
                break;
            case TOKEN_GREATER:
                printf("CREATEFRAME\n");
                printf("PUSHS string@>\n");
                printf("CALL $do_operation\n");
                break;
            case TOKEN_GREATER_EQ:
                printf("CREATEFRAME\n");
                printf("PUSHS string@>=\n");
                printf("CALL $do_operation\n");
                break;
            case TOKEN_EQ:
                printf("CREATEFRAME\n");
                printf("PUSHS string@==\n");
                printf("CALL $do_operation\n");
                break;
            case TOKEN_NOT_EQ:
                printf("CREATEFRAME\n");
                printf("PUSHS string@!=\n");
                printf("CALL $do_operation\n");
                break;
            default:
                error_exit(ERROR_SYNTAX);
                break;
        }
    }
}
