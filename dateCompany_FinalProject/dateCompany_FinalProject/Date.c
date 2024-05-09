#define _CRTDBG_MAP_ALLOC

#include <stdio.h>
#include <crtdbg.h>

#include <string.h>
#include <stdlib.h>
#include "General.h"
#include "Date.h"

const int DAY_MONTHS[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
#define SPECIAL_TAV '#'
#define D_M_Y

void getCorrectDate(Date* pDate)
{
	char date[MAX_STR_LEN];
	int ok = 1;

	do {
		printf("Enter Meet Date dd%c%cmm%c%cyyyy  minimum year %d\t",
			SPECIAL_TAV, SPECIAL_TAV, SPECIAL_TAV, SPECIAL_TAV, MIN_YEAR);
		myGets(date, MAX_STR_LEN, stdin);
		ok = checkDate(date, pDate);
		if (!ok)
			printf("Error try again\n");
	} while (!ok);
}


int	 checkDate(char* date, Date* pDate)
{
	int day, month, year;
	if (strlen(date) != 12)
		return 0;
	if ((date[2] != SPECIAL_TAV) || (date[3] != SPECIAL_TAV)
		|| (date[6] != SPECIAL_TAV) || (date[7] != SPECIAL_TAV))
		return 0;
	sscanf(date, "%d%*c%*c%d%*c%*c%d", &day, &month, &year);
	if (day < 1 || month < 1 || month > 12 || year < MIN_YEAR)
		return 0;

	if (day > DAY_MONTHS[month - 1])
		return 0;

	pDate->day = day;
	pDate->month = month;
	pDate->year = year;

	return 1;
}

int compareDates(const void* v1, const void* v2) {
	const Date* date1 = (const Date*)v1;
	const Date* date2 = (const Date*)v2;

	if (date1->year != date2->year) return date1->year - date2->year;
	else if (date1->month != date2->month) return date1->month - date2->month;
	else return date1->day - date2->day;

}

void printDate(const Date* pDate)
{
	printf(BOLD_PRINT "Date:" RESET_PRINT " %d/%d/%d", pDate->day, pDate->month, pDate->year);
}

int writeDateToTXT(FILE* fp,const Date* pDate) {
	fprintf(fp,"%d\n%d\n%d\n", pDate->day, pDate->month, pDate->year);
	return 1;
}

int readDateFromTXT(FILE* fp, Date* pDate) {
	if(fscanf(fp, "%d\n%d\n%d\n", &pDate->day, &pDate->month, &pDate->year) !=3) return 0;

	return 1;
}