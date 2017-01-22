#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main_aux.h"



//concate string b at the end of string a
void Sconcate (char s1[], char s2[]) {

	  int i, j;

	   i = strlen(s1);

	   for (j = 0; s2[j] != '\0'; i++, j++) {
		   if (s2[j] != '\n')
	      s1[i] = s2[j];
	   }

	   s1[i] = '\0';

}

void removeNewline (char s1[]) {
	s1[strlen(s1)-1] = '\0';
}


void FreeOneArrayOfPoints(SPPoint** array) {
	int size = (int) (sizeof(array) / sizeof(SPPoint*));
	for (int i = 0; i < size; i++)
		spPointDestroy(array[i]);
	free(array);
}

//Free all Elements in DB then free DB itself
//DB is an array of arrays of SPPoints

void FreeDB(SPPoint*** DB, int size) {
	for (int j = 0; j < size; j++)
		FreeOneArrayOfPoints(DB[j]);
	free(DB);
}

void createAllDB(int* NumOfSiftExtracted, SPPoint**** RGB_DB,
		SPPoint**** SIFTS_DB, char* dir, char* picName, int HowManypic,
		char* sufName, int nBin, int nSift) {

	/** Allocate 2 Data-Bases
	 *  RGB_DB -> Data Base for RGB_histograms
	 *  SIFTS_DB -> Data Base for SIFT
	 */

	*RGB_DB = (SPPoint***) malloc(sizeof(SPPoint**) * HowManypic);
	if (*RGB_DB == NULL) {
		printf(MEM_PROBLEMS);
		return;
	}

	*SIFTS_DB = (SPPoint***) malloc(sizeof(SPPoint**) * HowManypic);
	if (*SIFTS_DB == NULL) {
		printf(MEM_PROBLEMS);
		free(*RGB_DB);
		return;
	}

	//initialize array of ints for number of sifts extracted from each image
	NumOfSiftExtracted = (int*) malloc(sizeof(int) * HowManypic);
	if (NumOfSiftExtracted == NULL) {
		printf(MEM_PROBLEMS);
		free(*SIFTS_DB);
		free(*RGB_DB);
		return;
	}

	//Inserts values to DB for each image
	printf("__main_aux__ - howmanypic: %d", HowManypic);
	for (int i = 0; i < HowManypic; i++) {

		//Concate path
		char C[2];
		sprintf(C,"%d",i);
		char src[STRLENGTH] = "";
		Sconcate(src,dir);
		Sconcate(src,picName);
		Sconcate(src,C);
		Sconcate(src,sufName);
		printf("%s",src);


		(*RGB_DB)[i] = spGetRGBHist(src, i, nBin);

		//Allocation failed
		if (RGB_DB[i] == NULL) {
			FreeDB(*RGB_DB, i);
			FreeDB(*SIFTS_DB, i);
			free(NumOfSiftExtracted);
			return;
		}


		(*SIFTS_DB)[i] = spGetSiftDescriptors(src, i, nSift,
				&NumOfSiftExtracted[i]);
		//Allocation failed
		if (SIFTS_DB[i] == NULL) {
			//we have one more in RGB
			FreeDB(*RGB_DB, i + 1);
			FreeDB(*SIFTS_DB, i);
			free(NumOfSiftExtracted);
			return;
		}

	}

}
void terminate(SPPoint*** RGB_DB, SPPoint*** SIFTS_DB, int* NumOfSiftExtracted,
		int HowManypic, bool print) {
	FreeDB(RGB_DB, HowManypic);
	FreeDB(SIFTS_DB, HowManypic);
	free(NumOfSiftExtracted);
	if (print)
		printf(EXIT);
	return;
}

