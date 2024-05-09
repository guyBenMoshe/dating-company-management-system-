#define _CRTDBG_MAP_ALLOC

#include <stdio.h>
#include <crtdbg.h>
#include <stdlib.h>
#include <string.h>

#include "fileHelper.h"
#include "General.h"
#include "Macros.h"

int	 writeStringToFile(const char* str, FILE* fp, const char* msg)
{
	int length = (int)strlen(str);
	length++;
	if (!writeIntToFile(length, fp, msg))
		return 0;

	if (!writeCharsToFile(str, length, fp, msg))
		return 0;


	return 1;
}

int	 writeCharsToFile(const char* arr, int size, FILE* fp, const char* msg)
{
	if (fwrite(arr, sizeof(char), size, fp) != size)
		PRINT_RETURN_ZERO(msg);

	return 1;

}

int	 writeIntToFile(int val, FILE* fp, const char* msg)
{
	if (fwrite(&val, sizeof(int), 1, fp) != 1)
		PRINT_RETURN_ZERO(msg);

	return 1;
}

char* readStringFromFile(FILE* fp, const char* msg)
{
	char* str;
	int length;
	if (!readIntFromFile(&length, fp, msg))
		return NULL;
	str = (char*)malloc((length) * sizeof(char));
	CHECK_PTR_PRINT_RETURN_NULL(str, msg);

	if (fread(str, sizeof(char), length, fp) != length)
		FREE_PRINT_RETURN_NULL(str, msg);
	return str;
}

int readFixSizeStrFromFile(char* arr, FILE* fp, const char* msg)
{
	int len;
	if (!readIntFromFile(&len, fp, msg))
		return 0;

	if (!readCharsFromFile(arr, len, fp, msg))
		return 0;

	return 1;
}

int	 readCharsFromFile(char* arr, int length, FILE* fp, const char* msg)
{
	if (fread(arr, sizeof(char), length, fp) != length)
		PRINT_RETURN_ZERO(msg);
	return 1;
}

int	  readIntFromFile(int* pVal, FILE* fp, const char* msg)
{
	if (fread(pVal, sizeof(int), 1, fp) != 1)
		PRINT_RETURN_ZERO(msg);

	return 1;

}


char* readDynStringFromTextFile(FILE* fp)
{
	char temp[MAX_STR_LEN];
	myGets(temp, sizeof(temp), fp);
	return getDynStr(temp);
}