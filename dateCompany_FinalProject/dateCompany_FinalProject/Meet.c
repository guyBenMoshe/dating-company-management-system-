#define _CRTDBG_MAP_ALLOC

#include <stdio.h>
#include <crtdbg.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <Windows.h>

#include "Meet.h"
#include "fileHelper.h"

int		initMeet(Meet* theMeet, Place* pPlace,PeopleManager* pManager) {
	Client c1, c2;
	MatchMaker* mm;
	pickTwoClients(&c1, &c2, &pManager->L_Clients);
	if (&c1 != NULL && &c2 != NULL) {
		strcpy(theMeet->client1ID, c1.id);
		strcpy(theMeet->client2ID, c2.id);
	}
	else {
		printf("error picking two clients");
		return 0;
	}
	mm = setMM_ToMeet(&pManager->L_MatchMakers);
	mm->matchCounter++;
	theMeet->matchMakerSN = mm->sn;
	theMeet->thePlace = pPlace;
	getCorrectDate(&theMeet->date);
	return 1;
}

void pickTwoClients(Client* c1, Client* c2, LIST* l_clients) {
	int cont = 0;
	Client* c1tmp;
	Client* c2tmp;
	do {
		c1tmp = setClientToMeet(l_clients);
		c2tmp = setClientToMeet(l_clients);
		cont = isSameClient(c1tmp, c2tmp);
		if (cont)
		printf("client can't date himself obviously...(; \n");
		else if (!(c1tmp->gender == c2tmp->attractionGender && c1tmp->attractionGender == c2tmp->gender)) {
			printf("it is not possible to pick clients who are not attracted to each other... ;)\n");
			cont = 1;
		}
	} while(cont);
	*c1 = *c1tmp;
	*c2 = *c2tmp;
}

Client* setClientToMeet(const LIST* clientList)
{
	char id[MAX_STR_LEN];
	Client* c;

		do
		{
			getClientId(id);
			c = findClientById(clientList, id);
			if (c == NULL)
				printf("No Client with this id - try again\n");
		} while (c == NULL);

	return c;
}
MatchMaker* setMM_ToMeet(LIST* MM_List) {
	int sn;
	MatchMaker* mm;

	do
	{
		getMM_SN(&sn);
		mm = findMMBySn(MM_List,sn);
		if (mm == NULL)
			printf("No Match Maker with this code - try again\n");
	} while (mm == NULL);

	return mm;
}

int		compareMeetByDate(const void* m1, const void* m2) {
	const Meet* meet1 = *(const Meet**)m1;
	const Meet* meet2 = *(const Meet**)m2;
	return compareDates(&meet1->date, &meet2->date);
}

int		compareMeetByPlace(const void* m1, const void* m2) {
	const Meet* meet1 = *(const Meet**)m1;
	const Meet* meet2 = *(const Meet**)m2;
	return comparePlacesByAddress(meet1->thePlace, meet2->thePlace);
}

int		compareMeetByMatchMaker(const void* m1, const void* m2) {
	const Meet* MM1 = *(const Meet**)m1;
	const Meet* MM2 = *(const Meet**)m2;
	return (MM1->matchMakerSN - (MM2->matchMakerSN));
}

void	printMeet(const Meet* theMeet, const PeopleManager* pManager) {
	int matchPrecent;
	MatchMaker* MM_Tmp = findMMBySn(&pManager->L_MatchMakers, theMeet->matchMakerSN);
	Client* cTemp1 = findClientById(&pManager->L_Clients, theMeet->client1ID);
	Client* cTemp2 = findClientById(&pManager->L_Clients, theMeet->client2ID);
	printf(KAV_PRINT BOLD_PRINT"\nMeet Details\t" RESET_PRINT "\n");
	printf("%s", "- ");
	printDate(&theMeet->date);
	printf("\n-" BOLD_PRINT " Match Maker: " RESET_PRINT "%s, %d \n", MM_Tmp->name,MM_Tmp->sn);
	printf("-" BOLD_PRINT " first client:" RESET_PRINT " %s, %s <----> second client: %s, %s\n", cTemp1->name, cTemp1->id, cTemp2->name, cTemp2->id);
	printf("-" BOLD_PRINT " meet location--> " RESET_PRINT);
	printPlace(theMeet->thePlace);
	printf("%s", "- ");
	matchPrecent = (int)calcMatchPrecent(cTemp1, cTemp2);
	printMatchPrecent(matchPrecent);
}

