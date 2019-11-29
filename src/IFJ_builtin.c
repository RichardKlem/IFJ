//vestavene funkce
#include "IFJ_builtin.h"
#include "symtable.h"
#include <stdio.h>
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
    printf("LABEL $inputi\n");
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
    printf("LABEL $inputf\n");
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
    printf("LABEL $inputs\n");
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
    printf("LABEL $print\n");
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
    printf("LABEL $len\n");
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
    printf("LABEL $substr\n");
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
    printf("DEFVAR LF$tmp\n");
    printf("LABEL $while_substr\n");
    printf("LT LF@tmp LF@param2 LF@param3\n");
    printf("JUMPIFNEQ $end_while_substr LF@tmp bool@true\n");
    printf("GETCHAR LF@tmp LF@param1 LF@param2\n");
    printf("CONCAT LF@%cret LF@tmp\n", '%');
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
    printf("LABEL $chr\n");
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
    printf("DEFVAR LF@param1\n");
    printf("POP LF@param1\n");
    printf("DEFVAR LF@param2\n");
    printf("POP LF@param2\n");
    printf("DEFVAR LF@param3\n");
    printf("POP LF@param3\n");
    printf("DEFVAR LF@param2$type\n");
    printf("TYPE LF@param2$type LF@param2\n");
    printf("DEFVAR LF@param3$type\n");
    printf("TYPE LF@param3$type LF@param3\n");
    printf("JUMPIFEQ $operation_plus LF@param1 string@+\n");
    printf("JUMPIFEQ $operation_min LF@param1 string@-\n");
    printf("JUMPIFEQ $operation_mul LF@param1 string@*\n");
    printf("JUMPIFEQ $operation_div LF@param1 string@/\n");
    printf("JUMPIFEQ $operation_intdiv LF@param1 string@//\n");
    printf("JUMPIFEQ $operation_< LF@param1 string@<\n");
    printf("JUMPIFEQ $operation_> LF@param1 string@>\n");
    printf("JUMPIFEQ $operation_== LF@param1 string@==\n");
    printf("JUMPIFEQ $operation_!= LF@param1 string@!=\n");
    printf("JUMPIFEQ $operation_<= LF@param1 string@<=\n");
    printf("JUMPIFEQ $operation_>= LF@param1 string@>=\n");
    printf("LABEL $operation_plus\n");
    printf("");
    printf("LABEL $operation_min\n");
    printf("LABEL $operation_mul\n");
    printf("LABEL $operation_div\n");
    printf("LABEL $operation_intdiv\n");

    //<
    printf("LABEL $operation_<\n");
    printf("DEFVAR LF@tmp_bool\n");
    printf("LT LF@tmp_bool LF@param2 LF@param3\n");
    printf("JUMPIFEQ $operation_<_false LF@tmp_bool bool@false\n");
    //true
    printf("PUSH int@1\n");
    printf("JUMP $end_do_operation\n");
    printf("LABEL $operation_<_false\n");
    //false
    printf("PUSH int@0\n");
    printf("JUMP $end_do_operation\n");

    //>
    printf("LABEL $operation_>\n");
    printf("DEFVAR LF@tmp_bool\n");
    printf("GT LF@tmp_bool LF@param2 LF@param3\n");
    printf("JUMPIFEQ $operation_>_false LF@tmp_bool bool@false\n");
    //true
    printf("PUSH int@1\n");
    printf("JUMP $end_do_operation\n");
    printf("LABEL $operation_>_false\n");
    //false
    printf("PUSH int@0\n");
    printf("JUMP $end_do_operation\n");

    //==
    printf("LABEL $operation_==\n");
    printf("DEFVAR LF@tmp_bool\n");
    printf("EQ LF@tmp_bool LF@param2 LF@param3\n");
    printf("JUMPIFEQ $operation_==_false LF@tmp_bool bool@false\n");
    //true
    printf("PUSH int@1\n");
    printf("JUMP $end_do_operation\n");
    printf("LABEL $operation_==_false\n");
    //false
    printf("PUSH int@0\n");
    printf("JUMP $end_do_operation\n");

    //!=
    printf("LABEL $operation_!=\n");
    printf("DEFVAR LF@tmp_bool\n");
    printf("EQ LF@tmp_bool LF@param2 LF@param3\n");
    printf("JUMPIFNEQ $operation_!=_false LF@tmp_bool bool@false\n");
    //true
    printf("PUSH int@1\n");
    printf("JUMP $end_do_operation\n");
    printf("LABEL $operation_!=_false\n");
    //false
    printf("PUSH int@0\n");
    printf("JUMP $end_do_operation\n");
    
    //<=
    printf("LABEL $operation_<=\n");
    printf("DEFVAR LF@tmp_bool\n");
    printf("GT LF@tmp_bool LF@param2 LF@param3\n");
    printf("JUMPIFNEQ $operation_<=_false LF@tmp_bool bool@false\n");
    //true
    printf("PUSH int@1\n");
    printf("JUMP $end_do_operation\n");
    printf("LABEL $operation_<=_false\n");
    //false
    printf("PUSH int@0\n");
    printf("JUMP $end_do_operation\n");

    //>=
    printf("LABEL $operation_>=\n");
    printf("DEFVAR LF@tmp_bool\n");
    printf("LT LF@tmp_bool LF@param2 LF@param3\n");
    printf("JUMPIFNEQ $operation_>=_false LF@tmp_bool bool@false\n");
    //true
    printf("PUSH int@1\n");
    printf("JUMP $end_do_operation\n");
    printf("LABEL $operation_>=_false\n");
    //false
    printf("PUSH int@0\n");
    printf("JUMP $end_do_operation\n");

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
    printf("MOVE TF@%c1 %s\n", '%', "arg");
    printf("CALL $print\n");
}

