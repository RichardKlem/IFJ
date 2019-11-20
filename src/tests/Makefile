src_c=IFJ_scanner_test_app.c ../scanner/IFJ_scanner.c ../IFJ_error.c ../IFJ_stack.c
src_h=../scanner/IFJ_scanner.h ../IFJ_error.h ../IFJ_stack.h
lib_include=-I../ -I../scanner/

all: test_app

test_app: ${src_c} ${src_h}
	gcc ${src_c} ${lib_include} -o scanner_test_app

clean:
	rm -f scanner_test_app
