#pragma once
#include "List.h"

#define MAX_SN 999
#define MIN_SN 1
#define ID_LEN 9

typedef enum {
	eFemale, eMale, eNofGenderTypes
} eGender;

static const char* GenderTypeStr[eNofGenderTypes]
= {"Female", "Male"};

typedef struct {
	char* name;
	char id[ID_LEN + 1];
	eGender gender;
	eGender attractionGender;
}Client;



int		initClient(Client* client, LIST* clientArr);
eGender getGender(Client* client, char* msg);
void	changeClientName(char* name, int count, char** wordsArray);//upper case
int		getClientName(Client* client);
void	getClientId(char* id);
void	initClientID(char* id, const LIST* clientArr);
int		compareClientByName(const char* name, const void* c1);
int		isClientsSuitable(const void* c1, const void* c2);
int		isSameClient(const Client* pClient1, const Client* pClient2);
int		checkUniqeId(char* sn, const LIST* clientsList);
int		isClientId(const Client* pClient, const char* sn);
Client* findClientById(const LIST* clientsList, const char* sn);
void	printClient(const void* val);
int		writeClientToBinary(FILE* fp, const Client* theClient);
int		writeClientToTxtFile(FILE* fp, const Client* theClient);
int		readClientFromTxtFile(FILE* fp, Client* theClient);
int		readClientFromBinary(FILE* fp, Client* client);
void	freeClient(void* c);
size_t	calcMatchPrecent(Client* c1, Client* c2);
void	extract_first_word(const char* input, char* output);
