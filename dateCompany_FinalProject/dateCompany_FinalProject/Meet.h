#pragma once
#include "Place.h"
#include "Date.h"
#include "PeopleManager.h"


#define MAX_DURATION 7.0
#define MIN_DURATION 0.5


typedef struct {
	int matchMakerSN;
	char client1ID[ID_LEN+1];
	char client2ID[ID_LEN+1];
	Place* thePlace;
	Date date;
}Meet;


int		initMeet(Meet* theMeet, Place* pPlace, PeopleManager* pManager);
Client*	setClientToMeet(const LIST* clientList);
MatchMaker*	setMM_ToMeet(LIST* MM_List);
void	pickTwoClients(Client* c1, Client* c2, LIST* l_clients);
int		compareMeetByDate(const void* m1, const void* m2);
int		compareMeetByPlace(const void* m1, const void* m2);
int		compareMeetByMatchMaker(const void* m1, const void* m2);
void	printMeet(const Meet* theMeet, const PeopleManager* pManager);
void	printMatchPrecent(int matchPrecent);
int		writeMeetToBinary(FILE* fp, const Meet* theMeet);
int		writeMeetToTxt(FILE* fp, const Meet* theMeet);
int		readMeetFromTxtFile(FILE* fp, Meet* theMeet, Place* placeArr, int count);
int		readMeetFromBinary(FILE* fp, Meet* theMeet, Place* placeArr, int countPlaces);
void	freeMeet(void* theMeet);
void	printDots();