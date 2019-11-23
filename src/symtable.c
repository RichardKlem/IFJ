#include "symtable.h"
#include "string.h"

int name_to_key(char * name){
    int key = 0;
    while (*name){
        key = key*256 + *name;
        name++;
    }
    return key;
}

void BSTInit (tBSTNodePtr *RootPtr) {
    if (RootPtr == NULL) {
        fprintf(stderr, "ERROR: Do funkce BSTInit byl predan chybny ukazatel"
                        "na RootPtr\n");
        exit(1);
    }

    *RootPtr = NULL;
}

int BSTSearch (tBSTNodePtr RootPtr, char * name, Record *Content)	{
    int K = name_to_key(name);

    if(RootPtr == NULL)
        return 0;
    else {
        if (RootPtr->Key == K)) {
            if (Content != NULL) {
                *Content = RootPtr->BSTNodeCont;
                return 1;
            }
            else {
                fprintf(stderr, "WARNING: Funkce BSTSearch nalezla uzel se zadanym klicem, ale jeho hodnota"
                                "nelze ulozit, protoze Content == NULL !\n");
                return 1;
            }
    }
	if (RootPtr->Key < K)
        return BSTSearch(RootPtr->RPtr, K, Content);
	else
	    return BSTSearch(RootPtr->LPtr, K, Content);
    }
}

void BSTInsert (tBSTNodePtr* RootPtr, char * name, Record Content)	{
	int K = name_to_key(name);

	if (RootPtr == NULL){
	    fprintf(stderr, "ERROR: Do funkce BSTInsert byl predan neplatny ukazatel na RootPtr\n");
	    exit(1); //uvnitr teto funkce nelze osetrit zadny mozny memory_leak
	}

	if (*RootPtr == NULL){ //jsme v koncovem uzlu, vytvorime zde novy uzel
        tBSTNodePtr new = (tBSTNodePtr)malloc(sizeof(struct tBSTNode));

        if (new == NULL){
            fprintf(stderr, "ERROR: Chyba alokace uzlu stromu\n");
            BSTDispose(RootPtr);
            exit(1);
        }
		new->Key = K;
		//nastveni hodnot
		new->BSTNodeCont = Content;
		new->LPtr = NULL;
		new->RPtr = NULL;
		*RootPtr = new;
		return;
	}
	else{
		if ((*RootPtr)->Key == K){ //narazili jsme na uzel se shodnym klicem, aktualizujeme
			(*RootPtr)->BSTNodeCont = Content;
			return;
		}
		else{
			if ((*RootPtr)->Key < K) //pokud je klic v pravem podstromu
				BSTInsert(&(*RootPtr)->RPtr, K, Content);
			else
				BSTInsert(&(*RootPtr)->LPtr, K, Content);
		}
	}
}

void ReplaceByRightmost (tBSTNodePtr PtrReplaced, tBSTNodePtr *RootPtr) {
    if (RootPtr == NULL || *RootPtr == NULL) {
        fprintf(stderr, "Do funkce ReplaByRightMost byl predan chybny ukazatel\n");
        exit(1);
    }

    if ((*RootPtr)->RPtr != NULL) //pokud ma uzel praveho syna
        ReplaceByRightmost(PtrReplaced, &(*RootPtr)->RPtr); //hledame prvek k vymene v pravem podstromu
    else { //nasli jsme nejpravejsi prvek
        PtrReplaced->Key = (*RootPtr)->Key;
        PtrReplaced->BSTNodeCont = (*RootPtr)->BSTNodeCont;
        BSTDelete(RootPtr, (*RootPtr)->Key); //smazeme prvek
    }
}

void BSTDelete (tBSTNodePtr *RootPtr, char * name) {
    int K = name_to_key(name);

    if (RootPtr == NULL) {
        fprintf(stderr, "WARNING: Funkci BTDisposeTree byl predan chybny"
                        "ukazatel na RootPtr\n");
        return; //zde pouze vypisi warning, ale mohlo by byt brano i jako error
    }

    if (*RootPtr == NULL) //dorazili jsme na konec a nic se nenaslo, nedelame nic
        return;

    if ((*RootPtr)->Key > K) {
        BSTDelete(&(*RootPtr)->LPtr, K); //pokracujeme v hledani v levem podstromu
        return;
    }
    else if ((*RootPtr)->Key < K) {
        BSTDelete(&(*RootPtr)->RPtr, K); //pokracujeme v hledani v pravem podstromu
        return;
    }
    else { //nasli jsme uzel ke smazani
        if ((*RootPtr)->LPtr == NULL && (*RootPtr)->RPtr == NULL) { //pokud uzel nema syny
            free(*RootPtr);
            *RootPtr = NULL;
            return;
        }
        else if ((*RootPtr)->LPtr == NULL) { //pokud uzel nema leveho syna
            tBSTNodePtr del = *RootPtr;
            *RootPtr = (*RootPtr)->RPtr; //napojime jeho praveho syna na jeho otce
            free(del);
            return;
        }
        else if ((*RootPtr)->RPtr == NULL) { //pokud uzel nema praveho syna
            tBSTNodePtr del = *RootPtr;
            *RootPtr = (*RootPtr)->LPtr; //napojime jeho leveho syna na jeho otce
            free(del);
            return;
        }
        else { //uzel ma oba syny
            ReplaceByRightmost((*RootPtr), &(*RootPtr)->LPtr); //nahradime hodnotou z leveho podstromu
            return;
        }
    }
}

void BSTDispose (tBSTNodePtr *RootPtr) {
    if (RootPtr != NULL && *RootPtr != NULL) {
        BSTDispose(&((*RootPtr)->LPtr));
        BSTDispose(&((*RootPtr)->RPtr));
        free(*RootPtr);
        BSTInit(RootPtr);
    }
}
