/* ******************************* IFJ_scanner.c ****************************
 *  Predmet: Formalni jazyky a prekladace (IFJ) - FIT VUT v Brne
 *  Nazev projektu: Implementace prekladace imperativniho jazyka IFJ19
 *  Autori: Beranek Tomas (xberan46) - vedouci
 *          Haderka Martin (xhader00)
 *          Slobodnik Simon (xslobo06)
 *          Klem Richard (xklemr00)
 *  Datum vytvoreni: 12.10.2019
 * ************************************************************************** */

#ifndef IFJ_INSTGEN_H
#define IFJ_INSTGEN_H

typedef enum
{
    MOVE,
    CREATEFRAME,
    PUSHFRAME,
    POPFARME,
    DEFVAR,
    CALL,
    RETURN,
    PUSHS,
    POPS,
    CLEARS,
    ADD,
    SUB,
    MUL,
    DIV,
    IDIV,
    ADDS,
    SUBS,
    MULS,
    DIVS,
    IDIVS,
    LT,
    GT,
    EQ,
    LTS,
    GTS,
    EQS,
    AND,
    OR,
    NOT,
    ANDS,
    ORS,
    NOTS,
    INT2FLOAT,
    FLOAT2INT,
    INT2CHAR,
    STRI2INT,
    INT2FLOATS,
    FLOAT2INTS,
    INT2CHARS,
    STRI2INTS,
    READ,
    WRITE,
    CANCAT,
    STRLEN,
    GETCHAR,
    SETCHAR,
    CONCAT,
    TYPE,
    LABEL,
    JUMP,
    JUMPIFEQ,
    JUMPIFNEQ,
    JUMPIFEQS,
    JUMPIFNEQS,
    EXIT,
    BREAK,
    DPRINT,
    COMMENT,
    HEADER
} enInstruction;

typedef enum
{
    GF,
    LF,
    TF
} tFrame;

typedef enum
{
    DT_INT,
    DT_DOUBLE,
    DT_STRING,
    DT_BOOL,
    DT_NIL,
    DT_ID
} tDatTyp;

typedef struct
{
    //TODO
    tFrame frame;
    char* value;
    tDatTyp datTyp;
} tArgument;

typedef struct
{
    enInstruction instType;
    char* arg1;
    char* arg2;
    char* arg3;
} tInstruction;

typedef struct tDLElem {
    tInstruction instr;
    struct tDLElem *lptr;
    struct tDLElem *rptr;
} *tDLElemPtr;

typedef struct {
    tDLElemPtr First;
    tDLElemPtr Act;
    tDLElemPtr Last;
} tDLList;
/* funkce pro spracovani instrukci*/
void generateInst(tDLElemPtr *List, enInstruction instType, char *arg1, char *arg2, char *arg3);
void writeInstruction(tDLElemPtr *List, enInstruction insType, tArgument *arg1, tArgument *arg2, tArgument *arg3);
void printInst(tDLList *List);
/* funkce dvousmerne vazaneho seznamu */
void DLInitList (tDLList *);
void DLDisposeList (tDLList *);
void DLInsertFirst (tDLList *, tInstruction);
void DLInsertLast(tDLList *, tInstruction);
void DLFirst (tDLList *);
void DLLast (tDLList *);
void DLCopyFirst (tDLList *, tInstruction *);
void DLCopyLast (tDLList *, tInstruction *);
void DLDeleteFirst (tDLList *);
void DLDeleteLast (tDLList *);
void DLPostDelete (tDLList *);
void DLPreDelete (tDLList *);
void DLPostInsert (tDLList *, tInstruction);
void DLPreInsert (tDLList *, tInstruction);
void DLCopy (tDLList *, tInstruction *);
void DLActualize (tDLList *, tInstruction);
void DLSucc (tDLList *);
void DLPred (tDLList *);
int DLActive (tDLList *);
#endif //IFJ_INSTGEN_H
