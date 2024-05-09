#pragma once


#define MIN_YEAR 2023

typedef struct
{
	int			day;
	int			month;
	int			year;
}Date;

void	getCorrectDate(Date* pDate);
int		checkDate(char* date, Date* pDate);
void	printDate(const Date* pDate);
int		compareDates(const void* v1, const void* v2);
int		writeDateToTXT(FILE* fp,const Date* pDate);
int		readDateFromTXT(FILE* fp, Date* pDate);


