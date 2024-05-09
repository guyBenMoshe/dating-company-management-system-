#pragma once
#include "General.h"

typedef struct {
	char city[MAX_STR_LEN];
	char street[MAX_STR_LEN];
	int number;
}Address;

int		getCorrectAddress(Address* pAddress);
void	printAddress(const Address* pAddress);
int		compareAddresses(const void* v1, const void* v2);
int		isSameAddress(Address* a1, Address* a2);
int		getCityNStreet(char* address, char* msg);
void	changeAddressName(char* name, int count, char** wordsArray);
void	writeAddressToTXT(FILE* fp,const Address* theAddress);
int		readAddressFromTXT(FILE* fp, Address* theAddress);
void	freeAddress(Address* pA);
