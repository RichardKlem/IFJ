src_c=../scanner/IFJ_scanner.c ../IFJ_error.c ../IFJ_stack.c ../parser/IFJ_precedence_DLL.c ../parser/IFJ_precedence_table.c ../parser/IFJ_precedence_syntactic_analysis.c
src_h=../scanner/IFJ_scanner.h ../IFJ_error.h ../IFJ_stack.h ../parser/IFJ_precedence_DLL.h ../parser/IFJ_precedence_table.h ../parser/IFJ_precedence_syntactic_analysis.h
lib_include=-I../ -I../scanner/ -I../parser/

all: ${src_c} ${src_h}
	gcc ${src_c} ${lib_include} -o ahoj

