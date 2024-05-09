#define _CRTDBG_MAP_ALLOC

#include <stdio.h>
#include <crtdbg.h>
#include <string.h>
#include <stdlib.h>
#include "General.h"
#include "Date.h"

#include "CompanyFile.h"
#include "fileHelper.h"
#include "Macros.h"

int initCompanyFromFile(Company* pComp, const char* fileName, int choice)
{
	FILE* fp;
	char temp[MAX_STR_LEN];
	pComp->sortType = eNotSorted; 
	
	if (!choice) {
		fp = fopen(fileName, "r");
		CHECK_PTR_RETURN_ZERO(fp);
		myGets(temp, MAX_STR_LEN, fp);
		pComp->name = getDynStr(temp);
		if (fscanf(fp, "%d", &pComp->pFM.paymentPerMeet) != 1) return 0;
		if (fscanf(fp, "%d", &pComp->meetsCount) != 1) return 0;
		if (fscanf(fp, "%d", &pComp->placesCount) != 1) return 0;
		if (!readcompanyFromFile (fp, pComp, readMeetFromTxtFile, readPlaceFromTxtFile)) return 0;
	}
	else {
		fp = fopen(fileName, "rb");
		CHECK_PTR_RETURN_ZERO(fp);
		pComp->name = readStringFromFile(fp, "error reading company name!");
		if (!readIntFromFile(&pComp->pFM.paymentPerMeet, fp, "error reading payment pet meet")) return 0;
		if (fread(&pComp->meetsCount, sizeof(int), 1, fp) != 1)
			return 0;
		if (fread(&pComp->placesCount, sizeof(int), 1, fp) != 1)
			return 0;
		if (!readcompanyFromFile(fp, pComp, readMeetFromBinary, readPlaceFromBinary)) return 0;
	}

	fclose(fp);
	return 1;
}

int writeCompanyToBinary(const char* binFileName, const Company* company)
{
	FILE* fp = fopen(binFileName, "wb");
	CHECK_PTR_RETURN_ZERO(fp);

	if (!writeStringToFile(company->name, fp, "error writing companys name to File")) 
		CLOSE_RETURN_ZERO(fp);
	
	if (!writeIntToFile(company->pFM.paymentPerMeet, fp, "erroe writing payment per meet to File")) 
		CLOSE_RETURN_ZERO(fp);

	if (!writeIntToFile(company->meetsCount, fp, "erroe writing meets count to File")) 
		CLOSE_RETURN_ZERO(fp);

	if (!writeIntToFile(company->placesCount, fp, "erroe writing places count to File")) 
		CLOSE_RETURN_ZERO(fp);

	if (!writePlacesArrToBinary(fp, company->placeArr, company->placesCount)) //all places
		CLOSE_RETURN_ZERO(fp);

	if (!writeMeetsArrToBinary(fp, company->meetsArr, company->meetsCount)) //all meets
		CLOSE_RETURN_ZERO(fp);

	CLOSE_RETURN_ONE(fp);
}

int writecompanyToTxtFile(const char* txtFileName, const Company* company)
{
	FILE*  fp = fopen(txtFileName, "w");
	CHECK_PTR_RETURN_ZERO(fp);

	fprintf(fp, "%s\n", company->name);
	fprintf(fp, "%d\n", company->pFM.paymentPerMeet);
	fprintf(fp, "%d\n", company->meetsCount);
	fprintf(fp, "%d\n", company->placesCount);
	if(!writePlacesArrToTxtFile(fp, company->placeArr, company->placesCount)) return 0;
	if(!writeMeetsArrToTXT(fp, company->meetsArr, company->meetsCount)) return 0;

	CLOSE_RETURN_ONE(fp);
}

int writeMeetsArrToBinary(FILE* fp, const Meet** meetsArr, int countMeets) {
	for (int i = 0; i < countMeets; i++) {
		if (!writeMeetToBinary(fp, meetsArr[i])) return 0;
	}
	return 1;
}

int writePlacesArrToBinary(FILE* fp, const Place* PlacesArr, int countPlaces) {
	for (int i = 0; i < countPlaces; i++) {
		if (!writePlaceToBinary(fp, &PlacesArr[i])) return 0;
	}
	return 1;
}

int writeMeetsArrToTXT(FILE* fp, const Meet** meetsArr, int countMeets) {
	for (int i = 0; i < countMeets; i++) {
		if (!writeMeetToTxt(fp, meetsArr[i])) return 0;
	}
	return 1;
}

int writePlacesArrToTxtFile(FILE* fp, const Place* PlacesArr, int countPlaces) {
	for (int i = 0; i < countPlaces; i++) {
		if (!writePlaceToTxtFile(fp, &PlacesArr[i])) return 0;
	}
	return 1;
}
Meet** readMeetsArr(FILE* fp, Place* placeArr, int placeCount, int meetCount, int(*readmeet)(FILE*, Meet*, Place*, int))
{
	Meet** meetArr;

	meetArr = (Meet**)malloc(sizeof(Meet*) * meetCount);
	CHECK_PTR_PRINT_RETURN_NULL(meetArr, "error ellocating");
	
	for (int i = 0; i < meetCount; i++) {
		meetArr[i] = (Meet*)calloc(1, sizeof(Meet));
		if (meetArr[i] == NULL) {
			return NULL;
		}
		if (!readmeet(fp, meetArr[i], placeArr, placeCount)) return 0;
	}
	return meetArr;
}

Place* readPlacesArr(FILE* fp, int placeCount, int(*readplace)(FILE*, Place*))
{
	Place* placrArr;
	placrArr = (Place*)malloc(sizeof(Place) * placeCount);
	CHECK_PTR_PRINT_RETURN_NULL(placrArr, "error ellocating");

	for (int i = 0; i < placeCount; i++) {
		if (!readplace(fp, &placrArr[i])) {
			return NULL;
		}
	}

	return placrArr;
}

int readcompanyFromFile(FILE* fp, Company* pComp, int(*readMeet)(FILE*, Meet*, Place*, int), int(*readPlace)(FILE*, Place*))
{
	pComp->placeArr = readPlacesArr(fp, pComp->placesCount, readPlace);
	CHECK_CLOSE_RETURN_ZERO(pComp->placeArr, fp);

	pComp->meetsArr = readMeetsArr(fp, pComp->placeArr, pComp->placesCount, pComp->meetsCount, readMeet);
	CHECK_CLOSE_RETURN_ZERO(pComp->meetsArr, fp);

	return 1;
}

