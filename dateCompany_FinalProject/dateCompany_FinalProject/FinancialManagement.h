#pragma once
#include "Meet.h"
//// payment recived from the place 
#define COFFE_PROFIT 30
#define RESTURANT_PROFIT 100
#define PARTY_PROFIT 70

#define MAX_CLIENT_PAYMENT 80
#define MIN_CLIENT_PAYMENT 20

typedef struct {
	int outcome;
	int income;
	int paymentPerMeet;
}FinancialManagement;

int		initFinancialManagement(FinancialManagement* pFM);
void	printFM(const FinancialManagement* pFM);
int		calcCompanyProfit(FinancialManagement* pFM, Meet** meetArr, int counter, PeopleManager* pManager);
void	initPayment(int* paymentPerMeet);
void	getRaise(FinancialManagement* pFM, PeopleManager* pManager);
