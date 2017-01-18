#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main_aux.h"

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

void createAllDB(int* NumOfSiftExtracted, SPPoint*** RGB_DB,
		SPPoint*** SIFTS_DB, char* dir, char* picName, int HowManypic,
		char* sufName, int nBin, int nSift) {

	/** Allocate 2 Data-Bases
	 *  RGB_DB -> Data Base for RGB_histograms
	 *  SIFTS_DB -> Data Base for SIFT
	 */

	RGB_DB = (SPPoint***) malloc(sizeof(SPPoint**) * HowManypic);
	if (RGB_DB == NULL) {
		return;
	}

	SIFTS_DB = (SPPoint***) malloc(sizeof(SPPoint**) * HowManypic);
	if (SIFTS_DB == NULL) {
		free(RGB_DB);
		return;
	}

	//initialize array of ints for number of sifts extracted from each image
	NumOfSiftExtracted = (int*) malloc(sizeof(int) * HowManypic);
	if (NumOfSiftExtracted == NULL) {
		free(SIFTS_DB);
		free(RGB_DB);
		return;
	}

	//Inserts values to DB for each image
	for (int i = 0; i < HowManypic; i++) {

		//Concate path
		char src[STRLENGTH];
		char C = i + '0';
		strcpy(src, dir);
		strcat(src, picName);
		strcat(src, &C);
		strcat(src, sufName);

		RGB_DB[i] = spGetRGBHist(src, i, nBin);

		//Allocation failed
		if (RGB_DB[i] == NULL) {
			FreeDB(RGB_DB, i);
			FreeDB(SIFTS_DB, i);
			free(NumOfSiftExtracted);
			return;
		}

		SIFTS_DB[i] = spGetSiftDescriptors(src, i, nSift,
				&NumOfSiftExtracted[i]);
		//Allocation failed
		if (SIFTS_DB[i] == NULL) {
			//we have one more in RGB
			FreeDB(RGB_DB, i + 1);
			FreeDB(SIFTS_DB, i);
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
	SPBPQueue* Closeset5 = spBPQueueCreate(5);
	//Allocation failed
	if (Closeset5 == NULL) {
		return -1;
	}

	for (int i = 0; i < HowManypic; i++) {
		double dis = spRGBHistL2Distance(RBG_HIST, RGB_DB[i]);
		spBPQueueEnqueue(Closeset5, i, dis);
	}

	//Allocate BPQueueElement for peeking from queue
	BPQueueElement* ElemForEnqueue = (BPQueueElement*) malloc(
			sizeof(BPQueueElement));

	//Allocation failed
	if (ElemForEnqueue == NULL) {
		spBPQueueDestroy(Closeset5);
		return -1;
	}

	//print 5 closest
	printf(PRINT_BY_GLOBAL);
	for (int i = 0; i < 5; i++) {
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
	if (arrayOfAll == NULL)
		return -1;

	//insert zero for each cell in the array
	for (int i = 0; i < HowManypic; i++)
		arrayOfAll[i] = 0;

	//Checks each sppoint in siftquery array
	for (int i = 0; i < howManySiftsQuery; i++){
		int* tempArrayForSift = spBestSIFTL2SquaredDistance(5, SIFTS_Query[i] ,
				SIFTS_DB, HowManypic,
				NumOfSiftExtracted);

		//Allocation failed
		if (tempArrayForSift == NULL){
			free(arrayOfAll);
			return -1;
		}

		//increment value of pic with close sift
		for (int j = 0; j < 5; j++)
			arrayOfAll[tempArrayForSift[j]]++;

		//free tempArrayForSift before next iteration
		free(tempArrayForSift);
	}

	//print 5 closest
	qsort(arrayOfAll,HowManypic,sizeof(int),cmp);
	printf(PRINT_BY_LOCAL);
	for (int i = 0; i < min(5,HowManypic); i++){
		if (i > 0)
			printf(",");
		printf("%d",arrayOfAll[i]);
	}
	printf("\n");

	//free resources and return 1 because everything is OK
	free(arrayOfAll);
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
