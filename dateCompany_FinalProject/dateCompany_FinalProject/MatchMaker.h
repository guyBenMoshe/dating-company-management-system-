#pragma once

#include	"List.h"
#define MAX_YEARS 7
#define	MAX_SALARY 120
#define	MIN_SALARY 10
#define MAX_NAME_LEN 32
#define MAX_SN 999
#define MIN_SN 1

#define SALARY0_2 20
#define SALARY2_5 40
#define SALARY5_7 80
typedef unsigned char BYTE;

typedef struct {
	char* name;
	int sn;
	int matchCounter;
	int salary;
	int yearsInCompany;
}MatchMaker;

int		initMatchMaker(MatchMaker* pMatchMaker, LIST* matchMakersList);
int		getYearsInCompany(MatchMaker* pMatchMaker);
void	getMM_SN(int* num);
int		checkMM_UniqSN(int sn, const LIST* MMs_List);
void	initMM_SN(int* num, LIST* MMArr);
int		isMMSn(const MatchMaker* pMatchMaker, const int sn);
MatchMaker* findMMBySn(const LIST* MMsList, const int sn);
MatchMaker* matchMakeWithMaxMeets(const LIST* MMsList);
void	setSalary(MatchMaker* pMatchMaker);
int		getMatchMakerName(MatchMaker* pMatchMaker);
void	changeMM_Name(char* name, int count, char** wordsArray);//upper case
int		compareMatchMakerByName(const char* name, const void* m1);
void	printMatchMaker(const void* pMatchMaker);
int		write_Compressed_MatchMakerToBinary(FILE* fp, const MatchMaker* pMatchMaker);
int		writeMatchMakerToTxtFile(FILE* fp, const MatchMaker* pMatchMaker);
int		readMMFromTxtFile(FILE* fp, MatchMaker* pMatchMaker);
int		read_Compressed_MatchMakerFromBinary(FILE* fp, MatchMaker* pMatchMaker);
void	freeMM(void* m);
