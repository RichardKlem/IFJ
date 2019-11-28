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
                                            GT GF@tmp GF@tmp2 GF@tmp3
                                            JUMPIFEQ end_while GF@tmp bool@false
                                            GETCHAR tmp retezec GF@tmp2
                                            CONCAT a GF@tmp
                                            ADD GF@tmp2 GF@tmp2 int@1
                                            JUMP wjile
                                            LABEL end_while
    
    a = ord(řetězec. pozice)    STRI2INT a řetězec pozice

    a = chr(ASCII_kod)      INT2CHAR a ASCII_kod


*/