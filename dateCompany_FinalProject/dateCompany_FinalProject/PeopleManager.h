#pragma once

#include "List.h"
#include "Client.h"
#include "MatchMaker.h"

#define ERROE 0
#define FROM_USER 1
#define FROM_FILE 2


typedef struct {
	LIST L_Clients;
	LIST L_MatchMakers;
	int countClient;
	int countMatchMaker;
}PeopleManager;

int		initPeopleManager(PeopleManager* pPM, char* fileName, int choice);
int		addClient(PeopleManager* pManager);
int		addMatchMaker(PeopleManager* pManager);
int		atLeastOneValidMatchByGender(LIST* L_clients, int countClient);
void	printAllMatchMakers(const PeopleManager* pManager);
void	printAllClients(const PeopleManager* pManager);
void	freeManager(PeopleManager* pManager);
void	freeClientsArr(PeopleManager* pManager);
void	freeMatchMakersArr(PeopleManager* pManager);
NODE*	findPlaceInOrder(LIST* pList, char* name, int(*compare)(const char*, const void*));
int		saveManagerToFile(const PeopleManager* pPM, const char* txtFileName, const char* binFileName);
int		writeManagerToFile(const PeopleManager* pManager, FILE* fp, const int(*writeclient)(FILE*, Client*), const int(*writeMM)(FILE*, MatchMaker*));
int		readManagerFromFile(FILE* fp, PeopleManager* pPM, int(*readClient)(FILE*, Client*), int(*readMM)(FILE*, MatchMaker*));

