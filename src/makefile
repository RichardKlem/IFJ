CC=gcc
CFLAGS=-I. -Iscanner/ -Itests

a: IFJ_error.o IFJ_stack.o tests/IFJ_scanner_test_app.o scanner/IFJ_scanner.o
	$(CC) -o a IFJ_error.o IFJ_stack.o tests/IFJ_scanner_test_app.o scanner/IFJ_scanner.o