#define _CRTDBG_MAP_ALLOC

#include <stdio.h>
#include <crtdbg.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "PeopleManager.h"
#include "Macros.h"



int initPeopleManager(PeopleManager* pPM, char* fileName, int choice)
{
	FILE* fp;
	pPM->countClient = 0;
	pPM->countMatchMaker = 0;

	if (L_init(&pPM->L_Clients) && L_init(&pPM->L_MatchMakers)) {
		if (!choice) { 
			fp = fopen(fileName, "r");
			if (!fp) return FROM_USER;
			if (fscanf(fp, "%d", &pPM->countClient) != 1) return FROM_USER;//client count-txt
			if (fscanf(fp, "%d", &pPM->countMatchMaker) != 1) return FROM_USER;//match maker count-txt
			if (!readManagerFromFile(fp, pPM, readClientFromTxtFile, readMMFromTxtFile)) return FROM_USER;
			fclose(fp);
			return FROM_FILE;
		}
		else {
			fp = fopen(fileName, "rb");
			if (!fp) return FROM_USER;
			if (fread(&pPM->countClient, sizeof(int), 1, fp) != 1)//client count-bin
				return FROM_USER;
			if (fread(&pPM->countMatchMaker, sizeof(int), 1, fp) != 1)//match maker count-bin
				return FROM_USER;
			if (!readManagerFromFile(fp, pPM, readClientFromBinary, read_Compressed_MatchMakerFromBinary)) return 1;
			fclose(fp);
			return FROM_FILE;
		}
	}
	return ERROE;
}

int addClient(PeopleManager* pManager)
{
	Client* pClient = (Client*)calloc(1, sizeof(Client));
	if (!pClient)
		return 0;

	if (!initClient(pClient, &pManager->L_Clients))
	{
		freeClient(pClient);
		FREE_RETURN_ZERO(pClient);
	}

	NODE* pNode = findPlaceInOrder(&pManager->L_Clients, pClient->name, compareClientByName);
	L_insert(pNode, pClient);
	pManager->countClient++;
	return 1;
}

int addMatchMaker(PeopleManager* pManager)
{
	MatchMaker* pMM = (MatchMaker*)calloc(1, sizeof(MatchMaker));
	CHECK_PTR_RETURN_ZERO(pMM);

	if (!initMatchMaker(pMM, &pManager->L_MatchMakers))
	{
		freeMM(pMM);
		FREE_RETURN_ZERO(pMM);
	}

	NODE* pNode = findPlaceInOrder(&pManager->L_MatchMakers, pMM->name, compareMatchMakerByName);
	L_insert(pNode, pMM);
	pManager->countMatchMaker++;
	return 1;
}

int atLeastOneValidMatchByGender(LIST* L_clients, int countClient) {
	NODE* pTmpStati = L_clients->head.next;
	NODE* pTmpMove;

	while (pTmpStati) {
		pTmpMove = pTmpStati->next;
		while (pTmpMove)
		{
			if (isClientsSuitable(pTmpStati->key, pTmpMove->key))
			return 1;
			pTmpMove = pTmpMove->next;
		}
		pTmpStati = pTmpStati->next;
	}
	return 0;
}

void printAllMatchMakers(const PeopleManager* pManager)
{
	printf("there are %d Match Makers\n", pManager->countMatchMaker);
	L_print(&pManager->L_MatchMakers, (void*)printMatchMaker);
}

void printAllClients(const PeopleManager* pManager)
{
	printf("there are %d Clients \n", pManager->countClient);
	L_print(&pManager->L_Clients, (void*)printClient);
}

void freeManager(PeopleManager* pManager)
{
	freeClientsArr(pManager);
	freeMatchMakersArr(pManager);
}

void freeClientsArr(PeopleManager* pManager)
{
	L_free(&pManager->L_Clients, (void*)freeClient);

}

void freeMatchMakersArr(PeopleManager* pManager)
{
	L_free(&pManager->L_MatchMakers, (void*)freeMM);

}

NODE* findPlaceInOrder(LIST* pList, char* name, int(*compare)(const char*, const void*))
{
	NODE* tmp = &pList->head;

	if (pList == NULL) return tmp;

	while (tmp->next) {
		if (compare(name, tmp->next->key) > 0) tmp = tmp->next;
		else return tmp;
	}
	return tmp;
}

int saveManagerToFile(const PeopleManager* pPM, const char* txtFileName, const char* binFileName)
{
	FILE *fpt, *fpb;

	fpt = fopen(txtFileName, "w");
	CHECK_PTR_RETURN_ZERO(fpt);

	fpb = fopen(binFileName, "wb");
	CHECK_PTR_RETURN_ZERO(fpb);

	/////////////////txt
	fprintf(fpt, "%d\n", pPM->countClient);
	fprintf(fpt, "%d\n", pPM->countMatchMaker);

	/////////////////binary
	if (fwrite(&pPM->countClient, sizeof(int), 1, fpb) != 1) //client counter
		CLOSE_RETURN_ZERO(fpt);

	if (fwrite(&pPM->countMatchMaker, sizeof(int), 1, fpb) != 1) //match maker counter
		CLOSE_RETURN_ZERO(fpb);

	/////////////////txt
	if (!writeManagerToFile(pPM, fpt, writeClientToTxtFile, writeMatchMakerToTxtFile))
		CLOSE_RETURN_ZERO(fpt);

	/////////////////binary
	if (!writeManagerToFile(pPM, fpb, writeClientToBinary, write_Compressed_MatchMakerToBinary))
		CLOSE_RETURN_ZERO(fpb);

	fclose(fpt);
	fclose(fpb);
	return 1;
}

int writeManagerToFile(const PeopleManager* pManager, FILE* fp, const int(*writeclient)(FILE*, Client*),const int(*writeMM)(FILE*, MatchMaker*))
{
	NODE* ptmpClient = pManager->L_Clients.head.next;
	NODE* ptmpMM = pManager->L_MatchMakers.head.next;

	while (ptmpClient) { 
		writeclient(fp, ptmpClient->key);
		ptmpClient = ptmpClient->next;
	}
	
	while (ptmpMM)
	{
		writeMM(fp, ptmpMM->key);
		ptmpMM = ptmpMM->next;
	}
	return 1;
}

int readManagerFromFile(FILE* fp, PeopleManager* pPM, int(*readClient)(FILE*, Client*), int(*readMM)(FILE*, MatchMaker*))
{
	NODE* pNode;
	Client* ptmpClient;
	MatchMaker* ptmpMM;
////////////////////////////////////////////////////////////////////////////////////////client
	for (int i = 0; i < pPM->countClient; i++) {
		ptmpClient = (Client*)malloc(sizeof(Client));
		CHECK_PTR_FREE_RETURN_ZERO(ptmpClient, ptmpClient);
		
		if (!readClient(fp, ptmpClient)) return 0;

		pNode = findPlaceInOrder(&pPM->L_Clients, ptmpClient->name, compareClientByName);
		L_insert(pNode, ptmpClient);
	}
/////////////////////////////////////////////////////////////////////////////////////match maker
	for (int i = 0; i < pPM->countMatchMaker; i++) {
		ptmpMM = (MatchMaker*)malloc(sizeof(MatchMaker));
		CHECK_PTR_FREE_RETURN_ZERO(ptmpMM, ptmpMM);

		if (!readMM(fp, ptmpMM)) return 0;

		pNode = findPlaceInOrder(&pPM->L_MatchMakers, ptmpMM->name, compareMatchMakerByName);
		L_insert(pNode, ptmpMM);
	}
	return 1;
}


