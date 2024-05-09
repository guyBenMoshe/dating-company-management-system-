#define _CRTDBG_MAP_ALLOC

#include <stdio.h>
#include <crtdbg.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "Place.h"
#include "General.h"
#include "fileHelper.h"

int initPlace(Place* pPlace, Place* placeArr, int placeCount)
{
	if (!getPlaceName(pPlace)) return 0;
	pPlace->type = getPlaceType();
	setAddress(placeArr, pPlace, placeCount);
	return 1;
}

ePLaceType getPlaceType()
{
	int option;
	do {
		printf("Please enter one of the following types\n");
		for (int i = 0; i < eNofPlaceTypes; i++)
			printf("%d for %s\n", i, PlaceTypeStr[i]);
		scanf("%d", &option);
	} while (option < 0 || option >= eNofPlaceTypes);
	getchar();
	return (ePLaceType)option;
}

void setAddress(Place* placeArr, Place* pPlace, int placeCount ) {
		while (1) {

			getCorrectAddress(&pPlace->address);

			if (checkUniqAddress(&pPlace->address, placeArr,placeCount))//check if uniq
				break;

			printf("This address is already in use\n");
		}
}

int checkUniqAddress(Address* address, Place* placeArr, int placeCount) {
	for (int i = 0; i < placeCount; i++) {
		if(compareAddresses(&placeArr[i].address, address) == 0)return 0;
	}
	return 1;
}

int getPlaceName(Place* pPlace)
{
	char temp[255];
	char** wordsArray = NULL;
	int totalLength = 0;
	int count = 0;

	while (count == 0)
	{
		printf("Enter Place name\n");
		myGets(temp, MAX_STR_LEN, stdin);
		wordsArray = splitCharsToWords(temp, &count, &totalLength);
	}
	pPlace->name = (char*)calloc(totalLength + 2 + count, sizeof(char));
	if (!pPlace->name)
		return 0;

	changePlaceName(pPlace->name, count, wordsArray);
	pPlace->name[totalLength+count] = GERESH_CHAR;

	//clean temp data
	for (int i = 0; i < count; i++)
		free(wordsArray[i]);
	free(wordsArray);
	return 1;
}

void changePlaceName(char* name, int count, char** wordsArray)
{
	name[0] = GERESH_CHAR;

	for (int i = 0; i < count; i++)
	{
		wordsArray[i][0] = toupper(wordsArray[i][0]);
		strcat(name, wordsArray[i]);
		if (i != count - 1) //not last
		{
			strcat(name, " ");
		}
	}
}

int comparePlacesByAddress(const void* p1, const void* p2) {
	const Place* place1 = (const Place*)p1;
	const Place* place2 = (const Place*)p2;
	return compareAddresses(&place1->address, &place2->address);
}

void printPlace(const void* p)
{
	const Place* place = (const Place*)p;
	printf("Name:%-10s, type: %-10s	", place->name, PlaceTypeStr[place->type]);
	printAddress(&place->address);
}


int writePlaceToBinary(FILE* fp, const Place* thePlace)
{
	ePLaceType type = thePlace->type;
	char* name = thePlace->name;

	if (!writeStringToFile(name, fp, "error writing name to file")) //name
		return 0;

	if (fwrite(&thePlace->address, sizeof(Address), 1, fp) != 1)//address
		return 0;

	if (fwrite(&type, sizeof(int), 1, fp) != 1)//place type
		return 0;
	return 1;
}

int writePlaceToTxtFile(FILE* fp, const Place* thePlace)
{
	fprintf(fp, "%s\n", thePlace->name);
	writeAddressToTXT(fp, &thePlace->address);
	fprintf(fp, "%d\n",thePlace->type);
	return 1;
}

int readPlaceFromTxtFile(FILE* fp, Place* thePlace)
{
	char name[MAX_STR_LEN] = { 0 };

	if (!myGets(name, MAX_STR_LEN, fp)) return 0;//name
	if(!readAddressFromTXT(fp, &thePlace->address))return 0;//address
	if (fscanf(fp, "%d\n", &thePlace->type) != 1) return 0;// place type

	thePlace->name = _strdup(name);
	if (!thePlace->name) return 0;

	return 1;
}

int readPlaceFromBinary(FILE* fp, Place* thePlace)
{
	thePlace->name = readStringFromFile(fp, "error reading place name"); //name

	if (fread(&thePlace->address, sizeof(Address), 1, fp) != 1)//address
		return 0;
	if (fread(&thePlace->type, sizeof(int), 1, fp) != 1)//plan type
		return 0;
	return 1;
}


Place* findPlaceByAddress(Place* placeArr, Address* theAddress,int countPlaces) {
	for (int i = 0; i < countPlaces; i++) {
		if (compareAddresses(&placeArr[i].address, theAddress) == 0) return &placeArr[i];
	}
	return NULL;
}

void freePlace(void* p) {
	Place* place = (Place*)p;
	free(place->name);
}