void call_len(char* arg)
{
    printf("CREATEFRAME\n");
    printf("DEFVAR TF@%c1\n", '%');
    printf("MOVE TF@%c1 %s\n", '%', "arg");
    printf("CALL $len\n");
}

void call_substr(char* arg1, char* arg2, char* arg3)
{
    printf("CREATEFRAME\n");
    printf("DEFVAR TF@%c1\n", '%');
    printf("MOVE TF@%c1 %s\n", '%', "arg1");
    printf("DEFVAR TF@%c2\n", '%');
    printf("MOVE TF@%c2 %s\n", '%', "arg2");
    printf("DEFVAR TF@%c3\n", '%');
    printf("MOVE TF@%c3 %s\n", '%', "arg3");
    printf("CALL $substr\n");
}

void call_ord(char* arg1, char* arg2)
{
    printf("CREATEFRAME\n");
    printf("DEFVAR TF@%c1\n", '%');
    printf("MOVE TF@%c1 %s\n", '%', "arg1");
    printf("DEFVAR TF@%c2\n", '%');
    printf("MOVE TF@%c2 %s\n", '%', "arg2");
    printf("CALL $ord\n");
}

void call_chr(char* arg)
{
    printf("CREATEFRAME\n");
    printf("DEFVAR TF@%c1\n", '%');
    printf("MOVE TF@%c1 %s\n", '%', "arg");
    printf("CALL $chr\n");
}

void print_stack(tStack *sem_stack)
{
    while (!stackEmpty(&sem_stack)) {
        act_token = stackPop(&sem_stack);
        switch (act_token.type) {
            case TOKEN_INT:
                printf("PUSH int@%d\n", act_token.value.int_value);
            case TOKEN_DOUBLE:
                printf("PUSH float@%a\n", act_token.value.double_value);
            case TOKEN_STRING:
                printf("PUSH string@%s\n", act_token.value.string);
            case TOKEN_ID:
                int frame = getframe(act_token.value.string);
                if (frame == 1) {
                    printf("PUSH GF@%s\n", act_token.value.string);
                } else {
                    printf("PUSH LF@%s\n", act_token.value.string);
                }

            case TOKEN_KEYWORD:
                if (act_token.value.keyword_value == NONE) {
                    printf("PUSH nil@nil\n");
                } else {
                    error_exit(ERROR_SYNTAX); //dostanu jiný KEYWORD token než NONE
                }

            case TOKEN_MATH_PLUS:
                printf("PUSH string@+\n");
                printf("CALL $do_operation\n");
            case TOKEN_MATH_MINUS:
                printf("PUSH string@-\n");
                printf("CALL $do_operation\n");
            case TOKEN_MATH_MUL:
                printf("PUSH string@*\n");
                printf("CALL $do_operation\n");
            case TOKEN_MATH_DIV:
                printf("PUSH string@/\n");
                printf("CALL $do_operation\n");
            case TOKEN_MATH_INT_DIV:
                printf("PUSH string@//\n");
                printf("CALL $do_operation\n");

            case TOKEN_LESS:
                printf("PUSH string@<\n");
                printf("CALL $do_operation\n");
            case TOKEN_LESS_EQ:
                printf("PUSH string@<=\n");
                printf("CALL $do_operation\n");
            case TOKEN_GREATER:
                printf("PUSH string@>\n");
                printf("CALL $do_operation\n");
            case TOKEN_GREATER_EQ:
                printf("PUSH string@>=\n");
                printf("CALL $do_operation\n");
            case TOKEN_EQ:
                printf("PUSH string@==\n");
                printf("CALL $do_operation\n");
            case TOKEN_NOT_EQ:
                printf("PUSH string@!=\n");
                printf("CALL $do_operation\n");

            default:
                error_exit(ERROR_SYNTAX);
        }
    }
}
