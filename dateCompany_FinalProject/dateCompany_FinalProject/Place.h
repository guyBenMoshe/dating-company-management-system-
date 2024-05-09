#pragma once
#include "Address.h"


#define MAX_PC 9999
#define MIN_PC 1
#define GERESH_CHAR	'"'


typedef enum {
	coffe, eRestaurant,eParty, eNofPlaceTypes
} ePLaceType;

static const char* PlaceTypeStr[eNofPlaceTypes] = { "Coffe shop", "Restaurant", "Party"};


typedef struct {
	Address address;
	char* name;
	ePLaceType type;

}Place;

int			initPlace(Place* pPlace, Place* placeArr, int placeCount);
ePLaceType	getPlaceType();
void		setAddress(Place* placeArr, Place* pPlace, int placeCount);
int			checkUniqAddress(Address* address, Place* placeArr, int placeCount);

int		getPlaceName(Place* pPlace);
void	changePlaceName(char* name, int count, char** wordsArray);
void	printPlace(const void* thePlace);
int		comparePlacesByAddress(const void* p1, const void* p2);
int		writePlaceToBinary(FILE* fp, const Place* thePlace);
int		writePlaceToTxtFile(FILE* fp, const Place* thePlace);
int		readPlaceFromTxtFile(FILE* fp, Place* thePlace);
int		readPlaceFromBinary(FILE* fp, Place* thePlace);
Place* findPlaceByAddress(Place* placeArr, Address* theAddress, int countPlaces);
void	freePlace(void* thePlace);
