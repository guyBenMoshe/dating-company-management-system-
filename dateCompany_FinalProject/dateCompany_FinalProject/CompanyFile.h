#pragma once
#include "Company.h"

int		initCompanyFromFile(Company* company, const char* fileName, int choice);
int		writeCompanyToBinary(const char* fp,const Company* company);
int		writecompanyToTxtFile(const char* fp, const Company* company);
int		readcompanyFromFile(FILE* fp, Company* pComp, int(*readMeet)(FILE*, Meet*, Place*, int), int(*readPlace)(FILE*, Place*));
Meet**	readMeetsArr(FILE* fp, Place* placeArr, int placeCount, int meetCount, int(*readmeet)(FILE*, Meet*, Place*, int));
Place*	readPlacesArr(FILE* fp, int placeCount, int(*readplace)(FILE*, Place*));
int		writeMeetsArrToBinary(FILE* fp, const Meet** meetsArr, int countMeets);
int		writePlacesArrToBinary(FILE* fp, const Place* PlacesArr, int countPlaces);
int		writeMeetsArrToTXT(FILE* fp, const Meet** meetsArr, int countMeets);
int		writePlacesArrToTxtFile(FILE* fp, const Place* PlacesArr, int countPlaces);

