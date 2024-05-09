#define _CRTDBG_MAP_ALLOC

#include <stdio.h>
#include <crtdbg.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "FinancialManagement.h"

int initFinancialManagement(FinancialManagement* pFM)
{
	pFM->income = 0;
	pFM->outcome = 0;
	initPayment(&pFM->paymentPerMeet);
	return 1;
}

void initPayment(int* paymentPerMeet) {
	int temp;
	do {
		printf("Enter payment per meet: (%d - %d) \n", MIN_CLIENT_PAYMENT, MAX_CLIENT_PAYMENT);
		scanf("%d", &temp);
	} while (temp > MAX_CLIENT_PAYMENT || temp < MIN_CLIENT_PAYMENT);
	*paymentPerMeet = temp;
}

void getRaise(FinancialManagement* pFM, PeopleManager* pManager)
{
	int bonus;
	MatchMaker* MM = matchMakeWithMaxMeets(&pManager->L_MatchMakers);
	printf("the outstanding Match Maker is:\n");
	printMatchMaker(MM);
	
	do {
		printf("what raise would you like to give?\t");
		scanf("%d", &bonus);
	} while ((bonus + MM->salary) > MAX_SALARY || (bonus + MM->salary) < MIN_SALARY);
	MM->salary += bonus;
}

int calcCompanyProfit(FinancialManagement* pFM, Meet** meetArr, int counter, PeopleManager* pManager) {
	ePLaceType type;
	MatchMaker* MM;
	pFM->income = 0;
	pFM->outcome = 0;
	for (int i = 0; i < counter; i++) {

		type = meetArr[i]->thePlace->type;
		 MM = findMMBySn(&pManager->L_MatchMakers, meetArr[i]->matchMakerSN);

		switch (type)
		{
		case coffe:
			pFM->income += COFFE_PROFIT + (pFM->paymentPerMeet) * 2;
			pFM->outcome += MM->salary;
			break;
		case eRestaurant:
			pFM->income += RESTURANT_PROFIT + (pFM->paymentPerMeet) * 2;
			pFM->outcome += MM->salary;
			break;
		case eParty:
			pFM->income += PARTY_PROFIT + (pFM->paymentPerMeet) * 2;
			pFM->outcome += MM->salary;
			break;

		default:
			break;
		}
	}
	return 1;
}


void printFM(const FinancialManagement* pFM)
{
	printf("payment per date: %d \nincome: %d, outcome: %d, profit: %d", pFM->paymentPerMeet, pFM->income, pFM->outcome, pFM->income - pFM->outcome);
}