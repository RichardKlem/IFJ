CC=gcc
INCLUDE=
OBJ = IFJ_error.o IFJ_stack.o tests/IFJ_scanner_test_app.o scanner/IFJ_scanner.o
SRC_C=main.c IFJ_error.c IFJ_stack.c symtable.c IFJ_parser.c IFJ_scanner.c IFJ_stack_semantic.c IFJ_precedence_DLL.c IFJ_precedence_syntactic_analysis.c IFJ_precedence_table.c IFJ_stack_string.c

all: 
	gcc $(INCLUDE) $(SRC_C) -o compiler

test_app: $(OBJ)
	$(CC) -o scanner_test_app $(OBJ)
	make clean
	mv scanner_test_app tests/scanner_test_app

clean:
	rm -rf $(OBJ) infer-out/ compiler
