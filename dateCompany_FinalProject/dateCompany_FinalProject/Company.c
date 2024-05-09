#define _CRTDBG_MAP_ALLOC

#include <stdio.h>
#include <crtdbg.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "Company.h"
#include "General.h"
#include "Macros.h"

int initCompany(Company* company)
{
	company->name = getStrExactName("Company name ---> ");
	company->meetsArr = NULL;
	company->placeArr = NULL;
	company->placesCount = 0;
	company->meetsCount = 0;
	company->sortType = eNotSorted;
	if(!initFinancialManagement(&company->pFM)) return 0;

	return 1;
}

int addMeet(Company* pComp, PeopleManager* pManager)
{
	if (pManager->countClient < 2)
		PRINT_RETURN_ZERO("There are not enough clients to set a meet");

	if (!atLeastOneValidMatchByGender(&pManager->L_Clients, pManager->countClient)) 
		PRINT_RETURN_ZERO("There are not enough suitable people for set a meet");
	
	if (pManager->countMatchMaker == 0) 
		PRINT_RETURN_ZERO("There is no Match Makers in company");
	
	if (pComp->placesCount == 0) 
		PRINT_RETURN_ZERO("There is no place to set a meeting");

	Meet* pMEET = (Meet*)calloc(1, sizeof(Meet));

	CHECK_PTR_RETURN_ZERO(pMEET);

	Place* thePlaCe = choosePlace(pComp);
	printAllClients(pManager);
	printAllMatchMakers(pManager);
	initMeet(pMEET, thePlaCe, pManager);

	pComp->meetsArr = (Meet**)realloc(pComp->meetsArr, (pComp->meetsCount + 1) * sizeof(Meet*));

	CHECK_PTR_FREE_RETURN_ZERO(pComp->meetsArr,pMEET);

	pComp->meetsArr[pComp->meetsCount] = pMEET;
	pComp->meetsCount++;
	return 1;
}

int addPlace(Company* pComp)
{
	pComp->placeArr = (Place*)realloc(pComp->placeArr, (pComp->placesCount + 1) * sizeof(Place));
	CHECK_PTR_RETURN_ZERO(pComp->placeArr);
	initPlace(&pComp->placeArr[pComp->placesCount], pComp->placeArr, pComp->placesCount);
	pComp->placesCount++;

	return 1;
}

Place* choosePlace(Company* pComp)
{
	int index;

	printPlaceArr(pComp->placeArr, pComp->placesCount,1);

	do {
		printf("Choose a place from list, type its index\n");
		scanf("%d", &index);
	} while (index > pComp->placesCount || index < 0);
	
	return &pComp->placeArr[index-1];
}

void sortMeet(Company* pComp)
{
	pComp->sortType = getMeetSortType();
	int(*compare)(const void* air1, const void* air2) = NULL;

	switch (pComp->sortType)
	{
	case eByDate:
		compare = compareMeetByDate;
		break;
	case eByMatchMaker:
		compare = compareMeetByMatchMaker;
		break;
	case eByPlace:
		compare = compareMeetByPlace;
		break;
	}

	if (compare != NULL)
		qsort(pComp->meetsArr, pComp->meetsCount, sizeof(Meet*), compare);
}

void findMeet(const Company* company, const PeopleManager* manager)
{
	Meet** pMeet = NULL;
	Meet* tmpMeet = (Meet*)malloc(sizeof(Meet));
	Place* tmpP;

	CHECK_PTR_PRINT_RETURN(tmpMeet,"error ellocating place in memory");
	
	switch (company->sortType)
	{
	case eNotSorted:
		printf("The search cannot be performed, array not sorted\n");
		return;

	case eByDate:
		printf("date:");
		getCorrectDate(&tmpMeet->date);
		pMeet = bsearch(&tmpMeet, company->meetsArr, company->meetsCount, sizeof(Meet*), compareMeetByDate);
		break;

	case eByMatchMaker:
		printf("Match maker SN: ");
		getMM_SN(&tmpMeet->matchMakerSN);
		pMeet = bsearch(&tmpMeet, company->meetsArr, company->meetsCount, sizeof(Meet*), compareMeetByMatchMaker);
		break;

	case eByPlace:
			tmpP = (Place*)malloc(sizeof(Place));
			CHECK_PTR_PRINT_BREACK(tmpP, "error allocating tmp place");
			getCorrectAddress(&tmpP->address);
			tmpMeet->thePlace = tmpP;
			pMeet = bsearch(&tmpMeet, company->meetsArr, company->meetsCount, sizeof(Meet*), compareMeetByPlace);
		break;

	default:
		break;
	}

	if (!pMeet) {
		printf("Meet was not found");
		printf("\n");
	}

	else {
		printf("Meet found,");
		printMeet(*pMeet, manager);
		printf("\n");
		free(tmpMeet);
	}
}

eSortType getMeetSortType()
{
	int option;
	printf("\n\n");
	printf("Base on what field do you want to sort ? \n");

	do {
		for (int i = 0; i < eNofSortTypes - 1; i++)
			printf("Enter %d for %s\n", i + 1, SortTypeStr[i + 1]);
		scanf("%d", &option);
	} while (option < 0 || option >= eNofSortTypes);
	getchar();
	return (eSortType)option;
}

void printCompany(const Company* pComp, const PeopleManager* pManager)
{
	
		printf("company %s\n", pComp->name);
		printf("\n -------- Has %d Meets\n", pComp->meetsCount);
		printMeetsArr(pComp->meetsArr, pComp->meetsCount, pManager);
		printf("\n\n -------- Has %d Places to go on a date\n", pComp->placesCount);
		printPlaceArr(pComp->placeArr, pComp->placesCount,0);
}

void printMeetsArr(const Meet** meetArr,const int count, const PeopleManager* pManager)
{
	for (int i = 0; i < count; i++) {
		printMeet(meetArr[i], pManager);
	}
}
void printPlaceArr(Place* placesArr, int placeCount, int isPrintWithIndex)
{
	if (isPrintWithIndex) {
	for (int i = 0; i < placeCount; i++) {
		printf("%d) ", i+1);
		printPlace(&placesArr[i]);
		}
	} else
		generalArrayFunction(placesArr, placeCount, sizeof(Place), (void*)printPlace);
}


void freeMeetsArr(Meet** meetsArr, int meetCount)
{
	generalArrayFunction(meetsArr, meetCount, sizeof(Meet*), (void*)freeMeet);
}

void freePlacesArr(Place* placesArr, int placeCount)
{
	generalArrayFunction(placesArr, placeCount, sizeof(Place), (void*)freePlace);
}

void freeCmopany(Company* company)
{	
	freeMeetsArr(company->meetsArr, company->meetsCount);
	freePlacesArr(company->placeArr, company->placesCount);
	free(company->meetsArr);
	free(company->placeArr);
	free(company->name);
}
