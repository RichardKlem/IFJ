//vestavene funkce

#ifndef IFJ_BUILTIN_H
#define IFJ_BUILTIN_H


void generate_builtin();
void call_inputi();
void call_inputf();
void call_inputs();
void call_write(char* arg);
void call_len(char* arg);
void call_substr(char* arg1, char* arg2, char* arg3);
void call_ord(char* arg1, char* arg2);
void call_chr(char* arg);
//void print_stack(tStack *sem_stack);

#endif //IFJ_BUILTIN_H
