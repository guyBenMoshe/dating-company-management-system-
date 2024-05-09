#define _CRTDBG_MAP_ALLOC

#include <stdio.h>
#include <crtdbg.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>


#include "Client.h"
#include "General.h"
#include "MatchMaker.h"
#include "fileHelper.h"
#include "Macros.h"

int initClient(Client* client, LIST* clientArr){
	if (!getClientName(client)) return 0;
	initClientID(client->id, clientArr);
	client->gender = getGender(client, "choose client gender:\n");
	client->attractionGender = getGender(client, "choose client attraction gender:\n");

	return 1;
}

eGender getGender(Client* client, char* msg)
{
	int option;
	puts(msg);

	do {
		printf("Please enter one of the following types\n");
		for (int i = 0; i < eNofGenderTypes; i++)
			printf("%d for %s\n", i, GenderTypeStr[i]);
		scanf("%d", &option);
	} while (option < 0 || option >= eNofGenderTypes);
	getchar();
	return (eGender)option;
}

void initClientID(char* id, const LIST* clientArr) {

	while (1) {

		getClientId(id);//check if in range

		if (checkUniqeId(id, clientArr))//check if uniq
			break;

		printf("This sn already in use - enter a different id\n");
	}
}

void getClientId(char* id)
{
	char temp[MAX_STR_LEN];
	int ok = 1;
	do {
		ok = 1;
		printf("Enter client id  - %d numbers only\t", ID_LEN);
		myGets(temp, MAX_STR_LEN, stdin);
		if (strlen(temp) != ID_LEN)
		{
			printf("id should be %d numbers\n", ID_LEN);
			ok = 0;
		}
		else {
			for (int i = 0; i < ID_LEN; i++)
			{
				if (!isdigit(temp[i]))
				{
					printf("Need to be numbers only\n");
					ok = 0;
					break;
				}
			}
		}
	} while (!ok);

	strcpy(id, temp);
}



int getClientName(Client* client)
{
	char temp[255];
	char** wordsArray = NULL;
	int totalLength = 0;
	int count = 0;

	while (count == 0)
	{
		printf("Enter client name\n");
		myGets(temp, MAX_STR_LEN, stdin);
		wordsArray = splitCharsToWords(temp, &count, &totalLength);
	}
	client->name = (char*)calloc(totalLength + 2 + count, sizeof(char));
	CHECK_PTR_RETURN_ZERO(client->name);
	
	changeClientName(client->name, count, wordsArray);

	//clean temp data
	for (int i = 0; i < count; i++)
		free(wordsArray[i]);
	free(wordsArray);
	return 1;
}

void changeClientName(char* name, int count, char** wordsArray)
{
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

int	isClientId(const Client* pClient, const char* id)
{
	CHECK_PTR_RETURN_ZERO(pClient);
	if (strcmp(pClient->id, id) == 0)
		return 1; 
	return 0;
}

Client* findClientById(const LIST* clientsList, const char* id)
{
	NODE* ptmp = clientsList->head.next;
	while (ptmp)
	{
		if (isClientId(ptmp->key, id))
			return ptmp->key;
		ptmp = ptmp->next;
	}
	return NULL;
}


int checkUniqeId(char* id, const LIST* clientsList)
{
	Client* client = findClientById(clientsList, id);

	if (client != NULL)
		return 0;

	return 1;
}

int compareClientByName(const char* name, const void* c1)
{
	const Client* client1 = (const Client*)c1;
	return strcmp(name, client1->name);
}


int isClientsSuitable(const void* c1, const void* c2)
{
	const Client* client1 = (const Client*)c1;
	const Client* client2 = (const Client*)c2;
	
	return (client1->gender == client2->attractionGender && client1->attractionGender == client2->gender ? 1 : 0);
}

void printClient(const void* c)
{
	const Client* pClient = (const Client*)c;
	printf("Client name:%-20s| ", pClient->name);
	printf("ID: %-10s| ", pClient->id);
	printf("Gender: %-10s| ", GenderTypeStr[pClient->gender]);
	printf("Atrraction Gender: %s\n", GenderTypeStr[pClient->attractionGender]);
}

int writeClientToBinary(FILE* fp, const Client* theClient)
{
	char* name = theClient->name;
	eGender type = theClient->gender;
	eGender typeAttract = theClient->attractionGender;

	if (!writeStringToFile(name, fp, "error writing name")) //write name
		return 0;

	if(!writeCharsToFile(theClient->id, ID_LEN, fp, "error write id to file")) // id
		return 0;

	if (fwrite(&type, sizeof(int), 1, fp) != 1)//gender
		return 0;

	if (fwrite(&typeAttract, sizeof(int), 1, fp) != 1)//attraction gender
		return 0;

	return 1;
}

int writeClientToTxtFile(FILE* fp, const Client* theClient)
{
	fprintf(fp, "%s\n%s\n%d\n%d\n", theClient->name, theClient->id, theClient->gender, theClient->attractionGender);
	return 1;
}

int readClientFromTxtFile(FILE* fp,Client* theClient)
{
	char name[MAX_STR_LEN] = { 0 };
	
	if (!myGets(name, MAX_STR_LEN, fp)) return 0;//name
	if (!myGets(theClient->id, MAX_STR_LEN, fp)) return 0;//id
	if (fscanf(fp, "%d\n", &theClient->gender) != 1) return 0;//gender type
	if (fscanf(fp, "%d\n", &theClient->attractionGender) != 1) return 0;//gender attraction type

	theClient->name = _strdup(name);
	if (!theClient->name) return 0;

	return 1;
}

int readClientFromBinary(FILE* fp, Client* client)
{
	client->name = readStringFromFile(fp, "error reading company name");//name
	if(!readCharsFromFile(client->id, ID_LEN, fp, "error reading ID from file")) return 0;//id
	client->id[ID_LEN] = '\0';
	if (fread(&client->gender, sizeof(int), 1, fp) != 1)//gender
		return 0;
	if (fread(&client->attractionGender, sizeof(int), 1, fp) != 1)//attraction gender
		return 0;
	
	return 1;
}

void freeClient(void* c)
{
	Client* cTmp = (Client*)c;
	free(cTmp->name);
	free(cTmp);
}

int isSameClient(const Client* pClient1, const Client* pClient2)
{
	return (strcmp(pClient1->id, pClient2->id) == 0 ? 1 : 0);
}

size_t calcMatchPrecent(Client* c1, Client* c2) {
	size_t res;
	char name1[MAX_NAME_LEN] = { 0 };
	char name2[MAX_NAME_LEN] = { 0 };
	int sumLetters1 = 0, sumLetters2 = 0;
	int minus;
	extract_first_word(c1->name, name1);
	extract_first_word(c2->name, name2);

	for (int i = 0; i < strlen(name1); i++) {
		sumLetters1 += (name1[i] - 'a' + 1);
	}

	for (int i = 0; i < strlen(name2); i++) {
		sumLetters2 += (name2[i] - 'a' + 1);
	}
	minus = 2 * abs(sumLetters1 - sumLetters2);

	res = ((minus * 100) / (sumLetters1 + sumLetters2)) + (strlen(name1) + strlen(name2));

	return res > 100 ? 100 : res ;
}

void extract_first_word(const char* input, char* output) {
	int i;
	for (i = 0; input[i] != ' ' && input[i] != '\0'; i++) {
		output[i] = tolower(input[i]);
	}
}