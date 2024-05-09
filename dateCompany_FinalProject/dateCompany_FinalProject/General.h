#pragma once

#define MAX_STR_LEN 255

#define KAV_PRINT "\033[4m"
#define BOLD_PRINT "\033[1m" 
#define RESET_PRINT "\033[0m"

char* getStrExactName(const char* msg);
char* myGets(char* buffer, int size, FILE* source);
char* getDynStr(char* str);
char** splitCharsToWords(char* str, int* pCount, int* pTotalLength);
void generalArrayFunction(void* arr, int size, int typeSize, void(*func)(void* element));