//Return -1 if failed
//Return 1 if succed
int print_5_GlobalFeatures(SPPoint** RBG_HIST, SPPoint*** RGB_DB,
		int HowManypic) {
	//Allocate Priority Queue
	printf("\n**line131\n");
	SPBPQueue* Closeset5 = spBPQueueCreate(HOWMANYCLOSE);
	//Allocation failed
	if (Closeset5 == NULL) {
		printf("\n**line135\n");
		printf(MEM_PROBLEMS);
		return -1;
	}
	printf("\n*@*line139\n");
	printf("\n** HowManypic: %d", HowManypic);
	for (int i = 0; i < HowManypic; i++) {
		printf("\n*** in loop");
		double dis = spRGBHistL2Distance(RBG_HIST, RGB_DB[i]);
		printf("\n*** dis: %d", dis);
		spBPQueueEnqueue(Closeset5, i, dis);
	}

	//Allocate BPQueueElement for peeking from queue
	printf("\n**line146\n");
	BPQueueElement* ElemForEnqueue = (BPQueueElement*) malloc(
			sizeof(BPQueueElement));

	//Allocation failed
	if (ElemForEnqueue == NULL) {
		printf("\n**line152\n");
		printf(MEM_PROBLEMS);
		spBPQueueDestroy(Closeset5);
		return -1;
	}

	//print 5 closest
	printf("\n**line159\n");
	printf(PRINT_BY_GLOBAL);
	for (int i = 0; i < HOWMANYCLOSE; i++) {
		if (spBPQueueIsEmpty(Closeset5)) {
			spBPQueueDestroy(Closeset5);
			free(ElemForEnqueue);
			return 1;
		}
		if (i > 0)
			printf(",");
		spBPQueuePeek(Closeset5, ElemForEnqueue);
		spBPQueueDequeue(Closeset5);
		printf("%d", ElemForEnqueue->index);
	}
	printf("\n");

	//free memory
	spBPQueueDestroy(Closeset5);
	free(ElemForEnqueue);
	return 1;
}

//prints 5 closest local pictures. return -1 if fails
int print_5_LocalFeatures(SPPoint** SIFTS_Query, SPPoint*** SIFTS_DB,int* NumOfSiftExtracted, int howManySiftsQuery,
		int HowManypic) {

	//create array for all pictures
	int* arrayOfAll = (int*)malloc(sizeof(int)*HowManypic);
	if (arrayOfAll == NULL){
		printf(MEM_PROBLEMS);
		return -1;
	}
	//insert zero for each cell in the array
	for (int i = 0; i < HowManypic; i++)
		arrayOfAll[i] = 0;

	//Checks each sppoint in siftquery array
	for (int i = 0; i < howManySiftsQuery; i++){
		int* tempArrayForSift = spBestSIFTL2SquaredDistance(HOWMANYCLOSE, SIFTS_Query[i] ,
				SIFTS_DB, HowManypic,
				NumOfSiftExtracted);

		//Allocation failed
		if (tempArrayForSift == NULL){
			free(arrayOfAll);
			return -1;
		}

		//increment value of pic with close sift
		for (int j = 0; j < HOWMANYCLOSE; j++)
			arrayOfAll[tempArrayForSift[j]]++;

		//free tempArrayForSift before next iteration
		free(tempArrayForSift);
	}

	//After create array that contain for each index i how many times picture with index i was in top 5 sifts
	//create Priority for printing the 5 indexes with higher values

	//Allocate Priority Queue
		SPBPQueue* arrayOfAllQueue = spBPQueueCreate(HowManypic);
		//Allocation failed
		if (arrayOfAllQueue == NULL) {
			printf(MEM_PROBLEMS);
			free(arrayOfAll);
			return -1;
		}
	/**
	 *  MIGHT CONTAING A LOGIC PROBLEM. WILL BE CHECKED LATER
	 *
	 *  fill Queue with picture index as index, nubmer of sifts as value.
	 *  because we would like to get images with max num of sifts the values will be entered
	 *  after multiplication with (-1) and in that we max bacome min
	 */

	for (int i = 0; i < HowManypic; i++) {
		spBPQueueEnqueue(arrayOfAllQueue, i, (double)arrayOfAll[i]*(-1));
		}

	//Allocate BPQueueElement for peeking from queue
		BPQueueElement* ElemForEnqueue = (BPQueueElement*) malloc(
				sizeof(BPQueueElement));

	//Allocation failed
	if (ElemForEnqueue == NULL) {
		printf(MEM_PROBLEMS);
		free(arrayOfAll);
		spBPQueueDestroy(arrayOfAllQueue);
		return -1;
		}

	//print 5 closest
	printf(PRINT_BY_LOCAL);
	for (int i = 0; i < min(HOWMANYCLOSE,HowManypic); i++){
		if (i > 0)
			printf(",");
		spBPQueuePeek(arrayOfAllQueue, ElemForEnqueue);
		spBPQueueDequeue(arrayOfAllQueue);

		printf("%d", (ElemForEnqueue->index));
	}
	printf("\n");

	//free resources and return 1 because everything is OK
	free(arrayOfAll);
	spBPQueueDestroy(arrayOfAllQueue);
	free(ElemForEnqueue);

	return 1;



}

//comperator for sort in decending order
int cmp (const void* num1, const void* num2) {
	return (*(int*) num2 - (*(int*)num1));
}

int min (int num1, int num2) {
	if (num1 < num2)
		return num1;
	return num2;
}