void printMatchPrecent(int matchPrecent) {
	printf(BOLD_PRINT "numerology Match Precent: " RESET_PRINT);
	if (matchPrecent < 25) {
		printf("\x1b[31m%d ", matchPrecent);
		printf("\x1b[31m%c", '%');
	}
	else if (matchPrecent >= 25 && matchPrecent < 50) {
		printf("\x1b[38;5;208m%d ", matchPrecent);
		printf("\x1b[38;5;208m%c",'%');
	}
	else if (matchPrecent >= 50 && matchPrecent < 75) {
		printf("\x1b[33m%d ", matchPrecent);
		printf("\x1b[33m%c", '%');
	}
	else {
		printf("\x1b[32m%d ", matchPrecent);
		printf("\x1b[32m%c", '%');
	}
	printf("\x1b[0m\n");
}
int		writeMeetToBinary(FILE* fp, const Meet* theMeet) {
	
	if (fwrite(&theMeet->date, sizeof(Date), 1, fp) != 1)//date
		return 0;
	if (fwrite(&theMeet->matchMakerSN, sizeof(int), 1, fp) != 1)//MM-SN
		return 0;
	if (!writeCharsToFile(theMeet->client1ID, ID_LEN,fp, "error write ID to file"))//Client1
		return 0;
	if (!writeCharsToFile(theMeet->client2ID, ID_LEN,fp, "error write ID to file"))//Client2
		return 0;
	if (fwrite(&theMeet->thePlace->address, sizeof(Address), 1, fp) != 1)//address
		return 0;

	return 1;
}

int		writeMeetToTxt(FILE* fp, const Meet* theMeet) {
	writeDateToTXT(fp, &theMeet->date);
	fprintf(fp, "%d\n", theMeet->matchMakerSN);
	fprintf(fp, "%s\n", theMeet->client1ID);
	fprintf(fp, "%s\n", theMeet->client2ID);
	writeAddressToTXT(fp, &theMeet->thePlace->address);
	return 1;
}

int		readMeetFromTxtFile(FILE* fp, Meet* theMeet, Place* placeArr, int count) {
	Address tmpA;
	readDateFromTXT(fp, &theMeet->date);
	if(fscanf(fp, "%d\n", &theMeet->matchMakerSN) != 1) return 0;
	if (fscanf(fp, "%s\n", &theMeet->client1ID) != 1) return 0;
	if (fscanf(fp, "%s\n", &theMeet->client2ID) != 1) return 0;
	readAddressFromTXT(fp, &tmpA);
	theMeet->thePlace = findPlaceByAddress(placeArr, &tmpA, count);
	if (!theMeet->thePlace) return 0;
	return 1;
}

int		readMeetFromBinary(FILE* fp, Meet* theMeet, Place* placeArr, int countPlaces) {
	Address tmpA;
	if (fread(&theMeet->date, sizeof(Date), 1, fp) != 1)  //Date
		return 0;
	if (fread(&theMeet->matchMakerSN, sizeof(int), 1, fp) != 1)  //MM-SN
		return 0;
	if (!readCharsFromFile(theMeet->client1ID, ID_LEN, fp,"error reading ID from file"))  //Client1
		return 0;
	theMeet->client1ID[ID_LEN] = '\0';
	if (!readCharsFromFile(theMeet->client2ID, ID_LEN, fp, "error reading ID from file"))  //Client2
		return 0;
	theMeet->client2ID[ID_LEN] = '\0';
	if (fread(&tmpA, sizeof(Address), 1, fp) != 1)//address
		return 0;
	theMeet->thePlace = findPlaceByAddress(placeArr, &tmpA, countPlaces);
	if (!theMeet->thePlace)
	{
		return 0;
	}

	return 1;
}
void	freeMeet(void* m) {
	Meet* meet = *(Meet**)m;

	free(meet);
}
 
void printDots() {
	int i;
	int num_dots = 10; // Number of dots to print

	for (i = 0; i < num_dots; i++) {
		printf(".");
		fflush(stdout); 
		Sleep(150); 
	}
}