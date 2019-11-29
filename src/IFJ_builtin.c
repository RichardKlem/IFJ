//vestavene funkce
#include "IFJ_builtin.h"
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
    //Definovani vestavene funkce inputi
    printf("#def inputi()\n");
    printf("JUMP end_$inputi\n");
    printf("LABEL $inputi\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@\cret\n", '%');
    printf("MOVE LF@\cret nil@nil\n", '%');
    printf("READ LF@\cret int\n", '%');
    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL end_$inputi\n");
    printf("\n");

    //Definovani vestavene funkce inputf
    printf("#def inputf()\n");
    printf("JUMP end_$inputf\n");
    printf("LABEL $inputf\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@\cret\n", '%');
    printf("MOVE LF@\cret nil@nil\n", '%');
    printf("READ LF@\cret float\n", '%');
    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL end_$inputf\n");
    printf("\n");

    //Definovani vestavene funkce inputs
    printf("#def inputs()\n");
    printf("JUMP end_$inputs\n");
    printf("LABEL $inputs\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@\cret\n", '%');
    printf("MOVE LF@\cret nil@nil\n", '%');
    printf("READ LF@\cret string\n", '%');
    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL end_$inputs\n");
    printf("\n");

    //Definovani vestavene funkce write
    printf("#def print()\n");
    printf("JUMP end_$print\n");
    printf("LABEL $print\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@param\n");
    printf("MOVE LF@param LF@\c1\n", '%');
    printf("WRITE LF@param\n", '%');
    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL end_$print\n");
    printf("\n");

    //Definovani vestavene funkce len
    printf("#def len()\n");
    printf("JUMP end_$len\n");
    printf("LABEL $len\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@\cret\n", '%');
    printf("MOVE LF@\cret nil@nil\n", '%');
    printf("DEFVAR LF@param\n");
    printf("MOVE LF@param LF@\c1\n", '%', '%');
    printf("STRLEN LF@\cret LF@param\n", '%');
    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL end_$len\n");
    printf("\n");

    //Definovani vestavene funkce substr
    printf("#def substr()\n");
    printf("JUMP end_$substr\n");
    printf("LABEL $substr\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@\cret\n", '%');
    printf("MOVE LF@\cret string@\n", '%');
    printf("DEFVAR LF@param1\n");
    printf("MOVE LF@param1 LF@\c1\n", '%');
    printf("DEFVAR LF@param2\n", '%');
    printf("MOVE LF@param2 LF@\c2\n", '%');
    printf("DEFVAR LF@param3\n", '%');
    printf("MOVE LF@param3 LF@\c3\n", '%');
    printf("ADD LF@param3 LF@param3 LF@param2\n");
    printf("DEFVAR LF$tmp\n");
    printf("LABEL $while_substr\n");
    printf("LT LF@tmp LF@param2 LF@param3\n");
    printf("JUMPIFNEQ $end_while_substr LF@tmp bool@true\n");
    printf("GETCHAR LF@tmp LF@param1 LF@param2\n");
    printf("CONCAT LF@\cret LF@tmp\n", '%');
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
    printf("LABEL $ord\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@\cret\n", '%');
    printf("MOVE LF@\cret nil@nil\n", '%');
    printf("DEFVAR LF@param1\n");
    printf("MOVE LF@param1 LF@\c1\n", '%');
    printf("DEFVAR LF@param2\n");
    printf("MOVE LF@param2 LF@\c2\n", '%');
    printf("STRI2INT LF@\cret LF@param1 LF@param2\n", '%');
    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL end_$ord\n");
    printf("\n");

    //Definice vestavene funkce chr
    printf("#def chr()\n");
    printf("JUMP end_$chr\n");
    printf("LABEL $chr\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@\cret\n", '%');
    printf("MOVE LF@\cret nil@nil\n", '%');
    printf("DEFVAR LF@param1\n");
    printf("MOVE LF@param1 LF@\c1\n", '%');
    printf("INT2CHAR LF@\cret LF@param1\n", '%');
    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL end_$chr\n");
    printf("\n");
}