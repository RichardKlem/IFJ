CC=gcc
INCLUDE=-Iscanner/ -Itests/ -Iparser/ -I./
OBJ = IFJ_error.o IFJ_stack.o tests/IFJ_scanner_test_app.o scanner/IFJ_scanner.o
SRC_C=main.c IFJ_error.c IFJ_stack.c symtable.c parser/IFJ_parser.c scanner/IFJ_scanner.c IFJ_stack_semantic.c

all: 
	gcc $(INCLUDE) $(SRC_C) -o compiler

test_app: $(OBJ)
	$(CC) -o scanner_test_app $(OBJ)
	make clean
	mv scanner_test_app tests/scanner_test_app

clean:
	rm -rf $(OBJ) infer-out/ compiler
