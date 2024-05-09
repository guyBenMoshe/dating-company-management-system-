#pragma once

#include "Meet.h"
#include "FinancialManagement.h"
#include "PeopleManager.h"

typedef enum {
	eNotSorted, eByPlace, eByMatchMaker, eByDate, eNofSortTypes
} eSortType;

static const char* SortTypeStr[eNofSortTypes]
= { "Not Sorted", "Place Address", "Match Maker", "Date" };


typedef struct {
	char*	name;
	Meet**	meetsArr;
	Place*	placeArr;
	int		meetsCount;
	int		placesCount;
	eSortType sortType;
	FinancialManagement	pFM;

}Company;

int		initCompany(Company* company);
int		addMeet(Company* pComp, PeopleManager* pManager);
int		addPlace(Company* company);
Place*	choosePlace(Company* company);
void	sortMeet(Company* company);
void	findMeet(const Company* company, const PeopleManager* pManager);
eSortType getMeetSortType();
void	printCompany(const Company* company, const PeopleManager* pManager);
void	printMeetsArr(const Meet** meetArr, const int size, const PeopleManager* pManager);
void	printPlaceArr(Place* placesArr,int size, int isPrintWithIndex);
void	freeMeetsArr(Meet** meetsArr, int count);
void	freePlacesArr(Place* placesArr, int count);
void	freeCmopany(Company* company);
