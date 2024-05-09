#define _CRTDBG_MAP_ALLOC

#include <stdio.h>
#include <crtdbg.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "MatchMaker.h"
#include "General.h"
#include "Macros.h"

int	initMatchMaker(MatchMaker* pMatchMaker, LIST* matchMakersList) {
	if (!getMatchMakerName(pMatchMaker))return 0;
	initMM_SN(&pMatchMaker->sn, matchMakersList);
	pMatchMaker->matchCounter = 0;
	pMatchMaker->yearsInCompany = getYearsInCompany(pMatchMaker);
	setSalary(pMatchMaker);

	return 1;
}

int getYearsInCompany(MatchMaker* pMatchMaker)
{
	int num;
	do {
		printf("Enter Match Maker years in company - between %d to %d\n", 0, MAX_YEARS);
		scanf("%d", &num);
	} while (!(num >= MIN_SN && num <= MAX_SN));

	return num;
}

int	checkMM_UniqSN(int sn, const LIST* MMs_List)
{
	MatchMaker* MM = findMMBySn(MMs_List, sn);// MM = match maker

	if (MM != NULL)
		return 0;

	return 1;
}

void getMM_SN(int* num)
{
	int temp;
	do {
		printf("Enter Match Maker Sequence number - between %d to %d\n", MIN_SN, MAX_SN);
		scanf("%d", &temp);
	} while (!(temp >= MIN_SN && temp <= MAX_SN));
	*num = temp;
}

void initMM_SN(int* num, LIST* MMArr) {

	while (1) {

		getMM_SN(num);//check if in range

		if (checkMM_UniqSN(*num, MMArr))//check if exist
			break;

		printf("This sn already in use - enter a different sn\n");
	}
}

int getMatchMakerName(MatchMaker* pMatchMaker)
{
	char temp[MAX_NAME_LEN];
	char** wordsArray = NULL;
	int totalLength = 0;
	int count = 0;

	while (count == 0)
	{
		printf("Enter Match Maker name\n");
		myGets(temp, MAX_NAME_LEN, stdin);
		wordsArray = splitCharsToWords(temp, &count, &totalLength);
	}
	pMatchMaker->name = (char*)calloc(totalLength + 2 + count, sizeof(char));
	if (!pMatchMaker->name)
		return 0;

	changeMM_Name(pMatchMaker->name, count, wordsArray);

	//clean temp data
	for (int i = 0; i < count; i++)
		free(wordsArray[i]);
	free(wordsArray);
	return 1;
}
void setSalary(MatchMaker* pMatchMaker)
{
	if (pMatchMaker->yearsInCompany <= 2 && pMatchMaker->yearsInCompany >= 0) {
		pMatchMaker->salary = SALARY0_2;
	}
	else if (pMatchMaker->yearsInCompany <= 5 && pMatchMaker->yearsInCompany > 2)
		pMatchMaker->salary = SALARY2_5;
	else pMatchMaker->salary = SALARY5_7;
	
}

void changeMM_Name(char* name, int count, char** wordsArray)
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

int compareMatchMakerByName(const char* name, const void* m1)
{
	const MatchMaker* MM1 = (const MatchMaker*)m1;
	return strcmp(name, MM1->name);
}

void printMatchMaker(const void* mm)
{
	const MatchMaker* pMM = (const MatchMaker*)mm;

	printf("Match Maker name:%-15s| ", pMM->name);
	printf("sequence number: %-5d| ", pMM->sn);
	printf("Match Counter: %-5d| ", pMM->matchCounter);
	printf("Salary: %-5d| ", pMM->salary);
	printf("Years In Company: %-5d\n", pMM->yearsInCompany);
}

int	isMMSn(const MatchMaker* pMatchMaker, const int sn) {

	CHECK_PTR_RETURN_ZERO(pMatchMaker);
	if (pMatchMaker->sn == sn)
		return 1;
	return 0;
}

