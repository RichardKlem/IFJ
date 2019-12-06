CC=gcc
SRC_C=main.c IFJ_error.c IFJ_stack.c symtable.c IFJ_parser.c IFJ_scanner.c IFJ_stack_semantic.c IFJ_precedence_DLL.c IFJ_precedence_syntactic_analysis.c IFJ_precedence_table.c IFJ_stack_string.c IFJ_builtin.c c204.c

all: 
	gcc $(INCLUDE) $(SRC_C) -o compiler

clean:
	rm -rf $(OBJ) infer-out/ compiler

test: parser

parser:
	cd tests/parser_test_examples && python3 compiler-tester.py

zip:
	zip xberan46.zip *.c *.h rozdeleni tests/ Makefile

