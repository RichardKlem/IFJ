CC=gcc
CFLAGS=-I. -Iscanner/ -Itests
OBJ = IFJ_error.o IFJ_stack.o tests/IFJ_scanner_test_app.o scanner/IFJ_scanner.o
scanner_test_app: $(OBJ)
	$(CC) -o scanner_test_app $(OBJ)
	make clean
	mv scanner_test_app tests/scanner_test_app

clean:
	rm $(OBJ)