MatchMaker* findMMBySn(const LIST* MMsList, const int sn)
{
	NODE* ptmp = MMsList->head.next;

	while (ptmp)
	{
		if (isMMSn(ptmp->key, sn))
			return ptmp->key;
		ptmp = ptmp->next;
	}
	return NULL;
}

MatchMaker* matchMakeWithMaxMeets(const LIST* MMsList)
{
	NODE* ptmp = MMsList->head.next;
	int max = 0;
	MatchMaker* maxMeetsMM = NULL;
	while (ptmp)
	{
		if (((MatchMaker*)ptmp->key)->matchCounter >= max) {
			maxMeetsMM = ptmp->key;
			max = ((MatchMaker*)ptmp->key)->matchCounter;
		}
		ptmp = ptmp->next;
	}
	return maxMeetsMM;
}


int	write_Compressed_MatchMakerToBinary(FILE* fp, const MatchMaker* pMatchMaker) {

	BYTE data[4] = { 0 };
	int len = (int)strlen(pMatchMaker->name);
	data[0] = len << 3 | pMatchMaker->sn >> 7;
	data[1] = (pMatchMaker->sn & 0x7f) << 1 | pMatchMaker->matchCounter >> 6;
	data[2] = (pMatchMaker->matchCounter & 0x3f) << 2 | pMatchMaker->salary >> 5;
	data[3] = (pMatchMaker->salary & 0x1f) << 3 | pMatchMaker->yearsInCompany;

	if (fwrite(&data, sizeof(BYTE), 4, fp) != 4) return 0;
	if (fwrite(pMatchMaker->name, sizeof(char), len, fp) != len) return 0;

	return 1;
}

int read_Compressed_MatchMakerFromBinary(FILE* fp, MatchMaker* pMatchMaker)
{
	BYTE data[4];
	if (fread(&data, sizeof(BYTE), 4, fp) != 4) return 0;

	int len = (data[0] >> 3) & 0x1f;
	pMatchMaker->sn = (data[0] & 0x7) << 7 | ((data[1] >> 1) & 0x7f);
	pMatchMaker->matchCounter = ((data[1] & 0x1) << 6) | ((data[2] >> 2) & 0x3f);
	pMatchMaker->salary = ((data[2] & 0x3) << 5) | ((data[3] >> 3) & 0x1f);
	pMatchMaker->yearsInCompany = data[3] & 0x7;


	pMatchMaker->name = (char*)calloc(len + 1, sizeof(char));
	if (!pMatchMaker->name) return 0;

	if (fread(pMatchMaker->name, sizeof(char), len, fp) != len) {
		free(pMatchMaker->name);
		return 0;
	}
	return 1;
}

int writeMatchMakerToTxtFile(FILE* fp, const MatchMaker* pMatchMaker)
{
	fprintf(fp, "%s\n%d\n%d\n%d\n%d\n", pMatchMaker->name, pMatchMaker->sn, pMatchMaker->matchCounter, pMatchMaker->yearsInCompany, pMatchMaker->salary);
	return 1;
}

int readMMFromTxtFile(FILE* fp, MatchMaker* pMatchMaker)
{
	char name[MAX_STR_LEN] = { 0 };

	if (!myGets(name, MAX_STR_LEN, fp)) return 0;//name
	if (fscanf(fp, "%d\n", &pMatchMaker->sn) != 1) return 0;//sn
	if (fscanf(fp, "%d\n", &pMatchMaker->matchCounter) != 1) return 0;//match counter
	if (fscanf(fp, "%d\n", &pMatchMaker->yearsInCompany) != 1) return 0;//years in company
	if (fscanf(fp, "%d\n", &pMatchMaker->salary) != 1) return 0;//salary


	pMatchMaker->name = _strdup(name);
	if (!pMatchMaker->name) return 0;

	return 1;
}

void freeMM(void* m)
{
	MatchMaker* mTmp = (MatchMaker*)m;
	free(mTmp->name);
	free(mTmp);
}

