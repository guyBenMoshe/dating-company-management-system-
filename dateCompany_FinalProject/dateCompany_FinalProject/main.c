#define _CRTDBG_MAP_ALLOC

#include <stdio.h>
#include <crtdbg.h>
#include <stdlib.h>
#include "string.h"


#include "Company.h"
#include "CompanyFile.h"
#include "PeopleManager.h"

typedef enum {
	eAddClient,eAddMatchMaker, eAddPlace, eAddDate, ePrintData, eSortDate, eFindDate, eBonus, eUpdatePayment, eCalcMatchPrecent, eNofOptions
}enumOption;

const char* str[eNofOptions] = {"Add Client", "Add Match Maker","Add Place", "Add Meet","Print DATA", "Sort Meets", "Find Meet",
 "Give Raise","Update Payment Per Meet", "Calc Match Precent" };

typedef enum {
	eShowCompany, showPM, printFinancialManager, eNofOptions2
}enumPrint;

const char* strPrint[eNofOptions2] = {"Print Company","print all clients and match makers", "print Financial Manager" };


#define EXIT			-1
#define COMPANY_FILE_NAME_SRC_TXT "company_TXT.txt"
#define COMPANY_FILE_NAME_SRC_BIN "company_BIN.bin"


#define COMPANY_FILE_NAME_DST_TXT "company_TXT.txt"
#define COMPANY_FILE_NAME_DST_BIN "company_BIN.bin"

#define MANAGER_FILE_NAME_SRC_TXT "manager_TXT.txt"
#define MANAGER_FILE_NAME_SRC_BIN "manager_BIN.bin"


#define MANAGER_FILE_NAME_DST_TXT "manager_TXT.txt"
#define MANAGER_FILE_NAME_DST_BIN "manager_BIN.bin"

int menu();
int initManagerAndCompany(PeopleManager* manager, Company* company);
int saveDataToFiles(PeopleManager* manager, Company* pComp);
void showFunctions(Company* company, PeopleManager* manager);
int printMenu();
void matchPrecentFunction(PeopleManager* manager);




int option;
int stop = 0;
int matchPrecent;

int main() {

	Company			company;
	PeopleManager	manager;

	initManagerAndCompany(&manager, &company);
	do
	{
		option = menu();
		switch (option)
		{

		case eAddClient:
			if (!addClient(&manager))
				printf("Error adding client\n");
			break;

		case eAddMatchMaker:
			if (!addMatchMaker(&manager))
				printf("Error adding Match Maker\n");
			break;

		case eAddPlace:
			if (!addPlace(&company))
				printf("Error adding Place\n");
			break;

		case eAddDate:
			if (!addMeet(&company, &manager))
				printf("Error adding Meet\n");
			break;

		case ePrintData:
			showFunctions(&company, &manager);
			break;

		case eSortDate:
			sortMeet(&company);
			break;

		case eFindDate:
			findMeet(&company, &manager);
			break;

		case eBonus:
			getRaise(&company.pFM, &manager);
			break;
		case eUpdatePayment:
			printf("Current Payment: %d\n", company.pFM.paymentPerMeet);
			initPayment(&company.pFM.paymentPerMeet);
			break;
		case eCalcMatchPrecent:
			matchPrecentFunction(&manager);
			break;

		case EXIT:
			printf("Bye Bye\n");
		
			stop = 1;
			break;

		default:
			printf("Wrong option\n");
			break;
		}
	} while (!stop);
	saveDataToFiles(&manager, &company);

	freeManager(&manager);
	freeCmopany(&company);

	_CrtDumpMemoryLeaks();
	return 1;
}
void showFunctions(Company* company, PeopleManager* manager) {

	 int choice = printMenu();

	switch (choice)
	{

	case eShowCompany:
		printCompany(company, manager);
		break;

	case showPM:
		printAllClients(manager);
		printAllMatchMakers(manager);
		break;

	case printFinancialManager:
		calcCompanyProfit(&company->pFM, company->meetsArr, company->meetsCount, manager);
		printFM(&company->pFM);
		break;

	default:
		printf("Wrong option\n");
		break;
	}
}

int menu()
{
	int option;
	printf("\n\n");
	printf("Please choose one of the following options\n");
	for (int i = 0; i < eNofOptions; i++)
		printf("%d - %s\n", i, str[i]);
	printf("%d - Quit\n", EXIT);
	scanf("%d", &option);
	//clean buffer
	char tav;
	scanf("%c", &tav);
	return option;
}

int printMenu() {
	int option;
	printf("Please choose one of the following options\n");
	for (int i = 0; i < eNofOptions2; i++)
		printf("%d - %s\n", i, strPrint[i]);
	scanf("%d", &option);
	//clean buffer
	char tav;
	scanf("%c", &tav);
	return option;
}

int initManagerAndCompany(PeopleManager* manager, Company* company) {
	int choice;
	char *cFile = NULL, *mFile = NULL;
	int res;
	printf("for reading from TEXT FILE press - 0, for reading from BINARY FILE press any else button\n");
	scanf("%d", &choice);

	if (!choice) {
		cFile = COMPANY_FILE_NAME_SRC_TXT;
		mFile = MANAGER_FILE_NAME_SRC_TXT;
	}
	else {
		cFile = COMPANY_FILE_NAME_SRC_BIN;
		mFile = MANAGER_FILE_NAME_SRC_BIN;
	}
	res = initPeopleManager(manager, mFile, choice);

	if (!res) return 0;

	if (res == FROM_FILE) {
		if (!initCompanyFromFile(company, cFile, choice))
			return initCompany(company);
	} else return initCompany(company);

	return 1;
}

int saveDataToFiles(PeopleManager* manager, Company* pComp) {

	if (!saveManagerToFile(manager, MANAGER_FILE_NAME_DST_TXT, MANAGER_FILE_NAME_DST_BIN)) return 0;
	if (!writeCompanyToBinary(COMPANY_FILE_NAME_DST_BIN, pComp)) return 0;
	if (!writecompanyToTxtFile(COMPANY_FILE_NAME_DST_TXT, pComp)) return 0;
	return 1;
}

void matchPrecentFunction(PeopleManager* manager) {
	int matchPrecent;
	Client tmpC1;
	Client tmpC2;
	printAllClients(manager);
	pickTwoClients(&tmpC1, &tmpC2, &manager->L_Clients);
	matchPrecent = (int)calcMatchPrecent(&tmpC1, &tmpC2);
	printDots();
	printMatchPrecent(matchPrecent);
}