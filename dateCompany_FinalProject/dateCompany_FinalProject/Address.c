#define _CRTDBG_MAP_ALLOC

#include <stdio.h>
#include <crtdbg.h>

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "Address.h"
#include "General.h"
#include "Macros.h"

#define SPECIAL_TAV "_"

int getCorrectAddress(Address* pAddress)
{
	char tmpC[MAX_STR_LEN] = { 0 };
	char tmpS[MAX_STR_LEN] = { 0 };
	getCityNStreet(tmpC, "enter City: ");
	getCityNStreet(tmpS, "enter Street: ");
	printf("enter number: \n");
	scanf("%d", &pAddress->number);
	strcpy(pAddress->city, tmpC);
	strcpy(pAddress->street, tmpS);
	return 1;
}


void printAddress(const Address* pAddress)
{
	printf("Addres --> %s, %s, %d\n", &pAddress->city, &pAddress->street, pAddress->number);
}

int compareAddresses(const void* a1, const void* a2)
{
	const Address* address1 = (const Address*)a1;
	const Address* address2 = (const Address*)a2;

	if (strcmp(address1->city, address2->city) != 0) return strcmp(address1->city, address2->city);
	else if (strcmp(address1->street, address2->street) != 0) return strcmp(address1->street, address2->street);
	else return address1->number - address2->number;
}

int	getCityNStreet(char* address, char* msg)
{
	char temp[255];
	char** wordsArray = NULL;
	int totalLength = 0;
	int count = 0;

	while (count == 0)
	{
		puts(msg);
		myGets(temp, MAX_STR_LEN, stdin);
		wordsArray = splitCharsToWords(temp, &count, &totalLength);
	}
	CHECK_PTR_RETURN_ZERO(address);

	changeAddressName(address, count, wordsArray);

	//clean temp data
	for (int i = 0; i < count; i++)
		free(wordsArray[i]);
	free(wordsArray);

	return 1;
}

void changeAddressName(char* name, int count, char** wordsArray)///do generic
{
	for (int i = 0; i < count; i++)
	{
		wordsArray[i][0] = toupper(wordsArray[i][0]);
		strcat(name, wordsArray[i]);
		if (i != count - 1) //not last
		{
			strcat(name, SPECIAL_TAV);
		}
	}
}

void writeAddressToTXT(FILE* fp, const Address* theAddress ) {
	fprintf(fp, "%s\n%s\n%d\n", &theAddress->city, &theAddress->street, theAddress->number);
}

int readAddressFromTXT(FILE* fp, Address* theAddress) {
	char city[MAX_STR_LEN] = { 0 };
	char street[MAX_STR_LEN] = { 0 };

	if (!myGets(city, MAX_STR_LEN, fp)) return 0;//city
	if (!myGets(street, MAX_STR_LEN, fp)) return 0;//street
	if (fscanf(fp, "%d\n", &theAddress->number) != 1) return 0;//number
	strcpy(theAddress->city, city);
	strcpy(theAddress->street, street);
	return 1;
}
int isSameAddress(Address* a1, Address* a2) {

	if (!a1 || !a2)
		return 0;
	if (compareAddresses(a1, a2) != 0 ) return 1;

	return 0;
}

void freeAddress(Address* pA)
{
	free(pA->city);
	free(pA->street);
}
